#include "ast.hh"
#include <stdlib.h>

template class Number_Ast<double>;
template class Number_Ast<int>;
int Ast::labelCounter;

//////// AST

Ast::Ast(){}

Ast::~Ast(){}

bool Ast::is_value_zero()
{
	return NULL;
}

Symbol_Table_Entry & Ast::get_symbol_entry()
{
	Symbol_Table_Entry s;
	return s;
}

Data_Type Ast::get_data_type()
{
	return node_data_type;
}

void Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Ast::check_ast()
{
	return true;
}

//////// ASSIGNMENT AST

Assignment_Ast::~Assignment_Ast(){}

Assignment_Ast::Assignment_Ast(Ast* temp_lhs, Ast* temp_rhs, int line)
{
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = temp_lhs->get_data_type();
	lhs = temp_lhs;
	rhs = temp_rhs;
}

void Assignment_Ast::print(ostream &file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "Asgn:\n";
	file_buffer << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

bool Assignment_Ast::check_ast()
{
	if (lhs->get_data_type() == rhs->get_data_type()){
		return true;
	}
	else{
		cerr << "cs316: Error: Line: " << lineno << ": Assignment statement data type not compatible\n";
		return false;
	}
}

///////// NAME AST

Name_Ast::~Name_Ast(){}

Name_Ast::Name_Ast(string &name, Symbol_Table_Entry &var_entry, int line)
{
	lineno = line;
	ast_num_child = unary_arity;
	node_data_type = var_entry.get_data_type();
	variable_symbol_entry = &var_entry;
	if (name != variable_symbol_entry->get_variable_name()){
		cerr << "cs316: Error: Line: " << lineno << ": Variable's symbol entry is not matching its name\n";
		exit(1); 
	}
}

void Name_Ast::print(ostream &file_buffer)
{
	file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}

Data_Type Name_Ast::get_data_type()
{
	return node_data_type;
}

void Name_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

Symbol_Table_Entry& Name_Ast::get_symbol_entry()
{
	return *variable_symbol_entry;
}

///////// NUMBER AST

template<typename T>
Number_Ast<T>::~Number_Ast(){}

template<typename T>
Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line)
{
	lineno = line;
	ast_num_child = unary_arity;
	node_data_type = constant_data_type;
	constant = number;
}

template<typename T>
void Number_Ast<T>::print(ostream &file_buffer)
{
	file_buffer << "Num : " << constant;
}

template<typename T>
Data_Type Number_Ast<T>::get_data_type()
{
	return node_data_type;
}

template<typename T>
void Number_Ast<T>::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

template<typename T>
bool Number_Ast<T>::is_value_zero()
{
	return constant == 0;
}

///////// ARITHMETIC EXPRESSION AST

Data_Type Arithmetic_Expr_Ast::get_data_type()
{
	return node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast()
{
	if(ast_num_child == 2)
	{
		if(lhs->get_data_type() != rhs->get_data_type())
		{
			cerr << "cs316: Error: Line: " << lineno << ": Arithmetic statement data type not compatible\n";
			return false;
		}
	}
	return true;
}

///////// PLUS AST

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
}

void Plus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Arith: PLUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

///////// MINUS AST

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
}

void Minus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Arith: MINUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

///////// DIVIDE AST

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
}

void Divide_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Arith: DIV\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

///////// MULTIPLY AST

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
}

void Mult_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Arith: MULT\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

///////// UNARY MINUS AST

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = unary_arity;
	node_data_type = l->get_data_type();
}

void UMinus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Arith: UMINUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";
}

///////// CONDITIONAL EXPRESSION AST

Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* cond1, Ast* l, Ast* r, int line)
{
	cond = cond1;
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = ternary_arity;
	node_data_type = l->get_data_type();
	labelCounter++;
}

void Conditional_Expression_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "Cond:\n";
	file_buffer << AST_NODE_SPACE << "IF_ELSE";
	cond->print(file_buffer);
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

///////// RELATIONAL EXPRESSION AST

Relational_Expr_Ast::Relational_Expr_Ast(Ast* lhs, Relational_Op rop, Ast * rhs, int line)
{
	lhs_condition = lhs;
	rhs_condition = rhs;
	rel_op = rop;
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = lhs_condition->get_data_type();
}

Data_Type Relational_Expr_Ast::get_data_type()
{
	return node_data_type;
}

void Relational_Expr_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast()
{
	if(lhs_condition->get_data_type() != rhs_condition->get_data_type())
	{
		cerr << "cs316: Error: Line: " << lineno << ": Relational expression data type not compatible\n";
		return false;
	}
	return true;
}

void Relational_Expr_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Condition: ";
	if (rel_op == less_equalto) {
		file_buffer << "LE";
	}
	else if (rel_op == less_than) {
		file_buffer << "LT";
	}
	else if (rel_op == greater_than) {
		file_buffer << "GT";
	}
	else if (rel_op == greater_equalto) {
		file_buffer << "GE";
	}
	else if (rel_op == equalto) {
		file_buffer << "EQ";
	}
	else if (rel_op == not_equalto) {
		file_buffer << "NE";
	}
	file_buffer << "\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs_condition->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs_condition->print(file_buffer);
	file_buffer << ")";
}

///////// LOGICAL EXPRESSION AST

Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line)
{
	lhs_op = lhs;
	rhs_op = rhs;
	bool_op = bop;
	lineno = line;
	if (bop == _logical_not) {
		ast_num_child = unary_arity;
	}
	else {
		ast_num_child = binary_arity;
	}
	node_data_type = void_data_type;
}

Data_Type Logical_Expr_Ast::get_data_type()
{
	return node_data_type;
}

void Logical_Expr_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Logical_Expr_Ast::check_ast()
{
	if (lhs_op->get_data_type() == double_data_type){
		cerr << "cs316: Error: Line: " << lineno << ": Logical expression cannot handle this\n";
		exit(1);
		return false;
	}
	return true;
}

void Logical_Expr_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Condition: ";
	if (bool_op == _logical_not) {
		file_buffer << "NOT";
	}
	else if (bool_op == _logical_or) {
		file_buffer << "OR";
	}
	else if (bool_op == _logical_and) {
		file_buffer << "AND";
	}
	if (bool_op != _logical_not) {
		file_buffer << "\n";
		file_buffer << AST_SUB_NODE_SPACE << "LHS (";
		lhs_op->print(file_buffer);
		file_buffer << ")";
	}
	file_buffer << "\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs_op->print(file_buffer);
	file_buffer << ")";
}

///////// SELECTION STATEMENT AST

Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond1, Ast* then_part1, Ast* else_part1, int line)
{
	cond = cond1;
	then_part = then_part1;
	else_part = else_part1;
	lineno = line;
	ast_num_child = ternary_arity;
	node_data_type = void_data_type;
}

Data_Type Selection_Statement_Ast::get_data_type()
{
	return node_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast()
{
	return true;
}

void Selection_Statement_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "IF : \n";
	file_buffer << AST_SPACE << "CONDITION (";
	cond->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SPACE << "THEN (";
	then_part->print(file_buffer);
	file_buffer << ")";
	if (else_part != NULL) {
		file_buffer << "\n";
		file_buffer << AST_SPACE << "ELSE (";
		else_part->print(file_buffer);
		file_buffer << ")";
	}
}

///////// ITERATION STATEMENT AST

Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond1, Ast* body1, int line, bool do_form)
{
	cond = cond1;
	body = body1;
	is_do_form = do_form;
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = void_data_type;
}

Data_Type Iteration_Statement_Ast::get_data_type()
{
	return node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Iteration_Statement_Ast::check_ast()
{
	return true;
}

void Iteration_Statement_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	if (is_do_form) {
		file_buffer << AST_SPACE << "DO (";
		body->print(file_buffer);
		file_buffer << ")\n";
		file_buffer << AST_SPACE << "WHILE CONDITION (";
		cond->print(file_buffer);
		file_buffer << ")";
	}
	else {
		file_buffer << AST_SPACE << "WHILE : \n";
		file_buffer << AST_SPACE << "CONDITION (";
		cond->print(file_buffer);
		file_buffer << ")\n";
		file_buffer << AST_SPACE << "BODY (";
		body->print(file_buffer);
		file_buffer << ")";
	}
}

///////// SEQUENCE AST

Sequence_Ast::Sequence_Ast(int line)
{
	statement_list = *(new list<Ast *>);
	sa_icode_list = *(new list<Icode_Stmt *>);
	lineno = line;
	ast_num_child = zero_arity;
	node_data_type = void_data_type;
	labelCounter++;
}

void Sequence_Ast::ast_push_back(Ast * ast)
{
	statement_list.push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer)
{
	list<Ast *>::iterator it;
	for (it = statement_list.begin(); it != statement_list.end(); it++) {
		file_buffer << "\n" << AST_NODE_SPACE;
		(*(it))->print(file_buffer);
	}
}

Print_Ast::Print_Ast(Ast *v, int line){
	var = v;
	lineno = line;
}

void Print_Ast::print(ostream & file_buffer){
	file_buffer << "\n";
	file_buffer << AST_SPACE << "Print :\n";
	file_buffer << AST_SUB_NODE_SPACE << "(";
	var->print(file_buffer);
	file_buffer << ")";

}

///////// CALL AST

Call_Ast::Call_Ast(string name, int line)
{
	procedure_name = name;
	lineno = line;
}

Data_Type Call_Ast::get_data_type()
{
	Procedure* procedure = program_object.get_procedure_prototype(procedure_name);
	return procedure->get_return_type();
}

void Call_Ast::set_register(Register_Descriptor * reg)
{
	return_value_reg = reg;
}

void Call_Ast::check_actual_formal_param(Symbol_Table & formal_param_list)
{
	list<Symbol_Table_Entry*> symList = formal_param_list.get_table();
	if(actual_param_list.size() == symList.size())
	{
		std::list<Ast*>::iterator it1 = actual_param_list.begin();
		std::list<Symbol_Table_Entry*>::iterator it2 = symList.begin();
		while(it1 != actual_param_list.end() && it2 != symList.end()){
			if ((*it1)->get_data_type() != (*it2)->get_data_type()) {
				cerr <<"cs316: Error: Actual and formal parameters data types are not matching\n";
				exit(1);
			}
			it1++;
			it2++;
		}
	}
	else {
		cerr <<"cs316: Error: Actual and formal parameter count do not match\n";
		exit(1);
	}
}

void Call_Ast::set_actual_param_list(list<Ast *> & param_list)
{
	actual_param_list = param_list;
}

void Call_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "FN CALL: " << procedure_name  << "(";
	list<Ast *>::iterator it = actual_param_list.begin();
	for (it = actual_param_list.begin(); it != actual_param_list.end(); it++) {
		file_buffer << "\n" << AST_NODE_SPACE;
		(*it)->print(file_buffer);	
	}
	file_buffer << ")";
}

///////// RETURN AST

Return_Ast::Return_Ast(Ast * ret_val, string name, int line)
{
	lineno = line;
	return_value = ret_val;
	proc_name = name;
}

Data_Type Return_Ast::get_data_type()
{
	Procedure* procedure = program_object.get_procedure_prototype(proc_name);
	if (procedure->get_return_type() == void_data_type && return_value == NULL){
		return procedure->get_return_type();
	}
	else if (return_value != NULL && procedure->get_return_type() == return_value->get_data_type()){
		return procedure->get_return_type();	
	}
	else {
		cerr << "cs316: Error: Return type not matching\n";
		exit(1);
	}
}

void Return_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "RETURN ";
	if (get_data_type() == void_data_type){
		file_buffer << "<NOTHING>";
	}
	else{
		return_value->print(file_buffer);
	}
	file_buffer << "\n";
}
