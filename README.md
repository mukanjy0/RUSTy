# Grammar

```text
Program → FunList

FunList → Fun FunList'
FunList' → Fun FunList'
FunList' → Ɛ

Fun → fn id ( ParamList ) Block

FunCall → id ( ArgList )

ArgList → Ɛ
ArgList → Exp [, ArgList]

ParamList → Ɛ
ParamList → Param [, ParamList]
Param → type id

Block → { StmtList }

StmtList → Stmt StmtList
StmtList → Stmt'
StmtList → Ɛ

Stmt → let [mut] id [: type] [= Exp ] ;
Stmt → id = Exp ;
Stmt → for id in Exp [..|..=] Exp Block
Stmt → while Exp Block
Stmt → println! ( Str [(, Exp)*] ) ;
Stmt → break [Exp] [;]
Stmt → return [Exp] [;]
Stmt → Exp [;]

Exp → NotExp
Exp → NotExp || Exp
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

TermExp → FactorExp
TermExp → FactorExp * FactorExp
TermExp → FactorExp / FactorExp

FactorExp → ( Exp )
FactorExp → number
FactorExp → id
FactorExp → FunCall ( [Exp (, Exp)*] )
FactorExp → if Exp Block [(else if Block)*] [else Block]
FactorExp → loop Block

```
