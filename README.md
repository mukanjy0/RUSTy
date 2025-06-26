# Grammar

```
Program -> FunDecList

FunList -> Fun FunList'
FunList' -> Fun FunList'
FunList' -> Ɛ

Fun -> fn id ( ParamList ) Block

ParamList = Ɛ
ParamList = Param [, ParamList]
Param = type id

Block -> { StmtList }

StmtList -> Stmt StmtList
StmtList -> Stmt'
StmtList = Ɛ

Stmt -> Stmt' ;
Stmt' -> let [mut] id [: type] [= Stmt']
Stmt' -> id = AExp
Stmt' -> if AExp Block [(else if Block)*] [else Block]
Stmt' -> println! ( Str [, (id)*] )
Stmt' -> return [AExp] [;]
Stmt' -> AExp

AExp -> BExp
AExp -> BExp || AExp
AExp -> BExp && AExp

BExp -> [!] CExp

CExp -> Exp > Exp
CExp -> Exp < Exp
CExp -> Exp >= Exp
CExp -> Exp <= Exp
CExp -> Exp == Exp
CExp -> Exp != Exp
```
