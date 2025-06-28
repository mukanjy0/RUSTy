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
Stmt -> break [Exp] ;
Stmt -> return [Exp] [;]
Stmt -> Exp [;]

Exp -> IfExp
Exp -> LoopExp
Exp -> LogicalExp

IfExp -> if Exp Block [(else if Block)*] [else Block]
LoopExp -> loop Block

LogicalExp -> LogicalExp
LogicalExp -> LogicalExp || NotExp
LogicalExp -> LogicalExp && NotExp

NotExp -> ! NotExp
NotExp -> RelationalExp

RelationalExp -> ArithmeticExp > ArithmeticExp
RelationalExp -> ArithmeticExp < ArithmeticExp
RelationalExp -> ArithmeticExp >= ArithmeticExp
RelationalExp -> ArithmeticExp <= ArithmeticExp
RelationalExp -> ArithmeticExp == ArithmeticExp
RelationalExp -> ArithmeticExp != ArithmeticExp

ArithmeticExp -> TermExp
ArithmeticExp -> TermExp + ArithmeticExp
ArithmeticExp -> TermExp - ArithmeticExp

TermExp -> FactorExp
TermExp -> FactorExp * TermExp
TermExp -> FactorExp / TermExp

FactorExp -> ( Exp )
FactorExp -> number
FactorExp -> id
FactorExp -> FunCall ( Exp )

```
