from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import subprocess
import tempfile
import os
from fastapi.middleware.cors import CORSMiddleware
from pathlib import Path

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

@app.post("/compile")
async def compile_rust(rust_code: RustCode):
    try:
        # -------------------------------
        # Step 1: Write Rust source to a temp file
        # -------------------------------
        Path("input").mkdir(parents=True, exist_ok=True)
        Path("input/ass").mkdir(parents=True, exist_ok=True)

        with tempfile.NamedTemporaryFile(suffix=".rs", delete=False) as rust_file:
            rust_file.write(rust_code.code.encode("utf-8"))
            rust_file_path = rust_file.name

        asm_file_path = os.path.join("input/ass", os.path.basename(rust_file_path).replace(".rs", ".s"))

        # -------------------------------
        # Step 2: Compile the C++ backend (RUSTy)
        # -------------------------------
        cpp_dir = Path("src")
        cpp_source_files = [str(file) for file in cpp_dir.rglob("*.cpp")]
        cpp_source_files.append("main.cpp")
        compile_cmd = ["g++", "-std=c++20"] + cpp_source_files + ["-o", "main"]

        print("[1] Compiling RUSTy backend...")
        rusty_compile = subprocess.run(compile_cmd, capture_output=True, text=True)

        if rusty_compile.returncode != 0:
            print("\033[91mError: Failed to compile RUSTy backend\033[0m")
            print(rusty_compile.stderr)
            raise HTTPException(status_code=500, detail="Failed to compile RUSTy backend")

        # -------------------------------
        # Step 3: Run RUSTy on the Rust file
        # -------------------------------
        print("[2] Running RUSTy...")
        rusty_result = subprocess.run(["./main", rust_file_path], capture_output=True, text=True)

        if rusty_result.returncode != 0:
            print("\033[91mError: RUSTy execution failed\033[0m")
            print(rusty_result.stderr)
            raise HTTPException(status_code=500, detail="RUSTy execution failed")

        print("\033[92m[2] RUSTy execution complete\033[0m")

        # -------------------------------
        # Step 4: Compile generated assembly
        # -------------------------------
        print("[3] Compiling generated assembly...")
        compile_asm_cmd = ["g++", asm_file_path, "-o", "main_asm"]
        asm_compile = subprocess.run(compile_asm_cmd, capture_output=True, text=True)

        if asm_compile.returncode != 0:
            print("\033[91mError: Assembly compilation failed\033[0m")
            print(asm_compile.stderr)
            raise HTTPException(status_code=500, detail="Assembly compilation failed")

        print("\033[92m[3] Assembly compilation complete\033[0m")

        # -------------------------------
        # Step 5: Run the compiled assembly
        # -------------------------------
        print("[4] Executing assembly...")
        asm_execution = subprocess.run(["./main_asm"], capture_output=True, text=True)

        if asm_execution.returncode != 0:
            print("\033[91mError: Assembly execution failed\033[0m")
            print(asm_execution.stderr)
            raise HTTPException(status_code=500, detail="Assembly execution failed")

        print("\033[92m[4] Assembly execution complete\033[0m")

        # -------------------------------
        # Step 6: Read assembly code for return
        # -------------------------------
        with open(asm_file_path, "r") as asm_file:
            assembly_code = asm_file.read()

        # -------------------------------
        # Step 7: Clean up temporary files
        # -------------------------------
        for f in [rust_file_path, asm_file_path, "main", "main_asm"]:
            try:
                os.remove(f)
            except OSError:
                pass  # Ignore if file was already removed

        # -------------------------------
        # Step 8: Return results
        # -------------------------------
        return {
            "output": asm_execution.stdout,
            "assembly": assembly_code,
            "success": True
        }

    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))