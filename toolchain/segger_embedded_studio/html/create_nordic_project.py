import os
import subprocess
import sys

TIMEOUT_30_SEC = 30
TIMEOUT_5_MIN = 300

def runCMake(
    zephyrBaseDir,
    toolchainDir,
    studioDir,
    buildDir,
    sourceDir,
    boardDir,
    boardName,
    overlayFile,
    ncsToolchainVersion,
    cmakeExecutable,
    cmakeOption,
    pythonExecutable,
    ninjaExecutable,
    dtcExecutable,
):
    if cmakeExecutable == "":
        cmakeExecutable = "cmake"
    command = []
    command.append(cmakeExecutable)
    command.append("-GNinja")
    command.append("-DBOARD=" + boardName)
    command.append("-DBOARD_DIR=" + boardDir)
    command.append("-B" + buildDir)
    command.append("-S" + sourceDir)
    if ncsToolchainVersion != "":
        command.append("-DNCS_TOOLCHAIN_VERSION=" + ncsToolchainVersion)
    else:
        command.append("-DZEPHYR_TOOLCHAIN_VARIANT=gnuarmemb")
        command.append("-DGNUARMEMB_TOOLCHAIN_PATH=" + toolchainDir)
        if pythonExecutable != "":
            command.append("-DPYTHON_EXECUTABLE=" + pythonExecutable)
        if ninjaExecutable != "":
            command.append("-DCMAKE_MAKE_PROGRAM=" + ninjaExecutable)
        if dtcExecutable != "":
            command.append("-DDTC=" + dtcExecutable)

    if cmakeOption != "":
        command.append(cmakeOption)
    if os.path.exists(overlayFile):
        command.append("-DDTC_OVERLAY_FILE=" + overlayFile)
    command.append("-DEXTRA_KCONFIG_TARGETS=menuconfig_ses")
    command.append(
        "-DEXTRA_KCONFIG_TARGET_COMMAND_FOR_menuconfig_ses="
        + studioDir
        + "/html/configure_nordic_project_menuconfig.py"
    )

    sys.stdout.write(" ".join(command) + "\n")

    env = os.environ.copy()
    env["ZEPHYR_BASE"] = zephyrBaseDir
    process = subprocess.Popen(
        command,
        cwd=buildDir,
        env=env,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        encoding="UTF-8",
    )
    try:
        out, err = process.communicate(timeout=TIMEOUT_5_MIN)
    except subprocess.TimeoutExpired:
        process.kill()
        out, err = process.communicate()

    if len(out) > 0:
        sys.stdout.write(out)
    if len(err) > 0:
        sys.stdout.write(err)
    if process.returncode != 0:
        sys.stderr.write("error: cmake failed")
        sys.exit(1)

def rerunCMake(cmakeExecutable, buildDir):
    if cmakeExecutable == "":
        cmakeExecutable = "cmake"
    command = []
    command.append(cmakeExecutable)
    command.append(buildDir)

    process = subprocess.Popen(
        command,
        cwd=buildDir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        encoding="UTF-8",
    )
    try:
        out, err = process.communicate(timeout=TIMEOUT_5_MIN)
    except subprocess.TimeoutExpired:
        process.kill()
        out, err = process.communicate()

    if len(out) > 0:
        sys.stdout.write(out)
    if len(err) > 0:
        sys.stdout.write(err)
    if process.returncode != 0:
        sys.stderr.write("error: cmake failed")
        sys.exit(1)

def createProject(
    zephyrBaseDir,
    toolchainDir,
    studioDir,
    buildDir,
    boardDir,
    boardName,
    sourceDir,
    cmakeExecutable,
    cmakeOption,
    pythonExecutable,
    ninjaExecutable,
    dtcExecutable,
    cmakeRerun
):
    toolChainDir = toolchainDir + "/bin"
    projectName = os.path.basename(sourceDir)
    overlayFile = os.path.abspath(sourceDir).replace('\\','/') + "/" + boardName + ".overlay"

    if not os.path.exists(buildDir):
        os.mkdir(buildDir)

    if cmakeExecutable == "":
        cmakeExecutable = "cmake"

    if cmakeRerun:
        rerunCMake(cmakeExecutable, buildDir)
    else:
        runCMake(
            zephyrBaseDir,
            toolchainDir,
            studioDir,
            buildDir,
            sourceDir,
            boardDir,
            boardName,
            overlayFile,
            ncsToolchainVersion,
            cmakeExecutable,
            cmakeOption,
            pythonExecutable,
            ninjaExecutable,
            dtcExecutable,
        )

    process = subprocess.Popen(
        [cmakeExecutable, "--build", buildDir, "--", "-t", "query", "build.ninja"],
        cwd=buildDir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        encoding="UTF-8",
    )
    try:
        out, err = process.communicate(timeout=TIMEOUT_30_SEC)
    except subprocess.TimeoutExpired:
        process.kill()
        out, err = process.communicate()

    start = out.find("build.ninja:")
    if start != -1:
      start = out.find("RERUN_CMAKE")
    end = out.find("outputs:")
    if start != -1 and end != -1:
      dependencies = out[start + 11:end].replace("    | ", "").splitlines();

    xml = []
    xml.append("<!DOCTYPE CrossStudio_Project_File>")
    xml.append(
        '<solution Name="'
        + projectName
        + '" target="8" cmakelists_file_name="'
        + sourceDir + '/CMakeLists.txt'
        + '" cmakelists_project="'
        + projectName
        + ';build;app/libapp.a"'
        + ' cmakelists_end_section="# NORDIC SDK APP END"'
        + ' cmakelists_start_section="# NORDIC SDK APP START"'
        + ' cmake_command="'
        + cmakeExecutable
        + '" gnuarmemb_toolchain_directory="'
        + toolchainDir
        + '" source_directory="'
        + zephyrBaseDir
        + '" build_directory="'
        + buildDir
        + '" board_directory="'
        + boardDir
        + '" board_name="'
        + boardName
        + '" dts_compiled_file_name="'
        + buildDir
        + '/zephyr/zephyr.dts"'
        + ' dts_folder="dts files" dts_overlay_file_name="'
        + overlayFile
        + '" cmake_depends_on="'
        + ";".join(dependencies)
        + '">'
    )
    line = '  <configuration Name="Common"'
    line += ' build_toolchain_directory="' + toolChainDir + '"'
    line += (' external_archive_command="&quot;$(StudioDir)/bin/rm&quot; '
             '-f &quot;$(RelTargetPath)&quot; &amp;&amp; '
             '&quot;$(ToolChainDir)/arm-none-eabi-ar&quot; '
             '-rcs &quot;$(RelTargetPath)&quot; '
             '$(Objects)"')
    line += (' external_assemble_command='
             '"&quot;$(ToolChainDir)/arm-none-eabi-gcc&quot; '
             '$(AsmOptions) -MD -MF '
             '&quot;$(RelDependencyPath)&quot; '
             '-o &quot;$(RelTargetPath)&quot; '
             '-c &quot;$(RelInputPath)&quot;"')
    line += (' external_c_compile_command='
             '"&quot;$(ToolChainDir)/arm-none-eabi-gcc&quot; '
             '$(COnlyOptions) -MD -MF '
             '&quot;$(RelDependencyPath)&quot; '
             '-fno-diagnostics-show-caret '
             '-o &quot;$(RelTargetPath)&quot; '
             '-c &quot;$(RelInputPath)&quot;"')
    line += (' external_cpp_compile_command='
             '"&quot;$(ToolChainDir)/arm-none-eabi-gcc&quot; '
             '$(CppOnlyOptions) -MD -MF '
             '&quot;$(RelDependencyPath)&quot; '
             '-fno-diagnostics-show-caret '
             '-o &quot;$(RelTargetPath)&quot; '
             '-c &quot;$(RelInputPath)&quot;"')
    line += (' external_link_command='
             '"&quot;$(ToolChainDir)/arm-none-eabi-gcc&quot; '
             '$(Objects) '
             '$(LinkOptions) '
             '-o &quot;$(RelTargetPath)&quot;"')
    line += ' source_code_control_directory="' + zephyrBaseDir + '"'
    line += ' properties_filter="Debug"'

    file = open(boardDir + "/" + boardName + ".dts", "r")
    dtsfile = file.read()
    file.close()

    deviceName = ""
    segments = ""
    svdFile = ""
    n = dtsfile.find("#include <nordic/")
    if n != -1:
        m = dtsfile.find(".dtsi>")
        if m != -1:
            device = dtsfile[n + 17:m]
            if device == "nrf51822_qfaa":
                deviceName = "nRF51822_xxAA"
                segments = ("FLASH RX 0x00000000 0x00040000;"
                            "RAM RWX 0x20000000 0x00004000")
                svdFile = "nrf51.svd"
            elif device == "nrf51822_qfab":
                deviceName = "nRF51822_xxAB"
                segments = ("FLASH RX 0x00000000 0x00020000;"
                            "RAM RWX 0x20000000 0x00004000")
                svdFile = "nrf51.svd"
            elif device == "nrf51822_qfac":
                deviceName = "nRF51822_xxAC"
                segments = ("FLASH RX 0x00000000 0x00080000;"
                            "RAM RWX 0x20000000 0x00004000")
                svdFile = "nrf51.svd"
            elif device == "nrf52805_caaa":
                deviceName = "nRF52805_xxAA"
                segments = ("FLASH RX 0x00000000 0x00030000;"
                            "RAM RWX 0x20000000 0x00006000")
                svdFile = "nrf52805.svd"
            elif device == "nrf52810_qfaa":
                deviceName = "nRF52810_xxAA"
                segments = ("FLASH RX 0x00000000 0x00030000;"
                            "RAM RWX 0x20000000 0x00006000")
                svdFile = "nrf52810.svd"
            elif device == "nrf52811_qfaa":
                deviceName = "nRF52811_xxAA"
                segments = ("FLASH RX 0x00000000 0x00030000;"
                            "RAM RWX 0x20000000 0x00006000")
                svdFile = "nrf52811.svd"
            elif device == "nrf52820_qdaa":
                deviceName = "nRF52820_xxAA"
                segments = ("FLASH RX 0x00000000 0x00040000;"
                            "RAM RWX 0x20000000 0x00008000")
                svdFile = "nrf52820.svd"
            elif device == "nrf52832_ciaa":
                deviceName = "nRF52832_xxAA"
                segments = ("FLASH RX 0x00000000 0x00080000;"
                            "RAM RWX 0x20000000 0x00010000")
                svdFile = "nrf52.svd"
            elif device == "nrf52832_qfaa":
                deviceName = "nRF52832_xxAA"
                segments = ("FLASH RX 0x00000000 0x00080000;"
                            "RAM RWX 0x20000000 0x00010000")
                svdFile = "nrf52.svd"
            elif device == "nrf52832_qfab":
                deviceName = "nRF52832_xxAB"
                segments = ("FLASH RX 0x00000000 0x00020000;"
                            "RAM RWX 0x20000000 0x00004000")
                svdFile = "nrf52.svd"
            elif device == "nrf52833_qiaa":
                deviceName = "nRF52833_xxAA"
                segments = ("FLASH RX 0x00000000 0x00080000;"
                            "RAM RWX 0x20000000 0x00020000")
                svdFile = "nrf52833.svd"
            elif device == "nrf52840_qiaa":
                deviceName = "nRF52840_xxAA"
                segments = ("FLASH RX 0x00000000 0x00100000;"
                            "RAM RWX 0x20000000 0x00040000")
                svdFile = "nrf52840.svd"
            elif (
                device == "nrf9160_xxaa"
                or device == "nrf9160_sica"
                or device == "nrf9160ns_sica"
            ):
                deviceName = "nRF9160"
                segments = ("FLASH RX 0x00000000 0x00100000;"
                            "RAM RWX 0x20000000 0x00040000;"
                            "UICR R 0x00FF8000 0x00000810")
                svdFile = "nrf9160.svd"
            elif (device.startswith("nrf5340_cpuapp_qkaa") or
                  device.startswith("nrf5340_cpuappns_qkaa")):
                deviceName = "nRF5340_xxAA_APP"
                segments = ("FLASH RX 0x00000000 0x00100000;"
                            "RAM RWX 0x20000000 0x00080000")
                svdFile = "nrf5340_application.svd"
            elif device.startswith("nrf5340_cpunet_qkaa"):
                deviceName = "nRF5340_xxAA_NET"
                segments = ("FLASH RX 0x01000000 0x00040000;"
                            "RAM RWX 0x21000000 0x00010000")
                svdFile = "nrf5340_network.svd"
            if len(deviceName) > 0:
                line += ' arm_target_device_name="' + deviceName + '"'
            if len(segments) > 0:
                line += (
                    ' linker_section_placements_segments="'
                    + segments
                    + '"'
                )
            if len(svdFile) > 0:
                line += (
                    ' debug_register_definition_file='
                    '"$(PackagesDir)/nRF/Device/Registers/'
                    + svdFile
                    + '"'
                )
            if device.find("nrf52") != -1:
                line += (' arm_architecture="v7EM"'
                         ' arm_core_type="Cortex-M4"'
                         ' arm_fpu_type="FPv4-SP-D16"')
            elif device.find("nrf51") != -1:
                line += (' arm_architecture="v6M"'
                         ' arm_core_type="Cortex-M0"')
            elif device.find("nrf91") != -1 or device.find("nrf5340") != -1:
                line += (' arm_architecture="v8M_Mainline"'
                         ' arm_core_type="Cortex-M33"')
                if device != "nrf5340_cpunet_qkaa":
                    line += (' arm_fpu_type="FPv5-SP-D16"'
                             ' arm_v8M_has_cmse="Yes"'
                             ' arm_v8M_has_dsp="Yes"')
            line += (
                ' debug_target_connection="J-Link"'
                ' arm_target_interface_type="SWD"'
                ' debug_threads_script="$(StudioDir)/samples/ZephyrPlugin_CM4.js"'
            )
    line += " />"
    xml.append(line)

    xml.append(
        '  <folder Name="dts files">'
    )
    xml.append(
        '    <file file_name="' +
        boardDir +
        "/" +
        boardName +
        '.dts" />')
    xml.append(
        '    <file file_name="' +
        boardDir +
        "/" +
        boardName +
        '.yaml" />')
    xml.append(
        '    <file file_name="'
        + buildDir
        + '/zephyr/zephyr.dts" />'
    )
    xml.append(
        '    <file file_name="' +
        sourceDir +
        "/" +
        boardName +
        '.overlay" />')
    xml.append("  </folder>")

    if cmakeExecutable == "":
        cmakeExecutable = "cmake"
    process = subprocess.Popen(
        [cmakeExecutable, "--build", buildDir, "--", "-t", "query", "zephyr/merged.hex"],
        cwd=buildDir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        encoding="UTF-8",
    )
    try:
        out, err = process.communicate(timeout=TIMEOUT_30_SEC)
    except subprocess.TimeoutExpired:
        process.kill()
        out, err = process.communicate()

    start = out.find("zephyr/merged.hex:")
    if start != -1:
        start = out.find("CUSTOM_COMMAND")
    end = out.find("||")
    if start != -1 and end != -1:
        inputs = out[start + 14:end].strip().split("\n")
        xml.append('  <project Name="zephyr/merged.hex">')
        line = ('    <configuration Name="Common"'
                ' debug_initial_breakpoint="z_cstart"'
                ' debug_start_from_entry_point_symbol="No"'
                ' project_dependencies="all(build)"'
                ' external_build_file_name="$(ProjectDir)/zephyr/merged.hex"')
        n = 0
        for input in inputs:
            tinput = input.strip()
            if len(tinput) > 0 and tinput.rfind(".elf") == len(tinput) - 4:
                if n > 0:
                    line += (
                        " external_debug_symbols_file_name"
                        + str(n)
                        + '="$(ProjectDir)/'
                        + tinput
                        + '"'
                    )
                else:
                    line += (
                        ' external_debug_symbols_file_name="$(ProjectDir)/'
                        + tinput
                        + '"'
                    )
                n += 1
        line += (
            ' external_link_command=""'
            ' project_type="Externally Built Executable" />'
        )
        xml.append(line)
        xml.append("  </project>")

    process = subprocess.Popen(
        [studioDir + "/bin/ninja_import", "all"],
        cwd=buildDir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        encoding="UTF-8",
    )
    try:
        out, err = process.communicate(timeout=TIMEOUT_5_MIN)
    except subprocess.TimeoutExpired:
        process.kill()
        out, err = process.communicate()

    if len(out) > 0:
        sys.stdout.write(out)
    if len(err) > 0:
        sys.stdout.write(err)
    if process.returncode != 0:
        sys.stderr.write("error: ninja_import failed")
        sys.exit(1)

    xml.append('  <import file_name="build.emProject"'
               ' can_modify_solution="No" />')
    xml.append("</solution>")

    file = open(buildDir + "/" + projectName + ".emProject", "w")
    file.write("\n".join(xml))
    file.close()


if __name__ == "__main__":
    argv = sys.argv[1:]
    args = []
    cmakeExecutable = ""
    cmakeOption = ""
    cmakeRerun = False
    ncsToolchainVersion = ""
    pythonExecutable = ""
    ninjaExecutable = ""
    dtcExecutable = ""
    showHelp = False
    while len(argv):
        arg = argv.pop(0)
        if arg.startswith("--"):
            if arg == "--ncs-toolchain-version":
                ncsToolchainVersion = argv.pop(0)
            elif arg == "--cmake-executable":
                cmakeExecutable = argv.pop(0)
            elif arg == "--cmake-option":
                cmakeOption = argv.pop(0)
            elif arg == "--python-executable":
                pythonExecutable = argv.pop(0)
            elif arg == "--ninja-executable":
                ninjaExecutable = argv.pop(0)
            elif arg == "--dtc-executable":
                dtcExecutable = argv.pop(0)
            elif arg == "--cmake-rerun":
                cmakeRerun = True
            elif arg == "--help":
                showHelp = True
            else:
                sys.stderr.write("error: unknown option " + arg)
                sys.exit(1)
        else:
            args.append(arg)

    if showHelp or len(args) != 7:
        sys.stdout.write(
            "Usage: create_nordic_project.py "
            "[-h] "
            "[--cmake-executable cmakeExecutable] "
            "[--cmake-option cmakeOption]\n"
        )
        sys.stdout.write(
            "                                "
            "[--python-executable pythonExecutable] "
            "[--ninja-executable ninjaExecutable]\n"
        )
        sys.stdout.write(
            "                                "
            "[--dtc-executable dtcExecutable]\n"
        )
        sys.stdout.write(
            "                                "
            "zephyrBaseDir "
            "toolchainDir "
            "studioDir "
            "buildDir "
            "boardDir "
            "boardName "
            "sourceDir\n"
        )
        sys.stdout.write("\n")
        sys.stdout.write("Arguments:\n")
        sys.stdout.write("  zephyrBaseDir      Zephyr base directory\n")
        sys.stdout.write("  toolchainDir       GCC toolchain directory\n")
        sys.stdout.write("  studioDir          Embedded Studio directory\n")
        sys.stdout.write("  buildDir           Build directory\n")
        sys.stdout.write("  boardDir           Board directory\n")
        sys.stdout.write("  boardName          Board name\n")
        sys.stdout.write("  sourceDir          Source directory\n")
        sys.stdout.write("\n")
        sys.stdout.write("Optional arguments:\n")
        sys.stdout.write(
            "  "
            "--help"
            "                                  "
            "Show this help message and exit\n"
        )
        sys.stdout.write(
            "  "
            "--ncs-toolchain-version "
            "version"
            "NCS Toolchain version to use\n"
        )
        sys.stdout.write(
            "  "
            "--cmake-executable "
            "cmakeExecutable      "
            "CMake executable to use\n"
        )
        sys.stdout.write(
            "  "
            "--cmake-option "
            "cmakeOption              "
            "Add additional CMake option\n"
        )
        sys.stdout.write(
            "  "
            "--python-executable "
            "pythonExecutable    "
            "Python executable to use\n"
        )
        sys.stdout.write(
            "  "
            "--ninja-executable "
            "ninjaExecutable      "
            "Ninja executable to use\n"
        )
        sys.stdout.write(
            "  "
            "--dtc-executable "
            "dtcExecutable          "
            "DTC executable to use\n"
        )
        sys.stdout.write(
            "  "
            "--cmake-rerun"
            "Rerun CMake in build folder\n"
        )
        sys.exit(1)

    zephyrBaseDir = os.path.abspath(args[0])
    toolchainDir = os.path.abspath(args[1])
    studioDir = os.path.abspath(args[2])
    buildDir = os.path.abspath(args[3])
    boardDir = os.path.abspath(args[4])
    boardName = args[5]
    sourceDir = os.path.abspath(args[6])

    createProject(
        zephyrBaseDir,
        toolchainDir,
        studioDir,
        buildDir,
        boardDir,
        boardName,
        sourceDir,
        cmakeExecutable,
        cmakeOption,
        pythonExecutable,
        ninjaExecutable,
        dtcExecutable,
        cmakeRerun
    )
