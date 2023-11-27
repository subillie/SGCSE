#!/usr/bin/python3
import sys, os, subprocess
from os import path
from subprocess import PIPE


ROOT_DIR = path.dirname(path.abspath(__file__))
CONFIG_FILE = "config"
SECRET_FILE = "secret.txt"
TIMEOUT = 5
DELAY_PENALTY = 0.8
ALL_TARGET = "all"


# Exception must be handled by the caller.
def run_cmd(cmd_str, check=True):
    args = cmd_str.split()
    p = subprocess.run(args, check=check, stdout=PIPE, stderr=PIPE,
                       timeout=TIMEOUT)
    return "".join(map(chr, p.stdout))


def check(problem_name, exploit_name, point, is_delay):
    problem_dir = path.join(ROOT_DIR, problem_name)

    # Read secret file
    secret_path = path.join(problem_dir, SECRET_FILE)
    f = open(secret_path, "r")
    secret_key = f.read().strip()
    f.close()

    exploit_path = path.join(problem_dir, exploit_name)
    # Return if script does not exist.
    if not path.isfile(exploit_path):
        return (" ", 0.0)

    # Ensure that the script file has 'x' permission.
    os.chmod(exploit_path, 0o755)

    # Backup current working directory, and move into problem directory. This
    # is necessary because the target program will read the secret file from its
    # relative path.
    orig_cwd = os.getcwd()
    os.chdir(problem_dir)

    try:
        output = run_cmd("%s" % exploit_path)
        if secret_key in output:
            grading_str = "O"
        else:
            grading_str = "X"
    except subprocess.TimeoutExpired:
        grading_str = "T"
    except subprocess.CalledProcessError as e:
        grading_str = "E"

    os.chdir(orig_cwd)

    assert(grading_str is not None)
    obtained_point = point if grading_str == "O" else 0
    if is_delay:
        grading_str += " (Delay)"
        obtained_point *= DELAY_PENALTY
    return grading_str, obtained_point


def parse_config():
    f = open(path.join(ROOT_DIR, CONFIG_FILE))
    problem_list = []
    for line in f:
        tokens = line.strip().split()
        problem_name = tokens[0]
        exploit_name = tokens[1]
        point = int(tokens[2])
        problem_list.append((problem_name, exploit_name, point))
    f.close()
    return problem_list


def main():
    if len(sys.argv) not in [2, 3]:
        # --delay or --normal option is hidden.
        print("Usage: %s < %s | 4-1 | 4-2 | ... >" % (sys.argv[0], ALL_TARGET))
        exit(1)

    target = sys.argv[1]
    delay_flag = False
    csv_flag = False
    if len(sys.argv) == 3:
        csv_flag = True
        if sys.argv[2] == "--delay":
            delay_flag = True
        elif sys.argv[2] == "--normal":
            pass # Nothing to do if it's --normal.
        else:
            print("Invalid option: %s" % sys.argv[2])
            exit(1)

    problem_list = parse_config()
    total_point = 0.0
    for (problem_name, exploit_name, point) in problem_list:
        if target != ALL_TARGET and target != problem_name:
            continue
        if not csv_flag:
            print("[*] Grading %s ..." % problem_name)
        grading_str, obtained_point = \
            check(problem_name, exploit_name, point, delay_flag)
        total_point += obtained_point
        if csv_flag:
            print("%s, %.1f, " % (grading_str, obtained_point), end="")
        else:
            print("[*] Result: %s" % grading_str)
    if csv_flag:
        print("%.1f," % total_point)


if __name__ == "__main__":
    main()
