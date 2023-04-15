#!/usr/bin/python3
import sys, os, subprocess
from os import path
from subprocess import PIPE


SRC_DIR=path.dirname(path.abspath(__file__))
CONFIG_FILE = "config"
TIMEOUT = 3
DELAY_PENALTY = 0.8
ALL_TARGET = "all"
TESTCASE_DIRNAME = "testcase"
VALIDATOR_NAME = "validate"
EXECUTABLE_NAME = "main.bin"


# Exception must be handled by the caller.
def run_cmd(cmd_str, check=True):
    args = cmd_str.split()
    p = subprocess.run(args, check=check, stdout=PIPE, stderr=PIPE,
                       timeout=TIMEOUT)
    return "".join(map(chr, p.stdout))


def run_make(problem_dir, target):
    try:
        orig_cwd = os.getcwd()
        os.chdir(problem_dir)
        run_cmd("make %s" % target, check=False) # 'make' may fail.
        os.chdir(orig_cwd)
    except FileNotFoundError:
        print("Failed to move into %s (maybe deleted)" % problem_dir)
        exit(1)
    except Exception as e:
        print("Unexpected exception: %s" % str(e))
        exit(1)


def check(problem_name, src_file, tc_num, point, is_delay):
    problem_dir = path.join(SRC_DIR, problem_name)
    src_path = path.join(problem_dir, src_file)
    if not path.isfile(src_path): # Not submitted
        return (" ", 0.0)

    validator = path.join(problem_dir, VALIDATOR_NAME)
    if path.isfile(validator): # Skip if validator does not exist
        try:
            output = run_cmd("%s %s" % (validator, src_path))
            if output.strip() != "":
                return ("I" * tc_num, 0.0)
        except Exception as e:
            # Validator raised an error.
            return ("I" * tc_num, 0.0)

    # Build the problem directory.
    run_make(problem_dir, "clean")
    run_make(problem_dir, "")
    executable = path.join(problem_dir, EXECUTABLE_NAME)
    if not path.isfile(executable):
        return ("C" * tc_num, 0.0)

    grading_str = ""
    # Now start the grading with each testcase file.
    tc_dir = path.join(problem_dir, TESTCASE_DIRNAME)
    for i in range(tc_num):
        tc_path = path.join(tc_dir, "tc-%d" % (i + 1))
        ans_path = path.join(tc_dir, "ans-%d" % (i + 1))
        f = open(ans_path)
        ans = f.read()
        f.close()
        try:
            output = run_cmd("%s %s" % (executable, tc_path))
            if ans.strip() == output.strip():
                grading_str += "O"
            else:
                grading_str += "X"
        except subprocess.TimeoutExpired:
            grading_str += "T"
        except subprocess.CalledProcessError as e:
            print(e)
            grading_str += "E"

    ratio = float(grading_str.count("O")) / tc_num
    obtained_point = point * ratio
    if is_delay:
        grading_str += "(Delay)"
        obtained_point *= DELAY_PENALTY
    return grading_str, obtained_point


def parse_config():
    f = open(path.join(SRC_DIR, CONFIG_FILE))
    problem_list = []
    for line in f:
        tokens = line.strip().split(" ")
        problem_name = tokens[0]
        src_file = tokens[1]
        tc_num = int(tokens[2])
        point = int(tokens[3])
        problem_list.append((problem_name, src_file, tc_num, point))
    f.close()
    return problem_list


def main():
    if len(sys.argv) not in [2, 3]:
        # --delay option is hidden.
        print("Usage: %s < %s | 1-1 | 1-2 | ... >" % (sys.argv[0], ALL_TARGET))
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
    for (problem_name, src_file, tc_num, point) in problem_list:
        if target == ALL_TARGET or problem_name == target:
            if not csv_flag:
                print("[*] Grading %s ..." % problem_name)
            grading_str, obtained_point = \
                check(problem_name, src_file, tc_num, point, delay_flag)
            total_point += obtained_point
            if csv_flag:
                print("%s, %.1f, " % (grading_str, obtained_point), end="")
            else:
                print("[*] Result: %s" % grading_str)
    if csv_flag:
        print("%.1f," % total_point)


if __name__ == "__main__":
    main()
