# 🛠️ How to Run the Compiler

This project uses **Docker** and **Docker Compose** to simplify setup and execution. You don’t need to install any dependencies manually — everything runs inside containers.

---

## ✅ Prerequisites

* [Docker](https://www.docker.com/products/docker-desktop/) installed on your system.
* [Docker Compose](https://docs.docker.com/compose/) (usually included with Docker Desktop).

---

## 🚀 Steps to Run the Compiler

1. **Clone the repository** (if you haven’t already):

   ```bash
   git clone https://github.com/your-username/your-repo.git
   cd your-repo
   ```

2. **Start the application using Docker Compose**:

   ```bash
   docker compose up
   ```

   > This command will build the backend and frontend images and start both services.

3. **Access the compiler frontend**:
   Open your browser and navigate to:
   👉 [http://localhost:3000](http://localhost:3000)

---

## 🧪 Notes

* The backend is available internally at `http://backend:8000` (used by the frontend inside Docker).
* Any code you compile through the frontend will be handled via this backend.
* To stop the application, press `Ctrl+C` and run:

  ```bash
  docker compose down
  ```

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
