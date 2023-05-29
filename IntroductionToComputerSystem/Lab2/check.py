#!/usr/bin/python3
import sys, os, subprocess
from os import path
from subprocess import PIPE


SRC_DIR=path.dirname(path.abspath(__file__))
CONFIG_FILE = "config"
TIMEOUT = 3
DELAY_PENALTY = 0.8
ALL_TARGET = "all"
SECRET_FILE = "secret.txt"


# Exception must be handled by the caller.
def run_cmd(cmd_str, check=True):
    args = cmd_str.split()
    p = subprocess.run(args, check=check, stdout=PIPE, stderr=PIPE,
                       timeout=TIMEOUT)
    return "".join(map(chr, p.stdout))


def check(problem_name, src_file, point, is_delay):
    problem_dir = path.join(SRC_DIR, problem_name)
    src_path = path.join(problem_dir, src_file)

    # Read secret file
    secret_path = path.join(problem_dir, SECRET_FILE)
    f = open(secret_path, "r")
    secret_key = f.read().strip()
    f.close()

    if not path.isfile(src_path): # Not submitted
        return (" ", 0.0)

    # Backup current working directory, and move into problem directory.This
    # is essential for this Lab, because the target program will read the secret
    # file from its relative path.
    orig_cwd = os.getcwd()
    os.chdir(problem_dir)

    try:
        output = run_cmd("%s" % src_path) # Execute python script directly
        if secret_key in output:
            grading_str = "O"
        else:
            grading_str = "X"
    except subprocess.TimeoutExpired:
        grading_str = "T"
    except subprocess.CalledProcessError as e:
        print(e)
        grading_str = "E"

    os.chdir(orig_cwd)

    assert(grading_str is not None)
    obtained_point = point if grading_str == "O" else 0
    if is_delay:
        grading_str += " (Delay)"
        obtained_point *= DELAY_PENALTY
    return grading_str, obtained_point


def parse_config():
    f = open(path.join(SRC_DIR, CONFIG_FILE))
    problem_list = []
    for line in f:
        tokens = line.strip().split(" ")
        problem_name = tokens[0]
        src_file = tokens[1]
        point = int(tokens[3])
        problem_list.append((problem_name, src_file, point))
    f.close()
    return problem_list


def main():
    if len(sys.argv) not in [2, 3]:
        # --delay option is hidden.
        print("Usage: %s < %s | 2-1 | 2-2 | ... >" % (sys.argv[0], ALL_TARGET))
        exit(1)

    target = sys.argv[1]
    if target.endswith("/"):
        print("[*] Fixing %s into %s" % (target, target[:-1]))
        target = target[:-1]

    delay_flag = False
    csv_flag = False
    if len(sys.argv) == 3:
        csv_flag = True
        if sys.argv[2] == "--delay":
            delay_flag = True
        elif sys.argv[2] != "--normal": # Nothing to do if it's --normal.
            print("Invalid option: %s" % sys.argv[2])
            exit(1)

    problem_list = parse_config()
    total_point = 0.0
    for (problem_name, src_file, point) in problem_list:
        if target == ALL_TARGET or problem_name == target:
            if not csv_flag:
                print("[*] Grading %s ..." % problem_name)
            grading_str, obtained_point = \
                check(problem_name, src_file, point, delay_flag)
            total_point += obtained_point
            if csv_flag:
                print("%s, %.1f, " % (grading_str, obtained_point), end="")
            else:
                print("[*] Result: %s" % grading_str)
    if csv_flag:
        print("%.1f," % total_point)


if __name__ == "__main__":
    main()
