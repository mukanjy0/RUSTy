import subprocess
import os
from pathlib import Path

# cpp directory
cpp_dir = Path("src")
source_files = [str(file) for file in cpp_dir.glob("*.cpp")]
source_files.append("main.cpp")

# Compile
print("[1] Compiling...")
compile_cmd = ["g++"] + source_files + ["-o", "out/main"]
comp_rusty = subprocess.run(compile_cmd)

# Check for compilation errors
if comp_rusty.returncode != 0:
    print("\033[91mCompiling Error\033[0m") # Red text
    exit(1)


print("[2] Compiling Rust files...")
rust_dir = Path('input')
rust_output = []

for i, file in enumerate(rust_dir.glob("*.rs")):
    # Rust file compilation
    path = str(file)
    output_path = "out/" + str(file.name)[:-3]
    compile_cmd = ["rustc", path, "-o",output_path]

    print(f"\t[{i}] Compiling on Rust: {file}") 
    comp_rust = subprocess.run(compile_cmd, capture_output=True, text=True)

    # Successful compilation
    if comp_rust.returncode == 0:
        print(f"\033[92m\t[{i}] Rust compiling complete\033[0m")

        ejec_cmd = ["./" + output_path]
        result_rust = subprocess.run(ejec_cmd, capture_output=True, text=True)

        if result_rust.returncode == 0:
            print(f"\033[92m\t[{i}] Rust excecution complete\033[0m")
            rust_output.append(result_rust.stdout)
        else:  
            print(f"\033[91mExecuting error on {file}\033[0m")
            exit(1)
    else:
        print(f"\033[91mCompiling Error on {file}\033[0m")
        exit(1)
    

    print(f"\t[{i}] Compiling on RUSTy") 
    result_rusty = subprocess.run(["./out/main", str(file)], capture_output=True, text=True)
    if result_rusty.returncode == 0:
        print(f"\033[92m\t[{i}] RUSTy execution complete\033[0m")

        if rust_output[i] == result_rusty.stdout:
            print(f"\033[92m\t[{i}] RUSTy output matches Rust output\033[0m")
        else:
            print(f"\033[91m\t[{i}] RUSTy output does not match Rust output\033[0m")
            print(f"\033[33mRUSTy output:\n{result_rusty.stdout}\n---\033[0m")
            print(f"\033[34mRust output:\n{rust_output[i]}\n===\033[0m")
            exit(1)

    else:
        print(result_rusty.stdout)
        print(f"\033[91mRUSTy execution error on {file}\033[0m")
        print(result_rusty.stderr)


print("Compilación exitosa.\n")