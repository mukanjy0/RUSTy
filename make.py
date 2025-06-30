import subprocess
from pathlib import Path

# -------------------------------
# Step 1: Compile C++ backend (RUSTy)
# -------------------------------

# Define source directory and output directory
cpp_dir = Path("src")
output_dir = Path("out")
output_dir.mkdir(parents=True, exist_ok=True)  # Ensure output directory exists

# Collect C++ source files
# source_files = list(cpp_dir.glob("*.cpp"))
# source_files.append("main.cpp")
source_files = ["main.cpp"]

print("[1] Compiling RUSTy (C++)...")
compile_cpp_cmd = ["g++"] + source_files + ["-o", str(output_dir / "main")]
cpp_result = subprocess.run(compile_cpp_cmd)

# Check for C++ compilation errors
if cpp_result.returncode != 0:
    print("\033[91mError: C++ compilation failed\033[0m")  # Red text
    exit(1)

# -------------------------------
# Step 2: Compile and run each Rust file
# -------------------------------

rust_dir = Path("input")
rust_outputs = []

print("[2] Compiling and executing Rust files...")

for i, rust_file in enumerate(rust_dir.glob("*.rs")):
    rust_executable = output_dir / rust_file.stem

    print(f"\t[{i}] Compiling Rust: {rust_file}")
    compile_rust_cmd = ["rustc", str(rust_file), "-o", str(rust_executable)]
    rust_compile_result = subprocess.run(compile_rust_cmd, capture_output=True, text=True)

    if rust_compile_result.returncode != 0:
        print(f"\033[91mError: Rust compilation failed for {rust_file}\033[0m")
        exit(1)

    print(f"\033[92m\t[{i}] Rust compilation successful\033[0m")

    # Run compiled Rust program
    rust_run_result = subprocess.run(["./"+str(rust_executable)], capture_output=True, text=True)

    if rust_run_result.returncode != 0:
        print(f"\033[91mError: Rust execution failed for {rust_file}\033[0m")
        exit(1)

    print(f"\033[92m\t[{i}] Rust execution successful\033[0m")
    rust_outputs.append(rust_run_result.stdout)

    # -------------------------------
    # Step 3: Run RUSTy on the same file and compare results
    # -------------------------------

    print(f"\t[{i}] Running RUSTy (C++) with {rust_file}...")
    rusty_run_cmd = [str(output_dir / "main"), str(rust_file)]
    rusty_result = subprocess.run(rusty_run_cmd, capture_output=True, text=True)

    if rusty_result.returncode != 0:
        print(f"\033[91mError: RUSTy execution failed on {rust_file}\033[0m")
        exit(1)

    print(f"\033[92m\t[{i}] RUSTy execution successful\033[0m")

    # -------------------------------
    # Step 4: Compile generated assembly and compare outputs
    # -------------------------------

    print(f"\t[{i}] Compiling generated assembly for {rust_file.stem}...")

    assembly_file = Path("input/ass") / f"{rust_file.stem}.s"
    assembly_executable = output_dir / f"{rust_file.stem}_ass"

    compile_asm_cmd = ["g++", str(assembly_file), "-o", str(assembly_executable)]
    asm_compile_result = subprocess.run(compile_asm_cmd)

    if asm_compile_result.returncode != 0:
        print("\033[91mError: Assembly compilation failed\033[0m")
        exit(1)

    # Execute compiled assembly
    asm_run_result = subprocess.run([str(assembly_executable)], capture_output=True, text=True)

    if asm_run_result.returncode != 0:
        print(f"\033[91mError: Assembly execution failed on {rust_file}\033[0m")
        exit(1)

    # Compare output with original Rust execution
    if rust_outputs[i].split() == asm_run_result.stdout.split():
        print(f"\033[92m\t[{i}] ✅ Output match: RUSTy == Rust\033[0m")
    else:
        print(f"\033[91m\t[{i}] ❌ Output mismatch: RUSTy != Rust\033[0m")
        print(f"\033[33m--- RUSTy output:\n{asm_run_result.stdout}\033[0m")
        print(f"\033[34m--- Rust output:\n{rust_outputs[i]}\033[0m")
        exit(1)

# -------------------------------
# All Done
# -------------------------------
print("\n\033[92mAll compilations and comparisons were successful!\033[0m")
