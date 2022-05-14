const TIMEOUT_6_MIN = 360000;

function NORDIC_basename(path)
{
  path = path.replace(/\\/g, '/');
  var index = path.lastIndexOf('/');
  if (index >= 0)
    path = path.substr(index + 1);
  return path;
}

function NORDIC_dirname(path)
{
  path = path.replace(/\\/g, '/');
  var index = path.lastIndexOf('/');
  if (index >= 0)
    path = path.substr(0, index);
  return path;
}

function NORDIC_fixPath(path)
// convert Windows style path separators to Unix style.
// remove any trailing path separator
{
  if (!path) {
    return "";
  }
  var result = path.replace(/\\/g, '/');
  if (result[result.length - 1] == '/') {
    result = result.slice(0, -1);
  }
  return result;
}

function NORDIC_getPythonVersion(cmd)
{
  var majorVersion = 0;
  var result = studio.spawn(cmd, ["--version"], "", 1000);
  if (!result.error)
    {
      var version = result.stdout.match(/(\d+)\.(\d+)\.(\d+)/);
      if (version)
        {
          var majorVersionString = version[1];
          if (majorVersionString != "")
            majorVersion = parseInt(majorVersionString);
        }
    }
  return majorVersion;
}

function NORDIC_getPythonExecutable()
{
  var pythonCommand = "python3";
  var pythonVersion = NORDIC_getPythonVersion(pythonCommand);
  if (!pythonVersion)
    {
      pythonCommand = "python";
      pythonVersion = NORDIC_getPythonVersion(pythonCommand);
    }
  if (pythonVersion != 3)
    pythonCommand = "";
  return pythonCommand;
}

function NORDIC_createProject(zephyrBaseDir, toolchain, studioDir, buildDir, boardDir, boardName, cmakelistsFile, cmakeOption, deleteBuildDir)
{
  var sourceDir = NORDIC_dirname(cmakelistsFile);
  var projectName = NORDIC_basename(sourceDir) + ".emProject";

  studio.clearBuildLog();
  studio.beginBuildLogGroup("Creating solution " + projectName);

  var command = toolchain.ncs_toolchain_python;;
  if (command == "")
    {
      command = NORDIC_getPythonExecutable();
      if (command == "")
        {
          var error = "cannot find Python version 3 on the PATH";
          studio.outputBuildLogError(error);
          studio.endBuildLogGroup();
          throw error;
        }
    }

  var args = [ studioDir + "/html/create_nordic_project.py",
               zephyrBaseDir,
               toolchain.ncs_toolchain_gnuarmemb_path,
               studioDir,
               buildDir,
               boardDir,
               boardName,
               sourceDir,
               "--ncs-toolchain-version", toolchain.ncs_toolchain_version,
               "--cmake-executable", toolchain.ncs_toolchain_cmake,
               "--cmake-option", cmakeOption,
               "--python-executable", toolchain.ncs_toolchain_python,
               "--ninja-executable", toolchain.ncs_toolchain_ninja,
               "--dtc-executable", toolchain.ncs_toolchain_dtc
  ];

  if (deleteBuildDir)
    studio.removeDirectoryAndContents(buildDir);

  result = studio.spawn(command, args, studioDir, TIMEOUT_6_MIN)
  if (result.error)
    {
      var error = "create_nordic_project.py failed (" + result.error + ")";
      studio.outputBuildLogError(error);
      studio.endBuildLogGroup();
      throw error;
    }

  if (result.stdout != "")
    studio.outputBuildLogMessage(result.stdout);

  if (result.stderr != "")
    studio.outputBuildLogMessage(result.stderr);

  if (result.status != 0)
    {
      var error = "create_nordic_project.py failed (" + result.status + ")";
      studio.outputBuildLogError(error);
      studio.endBuildLogGroup();
      throw error;
    }

  studio.endBuildLogGroup();

  if (studio.openSolution(buildDir + "/" + projectName, false))
    if (!studio.setActiveProject("zephyr/merged.hex"))
      studio.setActiveProject("zephyr/zephyr.elf(build)");
}

function NORDIC_reloadCMakeProject(zephyrBaseDir, gnuArmEmbToolchainDir, cmakeExecutable, studioDir, buildDir, boardDir, boardName, cmakelistsFile)
{
  var sourceDir = NORDIC_dirname(cmakelistsFile);
  var projectName = NORDIC_basename(sourceDir) + ".emProject";

  studio.clearBuildLog();
  studio.beginBuildLogGroup("Creating solution " + projectName);

  var command = "" // toolchain.ncs_toolchain_python;;
  if (command == "")
    {
      command = NORDIC_getPythonExecutable();
      if (command == "")
        {
          var error = "cannot find Python version 3 on the PATH";
          studio.outputBuildLogError(error);
          studio.endBuildLogGroup();
          throw error;
        }
    }

  var args = [ studioDir + "/html/create_nordic_project.py",
               zephyrBaseDir,
               gnuArmEmbToolchainDir,
               studioDir,
               buildDir,
               boardDir,
               boardName,
               sourceDir,
               "--cmake-executable", cmakeExecutable,
               "--cmake-rerun"
  ];

  result = studio.spawn(command, args, studioDir, TIMEOUT_6_MIN)
  if (result.error)
    {
      var error = "create_nordic_project.py failed (" + result.error + ")";
      studio.outputBuildLogError(error);
      studio.endBuildLogGroup();
      throw error;
    }

  if (result.stdout != "")
    studio.outputBuildLogMessage(result.stdout);

  if (result.stderr != "")
    studio.outputBuildLogMessage(result.stderr);

  if (result.status != 0)
    {
      var error = "create_nordic_project.py failed (" + result.status + ")";
      studio.outputBuildLogError(error);
      studio.endBuildLogGroup();
      throw error;
    }

  studio.endBuildLogGroup();

  if (studio.openSolution(buildDir + "/" + projectName, false))
    if (!studio.setActiveProject("zephyr/merged.hex"))
      studio.setActiveProject("zephyr/zephyr.elf(build)");
}
