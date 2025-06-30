# Grammar

```
Program -> FunList

FunList -> Fun FunList'
FunList' -> Fun FunList'
FunList' -> Ɛ

Fun -> fn id ( ParamList ) Block

FunCall -> id ( ArgList )

ArgList = Ɛ
ArgList = Exp [, ArgList]

ParamList = Ɛ
ParamList = Param [, ParamList]
Param = type id

Block -> { StmtList }

StmtList -> Stmt StmtList
StmtList -> Stmt'
StmtList = Ɛ

Stmt -> let [mut] id [: type] [= Exp ] ;
Stmt -> id = Exp ;
Stmt -> for id in Exp .. Exp Block
Stmt -> for id in Exp ..= Exp Block
Stmt -> while Exp Block
Stmt -> println! ( Str [(, Exp)*] ) ;
Stmt -> break [Exp] [;]
Stmt -> return [Exp] [;]
Stmt -> Exp [;]

Exp -> NotExp
Exp -> NotExp || Exp
Exp -> NotExp && Exp

NotExp -> ! Exp
NotExp -> RelationalExp

RelationalExp -> ArithmeticExp > Exp
RelationalExp -> ArithmeticExp < Exp
RelationalExp -> ArithmeticExp >= Exp
RelationalExp -> ArithmeticExp <= Exp
RelationalExp -> ArithmeticExp == Exp
RelationalExp -> ArithmeticExp != Exp

ArithmeticExp -> TermExp
ArithmeticExp -> TermExp + Exp
ArithmeticExp -> TermExp - Exp

TermExp -> FactorExp
TermExp -> FactorExp * TermExp
TermExp -> FactorExp / TermExp

FactorExp -> ( Exp )
FactorExp -> number
FactorExp -> id
FactorExp -> FunCall ( [Exp (, Exp)*] )
FactorExp -> IfExp
FactorExp -> LoopExp

IfExp -> if Exp Block [(else if Block)*] [else Block]
LoopExp -> loop Block

```
