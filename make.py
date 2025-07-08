import subprocess
from pathlib import Path
import sys

out_dir = Path('output')
cpp_dir = Path('src')
rust_dir = Path('input')
# Collect Rust outputs
rust_outputs = {}
for file in rust_dir.glob('*.rs'):
    exec_path = out_dir / file.stem
    comp = subprocess.run(['rustc', str(file), '-o', str(exec_path)], capture_output=True, text=True)
    if comp.returncode == 0:
        run = subprocess.run([str(exec_path)], capture_output=True, text=True)
        if run.returncode == 0:
            rust_outputs[file.name] = run.stdout
        else:
            rust_outputs[file.name] = f"Runtime error:\n{run.stderr}"

    else:
        rust_outputs[file.name] = f"Compile error:\n{comp.stderr}"

# Compile RUSTy
source_files = [str(f) for f in cpp_dir.rglob('*.cpp')]
source_files.append('main.cpp')
compile_cmd = ['g++', '-std=c++20', *source_files, '-o', str(out_dir/'main')]
comp_rusty = subprocess.run(compile_cmd, capture_output=True, text=True)
if comp_rusty.returncode != 0:
    print('RUSTy compilation error:')
    print(comp_rusty.stderr)
    print('\nRust outputs:')
    for name, out in rust_outputs.items():
        print(f"\n{name}:\n{out}")
    sys.exit(1)

# Run RUSTy for each test case
for file in rust_dir.glob('*.rs'):
    result_rusty = subprocess.run([out_dir/'main', str(file)], capture_output=True, text=True)
    if result_rusty.returncode != 0:
        print(f"RUSTy error on {file.name}:")
        print(result_rusty.stderr)
        print('\nRust outputs:')
        for name, out in rust_outputs.items():
            print(f"\n{name}:\n{out}")
        sys.exit(1)

    print(f"== {file.name} ==")
    print('Rust:')
    print(rust_outputs[file.name])
    print('RUSTy:')
    print(result_rusty.stdout)