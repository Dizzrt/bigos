#!/usr/bin/env python3
# File: bigos.py
# Created by Dizzrt on 2023/08/07.
#
# Copyright (C) 2023 The BigOS Authors.
# Licensed under the GNU General Public License v3.0 only.
#

import sys
import signal
import getopt
import subprocess

SHORT_OPTS = "hbir"
LONG_OPTS = [
    "help",
    "run",
    "init-disk",
    "install-boot",
    "build-lib",
]

OPT_HELP = "HELP"
OPT_INIT_DISK = "INIT_DISK"
OPT_INSTALL_BOOT = "INSTALL_BOOT"
OPT_BUILD_LIB = "BUILD_LIB"
OPT_BUILD_KERNEL = "BUILD_KERNEL"
OPT_INSTALL_KERNEL = "INSTALL_KERNEL"
OPT_RUN = "RUN"

SUBPROCESS: subprocess.Popen = None


def help():
    print("Usage: bigos [option...]")
    print("  %-30s%s" % ("-h, --help", "Display this information"))
    print("  %-30s%s" % ("-b", "build kernel, but do not install"))
    print("  %-30s%s" % ("-i", "build and install kernel"))
    print("  %-30s%s" % ("-r, --run", "run kernel in bochs"))
    print("  %-30s%s" % ("--init-disk", "build and install mbr, dbr ..."))
    print("  %-30s%s" % ("--install-boot", "build and install bootloader"))
    print("  %-30s%s" % ("--build-lib", "build libs which kernel needs"))
    return None


def sigintHandler(signal, frame):
    if SUBPROCESS is not None:
        SUBPROCESS.send_signal(signal)
    else:
        sys.exit(-2)


class Log:
    @staticmethod
    def info(obj: object):
        print(str(obj))
        return None

    @staticmethod
    def success(obj: object):
        print("\033[32m" + str(obj) + "\033[0m")
        return None

    @staticmethod
    def warning(obj: object):
        print("\033[93m" + str(obj) + "\033[0m")
        return None

    @staticmethod
    def error(obj: object):
        print("\033[31merror:" + str(obj) + "\033[0m")
        return None

    @staticmethod
    def fatal(obj: object, code: int = 1):
        print("\033[31merror:" + str(obj) + "\033[0m")
        exit(code)


def parseOpts(argv) -> (dict, slice):
    try:
        opts, args = getopt.gnu_getopt(argv, SHORT_OPTS, LONG_OPTS)
    except getopt.GetoptError as error:
        Log.error(error)
        return ({}, [])

    taskMap = {}
    for opt, val in opts:
        if opt in ["-h", "--help"]:
            taskMap[OPT_HELP] = True
        elif opt in ["--init-disk"]:
            taskMap[OPT_INIT_DISK] = True
        elif opt in ["--install-boot"]:
            taskMap[OPT_INSTALL_BOOT] = True
        elif opt in ["--build-lib"]:
            taskMap[OPT_BUILD_LIB] = True
        elif opt in ["-b"]:
            taskMap[OPT_BUILD_KERNEL] = True
        elif opt in ["-i"]:
            taskMap[OPT_BUILD_KERNEL] = True
            taskMap[OPT_INSTALL_KERNEL] = True
        elif opt in ["-r"]:
            taskMap[OPT_RUN] = True

    return (taskMap, args)


def process(cmds):
    global SUBPROCESS
    SUBPROCESS = subprocess.Popen(
        cmds,
        shell=True,
        stdin=sys.stdin,
        stdout=sys.stdout,
        stderr=sys.stderr,
        close_fds=True,
        universal_newlines=True,
        bufsize=1,
    )
    SUBPROCESS.communicate()

    code = SUBPROCESS.returncode
    SUBPROCESS = None

    return code


def initDisk():
    return None


def installBoot():
    return None


def buildLibs():
    return None


def buildKernel():
    cmds = ["./t"]
    code = process(cmds)
    if code == 0:
        Log.success("build kernel successfully")
    else:
        Log.error("build kernel failed with return code: " + str(code))
    return None


def installKernel():
    Log.error("install kernel failed")
    return None


def run():
    cmds = ["make"]
    process(cmds)
    return None


def main(argv):
    taskMap, args = parseOpts(argv)
    if taskMap.get(OPT_HELP, False):
        help()
    if taskMap.get(OPT_INIT_DISK, False):
        initDisk()
    if taskMap.get(OPT_INSTALL_BOOT, False):
        installBoot()
    if taskMap.get(OPT_BUILD_LIB, False):
        buildLibs()
    if taskMap.get(OPT_BUILD_KERNEL, False):
        buildKernel()
    if taskMap.get(OPT_INSTALL_KERNEL, False):
        installKernel()
    if taskMap.get(OPT_RUN, False):
        run()
    return None


if __name__ == "__main__":
    signal.signal(signal.SIGINT, sigintHandler)
    main(sys.argv[1:])
