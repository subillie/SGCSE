import os
from subprocess import Popen, PIPE


class Program:
    def __init__(self, bin_path):
        self.proc = None
        if not os.path.isfile(bin_path):
            print("Unable to find %s (check the directory)" % bin_path)
            exit(1)
        self.proc = Popen(["stdbuf", "-o0", "-i0", bin_path],
                          stdin=PIPE, stdout=PIPE, stderr=PIPE, bufsize=0)

    def send_line(self, line):
        line_bytes = bytes(map(ord, line + "\n"))
        self.proc.stdin.write(line_bytes)

    def read_line(self):
        line_bytes = self.proc.stdout.readline()
        line_str = "".join(map(chr, line_bytes))
        return line_str.strip()

    def read(self):
        line_bytes = self.proc.stdout.read()
        line_str = "".join(map(chr, line_bytes))
        return line_str.strip()

    def __del__(self):
        if self.proc is not None:
            self.proc.kill()
