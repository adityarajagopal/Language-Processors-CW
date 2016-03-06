%define parse.error verbose
%{
#include "src/c_parser.h"

int yylex();
int yyerror(const char* s);

File* root = NULL;

File::File(ExternalDecl* _external_decl, File* _file) : external_decl(_external_decl), file(_file) {}
void File::print()
{
	if(file != NULL)
	{
		file->print(); 
	}
	if(external_decl != NULL)
	{
		external_decl->print();
	}
}

ExternalDecl::ExternalDecl(FuncDef* _func_def, Decl* _decl) : func_def(_func_def), decl(_decl) {}
void ExternalDecl::print()
{
	if(decl != NULL)
	{
		decl->print();
	}
	if(func_def != NULL)
	{
		func_def->print(); 
	}
}

FuncDef::FuncDef(DeclSpec* _decl_spec, Declr* _declr, CompStat* _comp_stat) : decl_spec(_decl_spec), declr(_declr), comp_stat(_comp_stat) {}
void FuncDef::print()
{
	if(decl_spec != NULL)
	{
		decl_spec->print(); 
	}
	if(declr != NULL)
	{
		declr->print(); 
	}
	if(comp_stat != NULL)
	{
		std::cout << "{" << std::endl;
		comp_stat->print(); 
		std::cout << "}" << std::endl;
	}
}

Decl::Decl(DeclSpec* _decl_spec, InitList* _init_list) : decl_spec(_decl_spec), init_list(_init_list) {}
void Decl::print()
{
	if(decl_spec != NULL)
	{
		decl_spec->print();
	}
	if(init_list != NULL)
	{
		init_list->print(); 
	}
	std::cout << ";" << std::endl;
}

DeclSpec::DeclSpec(TypeSpec* _type_spec, DeclSpec* _decl_spec) : type_spec(_type_spec), decl_spec(_decl_spec) {}
void DeclSpec::print()
{
	if(type_spec != NULL)
	{
		type_spec->print();
	}
	if(decl_spec != NULL)
	{
		decl_spec->print();
	}
}

TypeSpec::TypeSpec(std::string i_type) : type(i_type) {}
void TypeSpec::print()
{
	std::cout << type << " ";
}

InitList::InitList(InitDeclr* _init_declr, InitList* _init_list) : init_declr(_init_declr), init_list(_init_list) {}
void InitList::print()
{
	if(init_declr != NULL)
	{
		init_declr->print();
	}
	if(init_list != NULL)
	{
		std::cout << ", ";
		init_list->print();
	}
}

InitDeclr::InitDeclr(Declr* _declr, InitVal* _init_val) : declr(_declr), init_val(_init_val) {}
void InitDeclr::print()
{
	if(declr != NULL) 
	{
		declr->print();
	}
	if(init_val != NULL)
	{
		std::cout << "= ";
		init_val->print(); 
	}
}

Declr::Declr(std::string _id, Declr* _declr, ParamList* _param_list) : id(_id), declr(_declr), param_list(_param_list) {}
void Declr::print()
{
	if(id != "")
	{
		std::cout << id << " "; 
	}
	if(declr != NULL)
	{
		declr->print();
	}
	if(param_list != NULL)
	{
		std::cout << "(";
		param_list->print(); 
		std::cout << ")" << std::endl;
	}
}

InitVal::InitVal(AssExpr* _ass_expr) : ass_expr(_ass_expr) {}
void InitVal::print()
{
	if(ass_expr != NULL)
	{
		ass_expr->print(); 
	}
}

ParamList::ParamList(ParamDecl* _param_decl, ParamList* _param_list) : param_decl(_param_decl), param_list(_param_list) {}
void ParamList::print()
{
	if(param_decl != NULL)
	{
		param_decl->print(); 
	}
	if(param_list != NULL)
	{
		param_list -> print(); 
	}
}

ParamDecl::ParamDecl(DeclSpec* _decl_spec, Declr* _declr) : decl_spec(_decl_spec), declr(_declr) {}
void ParamDecl::print() 
{
	if(decl_spec != NULL)
	{
		decl_spec->print(); 
	}
	if(declr != NULL)
	{
		declr->print(); 
	}
	std::cout << ", ";
}

AssExpr::AssExpr(CondExpr* _cond_expr, UnaryExpr* _unary_expr, std::string _ass_oper, AssExpr* _ass_expr) : cond_expr(_cond_expr), unary_expr(_unary_expr), ass_oper(_ass_oper), ass_expr(_ass_expr) {}
void AssExpr::print()
{
	if(cond_expr != NULL)
	{
		cond_expr->print(); 
	}
	if(unary_expr != NULL)
	{
		unary_expr->print();
	}
	if(ass_oper != "")
	{
		std::cout << ass_oper << " ";
	}
	if(ass_expr != NULL)
	{
		ass_expr->print();
	}
}

CondExpr::CondExpr(Expression* _expression) : expression(_expression) {}
void CondExpr::print()
{
	if(expression != NULL)
	{
		expression->print(); 
	}
}

PrimExpr::PrimExpr(std::string _value) : value(_value) {}
void PrimExpr::print()
{
	if(value != "")
	{
		std::cout << value << " ";
	}
}

CompStat::CompStat(StatList* _stat_list, DeclList* _decl_list) : stat_list(_stat_list), decl_list(_decl_list) {}
void CompStat::print()
{
	if(decl_list != NULL)
	{
		decl_list->print();
	}
	if(stat_list != NULL)
	{
		stat_list->print(); 
	}
}

DeclList::DeclList(Decl* _decl, DeclList* _decl_list) : decl(_decl), decl_list(_decl_list) {}
void DeclList::print()
{
	if(decl != NULL)
	{
		decl->print();
	}
	if(decl_list != NULL)
	{
		decl_list->print(); 
	}
}

StatList::StatList(Stat* _stat, StatList* _stat_list) : stat(_stat), stat_list(_stat_list) {}
void StatList::print()
{
	if(stat != NULL)
	{
		stat->print(); 
	}
	if(stat_list != NULL)
	{
		stat_list->print(); 
	}
}

Stat::Stat(CompStat* _comp_stat, ExprStat* _expr_stat, SelecStat* _selec_stat, LoopStat* _loop_stat, JumpStat* _jump_stat) : comp_stat(_comp_stat), expr_stat(_expr_stat), selec_stat(_selec_stat), loop_stat(_loop_stat), jump_stat(_jump_stat) {}
void Stat::print()
{
	if(comp_stat != NULL)
	{
		comp_stat->print(); 
	}
	if(expr_stat != NULL)
	{
		expr_stat->print(); 
	}/*
	if(selec_stat != NULL)
	{
		selec_stat->print(); 
	}
	if(loop_stat != NULL)
	{
		loop_stat->print(); 
	}
	if(jump_stat != NULL)
	{
		jump_stat->print(); 
	}*/
}

ExprStat::ExprStat(Expr* _expr) : expr(_expr) {}
void ExprStat::print()
{
	if(expr != NULL)
	{
		expr->print();
	}
}

Expression::Expression(Expression* _lhs, Expression* _rhs, std::string _op, UnaryExpr* _unary_expr) : lhs(_lhs), rhs(_rhs), op(_op), unary_expr(_unary_expr) {}
void Expression::print()
{
	if(op != "")
	{
		std::cout << op << " "; 
	}
	if(lhs != NULL)
	{
		lhs->print(); 
	}
	if(unary_expr != NULL)
	{
		unary_expr->print();
	}
	if(rhs != NULL)
	{
		rhs->print(); 
	}
}

UnaryExpr::UnaryExpr(PostFixExpr* _post_fix_expr, UnaryExpr* _unary_expr, std::string _unary_op) : post_fix_expr(_post_fix_expr), unary_expr(_unary_expr), unary_op(_unary_op) {}
void UnaryExpr::print()
{
	if(post_fix_expr != NULL)
	{
		post_fix_expr->print();
	}

	if(unary_op != "")
	{
		std::cout << unary_op << " ";
	}

	if(unary_expr != NULL)
	{
		unary_expr->print(); 
	}
}

PostFixExpr::PostFixExpr(PrimExpr* _prim_expr, PostFixExpr* _post_fix_expr, std::string _op) : prim_expr(_prim_expr), post_fix_expr(_post_fix_expr), op(_op) {}
void PostFixExpr::print()
{
	if(prim_expr != NULL)
	{
		prim_expr->print(); 
	}
	if(post_fix_expr != NULL)
	{
		post_fix_expr->print(); 
	}
	if(op != "")
	{
		std::cout << op << " ";
	}
}

Expr::Expr(AssExpr* _ass_expr, Expr* _expr) : ass_expr(_ass_expr), expr(_expr) {}
void Expr::print() 
{
	if(ass_expr != NULL)
	{
		ass_expr->print(); 
	}
	if(expr != NULL)
	{
		expr->print(); 
	}
}

%}

%union 
{
	char* string;
	int i_num; 
	float f_num;
	class Node* tree_node;
	class File* File;
	class ExternalDecl* Ext_Decl;
	class FuncDef* Func_Def; 
	class Decl* Decl;
	class DeclSpec* Decl_Spec;
	class TypeSpec* Type_Spec;
	class InitList* Init_List;
	class InitDeclr* Init_Declr;
	class Declr* Declr;
	class InitVal* Init_Val;
	class ParamList* Param_List;
	class ParamDecl* Param_Decl; 
	class Iden* Iden;
	class Expr* _Expr;
	class AssExpr* Ass_Expr;
	class PrimExpr* Prim_Expr;
	class CondExpr* Cond_Expr;
	class CompStat* Comp_Stat;
	class DeclList* Decl_List;
	class StatList* Stat_List;
	class Stat* Stat;
	class ExprStat* Expr_Stat;
	class Expression* Express;
	class UnaryExpr* Unary_Expr;
	class PostFixExpr* Postfix_Expr;
}

%token SEMICOLON COMMA LCURLY RCURLY LBRAC RBRAC
%token INT FLOAT DOUBLE BOOL
%token LONG UNSIGNED SIGNED CONST SHORT
%token VOID STRUCT UNION CHAR TYPEDEF VOLATILE STRING
%token IDENTIFIER INT_VAL FLOAT_VAL STRING_LIT
%token IF ELSE FOR WHILE
%token GOTO_KWD CONTINUE_KWD BREAK RETURN 
%token EQUALS MUL_EQUALS DIV_EQUALS MOD_EQUALS ADD_EQUALS SUB_EQUALS LEFT_EQUALS RIGHT_EQUALS AND_EQUALS OR_EQUALS XOR_EQUALS ADD SUB MULT DIV MOD
%token QUESTION_MARK COLON OR AND BW_OR BW_XOR BW_AND EQUAL_EQUAL NOT_EQUAL LT GT LE GE LEFT_SHIFT RIGHT_SHIFT INC DEC BW_NOT NOT
%token ENUM CHAR_KWD FLOAT_KWD DOUBLE_KWD AUTO EXTERN REGISTER STATIC DO SWITCH CASE SIZEOF DEFAULT TYPE
%type<tree_node> selection_statement loop_statement jump_statement do_statement
%type<string> IDENTIFIER EQUALS MUL_EQUALS DIV_EQUALS MOD_EQUALS ADD_EQUALS SUB_EQUALS LEFT_EQUALS RIGHT_EQUALS AND_EQUALS OR_EQUALS XOR_EQUALS QUESTION_MARK COLON assign_oper OR AND BW_OR BW_XOR BW_AND EQUAL_EQUAL NOT_EQUAL LT GT LE GE LEFT_SHIFT RIGHT_SHIFT ADD SUB MULT DIV MOD unary_oper INC DEC BW_NOT NOT TYPE CHAR STRING INT_VAL FLOAT_VAL

%type<File> file
%type<Ext_Decl> external_decl
%type<Func_Def> function_def
%type<Decl> decl 
%type<Decl_Spec> decl_specifiers
%type<Type_Spec> type_specifier
%type<Init_List> init_list
%type<Init_Declr> init_declarator
%type<Declr> declarator
%type<Init_Val> initial_val
%type<Param_List> param_list
%type<Param_Decl> param_decl
%type<Ass_Expr> assign_expr
%type<_Expr> expr
%type<Cond_Expr> conditional_expr
%type<Comp_Stat> compound_statement
%type<Decl_List> decl_list
%type<Stat_List> statement_list
%type<Stat> statement
%type<Expr_Stat> expr_statement
%type<Express> logical_or_expr logical_and_expr incl_or_expr excl_or_expr and_expr bool_equal_expr comparison_expr shift_expr addsub_expr multdivmod_expr 
%type<Unary_Expr> unary_expr
%type<Postfix_Expr> postfix_expr
%type<Prim_Expr> primary_expr
%% 

file			: external_decl {$$ = new File($1); root = $$;} 
	 			| file external_decl {$$ = new File($2, $1); root = $$;}
				;

external_decl	: function_def {$$ = new ExternalDecl($1);}
				| decl {$$ = new ExternalDecl(NULL, $1);}
				;

function_def	: decl_specifiers declarator compound_statement {$$ = new FuncDef($1,$2,$3);} 
				;

decl			: decl_specifiers SEMICOLON {$$ = new Decl($1);}
				| decl_specifiers init_list SEMICOLON {$$ = new Decl($1, $2);}
				;	

decl_specifiers	: type_specifier {$$ = new DeclSpec($1);}
				| type_specifier decl_specifiers {$$ = new DeclSpec($1, $2);} 
				;

decl_list		: decl {$$ = new DeclList($1);}
				| decl decl_list {$$ = new DeclList($1,$2);}
				;

type_specifier	: TYPE {$$ = new TypeSpec($1);}
				;

init_list		: init_declarator {$$ = new InitList($1); } 
				| init_declarator COMMA init_list {$$ = new InitList($1, $3);} 
				;

init_declarator	: declarator {$$ = new InitDeclr($1);} 
				| declarator EQUALS initial_val {$$ = new InitDeclr($1, $3);}
				;

statement_list	: statement {$$ = new StatList($1);}
				| statement statement_list {$$ = new StatList($1,$2);}
				

statement		: compound_statement {$$ = new Stat($1);} 
				| expr_statement {$$ = new Stat(NULL,$1);}
				| selection_statement {$$ = new Stat();}
				| loop_statement {$$ = new Stat();}
				| jump_statement {$$ = new Stat();}
				;

declarator		: IDENTIFIER {$$ = new Declr($1);} 
				| declarator LBRAC param_list RBRAC {$$ = new Declr("", $1, $3);} 
				| declarator LBRAC RBRAC {$$ = new Declr("", $1);}
				;

param_list		: param_decl {$$ = new ParamList($1);}
				| param_decl COMMA param_list {$$ = new ParamList($1, $3);}
				;

param_decl		: decl_specifiers declarator {$$ = new ParamDecl($1,$2);} 
				;

compound_statement	: LCURLY RCURLY {$$ = new CompStat();} 
					| LCURLY statement_list RCURLY {$$ = new CompStat($2);} 
					| LCURLY decl_list RCURLY {$$ = new CompStat(NULL,$2);} 
					| LCURLY decl_list statement_list RCURLY {$$ = new CompStat($3,$2);}
					;

initial_val		: assign_expr {$$ = new InitVal($1);}
				;

selection_statement : IF LBRAC expr RBRAC statement 
					| IF LBRAC expr RBRAC statement ELSE statement 
					;

loop_statement	: WHILE LBRAC expr RBRAC statement
				| do_statement
				| FOR LBRAC expr_statement expr_statement RBRAC statement 
				| FOR LBRAC expr_statement expr_statement expr RBRAC statement 
				;

do_statement 	: DO statement WHILE LBRAC expr RBRAC SEMICOLON 
				;

expr_statement 	: SEMICOLON {$$ = new ExprStat();}
				| expr SEMICOLON {$$ = new ExprStat($1);}
				;

jump_statement	: GOTO_KWD IDENTIFIER SEMICOLON 
				| RETURN SEMICOLON 
				| RETURN expr SEMICOLON 
				;

expr			: assign_expr {$$ = new Expr($1);}  
				| expr COMMA assign_expr {$$ = new Expr($3,$1);}
				;

assign_expr		: conditional_expr {$$ = new AssExpr($1);}
				| unary_expr assign_oper assign_expr {$$ = new AssExpr(NULL,$1,$2,$3);}  
				;

assign_oper		: EQUALS {}
				| MUL_EQUALS {}
				| DIV_EQUALS {}
				| MOD_EQUALS {}
				| ADD_EQUALS {}
				| SUB_EQUALS {}
				| LEFT_EQUALS {}
				| RIGHT_EQUALS {}
				| AND_EQUALS {}
				| XOR_EQUALS {}
				| OR_EQUALS {}
				;

conditional_expr : logical_or_expr {$$ = new CondExpr($1);}
			     | logical_or_expr QUESTION_MARK expr COLON conditional_expr
				 ;

logical_or_expr : logical_and_expr {$$ = new Expression($1);}
				| logical_or_expr OR logical_and_expr {$$ = new Expression($1,$3,"||");}
				;

logical_and_expr : incl_or_expr {$$ = new Expression($1);}
				 | logical_and_expr AND incl_or_expr{$$ = new Expression($1,$3,"&&");}
				 ;

incl_or_expr 	: excl_or_expr{$$ = new Expression($1);}
				| incl_or_expr BW_OR excl_or_expr{$$ = new Expression($1,$3,"|");}
				;

excl_or_expr	: and_expr{$$ = new Expression($1);}
				| excl_or_expr BW_XOR and_expr{$$ = new Expression($1,$3,"^");}
				;

and_expr		: bool_equal_expr{$$ = new Expression($1);}
				| and_expr BW_AND bool_equal_expr{$$ = new Expression($1,$3,"&");}
				;

bool_equal_expr : comparison_expr{$$ = new Expression($1);}
				| bool_equal_expr EQUAL_EQUAL comparison_expr{$$ = new Expression($1,$3,"==");}
				| bool_equal_expr NOT_EQUAL comparison_expr{$$ = new Expression($1,$3,"!=");}
				;

comparison_expr : shift_expr{$$ = new Expression($1);}
				| comparison_expr LT shift_expr{$$ = new Expression($1,$3,"<");}
				| comparison_expr GT shift_expr{$$ = new Expression($1,$3,">");}
				| comparison_expr LE shift_expr{$$ = new Expression($1,$3,"<=");}
				| comparison_expr GE shift_expr{$$ = new Expression($1,$3,">=");}
				;

shift_expr 		: addsub_expr{$$ = new Expression($1);}
				| shift_expr LEFT_SHIFT addsub_expr{$$ = new Expression($1,$3,"<<");}
				| shift_expr RIGHT_SHIFT addsub_expr{$$ = new Expression($1,$3,">>");}
				;

addsub_expr		: multdivmod_expr {$$ = new Expression($1);}
				| addsub_expr ADD multdivmod_expr{$$ = new Expression($1,$3,"+");}
				| addsub_expr SUB multdivmod_expr{$$ = new Expression($1,$3,"-");}
				;

multdivmod_expr : unary_expr {$$ = new Expression(NULL,NULL,"",$1);}
				| multdivmod_expr MULT unary_expr{$$ = new Expression($1,NULL,"*",$3);}
				| multdivmod_expr DIV unary_expr{$$ = new Expression($1,NULL,"/",$3);}
				| multdivmod_expr MOD unary_expr{$$ = new Expression($1,NULL,"%",$3);}
				;

unary_expr		: postfix_expr {$$ = new UnaryExpr($1);}
				| INC unary_expr {$$ = new UnaryExpr(NULL,$2,$1);}
				| DEC unary_expr {$$ = new UnaryExpr(NULL,$2,$1);}
				| unary_oper unary_expr {$$ = new UnaryExpr(NULL,$2,$1);} 
				;

unary_oper		: BW_AND
				| MULT
				| ADD 
				| SUB
				| BW_NOT
				| NOT
				;

postfix_expr	: primary_expr {$$ = new PostFixExpr($1);} 
				| postfix_expr INC {$$ = new PostFixExpr(NULL, $1, $2);}
				| postfix_expr DEC {$$ = new PostFixExpr(NULL, $1, $2);}
				;

primary_expr	: IDENTIFIER {$$ = new PrimExpr($1);} 
				| INT_VAL {$$ = new PrimExpr($1);} 
				| FLOAT_VAL {$$ = new PrimExpr($1);}  
				| CHAR {$$ = new PrimExpr($1);}  
				| STRING {$$ = new PrimExpr($1);}  
				| LBRAC expr RBRAC {}
				;
%%

int yyerror(const char* s)
{
	std::cout << s << std::endl;
	return -1;
}

int main() 
{
	yyparse();
	root->print();
}
