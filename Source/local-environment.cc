#include "local-environment.hh"

int Eval_Result::get_int_value(){}
void Eval_Result::set_value(int value){}
double Eval_Result::get_double_value(){}
void Eval_Result::set_value(double value){}
bool Eval_Result::is_variable_defined(){}
void Eval_Result::set_variable_status(bool def){}

void Eval_Result_Value::set_value(int number){}
void Eval_Result_Value::set_value(double number){}
int Eval_Result_Value::get_int_value(){}
double Eval_Result_Value::get_double_value(){}

Eval_Result_Value_Int::Eval_Result_Value_Int()
{
	result_type = int_result;
}
Eval_Result_Value_Int::~Eval_Result_Value_Int(){}
void Eval_Result_Value_Int::set_value(int number)
{
	value = number;
	// defined = true;
}

void Eval_Result_Value_Int::set_value(double number)
{
	value = (int)number;
	// defined = true;
}

int Eval_Result_Value_Int::get_int_value()
{
	return value;
}

bool Eval_Result_Value_Int::is_variable_defined()
{
	return defined;
}

void Eval_Result_Value_Int::set_variable_status(bool def)
{
	defined = def;
}

void Eval_Result_Value_Int::set_result_enum(Result_Enum res)
{
	result_type = res;
}

Result_Enum Eval_Result_Value_Int::get_result_enum()
{
	return result_type;
}



Eval_Result_Value_Double::Eval_Result_Value_Double()
{
	result_type = double_result;
}

Eval_Result_Value_Double::~Eval_Result_Value_Double()
{

}

void Eval_Result_Value_Double::set_value(double number)
{
	value = number;
}

void Eval_Result_Value_Double::set_value(int number)
{
	value = (double)number;
}

double Eval_Result_Value_Double::get_double_value()
{
	return value;
}

void Eval_Result_Value_Double::set_variable_status(bool def)
{
	defined = def;
}

bool Eval_Result_Value_Double::is_variable_defined()
{
	return defined;
}

void Eval_Result_Value_Double::set_result_enum(Result_Enum res)
{
	result_type = res;
}

Result_Enum Eval_Result_Value_Double::get_result_enum()
{
	return result_type;
}



Local_Environment::Local_Environment()
{
	variable_table = *(new map<string, Eval_Result *>());
}

Local_Environment::~Local_Environment()
{
	// delete &variable_table;
}

void Local_Environment::print(ostream & file_buffer)
{
	map<string, Eval_Result *>::iterator it;
	for (it = variable_table.begin(); it != variable_table.end(); it++){
		Result_Enum res_type = it->second->get_result_enum();
		if (res_type == int_result){
			if (!is_variable_defined(it->first)){
				int val = it->second->get_int_value();
				file_buffer << VAR_SPACE << it->first << " : " << "undefined" << "\n";
			}
			else{
				int val = it->second->get_int_value();
				file_buffer << VAR_SPACE << it->first << " : " << val << "\n";
			}
		}
		else if (res_type == double_result){
			if (!is_variable_defined(it->first)){
				double val = it->second->get_double_value();
				file_buffer << VAR_SPACE << it->first << " : " << "undefined" << "\n";
			}
			else{
				double val = it->second->get_double_value();
				file_buffer << VAR_SPACE << it->first << " : " << val << "\n";
			}
		}
	}
}
	
bool Local_Environment::is_variable_defined(string name)
{
	map<string, Eval_Result *>::iterator it;
	for (it = variable_table.begin(); it != variable_table.end(); it++){
		if (it->first == name){
			return it->second->is_variable_defined();
		}
	}
	return false;
}
	
Eval_Result * Local_Environment::get_variable_value(string name)
{
	map<string, Eval_Result *>::iterator it;
	for (it = variable_table.begin(); it != variable_table.end(); it++){
		if (it->first == name){
			return it->second;
		}
	}
	return NULL;
}

void Local_Environment::put_variable_value(Eval_Result & value, string name)
{
	map<string, Eval_Result *>::iterator it;
	for (it = variable_table.begin(); it != variable_table.end(); it++){
		if (it->first == name){
			it->second = &value;
			return;
		}
	}
	variable_table[name] = &value;
}

bool Local_Environment::does_variable_exist(string name)
{
	map<string, Eval_Result *>::iterator it;
	for (it = variable_table.begin(); it != variable_table.end(); it++){
		if (it->first == name){
			return true;
		}
	}
	return false;
}



void Symbol_Table::create(Local_Environment & local_global_variables_table)
{

	list<Symbol_Table_Entry *>::iterator it;
	for (it = variable_table.begin(); it != variable_table.end(); it++){
		if ((*it)->get_data_type() == int_data_type){
			Eval_Result_Value_Int* er = new Eval_Result_Value_Int;
			er->set_result_enum(int_result);
			if (get_table_scope() == global){
				er->set_variable_status(true);
				er->set_value(0);
			}
			else{
				er->set_variable_status(false);
			}
			local_global_variables_table.put_variable_value(*(er), (*it)->get_variable_name());
		}
		else if ((*it)->get_data_type() == double_data_type){
			Eval_Result_Value_Double *er = new Eval_Result_Value_Double;
			er->set_result_enum(double_result);
			if (get_table_scope() == global){
				er->set_variable_status(true);
				er->set_value(0.00);
			}
			else{
				er->set_variable_status(false);
			}
			local_global_variables_table.put_variable_value(*(er), (*it)->get_variable_name());
		}
	}
}

