#ifndef __ASS6_14CS30006_TRANSLATOR_H
#define __ASS6_14CS30006_TRANSLATOR_H
#include <stack>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <cstring>
#include <string>
#include <vector>
using namespace std;


#define MAX 100000
//enum for Op_codes for Quad array.
enum opcodeType{
	//binary operators.
	PLUS = 1,
	
	LOGICAL_AND,
	IS_EQUAL,
	NOT_EQUAL,
	LESS_EQUAL,
	LOGICAL_OR,
	DIVIDE,
	AND,
	MODULO,
	SHIFT_LEFT,
	MINUS,
	MULT,
	SHIFT_RIGHT,
	XOR,
	OR,
	LESS,
	GREATER,
	
	GREATER_EQUAL,

	//unary operators.
	UNARY_PLUS,
	UNARY_MINUS,
	COMPLEMENT,
	NOT,

	//Conditional jump operators.
	IF_LESS,
	IF_GREATER,
	IF_LESS_EQUAL,
	IF_GREATER_EQUAL,
	IF_IS_EQUAL,
	IF_NOT_EQUAL,
	IF_EXPRESSION,
	IF_NOT_EXPRESSION,
	//Unconditional jump.
	GOTO,

	//assignment operator.
	COPY,

	//Array Access and Dereferencing.
	ARRAY_ACCESS,
	ARRAY_DEREFERENCE,

	//Procedure Call.
	CALL,
	PARAM,

	//Return Value
	RETURN_VOID,
	RETURN,

	//Address and Pointer Assignment Instructions.
	REFERENCE,
	DEREFERENCE,
	POINTER_ASSIGNMENT,

	//Function opcodes.
	_FUNCTION_START,
	_FUNCTION_END,

	//Inc and Dec operators.
	_INCREMENT,
	_DECREMENT,

	//type conversion
	C2I,
	C2D,
	I2C,
	D2C,
	I2D,
	D2I

	
};

class Quad;
class exp_attr;
class Quad_Array;
class Quad
{
public:
	opcodeType op;
	string arg_1, arg_2, result;
	void print();
};

class Quad_Array
{
public:
	//create an array of Quads
	vector<Quad> arr;
	//global pointer to maintain quad array index
	int index;
	//constructor, initialises array of quads to 0
	Quad_Array(){ index = 0; }

	//overloaded emits
	void emit(opcodeType op, string result, string arg_1, string arg_2 = 0);
	void emit(opcodeType op, string result, int num);
	void emit(opcodeType op, string result, double double_num);
	void emit(opcodeType op, string result, char char_const);

	void convD2I(exp_attr *e1, exp_attr *e2);
	void convD2C(exp_attr *e1, exp_attr *e2);
	void convI2D(exp_attr *e1, exp_attr *e2);
	void convI2C(exp_attr *e1, exp_attr *e2);
	void convC2I(exp_attr *e1, exp_attr *e2);
	void convC2D(exp_attr *e1, exp_attr *e2);
	
	//function responsible for backpatch
	void backpatch(list<int> bp_list, int i);
	void convInt2Bool(exp_attr *e);
	

	
};

//struct that updates the type information for each new type 
typedef struct type_inf
{
	string type_name;
	int size;
	int pointer_count;
	string pointer_type;
	string array_type;
	int no_of_params;
	int idx;
	vector<int> array_list;
	struct type_inf *next;
}type_inf;



typedef union init{
	int int_val;
	double double_val;
	char char_val;
	//string string_lit;
}init_value;


class symboltable;
//structure defines the fields of each row of the symtable
typedef struct symtab
{
	string id;
	type_inf type;
	int num_of_params;
	init_value *init_val;
	int size;
	int offset;
	int pointer_count;
	symboltable *nested_table;
}symtab;


//full symbol table
class symboltable
{
public:
	symtab sym_table[MAX];
	int no_of_entries;
	int offset;

	symboltable() 
	{
		for(int i=0; i<MAX; i++)
			sym_table[i].nested_table = NULL;
		no_of_entries = 0;
		offset = 0;
	}

	symtab* lookup(string s, string type_nm = "int" , int pc = 0);
	string gentemp(type_inf type);
	void print();
	//void update(symtab* sym);
};




//structure for maintaining function parameter definitions
typedef struct func_param_def
{
	string param_name;
	type_inf *type;
}func_def;

//structure fo maintaining list of parameters
typedef struct func_param_list
{
	list<func_def*> func_def_list;
}func_list;

//expression attribute for each non-terminal expression and statements
class exp_attr
{
public:
	string loc;
	list<int> truelist, falselist, nextlist;
	int instr;
	type_inf type;
	bool is_pointer_type;
	int dim ;
	string *inner;
	exp_attr()
	{
		dim = 0;
		inner = NULL;
		nextlist = list<int>();
	}
	int is_array_id;
};

class declaration
{
public:
	exp_attr *init;
	string dec_name;
	type_inf *type;
	func_list *param_list;
	vector<int> alist;
	int pointer_count;
};


typedef struct init_decl_list
{
	list<declaration*> dec_list;
}init_dec_list;

list<int> makelist(int index);
list<int> merge(list<int> a, list<int> b);


class Codegen
{
public:
	string func_name;
	string emptystr;
	string globalstrstart;
	int stack_size;
	int flag_param;
	int membindebp;
	map<int, string> gen_goto;
	int g_t_c;
	symtab *r;
	symboltable *newSym;
	Quad cur_q;
	Quad next_q;
	stack<vector<string> > stack_of_parameters;
	int no_of_params;
	int mem;
	int stack_req;
	int func_type;
	int func_type_r;
	int ptr_type;
	int array_param ;
	//string func_name;
	Codegen()
	{
		emptystr = "";
		func_name = "";
		stack_size = 0;
		globalstrstart = ".LC";
		r = NULL;
		newSym = NULL;
		g_t_c = 0;
		membindebp = 0;
		mem = 16;
		no_of_params = 0;
		func_type = 0;
		func_type_r = 0;
		ptr_type = 0;
		array_param = 0;

	}
	void create_Function_Prologue();
	void create_Function_Epilogue();
	void h_G();
	void Map_TAC_to_Assembly();
	void handle_strings();
	void set_GOTO_Labels_target();
	
	string generate_GOTO_Labels();
	void Bind_mem_Record();
	
};

 
#endif /* ASS6_14CS30006_TRANSLATOR_H */
