import subprocess
import tempfile
from pathlib import Path
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

COMPILER_PATH = (Path(__file__).resolve().parent / "rusty").resolve()

class CodeRequest(BaseModel):
    code: str


def build_compiler():
    if COMPILER_PATH.exists():
        return
    src_dir = Path("src")
    sources = [str(p) for p in src_dir.rglob("*.cpp")]
    sources.append("main.cpp")
    compile_cmd = ["g++", "-std=c++20", *sources, "-o", str(COMPILER_PATH)]
    result = subprocess.run(compile_cmd, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(f"Compiler build failed: {result.stderr}")


@app.on_event("startup")
def startup_event():
    build_compiler()


@app.post("/compile")
def compile_code(req: CodeRequest):
    with tempfile.TemporaryDirectory() as tmpdir:
        src_file = Path(tmpdir) / "input.rs"
        src_file.write_text(req.code)
        result = subprocess.run([str(COMPILER_PATH), str(src_file)], capture_output=True, text=True)
        asm_path = Path("a.s")
        asm_text = asm_path.read_text() if asm_path.exists() else ""
        if asm_path.exists():
            asm_path.unlink()
        if result.returncode != 0:
            raise HTTPException(status_code=400, detail=result.stderr or "Compilation failed")
        return {"assembly": asm_text, "compiler_output": result.stdout}


@app.post("/run")
def run_code(req: CodeRequest):
    with tempfile.TemporaryDirectory() as tmpdir:
        src_file = Path(tmpdir) / "main.rs"
        exe_file = Path(tmpdir) / "program"
        src_file.write_text(req.code)
        compile_cmd = ["rustc", str(src_file), "-o", str(exe_file)]
        result = subprocess.run(compile_cmd, capture_output=True, text=True)
        if result.returncode != 0:
            raise HTTPException(status_code=400, detail=result.stderr or "Rust compilation failed")
        run_result = subprocess.run([str(exe_file)], capture_output=True, text=True)
        if run_result.returncode != 0:
            raise HTTPException(status_code=400, detail=run_result.stderr or "Program execution failed")
        return {"output": run_result.stdout}

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)

