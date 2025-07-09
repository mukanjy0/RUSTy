# RUSTy Compiler

RUSTy is a small compiler for a subset of the Rust programming language. The core of the compiler is written in C++ and performs lexical analysis, parsing, semantic checks and assembly code generation. A Python service exposes the compiler through a simple API and a Next.js web application provides a user interface.

---

## Project structure

- `src/` – implementation of the compiler (lexical, syntactic and semantic stages).
- `main.cpp` – command line entry that takes a `.rs` file and produces `a.s` assembly.
- `server.py` – FastAPI server that builds the compiler on startup and exposes `/compile`, `/run` and `/run_rustc` endpoints.
- `ui/rusty` – Next.js frontend to interact with the server.
- `input/` – sample Rust programs used for testing.
- `make.py` – script that compares the output of RUSTy with the official `rustc` compiler.
- `makefile` and `CMakeLists.txt` – build configuration for the C++ sources.
- `requirements.txt` – Python dependencies for the API service.
- `shell.nix` – Nix development environment.

---

## Building the compiler

You can build the compiler using `make`:

```bash
make
```

This produces a `rusty` executable in the repository root. Alternatively, CMake can be used:

```bash
cmake -B build
cmake --build build
```

---

## Running tests

The `make.py` script compiles each file in `input/` with both `rustc` and the RUSTy compiler and shows any differences in the output.

```bash
python make.py
```

---

## API server

Install the Python requirements and start the FastAPI service:

```bash
pip install -r requirements.txt
python server.py
```

The server builds the compiler if necessary and listens on `http://localhost:8000`.

---

## Web interface

The frontend lives in `ui/rusty`.

```bash
cd ui/rusty
npm install
npm run dev
```

After starting the development server, open [http://localhost:3000](http://localhost:3000) to access the UI. If you run the frontend without Docker, create a `.env.local` file inside `ui/rusty` with `NEXT_PUBLIC_API_URL=http://localhost:8000` so the application knows where to send requests.

---

# Grammar

```text
Program → FunList

FunList → Fun FunList'
FunList' → Fun FunList'
FunList' → Ɛ

Fun → fn id '(' ParamList ')' [-> type] Block

FunCall → id '(' ArgList ')'

ArgList → Ɛ
ArgList → Exp [, ArgList]

ParamList → Ɛ
ParamList → Param [, ParamList]
Param → id : type

Block → { StmtList [; return [Exp]] }

StmtList → Stmt StmtList
StmtList → Stmt'
StmtList → Ɛ

// Declaration
Stmt → let [mut] id [= Rhs] ;
Stmt → let [mut] id : [&] type [= Exp] ;
Stmt → let [mut] id : '[' [&] type ; Exp ']' [= Rhs] ;
// Assignment
Stmt → [&] id = Rhs ;
Stmt → id (+-*/)= Exp ;
Stmt → SubscriptExp = Exp ;
Stmt → SubscriptExp (+-*/)= Exp ;
// Auxiliary
Rhs → Exp | ('[' Exp ; Exp ']') | ('[' [Exp (, Exp)*] ']')
// Flow control
Stmt → for id in Exp [..|..=] Exp Block
Stmt → while Exp Block
Stmt → break [Exp] [;]
Stmt → return ;
Stmt → return Exp ;
// Macro
Stmt → println! '(' str [(, Exp)*] ')' ;
// Exp
Stmt → Exp [;]

Exp → NotExp
Exp → NotExp '|''|' Exp
Exp → NotExp && Exp

// not operators can be chained
NotExp → ! NotExp
NotExp → RelationalExp

// comparison operators cannot be chained
RelationalExp → ArithmeticExp > ArithmeticExp
RelationalExp → ArithmeticExp < ArithmeticExp 
RelationalExp → ArithmeticExp >= ArithmeticExp 
RelationalExp → ArithmeticExp <= ArithmeticExp
RelationalExp → ArithmeticExp == ArithmeticExp 
RelationalExp → ArithmeticExp != ArithmeticExp 

ArithmeticExp → TermExp
ArithmeticExp → TermExp + ArithmeticExp
ArithmeticExp → TermExp - ArithmeticExp

TermExp → ReferenceFactorExp
TermExp → ReferenceFactorExp * ReferenceFactorExp
TermExp → ReferenceFactorExp / ReferenceFactorExp

ReferenceFactorExp → [&] FactorExp

FactorExp → '(' Exp ')'
FactorExp → Literal
FactorExp → id
FactorExp → FunCall ( [Exp (, Exp)*] )
FactorExp → if Exp Block [(else if Block)*] [else Block]
FactorExp → loop Block
FactorExp → SubscriptExp
FactorExp → SliceExp

SubscriptExp → id [ Exp ]
SliceExp → id '[' ([Exp] (..|..=) [Exp] ) ']'

Literal -> number
Literal -> boolean
Literal -> char
Literal -> str
Literal -> ()

```
