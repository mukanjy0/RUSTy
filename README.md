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

Block → { StmtList }

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
Stmt → return [Exp] [;]
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
