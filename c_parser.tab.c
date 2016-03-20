/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "src/c_parser.y" /* yacc.c:339  */

#include "src/c_parser.h"

#define TMP1 8
#define TMP2 9
#define TMP3 10

int yylex();
int yyerror(const char* s);

std::ofstream outfile;
File* root = NULL;
int offset = 0;
int num_arg = 0; 
int global_scope = 0; 
int arg_reg = 4;
int glbl_loop_num = 0;
int glbl_selec_num = 0;
int num_arguments = 0;
int is_func_dec = 0; 


std::map<Tag, int> OffsetMap;
std::map<std::string, std::vector<Tag> > VarTagMap;
std::map<std::string, int> FuncMap;
std::map<Tag, int> ReturnValMap;

std::vector<Tag> Arguments;

std::stringstream TUnit , Hdr, SetupFp, os, RestoreFp, Ftr, FuncInit;  

std::string set_offset()
{
	std::stringstream ss; 
	ss << "t_" << offset; 
	OffsetMap[ss.str()] = offset; 
	offset += 4; 
	return ss.str(); 
}

//Node::Node(int set) : tag(set_offset()) {}

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
//std::string File::set_offset() {return "";}
void File::generate_code() 
{
	if(external_decl != NULL) {external_decl->generate_code();}
	
	TUnit << Hdr.str() << SetupFp.str() << FuncInit.str() << os.str() << RestoreFp.str() << Ftr.str() << std::endl; 
	
	Hdr.str(""); 
	SetupFp.str("");
	os.str(""); 
	RestoreFp.str(""); 
	Ftr.str(""); 
	
	if(file != NULL) {file->generate_code();}
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
void ExternalDecl::generate_code()
{
	if(func_def != NULL){func_def->generate_code();}
	if(decl != NULL) {decl->generate_code();}
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
		std::cerr << "{" << std::endl;
		comp_stat->print(); 
		std::cerr << "}" << std::endl;
	}
}
void FuncDef::generate_code()
{
	FuncMap[declr->get_id()] = offset;

	if(comp_stat != NULL)
	{
		offset=0;
		comp_stat->get_max_arguments(offset);
		std::cerr << "function: " << declr->get_id() << std::endl; 
		std::cerr << "OFFSET: " << offset << std::endl; 
		if(offset < 4) {offset = 16;}
		else {offset = offset * 4;}
	}
	else
	{
		offset = 16;
	}


	Hdr << "\t" << ".align\t2" << std::endl; 
	Hdr << "\t" << ".globl\t" << declr->get_id();
	Hdr << std::endl;
	Hdr << "\t" << ".ent\t" << declr->get_id() << std::endl; 
	Hdr << "\t" << ".type\t" << declr->get_id(); 
	Hdr << ", @function" << std::endl; 
	Hdr << declr->get_id();  
	Hdr << ":" << std::endl;

	if(declr != NULL)
	{
		global_scope++;
		declr->generate_code(); 
		global_scope--;
	}
	
	if(comp_stat != NULL)
	{
		comp_stat->generate_code();
	}
	
	for(int i=0; i<Arguments.size(); i++)
	{
		if(i<4)
		{
			FuncInit << "\tsw\t$a" << i << "," << OffsetMap[Arguments[i]] << "($fp)" << std::endl; 
		}
		else
		{
			FuncInit << "\tlw\t$" << TMP1 << "," << offset+(i*4) << "($fp)" << std::endl; 
			FuncInit << "\tsw\t$" << TMP1 << "," << OffsetMap[Arguments[i]] << "($fp)" << std::endl; 	
		}
	}
	Arguments.clear(); 

	SetupFp << "\taddiu\t$sp,$sp,-" << offset+12 << std::endl;
	SetupFp << "\tsw\t$fp," << offset+4 << "($sp)" << std::endl;
	SetupFp << "\tsw\t$31," << offset+8 << "($sp)" << std::endl;
	SetupFp << "\tmove\t$fp,$sp" << std::endl;

	RestoreFp << "\tmove\t$sp,$fp" << std::endl; 
	RestoreFp << "\tlw\t$fp," << offset+4 << "($sp)" << std::endl; 
	RestoreFp << "\tlw\t$31," << offset+8 << "($sp)" << std::endl;
	RestoreFp << "\taddiu\t$sp,$sp," << offset+12 << std::endl; 
	RestoreFp << "\tj\t" << "$31" << std::endl;
	RestoreFp << "\tnop" << std::endl; 

	//Ftr << "\t" << ".cprestore\t16" << std::endl; 
	Ftr << "\t" << ".end\t" << declr->get_id(); 
	Ftr << std::endl; 
	Ftr << "\t" << ".size\t" << declr->get_id(); 
	Ftr << ", .-" << declr->get_id(); 
	Ftr << std::endl;
	
	offset = FuncMap[declr->get_id()];
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
	std::cerr << ";" << std::endl;
}
void Decl::generate_code()
{
	if(init_list != NULL)
	{
		init_list->generate_code();
		is_func_dec = 1;
	}
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
	std::cerr << type << " ";
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
		std::cerr << ", ";
		init_list->print();
	}
}
void InitList::generate_code()
{
	if(init_declr != NULL){init_declr->generate_code();}
	if(init_list != NULL){init_list->generate_code();}
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
		std::cerr << "= ";
		init_val->print(); 
	}
}
void InitDeclr::generate_code()
{
	if(declr != NULL){declr->generate_code();}
	if(init_val != NULL){init_val->generate_code();}

	std::string lhs_tag=""; 
	std::string rhs_tag="";
	if(init_val != NULL)
	{
		if(declr != NULL) declr->get_tag(lhs_tag); 
		init_val->get_tag(rhs_tag); 
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
	}
}

Declr::Declr(std::string _id, Declr* _declr, ParamList* _param_list, int _fd) : id(_id), declr(_declr), param_list(_param_list), func_dec(_fd) {}

void Declr::print()
{
	if(id != "")
	{
		std::cerr << id << " "; 
	}
	if(declr != NULL)
	{
		declr->print();
	}
	if(param_list != NULL)
	{
		std::cerr << "(";
		param_list->print(); 
		std::cerr << ")" << std::endl;
	}
}
void Declr::generate_code()
{
	if(param_list != NULL && is_func_dec == 0){param_list->generate_code();}
	else {is_func_dec = 0;}
	
	if(id != "")
	{
		tag = set_offset();
		VarTagMap[id].resize(global_scope + 1); 
		VarTagMap[id][global_scope] = tag;
	
		std::cerr << "TAG: " << tag << std::endl; 
		std::cerr << "ID: " << id << std::endl;
		std::cerr << "Global Scope: " << global_scope << std::endl;
		std::cerr << "vector: " << VarTagMap[id][global_scope] << std::endl; 
		std::cerr << std::endl; 
	}

	if(declr != NULL && func_dec == 0){declr->generate_code();}
}
std::string Declr::get_id()
{
	if(declr != NULL) {return declr->get_id();}	
	return id; 
}
void Declr::get_tag(std::string& _tag)
{
	if(id != "") {_tag = tag;}
	if(declr != NULL) {declr->get_tag(_tag);}
	if(param_list != NULL) {param_list->get_tag(_tag);}
}

InitVal::InitVal(AssExpr* _ass_expr) : ass_expr(_ass_expr) {}
void InitVal::print()
{
	if(ass_expr != NULL)
	{
		ass_expr->print(); 
	}
}
void InitVal::generate_code()
{
	if(ass_expr != NULL){ass_expr->generate_code();}
}
void InitVal::get_tag(std::string& _tag)
{
	if(ass_expr != NULL) {ass_expr->get_tag(_tag);}
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
void ParamList::generate_code()
{
	if(param_decl != NULL) {param_decl->generate_code();}
	if(param_list != NULL) {param_list->generate_code();}
}
void ParamList::get_tag(std::string& _tag)
{
	if(param_decl != NULL) {param_decl->get_tag(_tag);}
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
	std::cerr << ", ";
}
void ParamDecl::generate_code()
{
	if(declr != NULL) 
	{
		/*
		declr->generate_code();
		std::string d_tag; 
		declr->get_tag(d_tag); 
		os << "\tlw\t$" << TMP1 << "," << OffsetMap[d_tag] << "($fp)" << std::endl; 
		os << "move\t$" << TMP1 << ",$" << arg_reg << std::endl; 
		os << "\tsw\t$" << TMP1 << "," << OffsetMap[d_tag] << "($fp)" << std::endl; 
		
		arg_reg++; 
		*/
		declr->generate_code(); 
		std::string d_tag;
		declr->get_tag(d_tag);
		Arguments.push_back(d_tag); 
	}
}
void ParamDecl::get_tag(std::string& _tag)
{
	if(declr != NULL) {declr->get_tag(_tag);}
}

AssExpr::AssExpr(CondExpr* _cond_expr, UnaryExpr* _unary_expr, std::string _ass_oper, AssExpr* _ass_expr) : cond_expr(_cond_expr), unary_expr(_unary_expr), ass_oper(_ass_oper), ass_expr(_ass_expr) 
{
	/*if(ass_oper != "") {tag = set_offset();}*/
}
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
		std::cerr << ass_oper << " ";
	}
	if(ass_expr != NULL)
	{
		ass_expr->print();
	}
}
void AssExpr::generate_code()
{
	if(ass_oper != "") {tag = set_offset();}
	
	std::string lhs_tag="";
	std::string rhs_tag="";
	if(ass_oper != "")
	{	
		if(unary_expr != NULL) unary_expr->get_tag(lhs_tag);
		if(ass_expr != NULL) ass_expr->get_tag(rhs_tag);
		std::cerr << "TAG: " << tag << std::endl;
		std::cerr << "OP: " << ass_oper << std::endl; 
		std::cerr << "LHS: " << lhs_tag << std::endl; 
		std::cerr << "RHS: " << rhs_tag << std::endl;
		std::cerr << std::endl; 
	}
	
	if(cond_expr != NULL)
	{
		cond_expr->generate_code(); 
	}

	if(unary_expr != NULL)
	{
		unary_expr->generate_code();
	}
	
	if(ass_expr != NULL)
	{
		ass_expr->generate_code();
	}
	
	if(ass_oper == "+=")
	{
		lhs_tag="";
		rhs_tag="";
		unary_expr->get_tag(lhs_tag);
		ass_expr->get_tag(rhs_tag);
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\tadd" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}
	
	if(ass_oper == "=")
	{
		lhs_tag="";
		rhs_tag="";
		unary_expr->get_tag(lhs_tag);
		ass_expr->get_tag(rhs_tag);
		
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}
	
	
	else if(ass_oper == "-=")
	{
		lhs_tag="";
		rhs_tag="";
		unary_expr->get_tag(lhs_tag);
		ass_expr->get_tag(rhs_tag);
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\tsub" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl;
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}
	
	else if(ass_oper == "*=")
	{
		lhs_tag="";
		rhs_tag="";
		unary_expr->get_tag(lhs_tag);
		ass_expr->get_tag(rhs_tag);
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\tmul" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tnop" << std::endl; 
		os << "\tnop" << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl;
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}
	
	else if(ass_oper == "/=")
	{
		lhs_tag="";
		rhs_tag="";
		unary_expr->get_tag(lhs_tag);
		ass_expr->get_tag(rhs_tag);
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\tdiv" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tnop" << std::endl;
		os << "\tnop" << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl;
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	else if(ass_oper == "%=")
	{
		lhs_tag="";
		rhs_tag="";
		unary_expr->get_tag(lhs_tag);
		ass_expr->get_tag(rhs_tag);
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\trem" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tnop" << std::endl; 
		os << "\tnop" << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl;
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	else if(ass_oper == "<<=")
	{
		lhs_tag="";
		rhs_tag="";
		unary_expr->get_tag(lhs_tag);
		ass_expr->get_tag(rhs_tag);
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\tsllv" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl;
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	else if(ass_oper == ">>=")
	{
		lhs_tag="";
		rhs_tag="";
		unary_expr->get_tag(lhs_tag);
		ass_expr->get_tag(rhs_tag);
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\tsrav" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl;
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	else if(ass_oper == "&=")
	{
		lhs_tag="";
		rhs_tag="";
		unary_expr->get_tag(lhs_tag);
		ass_expr->get_tag(rhs_tag);
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\tand" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl;
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	else if(ass_oper == "^=")
	{
		lhs_tag="";
		rhs_tag="";
		unary_expr->get_tag(lhs_tag);
		ass_expr->get_tag(rhs_tag);
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\txor" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl;
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	else if(ass_oper == "|=")
	{
		lhs_tag="";
		rhs_tag="";
		unary_expr->get_tag(lhs_tag);
		ass_expr->get_tag(rhs_tag);
		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
		os << "\tor" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl;
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}
}
void AssExpr::get_tag(std::string& _tag)
{
	if(ass_oper != "") _tag = tag; 
	else if(unary_expr != NULL && _tag == "") unary_expr->get_tag(_tag); 
	else if(ass_expr != NULL && _tag == "") ass_expr->get_tag(_tag); 
	else if(cond_expr != NULL && _tag == "") cond_expr->get_tag(_tag);
}
void AssExpr::get_max_arguments(int& _offset)
{
	if(cond_expr != NULL) {cond_expr->get_max_arguments(_offset);}
	if(unary_expr != NULL) {unary_expr->get_max_arguments(_offset);}
	if(ass_expr != NULL) {ass_expr->get_max_arguments(_offset);}
}

CondExpr::CondExpr(Expression* _expression, IfElseExpr* _ie_expr) : expression(_expression), ie_expr(_ie_expr)
{
//	if(ie_expr != NULL) {tag = set_offset();}
}
void CondExpr::print()
{
	if(expression != NULL)
	{
		expression->print(); 
	}
	if(ie_expr != NULL)
	{
		ie_expr->print(); 
	}
}
void CondExpr::generate_code()
{
	//if(ie_expr != NULL) {tag = set_offset();}
	if(expression != NULL) {expression->generate_code();}
	if(ie_expr != NULL) {ie_expr->generate_code();}
}
void CondExpr::get_tag(std::string& _tag)
{
	if(ie_expr != NULL) {ie_expr->get_tag(_tag);} 
	else if(expression != NULL && _tag == "") expression->get_tag(_tag);
}
void CondExpr::get_max_arguments(int& _offset)
{
	if(expression != NULL) {expression->get_max_arguments(_offset);}	
}

PrimExpr::PrimExpr(std::string _value, int _flag, Expr* _e) : value(_value), expr(_e), flag(_flag) {}
void PrimExpr::print()
{
	if(value != "")
	{
		std::cerr << value << " ";
	}
	if(expr != NULL)
	{
		expr->print(); 
	}
}
void PrimExpr::generate_code() 
{
	int index = -1;
	if(value != "")
	{
		if(VarTagMap[value].empty())
		{
			std::cerr << "empty" << std::endl; 
			tag = set_offset();
			VarTagMap[value].resize(global_scope + 1); 
			VarTagMap[value][global_scope] = tag;
			index = global_scope;
		}
		else
		{
			std::cerr << "not empty" << std::endl;
			std::cerr << global_scope << std::endl; 
			std::cerr << VarTagMap[value].size() << std::endl;
			for(int i=global_scope; i>=0; i--)
			{
				if(i < VarTagMap[value].size())
				{
					if(!VarTagMap[value][i].empty())
					{
						tag = VarTagMap[value][i];
						index = i;
						break;
					}
				}
			}
			if(index == -1)
			{
				tag = set_offset(); 
				VarTagMap[value][global_scope] = tag; 
				index = global_scope; 
			}
		}
		std::cerr << "TAG = " << tag << std::endl; 
		std::cerr << "VAL = " << value << std::endl;
		std::cerr << global_scope << std::endl;
		std::cerr << std::endl; 
	}


	if (expr != NULL) 
	{
		expr->generate_code();
	}
	else
	{
		//os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[VarTagMap[value][global_scope]] << "($fp)" << std::endl; 
 		switch(flag)
		{
			case 1:
				os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[VarTagMap[value][index]] << "($fp)" << std::endl; 
				os << "\tli" << "\t$" << TMP1 << "," << std::stoi(value,NULL,10) << std::endl;
				os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[VarTagMap[value][index]] << "($fp)" << std::endl;
				break;
			case 2:
				os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[VarTagMap[value][index]] << "($fp)" << std::endl; 
				os << "\tli" << "\t$" << TMP1 << "," << std::stoi(value,NULL,8) << std::endl;  
				os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[VarTagMap[value][index]] << "($fp)" << std::endl;
				break;
			case 3: 
				os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[VarTagMap[value][index]] << "($fp)" << std::endl; 
				os << "\tli" << "\t$" << TMP1 << "," << std::stoi(value,NULL,16) << std::endl;  
				os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[VarTagMap[value][index]] << "($fp)" << std::endl;
				break;
			default:
				break;
		}
		//os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[VarTagMap[value][global_scope]] << "($fp)" << std::endl;
	}

}
void PrimExpr::get_tag(std::string& _tag)
{
	if(expr == NULL) {_tag = tag;}
	if(expr != NULL) {expr->get_tag(_tag);}
}
std::string PrimExpr::get_id()
{
	return value; 
}


CompStat::CompStat(StatList* _stat_list, DeclList* _decl_list) : stat_list(_stat_list), decl_list(_decl_list) 
{
	scope = global_scope;
}
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
void CompStat::generate_code()
{
	global_scope++;
	if(decl_list != NULL){decl_list->generate_code();}
	if(stat_list != NULL){stat_list->generate_code();}
	//resize all vectors everytime we exit a scope so that past variables in deepr scopes aren't kept
	for(std::map<Tag,std::vector<Tag> >::iterator it = VarTagMap.begin(); it != VarTagMap.end(); ++it)
	{
		it->second.resize(global_scope); 
	}
	global_scope--;
}
void CompStat::get_max_arguments(int& _offset)
{
	if(stat_list != NULL){stat_list->get_max_arguments(_offset);}
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
void DeclList::generate_code()
{
	if(decl != NULL){decl->generate_code();}
	if(decl_list != NULL){decl_list->generate_code();}
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
void StatList::generate_code()
{
	if(stat != NULL) {stat->generate_code();}
	if(stat_list != NULL) {stat_list->generate_code();}
}
void StatList::get_max_arguments(int& _offset)
{
	int current_scope_count = 0; 
	if(stat != NULL){stat->get_max_arguments(current_scope_count);}
	if(current_scope_count > _offset) {_offset = current_scope_count;}
	if(stat_list != NULL) {stat_list->get_max_arguments(_offset);}
}

/*
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
	}
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
	}
}
void Stat::generate_code()
{
	if(comp_stat != NULL) {comp_stat->generate_code();}
	if(expr_stat != NULL) {expr_stat->generate_code();}
}
*/

JumpStat::JumpStat(Expr* _expr, std::string _type) : expr(_expr), type(_type) {} 
void JumpStat::generate_code()
{
	if(type == "return")
	{	
		if(expr != NULL)
		{
			expr->generate_code();
			os << "\tlw\t"<< "$2,";
			std::string return_tag; 
			expr->get_tag(return_tag); 
			os << OffsetMap[return_tag] << "($fp)" << std::endl; 
		}
		else
		{
			os << "$0" << std::endl; 
		}
		
	}
}
void JumpStat::get_max_arguments(int& _offset) 
{
	if(expr != NULL) {expr->get_max_arguments(_offset);}	
}

ExprStat::ExprStat(Expr* _expr) : expr(_expr) {}
void ExprStat::print()
{
	if(expr != NULL)
	{
		expr->print();
	}
}
void ExprStat::generate_code()
{
	if(expr != NULL) {expr->generate_code();}
}
void ExprStat::get_tag(std::string& _tag)
{
	if(expr != NULL) {expr->get_tag(_tag);}
}
void ExprStat::get_max_arguments(int& _offset)
{
	if(expr != NULL){expr->get_max_arguments(_offset);}
}

Expression::Expression(Expression* _lhs, Expression* _rhs, std::string _op, UnaryExpr* _unary_expr) : lhs(_lhs), rhs(_rhs), op(_op), unary_expr(_unary_expr) {}
void Expression::get_tag(std::string& _tag)
{
	if(op != "") _tag = tag; 
	else if(lhs != NULL && _tag == "") lhs->get_tag(_tag); 
	else if(rhs != NULL && _tag == "") rhs->get_tag(_tag); 
	else if(unary_expr != NULL && _tag == "") unary_expr->get_tag(_tag);
}
void Expression::print()
{
	if(op != "")
	{
		std::cerr << op << " "; 
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
void Expression::generate_code()
{
	if(op !="") {tag = set_offset();}
	
	std::string tag_lhs="";
	std::string tag_rhs="";
	std::string tag_unary="";
	
	if(op != "")
	{
		std::cerr << "Tag = " << tag << std::endl; 
		std::cerr << "OP = " << op << std::endl;
		if(lhs != NULL) lhs->get_tag(tag_lhs); 
		if(rhs != NULL)	rhs->get_tag(tag_rhs); 
		std::cerr << "left = " << tag_lhs << std::endl; 
		std::cerr << "right = " << tag_rhs << std::endl; 
		std::cerr << "unary = " << tag_unary << std::endl; 
		std::cerr << std::endl; 
	}
	
	tag_lhs = "";
	tag_rhs = "";
	tag_unary = ""; 

	//std::stringstream ss; 
	if(rhs != NULL)
	{
		rhs->generate_code();
	}
	if(unary_expr != NULL)
	{
		std::cerr << "CALLING UNARY" << std::endl; 
		unary_expr->generate_code();
	}
	if(lhs != NULL)
	{
		lhs->generate_code();
	}
	
	if(op == "+")
	{
		if(lhs != NULL) lhs->get_tag(tag_lhs);
		if(rhs != NULL) rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tadd" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
		
	}

	if(op == "-")
	{
		if(lhs != NULL)	lhs->get_tag(tag_lhs);
		if(rhs != NULL)	rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tsub" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == "*")
	{
		if(lhs != NULL) lhs->get_tag(tag_lhs);
		if(unary_expr != NULL) unary_expr->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tmult" << "\t$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tmflo" << "\t$" << TMP1 << std::endl;
		os << "\tnop" << std::endl; 
		os << "\tnop" << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == "/")
	{
		if(lhs != NULL) lhs->get_tag(tag_lhs);
		if(unary_expr != NULL) unary_expr->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tdiv" << "\t$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tmflo" << "\t$" << TMP1 << std::endl;
		os << "\tnop" << std::endl; 
		os << "\tnop" << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}
	
	if(op == "%")
	{
		lhs->get_tag(tag_lhs);
		unary_expr->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tdiv" << "\t$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tmfhi" << "\t$" << TMP1 << std::endl;
		os << "\tnop" << std::endl; 
		os << "\tnop" << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == "<<")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tsllv" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == ">>")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tsrav" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == "<")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tslt" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}
	
	if(op == ">")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tsgt" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == "<=")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tsge" << "\t$" << TMP1 << ",$" << TMP2 << ",$" << TMP1 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == ">=")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tsge" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == "==")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tseq" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}
	
	if(op == "!=")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tsne" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}
	
	if(op == "&")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tand" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == "^")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\txor" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == "|")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tor" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl; 
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == "&&")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tsne" << "\t$" << TMP1 << ",$" << TMP1 << ",0" << std::endl; 
		os << "\tsne" << "\t$" << TMP2 << ",$" << TMP2 << ",0" << std::endl;
		os << "\tand" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl;
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}

	if(op == "||")
	{
		lhs->get_tag(tag_lhs);
		rhs->get_tag(tag_rhs);

		os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[tag_lhs] << "($fp)" << std::endl; 
		os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[tag_rhs] << "($fp)" << std::endl; 
		os << "\tsne" << "\t$" << TMP1 << ",$" << TMP1 << ",0" << std::endl; 
		os << "\tsne" << "\t$" << TMP2 << ",$" << TMP2 << ",0" << std::endl;
		os << "\tor" << "\t$" << TMP1 << ",$" << TMP1 << ",$" << TMP2 << std::endl;
		os << "\tsw" << "\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
	}
}
void Expression::get_max_arguments(int& _offset)
{
	if(lhs != NULL){lhs->get_max_arguments(_offset);}
	if(rhs != NULL){rhs->get_max_arguments(_offset);}
	if(unary_expr != NULL) {unary_expr->get_max_arguments(_offset);}
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
		std::cerr << unary_op << " ";
	}

	if(unary_expr != NULL)
	{
		unary_expr->print(); 
	}
}
void UnaryExpr::generate_code()
{
	if(post_fix_expr != NULL) {post_fix_expr->generate_code();}
	
	if(unary_expr != NULL)
	{
		tag = set_offset(); 
		std::cerr << "UNEXP: " << tag << std::endl; 
		unary_expr->generate_code(); 
		std::string rhs_tag; 
		unary_expr->get_tag(rhs_tag);

		if(unary_op == "++")
		{
			os << "\tlw\t$" << TMP1 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
			os << "\taddi\t$" << TMP1 << ",$" << TMP1 << ",1" << std::endl; 
			os << "\tsw\t$" << TMP1 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl;  
			os << "\tsw\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl;  
		}
		if(unary_op == "--")
		{
			os << "\tlw\t$" << TMP1 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl; 
			os << "\taddi\t$" << TMP1 << ",$" << TMP1 << ",-1" << std::endl; 
			os << "\tsw\t$" << TMP1 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl;  
			os << "\tsw\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl;  
		}
		if(unary_op == "-")
		{
			os << "\tlw\t$" << TMP1 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl;
			os << "\tneg\t$" << TMP1 << ",$" << TMP1 << std::endl;
			os << "\tsw\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
		}
		if(unary_op == "~")
		{
			os << "\tlw\t$" << TMP1 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl;
			os << "\tnot\t$" << TMP1 << ",$" << TMP1 << std::endl;
			os << "\tsw\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
		}
		if(unary_op == "!")
		{
			os << "\tlw\t$" << TMP1 << "," << OffsetMap[rhs_tag] << "($fp)" << std::endl;
			os << "\tmovn\t$" << TMP3 << ",$0,$" << TMP1 << std::endl;
			os << "\tli\t$" << TMP2 << ",1" << std::endl; 
			os << "\tmovz\t$" << TMP3 << ",$" << TMP2 << ",$" << TMP1 << std::endl; 
			os << "\tsw\t$" << TMP3 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
		}
	}
}
void UnaryExpr::get_tag(std::string& _tag)
{
	if(unary_expr != NULL) {_tag = tag;}
	if(post_fix_expr != NULL)
	{
		//if(_tag == "")
			post_fix_expr->get_tag(_tag); 
	}
}
void UnaryExpr::get_max_arguments(int& _offset)
{
	if(post_fix_expr != NULL) {post_fix_expr->get_max_arguments(_offset);}
	if(unary_expr != NULL) {unary_expr->get_max_arguments(_offset);}
}

PostFixExpr::PostFixExpr(PrimExpr* _prim_expr, PostFixExpr* _post_fix_expr, std::string _op, ArgList* _arg_list) : prim_expr(_prim_expr), post_fix_expr(_post_fix_expr), op(_op), arg_list(_arg_list) {}
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
		std::cerr << op << " ";
	}
}
void PostFixExpr::generate_code()
{
	if(prim_expr != NULL){prim_expr->generate_code();}
	if(post_fix_expr != NULL) {post_fix_expr->generate_code();}
	if(arg_list != NULL) 
	{
		tag = set_offset();
		
		num_arguments = 0; 
		arg_list->generate_code();

		os << "\tla\t$" << TMP1 << "," << post_fix_expr->get_id() << std::endl; 
		os << "\tjalr\t$" << TMP1 << std::endl; 
		os << "\tnop" << std::endl; 
		/*
		os << "\tjal\t" << post_fix_expr->get_id() << std::endl; 
		os << "\tnop" << std::endl; 
		*/
		os << "\tsw\t$2," << OffsetMap[tag] << "($fp)" << std::endl;  
	}
	if(op != "")
	{
		tag = set_offset();
		
		std::cerr << "PFEXPR: " << tag << std::endl; 

		std::string lhs_tag; 
		if(post_fix_expr != NULL) {post_fix_expr->get_tag(lhs_tag);}

		std::cerr << "TAG: " << tag << std::endl; 
		std::cerr << "OP: " << op << std::endl; 
		std::cerr << "LHS: " << lhs_tag << std::endl;
		std::cerr << std::endl;

		if(op == "++")
		{
			os << "\tlw\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
			os << "\tsw\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl;  
			os << "\taddi\t$" << TMP1 << ",$" << TMP1 << ",1" << std::endl; 
			os << "\tsw\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl;  
		}
		if(op == "--")
		{
			os << "\tlw\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl; 
			os << "\tsw\t$" << TMP1 << "," << OffsetMap[tag] << "($fp)" << std::endl;  
			os << "\taddi\t$" << TMP1 << ",$" << TMP1 << ",-1" << std::endl; 
			os << "\tsw\t$" << TMP1 << "," << OffsetMap[lhs_tag] << "($fp)" << std::endl;  
		}
	}
}
void PostFixExpr::get_tag(std::string& _tag)
{
	if(op != "") {_tag = tag;}
	if(prim_expr != NULL) {prim_expr->get_tag(_tag);}
	if(arg_list != NULL) {_tag = tag;}
}
void PostFixExpr::get_max_arguments(int& _offset)
{
	if(arg_list != NULL) {arg_list->get_max_arguments(_offset);}
}
std::string PostFixExpr::get_id() 
{
	if(post_fix_expr != NULL) {return post_fix_expr->get_id();}
	if(prim_expr != NULL) {return prim_expr->get_id();}
}

ArgList::ArgList(AssExpr* _ass_expr, ArgList* _arg_list) : ass_expr(_ass_expr), arg_list(_arg_list)
{
/*
	if(ass_expr != NULL)
	{
		num_arg++;
	}
	else if(ass_expr == NULL)
	{
		if(num_arg > offset)
		{
			offset = num_arg; 
		}
		num_arg=0; 
	}
*/
}
void ArgList::generate_code()
{
	if(arg_list != NULL){arg_list->generate_code();}
	if(ass_expr != NULL) 
	{
		ass_expr->generate_code(); 
		std::string ass_tag = ""; 
		ass_expr->get_tag(ass_tag);
		os << "\tlw\t$" << TMP1 << "," << OffsetMap[ass_tag] << "($fp)" << std::endl;
		if(num_arguments < 4)
		{
			os << "\tmove\t$a" << num_arguments << ",$" << TMP1 << std::endl; 
		}
		os << "\tsw\t$" << TMP1 << "," << num_arguments*4 << "($fp)" << std::endl;
		num_arguments++; 
	}
	
}
void ArgList::get_max_arguments(int& _offset)
{
	if(arg_list != NULL){arg_list->get_max_arguments(_offset);}
	if(ass_expr != NULL) {_offset++;}
}

Expr::Expr(AssExpr* _ass_expr, Expr* _expr) : ass_expr(_ass_expr), expr(_expr) {}
void Expr::print() 
{
	if(ass_expr != NULL)
	{
		ass_expr->print();
		std::cerr << std::endl;
	}
	if(expr != NULL)
	{
		expr->print(); 
	}
}
void Expr::generate_code()
{
	if(expr != NULL) {expr->generate_code();}
	if(ass_expr != NULL) {ass_expr->generate_code();}
}
void Expr::get_tag(std::string& _tag)
{
	if(ass_expr != NULL) ass_expr->get_tag(_tag);
	if(expr != NULL) expr->get_tag(_tag); 
}
void Expr::get_max_arguments(int& _offset)
{
	if(ass_expr != NULL) {ass_expr->get_max_arguments(_offset);}
	if(expr != NULL) {expr->get_max_arguments(_offset);}
}

LoopStat::LoopStat(ExprStat* _es1, ExprStat* _es2, Expr* _e, Stat* _s, DoStat* _ds) : expr_stat_1(_es1), expr_stat_2(_es2), expr(_e), stat(_s), do_stat(_ds) {}
void LoopStat::print()
{
	if(expr_stat_1 != NULL)
	{
		std::cerr << "for:" << std::endl;
		expr_stat_1->print(); 
	}
	if(expr_stat_2 != NULL)
	{
		expr_stat_2->print(); 
	}
	if(expr != NULL)
	{
		if(expr_stat_1 == NULL)
			std::cerr << "while:" << std::endl;
		expr->print(); 
		std::cerr << std::endl;
	}
	if(stat != NULL)
	{
		std::cerr << "\tbody:" << std::endl;
		stat->print(); 
	}
}
void LoopStat::generate_code()
{
	//increment loop number count
	int loop_num = glbl_loop_num; 
	//while loop
	if(expr_stat_1 == NULL)
	{
		if(expr != NULL)
		{
			loop_num++;
			glbl_loop_num++;
			os << "while_loop_" << loop_num << ":" << std::endl; 
			expr->generate_code(); 
			std::string tag_condition; 
			expr->get_tag(tag_condition); 
			os << "\tlw\t$" << TMP1 << "," << OffsetMap[tag_condition] << "($fp)" << std::endl; 
			os << "\tbeq\t$" << TMP1 << ",$0," << "end_loop_" << loop_num << std::endl;
			os << "\tnop" << std::endl; 
			if(stat != NULL) {stat->generate_code();}
			os << "\tb\t" << "while_loop_" << loop_num << std::endl; 
			os << "\tnop" << std::endl; 
			os << "end_loop_" << loop_num << ":" << std::endl; 
		}
	}
	//for loops
	else
	{
		loop_num++;
		glbl_loop_num++;
		expr_stat_1->generate_code();
		os << "for_loop_" << loop_num << ":" << std::endl; 
		if(expr_stat_2 != NULL) {expr_stat_2->generate_code();}
		std::string tag_condition = ""; 
		expr_stat_2->get_tag(tag_condition);
		os << "\tlw\t$" << TMP1 << "," << OffsetMap[tag_condition] << "($fp)" << std::endl; 
		os << "\tbeq\t$" << TMP1 << ",$0," << "end_loop_" << loop_num << std::endl;
		os << "\tnop" << std::endl;
		if(stat != NULL){stat->generate_code();}
		if(expr != NULL){expr->generate_code();}
		os << "\tb\t" << "for_loop_" << loop_num << std::endl;
		os << "\tnop" << std::endl; 
		os << "end_loop_" << loop_num << ":" << std::endl; 
	}
	//do while loop
	if(do_stat != NULL)
	{
		loop_num++;
		glbl_loop_num++;
		do_stat->generate_code();
	}
	loop_num--;
}
void LoopStat::get_max_arguments(int& _offset) {}

DoStat::DoStat(Stat* _stat, Expr* _expr) : stat(_stat), expr(_expr) {}
void DoStat::print() 
{
	if(stat != NULL)
	{
		std::cerr << "do:" << std::endl; 
		stat->print(); 
	}
	if(expr != NULL)
	{
		std::cerr << "while:" << std::endl;
		expr->print(); 
	}
}
void DoStat::generate_code()
{
	if(expr != NULL)
	{
		os << "dw_loop_" << glbl_loop_num << ":" << std::endl; 
		if(stat != NULL) {stat->generate_code();}
		expr->generate_code(); 
		std::string tag_condition; 
		expr->get_tag(tag_condition); 
		os << "\tlw\t$" << TMP1 << "," << OffsetMap[tag_condition] << "($fp)" << std::endl; 
		os << "\tbne\t$" << TMP1 << ",$0," << "dw_loop_" << glbl_loop_num << std::endl;
		os << "\tnop" << std::endl; 
		//os << "end_loop_" << glbl_loop_num << ":" << std::endl; 
	}
}

SelecStat::SelecStat(Expr* _e, Stat* _si, Stat* _se) : expr(_e), stat_if(_si), stat_else(_se) {}
void SelecStat::print() 
{
	if(expr != NULL)
	{
		std::cerr << "if: ";
		expr->print(); 
	}
	if(stat_if != NULL)
	{
		std::cerr << "\tbody:" << std::endl; 
		stat_if->print(); 
	}
	if(stat_else != NULL)
	{
		std::cerr << "else:" << std::endl;
		stat_else->print(); 
	}
}
void SelecStat::generate_code()
{
	int selec_num = glbl_selec_num; 
	if(stat_if != NULL)
	{
		selec_num++; 
		glbl_selec_num++;
		os << "if_" << selec_num << ":" << std::endl; 
		if(expr != NULL){expr->generate_code();}
		std::string tag_cond; 
		expr->get_tag(tag_cond);
		os << "\tlw\t$" << TMP1 << "," << OffsetMap[tag_cond] << "($fp)" << std::endl; 
		os << "\tbeq\t$" << TMP1 << ",$0," << "else_" << selec_num << std::endl;
		os << "\tnop" << std::endl;
		stat_if->generate_code();
		os << "\tb\t" << "if_out_" << selec_num << std::endl; 
		os << "else_" << selec_num << ":" << std::endl; 
	}
	if(stat_else != NULL)
	{
		//selec_num++;
		//glbl_selec_num++;
		stat_else->generate_code();
	}
	os << "if_out_" << selec_num << ":" << std::endl; 
	selec_num--;
}
void SelecStat::get_max_arguments(int& _offset) {}

IfElseExpr::IfElseExpr(Expression* _ic, Expr* _ie, CondExpr* _ee) : if_cond(_ic), if_expr(_ie), else_expr(_ee) {}
void IfElseExpr::print()
{
	if(if_cond != NULL)
	{
		std::cerr << "if: ";
		if_cond->print(); 
	}
	if(if_expr != NULL)
	{
		std::cerr << "\tbody: "; 
		if_expr->print(); 
	}
	if(else_expr != NULL)
	{
		std::cerr << "else: ";
		else_expr->print();
	}
}
void IfElseExpr::generate_code()
{
	tag = set_offset(); 

	std::string condition_tag = ""; 
	std::string true_tag = ""; 
	std::string false_tag = ""; 
	
	if(if_cond != NULL)
	{
		if_cond->generate_code();
		if_cond->get_tag(condition_tag);
	}
	if(if_expr != NULL)
	{
		if_expr->generate_code();
		if_expr->get_tag(true_tag);
	}
	if(else_expr != NULL)
	{
		else_expr->generate_code();
		else_expr->get_tag(false_tag);
	}

	os << "\tlw" << "\t$" << TMP1 << "," << OffsetMap[condition_tag] << "($fp)" << std::endl; 
	os << "\tlw" << "\t$" << TMP2 << "," << OffsetMap[true_tag] << "($fp)" << std::endl; 
	os << "\tlw" << "\t$" << TMP3 << "," << OffsetMap[false_tag] << "($fp)" << std::endl;
	//os << "\tmovn" << "\t$" << TMP2 << ",$" << TMP2 << ",$" << TMP1 << std::endl; 
	os << "\tmovz" << "\t$" << TMP2 << ",$" << TMP3 << ",$" << TMP1 << std::endl;
	os << "\tsw" << "\t$" << TMP2 << "," << OffsetMap[tag] << "($fp)" << std::endl; 
}
void IfElseExpr::get_tag(std::string& _tag)
{
	_tag = tag; 
}


#line 1699 "c_parser.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "c_parser.tab.h".  */
#ifndef YY_YY_C_PARSER_TAB_H_INCLUDED
# define YY_YY_C_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SEMICOLON = 258,
    COMMA = 259,
    LCURLY = 260,
    RCURLY = 261,
    LBRAC = 262,
    RBRAC = 263,
    INT = 264,
    FLOAT = 265,
    DOUBLE = 266,
    BOOL = 267,
    LONG = 268,
    UNSIGNED = 269,
    SIGNED = 270,
    CONST = 271,
    SHORT = 272,
    VOID = 273,
    STRUCT = 274,
    UNION = 275,
    CHAR = 276,
    TYPEDEF = 277,
    VOLATILE = 278,
    STRING = 279,
    IDENTIFIER = 280,
    INT_VAL = 281,
    FLOAT_VAL = 282,
    STRING_LIT = 283,
    OCT_VAL = 284,
    HEX_VAL = 285,
    IF = 286,
    ELSE = 287,
    FOR = 288,
    WHILE = 289,
    GOTO_KWD = 290,
    CONTINUE_KWD = 291,
    BREAK = 292,
    RETURN = 293,
    EQUALS = 294,
    MUL_EQUALS = 295,
    DIV_EQUALS = 296,
    MOD_EQUALS = 297,
    ADD_EQUALS = 298,
    SUB_EQUALS = 299,
    LEFT_EQUALS = 300,
    RIGHT_EQUALS = 301,
    AND_EQUALS = 302,
    OR_EQUALS = 303,
    XOR_EQUALS = 304,
    ADD = 305,
    SUB = 306,
    MULT = 307,
    DIV = 308,
    MOD = 309,
    QUESTION_MARK = 310,
    COLON = 311,
    OR = 312,
    AND = 313,
    BW_OR = 314,
    BW_XOR = 315,
    BW_AND = 316,
    EQUAL_EQUAL = 317,
    NOT_EQUAL = 318,
    LT = 319,
    GT = 320,
    LE = 321,
    GE = 322,
    LEFT_SHIFT = 323,
    RIGHT_SHIFT = 324,
    INC = 325,
    DEC = 326,
    BW_NOT = 327,
    NOT = 328,
    ENUM = 329,
    CHAR_KWD = 330,
    FLOAT_KWD = 331,
    DOUBLE_KWD = 332,
    AUTO = 333,
    EXTERN = 334,
    REGISTER = 335,
    STATIC = 336,
    DO = 337,
    SWITCH = 338,
    CASE = 339,
    SIZEOF = 340,
    DEFAULT = 341,
    TYPE = 342
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 1636 "src/c_parser.y" /* yacc.c:355  */

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
	class ArgList* Arg_List;
	class LoopStat* Loop_Stat;
	class DoStat* Do_Stat;
	class SelecStat* Selec_Stat;
	class IfElseExpr* IE_Expr;

#line 1864 "c_parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_C_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 1881 "c_parser.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   478

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  88
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  118
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  193

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   342

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1718,  1718,  1719,  1722,  1723,  1726,  1729,  1730,  1733,
    1734,  1737,  1738,  1741,  1744,  1745,  1748,  1749,  1752,  1753,
    1756,  1757,  1758,  1759,  1760,  1763,  1764,  1765,  1768,  1769,
    1772,  1775,  1776,  1777,  1778,  1781,  1784,  1785,  1788,  1789,
    1790,  1791,  1794,  1797,  1798,  1801,  1802,  1803,  1806,  1807,
    1810,  1811,  1814,  1815,  1816,  1817,  1818,  1819,  1820,  1821,
    1822,  1823,  1824,  1827,  1828,  1831,  1834,  1835,  1838,  1839,
    1842,  1843,  1846,  1847,  1850,  1851,  1854,  1855,  1856,  1859,
    1860,  1861,  1862,  1863,  1866,  1867,  1868,  1871,  1872,  1873,
    1876,  1877,  1878,  1879,  1882,  1883,  1884,  1885,  1888,  1889,
    1890,  1891,  1892,  1893,  1896,  1897,  1898,  1899,  1900,  1903,
    1904,  1907,  1908,  1909,  1910,  1911,  1912,  1913,  1914
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEMICOLON", "COMMA", "LCURLY", "RCURLY",
  "LBRAC", "RBRAC", "INT", "FLOAT", "DOUBLE", "BOOL", "LONG", "UNSIGNED",
  "SIGNED", "CONST", "SHORT", "VOID", "STRUCT", "UNION", "CHAR", "TYPEDEF",
  "VOLATILE", "STRING", "IDENTIFIER", "INT_VAL", "FLOAT_VAL", "STRING_LIT",
  "OCT_VAL", "HEX_VAL", "IF", "ELSE", "FOR", "WHILE", "GOTO_KWD",
  "CONTINUE_KWD", "BREAK", "RETURN", "EQUALS", "MUL_EQUALS", "DIV_EQUALS",
  "MOD_EQUALS", "ADD_EQUALS", "SUB_EQUALS", "LEFT_EQUALS", "RIGHT_EQUALS",
  "AND_EQUALS", "OR_EQUALS", "XOR_EQUALS", "ADD", "SUB", "MULT", "DIV",
  "MOD", "QUESTION_MARK", "COLON", "OR", "AND", "BW_OR", "BW_XOR",
  "BW_AND", "EQUAL_EQUAL", "NOT_EQUAL", "LT", "GT", "LE", "GE",
  "LEFT_SHIFT", "RIGHT_SHIFT", "INC", "DEC", "BW_NOT", "NOT", "ENUM",
  "CHAR_KWD", "FLOAT_KWD", "DOUBLE_KWD", "AUTO", "EXTERN", "REGISTER",
  "STATIC", "DO", "SWITCH", "CASE", "SIZEOF", "DEFAULT", "TYPE", "$accept",
  "file", "external_decl", "function_def", "decl", "decl_specifiers",
  "decl_list", "type_specifier", "init_list", "init_declarator",
  "statement_list", "statement", "declarator", "param_list", "param_decl",
  "compound_statement", "initial_val", "selection_statement",
  "loop_statement", "do_statement", "expr_statement", "jump_statement",
  "expr", "assign_expr", "assign_oper", "conditional_expr", "ie_expr",
  "logical_or_expr", "logical_and_expr", "incl_or_expr", "excl_or_expr",
  "and_expr", "bool_equal_expr", "comparison_expr", "shift_expr",
  "addsub_expr", "multdivmod_expr", "unary_expr", "unary_oper",
  "postfix_expr", "argument_list", "primary_expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342
};
# endif

#define YYPACT_NINF -81

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-81)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -69,   -81,    27,   -69,   -81,   -81,     8,   -69,   -81,   -81,
     -81,   -81,    40,    48,     2,   -81,   -81,    33,   139,    11,
     405,   -81,   -81,     1,   -81,   -81,   405,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   104,   106,   107,    90,   272,   -81,
     -81,   -81,   -81,   405,   405,   -81,   -81,   257,   -69,     8,
     198,   110,   257,   -81,   -81,   -81,   -81,   -81,   -81,    28,
     -81,   -81,   -81,     0,    59,    61,    58,    62,   -24,   -30,
      -5,    15,    -3,    60,   405,    -1,   -81,   -81,    33,   111,
     117,   -81,   -81,    12,   405,   328,   405,   119,   -81,    64,
     -81,   -81,    91,   -81,   -81,   118,   -81,   -81,   -81,   405,
     405,   405,   405,   405,   405,   405,   405,   405,   405,   405,
     405,   405,   405,   405,   405,   405,   405,   405,   405,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     405,   -81,   343,   -81,   -81,   121,   -81,   -69,   -81,    13,
     328,    20,   -81,   -81,   123,   -81,   -81,     6,    59,   -81,
      61,    58,    62,   -24,   -30,   -30,    -5,    -5,    -5,    -5,
      15,    15,    -3,    -3,   -81,   -81,   -81,   -81,   -81,   122,
     124,   -81,   257,   398,   257,   405,   405,   405,   -81,    99,
     257,    21,   -81,    22,   -81,   -81,   257,   -81,   257,   131,
     -81,   -81,   -81
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    13,     0,     2,     4,     5,     0,     9,     1,     3,
       7,    25,     0,    14,    16,    10,     8,     0,     0,     0,
       0,     6,    15,    16,    43,    31,     0,   116,   117,   111,
     112,   115,   113,   114,     0,     0,     0,     0,     0,   100,
     101,    99,    98,     0,     0,   102,   103,     0,    11,     0,
       0,     0,    18,    20,    22,    23,    39,    21,    24,     0,
      48,    50,    64,    63,    66,    68,    70,    72,    74,    76,
      79,    84,    87,    90,     0,    94,   104,    27,     0,     0,
      28,    17,    35,     0,     0,     0,     0,     0,    46,     0,
      95,    96,     0,    12,    33,     0,    32,    19,    44,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    62,    61,
       0,    97,     0,   107,   108,    30,    26,     0,   118,     0,
       0,     0,    45,    47,     0,    34,    49,     0,    67,    90,
      69,    71,    73,    75,    77,    78,    80,    81,    82,    83,
      85,    86,    88,    89,    91,    92,    93,    51,   105,   109,
       0,    29,     0,     0,     0,     0,     0,     0,   106,    36,
       0,     0,    38,     0,    65,   110,     0,    40,     0,     0,
      37,    41,    42
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -81,   133,   -81,   -81,    -6,    -4,    89,   -81,   130,   -81,
       4,   -45,    -2,     3,   -81,   125,   -81,   -81,   -81,   -81,
     -80,   -81,   -25,   -20,   -81,   -38,   -81,   -81,    50,    47,
      49,    52,    53,   -35,   -63,   -39,   -37,   -21,   -81,   -81,
     -23,   -81
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,     5,     6,    50,     7,    12,    13,
      51,    52,    23,    79,    80,    53,    81,    54,    55,    56,
      57,    58,    59,    60,   130,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
     170,    76
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      82,    83,    92,    15,    14,   140,   132,    18,    19,    19,
      99,    10,    48,    89,    49,    78,    99,    99,     1,    77,
     138,   172,    90,    91,    99,    99,    99,     8,   174,   188,
     189,    98,    99,    11,   108,   109,   110,   111,   106,   107,
      20,    20,    48,    16,    49,   156,   157,   158,   159,   116,
     117,   118,    17,   131,    95,   100,    97,   101,    11,   139,
     173,   141,   176,   112,   113,   114,   115,   143,    99,   133,
     134,   154,   155,   160,   161,   147,   135,   162,   163,   146,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   164,   165,   166,     1,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     167,    84,   169,    85,    86,    87,    96,   102,   104,   136,
     103,   137,   142,   105,   145,   144,   177,   179,    19,   182,
     175,   186,   178,    78,   192,   187,     9,    93,   184,    21,
     171,   190,    24,   191,    18,    25,    26,    22,   181,   150,
     183,   148,   151,     0,   185,   149,   152,   169,   153,     0,
      27,     0,     0,    28,    29,    30,    31,     0,    32,    33,
      34,     0,    35,    36,    37,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    39,
      40,    41,     0,     0,     0,     0,     0,     0,     0,     0,
      42,    24,     0,    18,    94,    26,     0,     0,     0,    43,
      44,    45,    46,     0,     0,     0,     0,     0,     0,    27,
       0,    47,    28,    29,    30,    31,     1,    32,    33,    34,
       0,    35,    36,    37,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
      41,     0,     0,     0,     0,     0,     0,     0,     0,    42,
      24,     0,    18,     0,    26,     0,     0,     0,    43,    44,
      45,    46,     0,     0,     0,    88,     0,     0,    27,    26,
      47,    28,    29,    30,    31,     0,    32,    33,    34,     0,
      35,    36,    37,    27,     0,    38,    28,    29,    30,    31,
       0,    32,    33,     0,     0,     0,     0,    39,    40,    41,
       0,     0,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,    39,    40,    41,     0,     0,    43,    44,    45,
      46,    24,     0,    42,     0,    26,     0,     0,     0,    47,
       0,     0,    43,    44,    45,    46,     0,     0,     0,    27,
      26,   168,    28,    29,    30,    31,     0,    32,    33,     0,
       0,     0,     0,     0,    27,     0,     0,    28,    29,    30,
      31,     0,    32,    33,     0,     0,     0,     0,    39,    40,
      41,     0,     0,     0,     0,     0,     0,     0,     0,    42,
       0,     0,     0,    39,    40,    41,     0,     0,    43,    44,
      45,    46,     0,     0,    42,    26,   180,     0,     0,     0,
       0,     0,    26,    43,    44,    45,    46,     0,     0,    27,
       0,     0,    28,    29,    30,    31,    27,    32,    33,    28,
      29,    30,    31,     0,    32,    33,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
      41,     0,     0,     0,     0,    39,    40,    41,     0,    42,
       0,     0,     0,     0,     0,     0,    42,     0,    43,    44,
      45,    46,     0,     0,     0,    43,    44,    45,    46
};

static const yytype_int16 yycheck[] =
{
      20,    26,    47,     7,     6,    85,     7,     5,     7,     7,
       4,     3,    18,    38,    18,    19,     4,     4,    87,     8,
       8,     8,    43,    44,     4,     4,     4,     0,     8,     8,
       8,     3,     4,    25,    64,    65,    66,    67,    62,    63,
      39,    39,    48,     3,    48,   108,   109,   110,   111,    52,
      53,    54,     4,    74,    50,    55,    52,    57,    25,    84,
     140,    86,    56,    68,    69,    50,    51,     3,     4,    70,
      71,   106,   107,   112,   113,   100,    78,   114,   115,    99,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    87,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
     130,     7,   132,     7,     7,    25,     6,    58,    60,     8,
      59,     4,     3,    61,     6,    34,     4,   172,     7,   174,
       7,    32,     8,   137,     3,   180,     3,    48,   176,    14,
     137,   186,     3,   188,     5,     6,     7,    17,   173,   102,
     175,   101,   103,    -1,   177,   176,   104,   177,   105,    -1,
      21,    -1,    -1,    24,    25,    26,    27,    -1,    29,    30,
      31,    -1,    33,    34,    35,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,     3,    -1,     5,     6,     7,    -1,    -1,    -1,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    82,    24,    25,    26,    27,    87,    29,    30,    31,
      -1,    33,    34,    35,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
       3,    -1,     5,    -1,     7,    -1,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    -1,     3,    -1,    -1,    21,     7,
      82,    24,    25,    26,    27,    -1,    29,    30,    31,    -1,
      33,    34,    35,    21,    -1,    38,    24,    25,    26,    27,
      -1,    29,    30,    -1,    -1,    -1,    -1,    50,    51,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    50,    51,    52,    -1,    -1,    70,    71,    72,
      73,     3,    -1,    61,    -1,     7,    -1,    -1,    -1,    82,
      -1,    -1,    70,    71,    72,    73,    -1,    -1,    -1,    21,
       7,     8,    24,    25,    26,    27,    -1,    29,    30,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    24,    25,    26,
      27,    -1,    29,    30,    -1,    -1,    -1,    -1,    50,    51,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    50,    51,    52,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    61,     7,     8,    -1,    -1,    -1,
      -1,    -1,     7,    70,    71,    72,    73,    -1,    -1,    21,
      -1,    -1,    24,    25,    26,    27,    21,    29,    30,    24,
      25,    26,    27,    -1,    29,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    -1,    -1,    -1,    -1,    50,    51,    52,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    70,    71,
      72,    73,    -1,    -1,    -1,    70,    71,    72,    73
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    87,    89,    90,    91,    92,    93,    95,     0,    89,
       3,    25,    96,    97,   100,    93,     3,     4,     5,     7,
      39,   103,    96,   100,     3,     6,     7,    21,    24,    25,
      26,    27,    29,    30,    31,    33,    34,    35,    38,    50,
      51,    52,    61,    70,    71,    72,    73,    82,    92,    93,
      94,    98,    99,   103,   105,   106,   107,   108,   109,   110,
     111,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   129,     8,    93,   101,
     102,   104,   111,   110,     7,     7,     7,    25,     3,   110,
     125,   125,    99,    94,     6,    98,     6,    98,     3,     4,
      55,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    50,    51,    52,    53,    54,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
     112,   125,     7,    70,    71,   100,     8,     4,     8,   110,
     108,   110,     3,     3,    34,     6,   111,   110,   116,   125,
     117,   118,   119,   120,   121,   121,   122,   122,   122,   122,
     123,   123,   124,   124,   125,   125,   125,   111,     8,   111,
     128,   101,     8,   108,     8,     7,    56,     4,     8,    99,
       8,   110,    99,   110,   113,   128,    32,    99,     8,     8,
      99,    99,     3
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    88,    89,    89,    90,    90,    91,    92,    92,    93,
      93,    94,    94,    95,    96,    96,    97,    97,    98,    98,
      99,    99,    99,    99,    99,   100,   100,   100,   101,   101,
     102,   103,   103,   103,   103,   104,   105,   105,   106,   106,
     106,   106,   107,   108,   108,   109,   109,   109,   110,   110,
     111,   111,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   113,   113,   114,   115,   115,   116,   116,
     117,   117,   118,   118,   119,   119,   120,   120,   120,   121,
     121,   121,   121,   121,   122,   122,   122,   123,   123,   123,
     124,   124,   124,   124,   125,   125,   125,   125,   126,   126,
     126,   126,   126,   126,   127,   127,   127,   127,   127,   128,
     128,   129,   129,   129,   129,   129,   129,   129,   129
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     3,     2,     3,     1,
       2,     1,     2,     1,     1,     3,     1,     3,     1,     2,
       1,     1,     1,     1,     1,     1,     4,     3,     1,     3,
       2,     2,     3,     3,     4,     1,     5,     7,     5,     1,
       6,     7,     7,     1,     2,     3,     2,     3,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     5,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     1,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     2,     2,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 1718 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.File) = new File((yyvsp[0].Ext_Decl)); root = (yyval.File);}
#line 3176 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 3:
#line 1719 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.File) = new File((yyvsp[-1].Ext_Decl), (yyvsp[0].File)); root = (yyval.File);}
#line 3182 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 4:
#line 1722 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Ext_Decl) = new ExternalDecl((yyvsp[0].Func_Def));}
#line 3188 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 5:
#line 1723 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Ext_Decl) = new ExternalDecl(NULL, (yyvsp[0].Decl));}
#line 3194 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 6:
#line 1726 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Func_Def) = new FuncDef((yyvsp[-2].Decl_Spec),(yyvsp[-1].Declr),(yyvsp[0].Comp_Stat));}
#line 3200 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 7:
#line 1729 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Decl) = new Decl((yyvsp[-1].Decl_Spec));}
#line 3206 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 8:
#line 1730 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Decl) = new Decl((yyvsp[-2].Decl_Spec), (yyvsp[-1].Init_List));}
#line 3212 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 9:
#line 1733 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Decl_Spec) = new DeclSpec((yyvsp[0].Type_Spec));}
#line 3218 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 10:
#line 1734 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Decl_Spec) = new DeclSpec((yyvsp[-1].Type_Spec), (yyvsp[0].Decl_Spec));}
#line 3224 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 11:
#line 1737 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Decl_List) = new DeclList((yyvsp[0].Decl));}
#line 3230 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 12:
#line 1738 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Decl_List) = new DeclList((yyvsp[-1].Decl),(yyvsp[0].Decl_List));}
#line 3236 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 13:
#line 1741 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Type_Spec) = new TypeSpec((yyvsp[0].string));}
#line 3242 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 14:
#line 1744 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Init_List) = new InitList((yyvsp[0].Init_Declr)); }
#line 3248 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 15:
#line 1745 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Init_List) = new InitList((yyvsp[-2].Init_Declr), (yyvsp[0].Init_List));}
#line 3254 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 16:
#line 1748 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Init_Declr) = new InitDeclr((yyvsp[0].Declr));}
#line 3260 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 17:
#line 1749 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Init_Declr) = new InitDeclr((yyvsp[-2].Declr), (yyvsp[0].Init_Val));}
#line 3266 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 18:
#line 1752 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Stat_List) = new StatList((yyvsp[0].Stat));}
#line 3272 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 19:
#line 1753 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Stat_List) = new StatList((yyvsp[-1].Stat),(yyvsp[0].Stat_List));}
#line 3278 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 20:
#line 1756 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3284 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 21:
#line 1757 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3290 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 22:
#line 1758 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3296 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 23:
#line 1759 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3302 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 24:
#line 1760 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3308 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 25:
#line 1763 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Declr) = new Declr((yyvsp[0].string));}
#line 3314 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 26:
#line 1764 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Declr) = new Declr("", (yyvsp[-3].Declr), (yyvsp[-1].Param_List), 1);}
#line 3320 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 27:
#line 1765 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Declr) = new Declr("", (yyvsp[-2].Declr), NULL, 1);}
#line 3326 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 28:
#line 1768 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Param_List) = new ParamList((yyvsp[0].Param_Decl));}
#line 3332 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 29:
#line 1769 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Param_List) = new ParamList((yyvsp[-2].Param_Decl), (yyvsp[0].Param_List));}
#line 3338 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 30:
#line 1772 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Param_Decl) = new ParamDecl((yyvsp[-1].Decl_Spec),(yyvsp[0].Declr));}
#line 3344 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 31:
#line 1775 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Comp_Stat) = new CompStat();}
#line 3350 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 32:
#line 1776 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Comp_Stat) = new CompStat((yyvsp[-1].Stat_List));}
#line 3356 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 33:
#line 1777 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Comp_Stat) = new CompStat(NULL,(yyvsp[-1].Decl_List));}
#line 3362 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 34:
#line 1778 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Comp_Stat) = new CompStat((yyvsp[-1].Stat_List),(yyvsp[-2].Decl_List));}
#line 3368 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 35:
#line 1781 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Init_Val) = new InitVal((yyvsp[0].Ass_Expr));}
#line 3374 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 36:
#line 1784 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Selec_Stat) = new SelecStat((yyvsp[-2]._Expr),(yyvsp[0].Stat));}
#line 3380 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 37:
#line 1785 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Selec_Stat) = new SelecStat((yyvsp[-4]._Expr),(yyvsp[-2].Stat),(yyvsp[0].Stat));}
#line 3386 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 38:
#line 1788 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Loop_Stat) = new LoopStat(NULL,NULL,(yyvsp[-2]._Expr),(yyvsp[0].Stat));}
#line 3392 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 39:
#line 1789 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Loop_Stat) = new LoopStat(NULL,NULL,NULL,NULL,(yyvsp[0].Do_Stat));}
#line 3398 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 40:
#line 1790 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Loop_Stat) = new LoopStat((yyvsp[-3].Expr_Stat),(yyvsp[-2].Expr_Stat),NULL,(yyvsp[0].Stat));}
#line 3404 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 41:
#line 1791 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Loop_Stat) = new LoopStat((yyvsp[-4].Expr_Stat),(yyvsp[-3].Expr_Stat),(yyvsp[-2]._Expr),(yyvsp[0].Stat));}
#line 3410 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 42:
#line 1794 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Do_Stat) = new DoStat((yyvsp[-5].Stat),(yyvsp[-2]._Expr));}
#line 3416 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 43:
#line 1797 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Expr_Stat) = new ExprStat();}
#line 3422 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 44:
#line 1798 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Expr_Stat) = new ExprStat((yyvsp[-1]._Expr));}
#line 3428 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 45:
#line 1801 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3434 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 46:
#line 1802 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.tree_node) = new JumpStat(NULL, "return");}
#line 3440 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 47:
#line 1803 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.tree_node) = new JumpStat((yyvsp[-1]._Expr), "return");}
#line 3446 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 48:
#line 1806 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval._Expr) = new Expr((yyvsp[0].Ass_Expr));}
#line 3452 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 49:
#line 1807 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval._Expr) = new Expr((yyvsp[0].Ass_Expr),(yyvsp[-2]._Expr));}
#line 3458 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 50:
#line 1810 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Ass_Expr) = new AssExpr((yyvsp[0].Cond_Expr));}
#line 3464 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 51:
#line 1811 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Ass_Expr) = new AssExpr(NULL,(yyvsp[-2].Unary_Expr),(yyvsp[-1].string),(yyvsp[0].Ass_Expr));}
#line 3470 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 52:
#line 1814 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3476 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 53:
#line 1815 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3482 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 54:
#line 1816 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3488 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 55:
#line 1817 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3494 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 56:
#line 1818 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3500 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 57:
#line 1819 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3506 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 58:
#line 1820 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3512 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 59:
#line 1821 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3518 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 60:
#line 1822 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3524 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 61:
#line 1823 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3530 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 62:
#line 1824 "src/c_parser.y" /* yacc.c:1661  */
    {}
#line 3536 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 63:
#line 1827 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Cond_Expr) = new CondExpr((yyvsp[0].Express));}
#line 3542 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 64:
#line 1828 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Cond_Expr) = new CondExpr(NULL, (yyvsp[0].IE_Expr));}
#line 3548 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 65:
#line 1831 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.IE_Expr) = new IfElseExpr((yyvsp[-4].Express),(yyvsp[-2]._Expr),(yyvsp[0].Cond_Expr));}
#line 3554 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 66:
#line 1834 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[0].Express));}
#line 3560 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 67:
#line 1835 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),"||");}
#line 3566 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 68:
#line 1838 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[0].Express));}
#line 3572 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 69:
#line 1839 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),"&&");}
#line 3578 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 70:
#line 1842 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[0].Express));}
#line 3584 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 71:
#line 1843 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[0].Express),(yyvsp[-2].Express),"|");}
#line 3590 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 72:
#line 1846 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[0].Express));}
#line 3596 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 73:
#line 1847 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),"^");}
#line 3602 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 74:
#line 1850 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[0].Express));}
#line 3608 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 75:
#line 1851 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),"&");}
#line 3614 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 76:
#line 1854 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[0].Express));}
#line 3620 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 77:
#line 1855 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),"==");}
#line 3626 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 78:
#line 1856 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),"!=");}
#line 3632 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 79:
#line 1859 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[0].Express));}
#line 3638 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 80:
#line 1860 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),"<");}
#line 3644 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 81:
#line 1861 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),">");}
#line 3650 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 82:
#line 1862 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),"<=");}
#line 3656 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 83:
#line 1863 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),">=");}
#line 3662 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 84:
#line 1866 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[0].Express));}
#line 3668 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 85:
#line 1867 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),"<<");}
#line 3674 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 86:
#line 1868 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),">>");}
#line 3680 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 87:
#line 1871 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[0].Express));}
#line 3686 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 88:
#line 1872 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),"+");}
#line 3692 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 89:
#line 1873 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),(yyvsp[0].Express),"-");}
#line 3698 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 90:
#line 1876 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression(NULL,NULL,"",(yyvsp[0].Unary_Expr));}
#line 3704 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 91:
#line 1877 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),NULL,"*",(yyvsp[0].Unary_Expr));}
#line 3710 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 92:
#line 1878 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),NULL,"/",(yyvsp[0].Unary_Expr));}
#line 3716 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 93:
#line 1879 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Express) = new Expression((yyvsp[-2].Express),NULL,"%",(yyvsp[0].Unary_Expr));}
#line 3722 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 94:
#line 1882 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Unary_Expr) = new UnaryExpr((yyvsp[0].Postfix_Expr));}
#line 3728 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 95:
#line 1883 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Unary_Expr) = new UnaryExpr(NULL,(yyvsp[0].Unary_Expr),(yyvsp[-1].string));}
#line 3734 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 96:
#line 1884 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Unary_Expr) = new UnaryExpr(NULL,(yyvsp[0].Unary_Expr),(yyvsp[-1].string));}
#line 3740 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 97:
#line 1885 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Unary_Expr) = new UnaryExpr(NULL,(yyvsp[0].Unary_Expr),(yyvsp[-1].string));}
#line 3746 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 104:
#line 1896 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Postfix_Expr) = new PostFixExpr((yyvsp[0].Prim_Expr));}
#line 3752 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 105:
#line 1897 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Postfix_Expr) = new PostFixExpr(NULL,(yyvsp[-2].Postfix_Expr));}
#line 3758 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 106:
#line 1898 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Postfix_Expr) = new PostFixExpr(NULL,(yyvsp[-3].Postfix_Expr),"",(yyvsp[-1].Arg_List));}
#line 3764 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 107:
#line 1899 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Postfix_Expr) = new PostFixExpr(NULL, (yyvsp[-1].Postfix_Expr), (yyvsp[0].string));}
#line 3770 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 108:
#line 1900 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Postfix_Expr) = new PostFixExpr(NULL, (yyvsp[-1].Postfix_Expr), (yyvsp[0].string));}
#line 3776 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 109:
#line 1903 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Arg_List) = new ArgList((yyvsp[0].Ass_Expr));}
#line 3782 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 110:
#line 1904 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Arg_List) = new ArgList((yyvsp[-2].Ass_Expr),(yyvsp[0].Arg_List));}
#line 3788 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 111:
#line 1907 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Prim_Expr) = new PrimExpr((yyvsp[0].string),0);}
#line 3794 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 112:
#line 1908 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Prim_Expr) = new PrimExpr((yyvsp[0].string),1);}
#line 3800 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 113:
#line 1909 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Prim_Expr) = new PrimExpr((yyvsp[0].string),2);}
#line 3806 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 114:
#line 1910 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Prim_Expr) = new PrimExpr((yyvsp[0].string),3);}
#line 3812 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 115:
#line 1911 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Prim_Expr) = new PrimExpr((yyvsp[0].string),4);}
#line 3818 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 116:
#line 1912 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Prim_Expr) = new PrimExpr((yyvsp[0].string),5);}
#line 3824 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 117:
#line 1913 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Prim_Expr) = new PrimExpr((yyvsp[0].string),6);}
#line 3830 "c_parser.tab.c" /* yacc.c:1661  */
    break;

  case 118:
#line 1914 "src/c_parser.y" /* yacc.c:1661  */
    {(yyval.Prim_Expr) = new PrimExpr("",-1,(yyvsp[-1]._Expr));}
#line 3836 "c_parser.tab.c" /* yacc.c:1661  */
    break;


#line 3840 "c_parser.tab.c" /* yacc.c:1661  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1916 "src/c_parser.y" /* yacc.c:1906  */


int yyerror(const char* s)
{
	std::cerr << s << std::endl;
	return -1;
}

int main() 
{
	yyparse();
	root->generate_code(); 
	std::cout << "\t.text\n" << TUnit.str(); 
}
