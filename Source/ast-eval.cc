#include "ast.hh"

template class Number_Ast<double>;
template class Number_Ast<int>;

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){}
Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env){}
void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){}
Eval_Result & Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){}

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	print(file_buffer);
	string name = (lhs->get_symbol_entry()).get_variable_name();
	if (lhs->get_data_type() == int_data_type){
		Eval_Result_Value_Int *res = new Eval_Result_Value_Int;
		res->set_value((rhs->evaluate(eval_env, file_buffer)).get_int_value());
		res->set_variable_status(true);
		if (eval_env.does_variable_exist(name)) {
			eval_env.put_variable_value(*res, name);	
		}
		else if (interpreter_global_table.does_variable_exist(name)) {
			interpreter_global_table.put_variable_value(*res, name);
		}
	}
	else if (lhs->get_data_type() == double_data_type){
		Eval_Result_Value_Double *res = new Eval_Result_Value_Double;
		res->set_value((rhs->evaluate(eval_env, file_buffer)).get_double_value());
		res->set_variable_status(true);
		if (eval_env.does_variable_exist(name)) {
			eval_env.put_variable_value(*res, name);	
		}
		else if (interpreter_global_table.does_variable_exist(name)) {
			interpreter_global_table.put_variable_value(*res, name);
		}
	}
	file_buffer << "\n";
	lhs->print_value(eval_env, file_buffer);
	file_buffer << "\n";
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	file_buffer << AST_SPACE;
	string name = variable_symbol_entry->get_variable_name();
	file_buffer << name << " : ";
	if (variable_symbol_entry->get_data_type() == int_data_type){
		if (eval_env.is_variable_defined(name)) {
			file_buffer << eval_env.get_variable_value(name)->get_int_value();
		}
		else if (interpreter_global_table.is_variable_defined(name)) {
			file_buffer << interpreter_global_table.get_variable_value(name)->get_int_value();
		}
	}
	else if (variable_symbol_entry->get_data_type() == double_data_type){
		if (eval_env.is_variable_defined(name)) {
			file_buffer << eval_env.get_variable_value(name)->get_double_value();
		}
		else if (interpreter_global_table.is_variable_defined(name)) {
			file_buffer << interpreter_global_table.get_variable_value(name)->get_double_value();
		}
	}
	file_buffer << "\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	string name = variable_symbol_entry->get_variable_name();
	if (eval_env.does_variable_exist(name)) {
		return *(eval_env.get_variable_value(name));
	}
	else if (interpreter_global_table.does_variable_exist(name)) {
		return *(interpreter_global_table.get_variable_value(name));
	}
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	string name = variable_symbol_entry->get_variable_name();
	if (eval_env.does_variable_exist(name)) {
		eval_env.put_variable_value(result, name);	
	}
	else if (interpreter_global_table.does_variable_exist(name)) {
		interpreter_global_table.put_variable_value(result, name);
	}
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	string name = variable_symbol_entry->get_variable_name();
	if (variable_symbol_entry->get_data_type() == int_data_type){
		Eval_Result_Value_Int *res = new Eval_Result_Value_Int;
		if (eval_env.is_variable_defined(name)) {
			res->set_value(eval_env.get_variable_value(name)->get_int_value());
		}
		else if (interpreter_global_table.is_variable_defined(name)) {
			res->set_value(interpreter_global_table.get_variable_value(name)->get_int_value());
		}
		else {
			cerr << "cs316: Error: Variable should be defined before its use\n";
			exit(1);
		}
		return *res;
	}
	else if (variable_symbol_entry->get_data_type() == double_data_type){
		Eval_Result_Value_Double *res = new Eval_Result_Value_Double;
		if (eval_env.is_variable_defined(name)) {
			res->set_value(eval_env.get_variable_value(name)->get_double_value());
		}
		else if (interpreter_global_table.is_variable_defined(name)) {
			res->set_value(interpreter_global_table.get_variable_value(name)->get_double_value());
		}
		else {
			cerr << "cs316: Error: Variable should be defined before its use\n";
			exit(1);
		}
		return *res;
	}
}

template <typename T>
Eval_Result & Number_Ast<T>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (get_data_type() == int_data_type){
		Eval_Result_Value_Int *res = new Eval_Result_Value_Int;
		res->set_value(constant);
		return *res;
	}
	else if (get_data_type() == double_data_type){
		Eval_Result_Value_Double *res = new Eval_Result_Value_Double;
		res->set_value(constant);
		return *res;
	}
}

Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type) {
		Eval_Result_Value_Int* eval = new Eval_Result_Value_Int();
		eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_int_value() + (rhs->evaluate(eval_env, file_buffer)).get_int_value());
		eval->set_result_enum(int_result);
		return *(eval);
	}
	else if (node_data_type == double_data_type) {
		Eval_Result_Value_Double* eval = new Eval_Result_Value_Double();
		eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_double_value() + (rhs->evaluate(eval_env, file_buffer)).get_double_value());
		eval->set_result_enum(double_result);
		return *(eval);
	}
	
}

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type) {
		Eval_Result_Value_Int* eval = new Eval_Result_Value_Int();
		eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_int_value() - (rhs->evaluate(eval_env, file_buffer)).get_int_value());
		eval->set_result_enum(int_result);
		return *(eval);
	}
	else if (node_data_type == double_data_type) {
		Eval_Result_Value_Double* eval = new Eval_Result_Value_Double();
		eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_double_value() - (rhs->evaluate(eval_env, file_buffer)).get_double_value());
		eval->set_result_enum(double_result);
		return *(eval);
	}
}

Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type) {
		Eval_Result_Value_Int* eval = new Eval_Result_Value_Int();
		if ((rhs->evaluate(eval_env, file_buffer)).get_int_value() == 0){
			cerr << "cs316: Error: Line: " << lineno << ": Divide by 0\n";
			exit(1);
		}
		eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_int_value() / (rhs->evaluate(eval_env, file_buffer)).get_int_value());
		eval->set_result_enum(int_result);
		return *(eval);
	}
	else if (node_data_type == double_data_type) {
		Eval_Result_Value_Double* eval = new Eval_Result_Value_Double();
		if ((rhs->evaluate(eval_env, file_buffer)).get_double_value() == 0.0){
			cerr << "cs316: Error: Line: " << lineno << ": Divide by 0\n";
			exit(1);
		}
		eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_double_value() / (rhs->evaluate(eval_env, file_buffer)).get_double_value());
		eval->set_result_enum(double_result);
		return *(eval);
	}
}

Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type) {
		Eval_Result_Value_Int* eval = new Eval_Result_Value_Int();
		eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_int_value() * (rhs->evaluate(eval_env, file_buffer)).get_int_value());
		eval->set_result_enum(int_result);
		return *(eval);
	}
	else if (node_data_type == double_data_type) {
		Eval_Result_Value_Double* eval = new Eval_Result_Value_Double();
		eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_double_value() * (rhs->evaluate(eval_env, file_buffer)).get_double_value());
		eval->set_result_enum(double_result);
		return *(eval);
	}
}

Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type) {
		Eval_Result_Value_Int* eval = new Eval_Result_Value_Int();
		eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_int_value() * (-1));
		eval->set_result_enum(int_result);
		return *(eval);
	}
	else if (node_data_type == double_data_type) {
		Eval_Result_Value_Double* eval = new Eval_Result_Value_Double();
		eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_double_value() * (-1));
		eval->set_result_enum(double_result);
		return *(eval);
	}
}

Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if ((cond->evaluate(eval_env, file_buffer)).get_int_value() == 1) {
		if(node_data_type == int_data_type) {
			Eval_Result_Value_Int* eval = new Eval_Result_Value_Int();
			eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_int_value());
			eval->set_result_enum(int_result);
			return *(eval);
		}
		else if (node_data_type == double_data_type) {
			Eval_Result_Value_Double* eval = new Eval_Result_Value_Double();
			eval->set_value((lhs->evaluate(eval_env, file_buffer)).get_double_value());
			eval->set_result_enum(double_result);
			return *(eval);
		}
	}
	else if ((cond->evaluate(eval_env, file_buffer)).get_int_value() == 0) {
		if(node_data_type == int_data_type) {
			Eval_Result_Value_Int* eval = new Eval_Result_Value_Int();
			eval->set_value((rhs->evaluate(eval_env, file_buffer)).get_int_value());
			eval->set_result_enum(int_result);
			return *(eval);
		}
		else if (node_data_type == double_data_type) {
			Eval_Result_Value_Double* eval = new Eval_Result_Value_Double();
			eval->set_value((rhs->evaluate(eval_env, file_buffer)).get_double_value());
			eval->set_result_enum(double_result);
			return *(eval);
		}
	}
}

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	double l, r;
	Data_Type dt = lhs_condition->get_data_type();
	if(dt == int_data_type) {
		l = (lhs_condition->evaluate(eval_env, file_buffer)).get_int_value();
		r = (rhs_condition->evaluate(eval_env, file_buffer)).get_int_value();
	}
	else if (dt == double_data_type) {
		l = (lhs_condition->evaluate(eval_env, file_buffer)).get_double_value();
		r = (rhs_condition->evaluate(eval_env, file_buffer)).get_double_value();
	}
	Eval_Result_Value_Int* eval = new Eval_Result_Value_Int();
	eval->set_result_enum(int_result);
	eval->set_value(0);
	if (rel_op == less_equalto) {
		if (l <= r) {
			eval->set_value(1);
		}
	}
	else if (rel_op == less_than) {
		if (l < r) {
			eval->set_value(1);
		}
	}
	else if (rel_op == greater_than) {
		if (l > r) {
			eval->set_value(1);
		}
	}
	else if (rel_op == greater_equalto) {
		if (l >= r) {
			eval->set_value(1);
		}
	}
	else if (rel_op == equalto) {
		if (l == r) {
			eval->set_value(1);
		}
	}
	else if (rel_op == not_equalto) {
		if (l != r) {
			eval->set_value(1);
		}
	}
	return *(eval);
}

Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	int r = (rhs_op->evaluate(eval_env, file_buffer)).get_int_value();
	Eval_Result_Value_Int* eval = new Eval_Result_Value_Int();
	eval->set_result_enum(int_result);
	eval->set_value(0);
	if (lhs_op == NULL) {
		if (bool_op == _logical_not) {
			if (r == 0) {
				eval->set_value(1);
			}
		}
	}
	else {
		int l = (lhs_op->evaluate(eval_env, file_buffer)).get_int_value();
		if (bool_op == _logical_or) {
			if ((l + r) >= 1) {
				eval->set_value(1);
			}
		}
		else if (bool_op == _logical_and) {
			if ((l * r) == 1) {
				eval->set_value(1);
			}
		}
	}
	return *(eval);
}

Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if ((cond->evaluate(eval_env, file_buffer)).get_int_value() == 1) {
		then_part->evaluate(eval_env, file_buffer);
	}
	else if ((cond->evaluate(eval_env, file_buffer)).get_int_value() == 0){
		if (else_part != NULL){
			else_part->evaluate(eval_env, file_buffer);
		}
	}

}

Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (is_do_form){
		body->evaluate(eval_env, file_buffer);
	}
	while ((cond->evaluate(eval_env, file_buffer)).get_int_value() == 1) {
		body->evaluate(eval_env, file_buffer);
	}
}

Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	list<Ast *>::iterator it;
	for (it = statement_list.begin(); it != statement_list.end(); it++) {
		(*(it))->evaluate(eval_env, file_buffer);
	}
}

Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){}
