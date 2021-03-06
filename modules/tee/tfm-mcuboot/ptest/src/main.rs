//! Parallel testing.
//!
//! mcuboot simulator is strictly single threaded, as there is a lock around running the C startup
//! code, because it contains numerous global variables.
//!
//! To help speed up testing, the Workflow configuration defines all of the configurations that can
//! be run in parallel.  Fortunately, cargo works well this way, and these can be run by simply
//! using subprocess for each particular thread.
//!
//! For now, we assume all of the features are listed under
//! jobs->environment->strategy->matric->features

use chrono::Local;
use log::{debug, error, warn};
use std::{
    collections::HashSet,
    fs::{self, OpenOptions},
    io::{ErrorKind, stdout, Write},
    process::{Command, Output},
    result,
    sync::{
        Arc,
        Mutex,
    },
    thread,
    time::Duration,
};
use std_semaphore::Semaphore;
use yaml_rust::{
    Yaml,
    YamlLoader,
};

type Result<T> = result::Result<T, failure::Error>;

fn main() -> Result<()> {
    env_logger::init();

    let workflow_text = fs::read_to_string("../.github/workflows/sim.yaml")?;
    let workflow = YamlLoader::load_from_str(&workflow_text)?;

    let ncpus = num_cpus::get();
    let limiter = Arc::new(Semaphore::new(ncpus as isize));

    let matrix = Matrix::from_yaml(&workflow)?;

    let mut children = vec![];
    let state = State::new(matrix.envs.len());
    let st2 = state.clone();
    let _status = thread::spawn(move || {
        loop {
            thread::sleep(Duration::new(15, 0));
            st2.lock().unwrap().status();
        }
    });
    for env in matrix.envs {
        let state = state.clone();
        let limiter = limiter.clone();

        let child = thread::spawn(move || {
            let _run = limiter.access();
            state.lock().unwrap().start(&env);
            let out = env.run();
            state.lock().unwrap().done(&env, out);
        });
        children.push(child);
    }

    for child in children {
        child.join().unwrap();
    }

    println!("");

    Ok(())
}

/// State, for printing status.
struct State {
    running: HashSet<String>,
    done: HashSet<String>,
    total: usize,
}

impl State {
    fn new(total: usize) -> Arc<Mutex<State>> {
        Arc::new(Mutex::new(State {
            running: HashSet::new(),
            done: HashSet::new(),
            total: total,
        }))
    }

    fn start(&mut self, fs: &FeatureSet) {
        let key = fs.textual();
        if self.running.contains(&key) || self.done.contains(&key) {
            warn!("Duplicate: {:?}", key);
        }
        debug!("Starting: {} ({} running)", key, self.running.len() + 1);
        self.running.insert(key);
        self.status();
    }

    fn done(&mut self, fs: &FeatureSet, output: Result<Option<Output>>) {
        let key = fs.textual();
        self.running.remove(&key);
        self.done.insert(key.clone());
        match output {
            Ok(None) => {
                // println!("Success {} ({} running)", key, self.running.len());
            }
            Ok(Some(output)) => {
                // Write the output into a file.
                let mut count = 1;
                let (mut fd, logname) = loop {
                    let name = format!("./failure-{:04}.log", count);
                    count += 1;
                    match OpenOptions::new()
                        .create_new(true)
                        .write(true)
                        .open(&name)
                    {
                        Ok(file) => break (file, name),
                        Err(ref err) if err.kind() == ErrorKind::AlreadyExists => continue,
                        Err(err) => {
                            error!("Unable to write log file to current directory: {:?}", err);
                            return;
                        }
                    }
                };
                writeln!(&mut fd, "Test failure {}", key).unwrap();
                writeln!(&mut fd, "time: {}", Local::now().to_rfc3339()).unwrap();
                writeln!(&mut fd, "----------------------------------------").unwrap();
                writeln!(&mut fd, "stdout:").unwrap();
                fd.write_all(&output.stdout).unwrap();
                writeln!(&mut fd, "----------------------------------------").unwrap();
                writeln!(&mut fd, "\nstderr:").unwrap();
                fd.write_all(&output.stderr).unwrap();
                error!("Failure {} log:{:?} ({} running)", key, logname,
                    self.running.len());
            }
            Err(err) => {
                error!("Unable to run test {:?} ({:?})", key, err);
            }
        }
        self.status();
    }

    fn status(&self) {
        let running = self.running.len();
        let done = self.done.len();
        print!(" {} running ({}/{}/{} done)\r", running, done, running + done, self.total);
        stdout().flush().unwrap();
    }
}

/// The extracted configurations from the workflow config
#[derive(Debug)]
struct Matrix {
    envs: Vec<FeatureSet>,
}

#[derive(Debug, Eq, Hash, PartialEq)]
struct FeatureSet {
    // The environment variable to set.
    env: String,
    // The successive values to set it to.
    values: Vec<String>,
}

impl Matrix {
    fn from_yaml(yaml: &[Yaml]) -> Result<Matrix> {
        let mut envs = vec![];

        let mut all_tests = HashSet::new();

        for y in yaml {
            let m = match lookup_matrix(y) {
                Some (m) => m,
                None => continue,
            };
            for elt in m {
                let elt = match elt.as_str() {
                    None => {
                        warn!("Unexpected yaml: {:?}", elt);
                        continue;
                    }
                    Some(e) => e,
                };
                let fset = match FeatureSet::decode(elt) {
                    Ok(fset) => fset,
                    Err(err) => {
                        warn!("Skipping: {:?}", err);
                        continue;
                    }
                };

                if false {
                    // Respect the groupings in the `.workflow.yml` file.
                    envs.push(fset);
                } else {
                    // Break each test up so we can run more in
                    // parallel.
                    let env = fset.env.clone();
                    for val in fset.values {
                        if !all_tests.contains(&val) {
                            all_tests.insert(val.clone());
                            envs.push(FeatureSet {
                                env: env.clone(),
                                values: vec![val],
                            });
                        } else {
                            warn!("Duplicate: {:?}: {:?}", env, val);
                        }
                    }
                }
            }
        }

        Ok(Matrix {
            envs: envs,
        })
    }
}

impl FeatureSet {
    fn decode(text: &str) -> Result<FeatureSet> {
        // The github workflow is just a space separated set of values.
        let values: Vec<_> = text
            .split(',')
            .map(|s| s.to_string())
            .collect();
        Ok(FeatureSet {
            env: "MULTI_FEATURES".to_string(),
            values: values,
        })
    }

    /// Run a test for this given feature set.  Output is captured and will be returned if there is
    /// an error.  Each will be run successively, and the first failure will be returned.
    /// Otherwise, it returns None, which means everything worked.
    fn run(&self) -> Result<Option<Output>> {
        for v in &self.values {
            let output = Command::new("bash")
               .arg("./ci/sim_run.sh")
               .current_dir("..")
               .env(&self.env, v)
               .output()?;
            if !output.status.success() {
                return Ok(Some(output));
            }
        }
        return Ok(None);
    }

    /// Convert this feature set into a textual representation
    fn textual(&self) -> String {
        use std::fmt::Write;

        let mut buf = String::new();

        write!(&mut buf, "{}:", self.env).unwrap();
        for v in &self.values {
            write!(&mut buf, " {}", v).unwrap();
        }

        buf
    }
}

fn lookup_matrix(y: &Yaml) -> Option<&Vec<Yaml>> {
    let jobs = Yaml::String("jobs".to_string());
    let environment = Yaml::String("environment".to_string());
    let strategy = Yaml::String("strategy".to_string());
    let matrix = Yaml::String("matrix".to_string());
    let features = Yaml::String("features".to_string());
    y
        .as_hash()?.get(&jobs)?
        .as_hash()?.get(&environment)?
        .as_hash()?.get(&strategy)?
        .as_hash()?.get(&matrix)?
        .as_hash()?.get(&features)?
        .as_vec()
}
