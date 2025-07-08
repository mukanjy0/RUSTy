from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import subprocess
import tempfile
import os
from fastapi.middleware.cors import CORSMiddleware
from pathlib import Path
from typing import Tuple, List, Dict, Optional
app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # O especifica los orígenes permitidos
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class RustCode(BaseModel):
    code: str

def run_command(cmd: List[str], timeout: int = 60) -> Tuple[str, Optional[str]]:
    """Run a command with a timeout and capture stdout/stderr.

    Returns a tuple (stdout, error). If error is None the command succeeded.
    """
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
        if result.returncode == 0:
            return result.stdout, None
        if result.returncode == 139:
            return result.stdout, "Segmentation fault"
        error_msg = result.stderr.strip() or f"Exited with code {result.returncode}"
        return result.stdout, error_msg
    except subprocess.TimeoutExpired:
        return "", "Timeout"

@app.post("/compile")
async def compile_rust(rust_code: RustCode):
    """Compile a single Rust source snippet using RUSTy and rustc for comparison."""
    Path("input").mkdir(parents=True, exist_ok=True)
    Path("input/ass").mkdir(parents=True, exist_ok=True)

    with tempfile.NamedTemporaryFile(suffix=".rs", delete=False) as rust_file:
        rust_file.write(rust_code.code.encode("utf-8"))
        rust_file_path = rust_file.name

    # RUSTy always generates assembly in a.s in the current directory
    asm_file_path = "a.s"

    # Compile RUSTy backend
    cpp_dir = Path("src")
    cpp_source_files = [str(file) for file in cpp_dir.rglob("*.cpp")]
    cpp_source_files.append("main.cpp")
    compile_cmd = ["g++", "-std=c++20"] + cpp_source_files + ["-o", "main"]

    _, error = run_command(compile_cmd)
    if error:
        return {"success": False, "error": f"Backend compile error: {error}"}

    # Run RUSTy
    _, error = run_command(["./main", rust_file_path])
    if error:
        return {"success": False, "error": f"RUSTy execution error: {error}"}

    # Compile generated assembly
    _, error = run_command(["g++", asm_file_path, "-o", "main_asm"])
    if error:
        return {"success": False, "error": f"Asm compile error: {error}"}

    # Execute assembly
    rusty_output, error = run_command(["./main_asm"])
    if error:
        return {"success": False, "error": f"Asm run error: {error}"}

    # Compile with rustc for comparison
    _, error = run_command(["rustc", rust_file_path, "-o", "main_rust"])
    if error:
        return {"success": False, "error": f"rustc compile error: {error}"}

    rust_output, error = run_command(["./main_rust"])
    if error:
        return {"success": False, "error": f"rustc run error: {error}"}

    with open(asm_file_path, "r") as asm_file:
        assembly_code = asm_file.read()

    for f in [rust_file_path, asm_file_path, "main", "main_asm", "main_rust"]:
        try:
            os.remove(f)
        except OSError:
            pass

    return {
        "output": rusty_output,
        "rust_output": rust_output,
        "assembly": assembly_code,
        "success": True,
        "match": rusty_output == rust_output,
    }


@app.get("/run-tests")
async def run_tests() -> Dict[str, List[Dict[str, str]]]:
    """Run all test files in the input directory and compare outputs."""
    Path("input/ass").mkdir(parents=True, exist_ok=True)

    cpp_dir = Path("src")
    cpp_source_files = [str(file) for file in cpp_dir.rglob("*.cpp")]
    cpp_source_files.append("main.cpp")
    compile_cmd = ["g++", "-std=c++20"] + cpp_source_files + ["-o", "main"]

    _, error = run_command(compile_cmd)
    if error:
        return {"results": [], "error": f"Backend compile error: {error}"}

    results: List[Dict[str, str]] = []
    for file in Path("input").glob("*.rs"):
        # RUSTy writes assembly to a.s regardless of the input file
        asm_file = Path("a.s")
        test_result: Dict[str, str] = {"file": file.name}

        _, err = run_command(["./main", str(file)])
        if err:
            test_result["error"] = f"RUSTy execution error: {err}"
            results.append(test_result)
            continue

        _, err = run_command(["g++", str(asm_file), "-o", "main_asm"])
        if err:
            test_result["error"] = f"Asm compile error: {err}"
            results.append(test_result)
            continue

        rusty_out, err = run_command(["./main_asm"])
        if err:
            test_result["error"] = f"Asm run error: {err}"
            results.append(test_result)
            continue

        _, err = run_command(["rustc", str(file), "-o", "main_rust"])
        if err:
            test_result["error"] = f"rustc compile error: {err}"
            results.append(test_result)
            continue

        rust_out, err = run_command(["./main_rust"])
        if err:
            test_result["error"] = f"rustc run error: {err}"
            results.append(test_result)
            continue

        test_result["rust_output"] = rust_out
        test_result["rusty_output"] = rusty_out
        test_result["success"] = str(rust_out == rusty_out)
        results.append(test_result)

        for f in [asm_file, "main_asm", "main_rust"]:
            try:
                os.remove(f)
            except OSError:
                pass

    try:
        os.remove("main")
    except OSError:
        pass

    return {"results": results}