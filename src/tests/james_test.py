import subprocess
import signal
import os
import glob
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
                stdout = subprocess.PIPE,\
                stdin = subprocess.PIPE,\
                stderr = subprocess.PIPE)


test_results = {}
for test_file in glob.glob('*/test_*.py'):
    test = JamesTest()
    test.QemuStart()

    test.GdbStart(test_file)
    test.gdb.wait()

    raw_output, _ = test.gdb.communicate()
    lines = raw_output.split(b'\n')
    successes = [line for line in lines if line.startswith(b'\x1b[92m')]
    fails = [line for line in lines if line.startswith(b'\x1b[31m')]

    test_results[test_file] = {
            'successes': successes,
            'fails': fails
            }

    test.QemuStop()

for test_file, results in test_results.items():
    print(f'<><><> {test_file} <><><>')
    for result, messages in results.items():
        print(f'    {result} ({len(messages)})')
        for message in messages:
            print(f'    {message.decode()}')
