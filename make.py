import subprocess
from pathlib import Path
import sys

# Directories
out_dir = Path('output')
cpp_dir = Path('src')
rust_dir = Path('input')

# Ensure output directory exists
out_dir.mkdir(exist_ok=True)
# Collect Rust outputs
rust_outputs = {}
for file in rust_dir.glob('*.rs'):
    exec_path = out_dir / f"{file.stem}_rust"
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
compiler_exec = out_dir / 'rusty'
compile_cmd = ['g++', '-std=c++20', *source_files, '-o', str(compiler_exec)]
comp_rusty = subprocess.run(compile_cmd, capture_output=True, text=True)
if comp_rusty.returncode != 0:
    print('RUSTy compilation error:')
    print(comp_rusty.stderr)
    print('\nRust outputs:')
    for name, out in rust_outputs.items():
        print(f"\n{name}:\n{out}")
    #sys.exit(1)

# Run RUSTy for each test case
for file in rust_dir.glob('*.rs'):
    # Run the RUSTy compiler to generate assembly (a.s)
    result_rusty = subprocess.run([compiler_exec, str(file)], capture_output=True, text=True)
    if result_rusty.returncode != 0:
        print(f"RUSTy error on {file.name}:")
        print(result_rusty.stderr)
        print('\nRust outputs:')
        for name, out in rust_outputs.items():
            print(f"\n{name}:\n{out}")
        #sys.exit(1)

    asm_path = Path('a.s')
    exe_path = out_dir / file.stem
    gcc_res = subprocess.run(['gcc', '-no-pie', str(asm_path), '-o', str(exe_path)], capture_output=True, text=True)
    asm_path.unlink(missing_ok=True)
    if gcc_res.returncode != 0:
        print(f"GCC error on {file.name}:")
        print(gcc_res.stderr)
        #sys.exit(1)

    run_res = subprocess.run([str(exe_path)], capture_output=True, text=True)
    if run_res.returncode != 0:
        print(f"Execution error on {file.name}:")
        print(run_res.stderr)
        #sys.exit(1)

    expected = rust_outputs.get(file.name, '')
    status = 'OK' if run_res.stdout == expected else 'DIFF'

    print(f"== {file.name} == {status}")
    print('Rust:')
    print(expected)
    print('RUSTy:')
    print(run_res.stdout)