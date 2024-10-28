import subprocess
import signal
import os
import time
import sys
import pty
import fcntl

class JamesTest():
    def __init__(self, makefile = 'Makefile'):
        self.makefile = makefile

    def QemuStart(self):
        cwd = os.getcwd()
        os.chdir('../')

        clean = subprocess.run(['/usr/bin/make', 'clean'],\
                stdout = subprocess.PIPE,\
                stdin = subprocess.PIPE,\
                stderr = subprocess.PIPE)
        clean.check_returncode()

        build = subprocess.run(['/usr/bin/make', 'build'],\
                stdout = subprocess.PIPE,\
                stdin = subprocess.PIPE,\
                stderr = subprocess.PIPE)
        build.check_returncode()

        self.qemu = subprocess.Popen(['/usr/bin/make', 'run'],\
                stdout = subprocess.PIPE,\
                stdin = subprocess.PIPE,\
                stderr = subprocess.PIPE)

        os.chdir(cwd)

    def QemuStop(self):
        os.kill(self.qemu.pid, signal.SIGTERM)

    def GdbStart(self, test_file):
        self.gdb = subprocess.Popen(\
                ['/usr/bin/gdb', f'--command={test_file}', '--quiet'],\
                stdin = subprocess.PIPE)


test = JamesTest()
test.QemuStart()

test.GdbStart(sys.argv[1])
test.gdb.wait()

test.QemuStop()
