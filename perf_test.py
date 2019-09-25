import subprocess
import csv

input_sizes = ["64", "128", "256", "1024"]
integer_inputs = [f"int{size}.in" for size in input_sizes]
float_inputs = [f"float{size}.in" for size in input_sizes]

operations = ["mm", "sm", "ts", "tr", "ad"]
unary_ops = ["tr", "ts", "sm"]
thread_numbers = ["1", "2", "4"]

def test_operation(op, size, threads):
    infile_dirs = "./CITS3402P1-Tests/inputs/"
    args = [
        "./CITS3402P1/x64/Release/CITS3402P1.exe",
        f"--{op}",
        "2.0" if op == "sm" else None,
        "-f",
        f"{infile_dirs}int{size}.in",
        None if op in unary_ops else f"{infile_dirs}int{size}.in",
        "-t",
        str(threads)
    ]
    args = [arg for arg in args if arg is not None]
    result = subprocess.run(args, capture_output=True, text=True, universal_newlines=True)
    return result.stdout.splitlines()[-1]

if __name__ == "__main__":
    with open("results2.csv", "w", newline="") as csv_fileobj:
        writer = csv.DictWriter(csv_fileobj, fieldnames=["op", "size", "threads", "trial", "time"])
        writer.writeheader()
        for op in operations:
            for size in input_sizes:
                for threads in thread_numbers:
                    for trial in range(100):
                        writer.writerow({
                            "op": op,
                            "size": int(size),
                            "threads": int(threads),
                            "trial": trial,
                            "time": float(test_operation(op, size, threads))
                        })