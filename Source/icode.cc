#include "icode.hh"

template class Const_Opd<int>;
template class Const_Opd<double>;


//////// Instruction Descriptor

Instruction_Descriptor::Instruction_Descriptor
(Tgt_Op op, string Name, string mnn, string ics, Icode_Format icf, Assembly_Format af)
{
	inst_op = op;
	mnemonic = mnn;
	ic_symbol = ics;
	ic_format = icf;
	assem_format = af;
	name = Name;
}

Instruction_Descriptor::Instruction_Descriptor(){}

Tgt_Op Instruction_Descriptor::get_op()
{
	return inst_op;
}

string Instruction_Descriptor::get_name()
{
	return name;
}

string Instruction_Descriptor::get_mnemonic()
{
	return mnemonic;
}

string Instruction_Descriptor::get_ic_symbol()
{
	return ic_symbol;
}

Icode_Format Instruction_Descriptor::get_ic_format()
{
	return ic_format;
}

Assembly_Format Instruction_Descriptor::get_assembly_format()
{
	return assem_format;
}

void print_instruction_descriptor(ostream & file_buffer)
{
	// TODO
}

//////// Ics_Opd

Register_Descriptor * Ics_Opd::get_reg(){}

//////// Mem_Addr_Opd

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se)
{
	symbol_entry = &se;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer)
{
	file_buffer << symbol_entry->get_variable_name();
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer)
{
	if (symbol_entry->get_symbol_scope() == global) {
		file_buffer << symbol_entry->get_variable_name();
	}
	else {
		int offset = symbol_entry->get_start_offset();
		// cout << symbol_entry->get_start_offset() << " and " << symbol_entry->get_end_offset() << endl;
		if (symbol_entry->get_ref_offset() == sp_ref){
			file_buffer << offset << "($sp)";
		}
		else{
			file_buffer << offset << "($fp)";
		}
	}
}

Mem_Addr_Opd & Mem_Addr_Opd::operator=(const Mem_Addr_Opd &rhs)
{
	symbol_entry = rhs.symbol_entry;
	return *this;
}

//////// Register_Addr_Opd

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor* rd)
{
	register_description = rd;
}

Register_Descriptor* Register_Addr_Opd::get_reg()
{
	return register_description;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer)
{
	file_buffer << register_description->get_name();
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer)
{
	file_buffer << "$" << register_description->get_name();
}

Register_Addr_Opd & Register_Addr_Opd::operator=(const Register_Addr_Opd & rhs)
{
	register_description = rhs.register_description;
	return *this;
}

//////// Const_Opd

template <class T>
Const_Opd<T>::Const_Opd(T Num)
{
	num = Num;
}

template <class T>
void Const_Opd<T>::print_ics_opd(ostream & file_buffer)
{
	file_buffer << num;
}

template <class T>
void Const_Opd<T>::print_asm_opd(ostream & file_buffer)
{
	file_buffer << num;
}

template <class T>
Const_Opd<T> & Const_Opd<T>::operator=(const Const_Opd & rhs)
{
	num = rhs.num;
	return *this;
}

//////// Icode_Stmt

Instruction_Descriptor & Icode_Stmt::get_op()
{
	return op_desc;
}

Ics_Opd * Icode_Stmt::get_opd1(){}
Ics_Opd * Icode_Stmt::get_opd2(){}
Ics_Opd * Icode_Stmt::get_result(){}
void Icode_Stmt::set_opd1(Ics_Opd * io){}
void Icode_Stmt::set_opd2(Ics_Opd * io){}
void Icode_Stmt::set_result(Ics_Opd * io){}

//////// Print_IC_Stmt

Print_IC_Stmt::Print_IC_Stmt()
{
	op_desc = *(machine_desc_object.spim_instruction_table[print]);
}

void Print_IC_Stmt::print_icode(ostream & file_buffer)
{
	file_buffer << "\t" << op_desc.get_name() << "\n";	
}

void Print_IC_Stmt::print_assembly(ostream & file_buffer)
{
	file_buffer << "\t" << op_desc.get_mnemonic() << "\n";
}

//////// Move_IC_Stmt

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * Opd1, Ics_Opd * Result)
{
	op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
	opd1 = Opd1;   
	result = Result;
}

Move_IC_Stmt & Move_IC_Stmt::operator=(const Move_IC_Stmt & rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	result = rhs.result;
	return *this;
}

Instruction_Descriptor & Move_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc;
}

Ics_Opd * Move_IC_Stmt::get_opd1()
{
	return opd1;
}

void Move_IC_Stmt::set_opd1(Ics_Opd * io)
{
	opd1 = io;
}

Ics_Opd * Move_IC_Stmt::get_result()
{
	return result;
}

void Move_IC_Stmt::set_result(Ics_Opd * io)
{
	result = io;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer)
{
	file_buffer << "\t" << op_desc.get_name() << ":    \t";
	result->print_ics_opd(file_buffer);
	file_buffer << " <- ";
	opd1->print_ics_opd(file_buffer);
	file_buffer << "\n";
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer)
{
	if (op_desc.get_assembly_format() == a_op_o1_r){
		file_buffer << "\t" << op_desc.get_mnemonic() << " ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		result->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
	else{
		file_buffer << "\t" << op_desc.get_mnemonic() << " ";
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
}

//////// Compute_IC_Stmt

Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * Opd1, Ics_Opd * Opd2, Ics_Opd * Result)
{
	op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
	opd1 = Opd1;   
	opd2 = Opd2;   
	result = Result;
}

Compute_IC_Stmt & Compute_IC_Stmt::operator=(const Compute_IC_Stmt & rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	result = rhs.result;
	return *this;
}

Instruction_Descriptor & Compute_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc;
}

Ics_Opd * Compute_IC_Stmt::get_opd1()
{
	return opd1;
}

void Compute_IC_Stmt::set_opd1(Ics_Opd * io)
{
	opd1 = io;
}

Ics_Opd * Compute_IC_Stmt::get_opd2()
{
	return opd2;
}

void Compute_IC_Stmt::set_opd2(Ics_Opd * io)
{
	opd2 = io;
}

Ics_Opd * Compute_IC_Stmt::get_result()
{
	return result;
}

void Compute_IC_Stmt::set_result(Ics_Opd * io)
{
	result = io;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer)
{
	if (op_desc.get_ic_format() == i_o1_op_o2){
		file_buffer << "\t" << op_desc.get_name() << ":    \t";
		opd1->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		opd2->print_ics_opd(file_buffer);
		file_buffer << "\n";
	}
	else if (op_desc.get_ic_format() == i_r_op_o1){
		file_buffer << "\t" << op_desc.get_name() << ":    \t";
		result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		opd1->print_ics_opd(file_buffer);
		file_buffer << "\n";
	}
	else{
		file_buffer << "\t" << op_desc.get_name() << ":    \t";
		result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		opd1->print_ics_opd(file_buffer);
		file_buffer << " , ";
		opd2->print_ics_opd(file_buffer);
		file_buffer << "\n";
	}
}

void Compute_IC_Stmt::print_assembly(ostream & file_buffer)
{
	if (op_desc.get_assembly_format() == a_op_o1_o2){
		file_buffer << "\t" << op_desc.get_mnemonic() << " ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd2->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
	else if (op_desc.get_assembly_format() == a_op_r_o1){
		file_buffer << "\t" << op_desc.get_mnemonic() << " ";
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
	else{
		file_buffer << "\t" << op_desc.get_mnemonic() << " ";
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd2->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
}

//////// Control_Flow_IC_Stmt

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op inst_op, Ics_Opd * Opd1, Ics_Opd * Opd2, string Label, int size)
{
	op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
	opd1 = Opd1;
	opd2 = Opd2;    
	offset = Label;
	actual_param_size = size;
}

Control_Flow_IC_Stmt & Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt & rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	offset = rhs.offset;
	actual_param_size = rhs.actual_param_size;
	return *this;
}

Instruction_Descriptor & Control_Flow_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1()
{
	return opd1;
}

void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io)
{
	opd1 = io;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd2()
{
	return opd2;
}

void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io)
{
	opd2 = io;
}

string Control_Flow_IC_Stmt::get_Offset()
{
	return offset;
}

void Control_Flow_IC_Stmt::set_Offset(string Label)
{
	offset = Label;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer)
{
	if (op_desc.get_ic_format() == i_op_st){
		if (op_desc.get_op() == bc1t || op_desc.get_op() == bc1f){
			file_buffer << "\t" << op_desc.get_name() << " " << offset;
			file_buffer << " ";
		}
		else if (op_desc.get_op() == j){
			file_buffer << "\t" << "goto " << offset;
		}
		else if (op_desc.get_op() == jal){
			file_buffer << "\t" << "call " << offset;
		}
		file_buffer << "\n";
	}
	else if (op_desc.get_ic_format() == i_op){
		file_buffer << "\t" << op_desc.get_name() << "\n";
	}
	else{
		file_buffer << "\t" << op_desc.get_name() << ":    \t";
		opd1->print_ics_opd(file_buffer);
		file_buffer << " , zero : goto ";
		file_buffer << offset;
		file_buffer << "\n";
	}
}

void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer)
{
	if (op_desc.get_op() == jal) {
		if (actual_param_size != 0)
			file_buffer << "\t" << "sub $sp, $sp, " << to_string(actual_param_size) << "\n";
		file_buffer << "\t" << op_desc.get_mnemonic() << " " << offset << "\n";
		if (actual_param_size != 0)
			file_buffer << "\t" << "add $sp, $sp, " << to_string(actual_param_size) << "\n";
	}
	else if (op_desc.get_assembly_format() == a_op_st) {
		file_buffer << "\t" << op_desc.get_mnemonic() << " " << offset;
		if (op_desc.get_op() == bc1t || op_desc.get_op() == bc1f){
			file_buffer << " ";
		}
		file_buffer << "\n";
	}
	else if (op_desc.get_assembly_format() == a_op) {
		file_buffer << "\t" << "j " << offset << "\n";
	}
	else {
		file_buffer << "\t" << op_desc.get_mnemonic() << " ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", $zero, ";
		file_buffer << offset;
		file_buffer << " \n";
	}
}

//////// Label_IC_Stmt

Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string Label)
{
	op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
	label = Label; 
}

Label_IC_Stmt & Label_IC_Stmt::operator=(const Label_IC_Stmt & rhs)
{
	op_desc = rhs.op_desc;
	label = rhs.label;
	return *this;
}

Instruction_Descriptor & Label_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc;
}

string Label_IC_Stmt::get_label()
{
	return label;
}

void Label_IC_Stmt::set_label(string Label)
{
	label = Label;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << label << ":    \t";
	file_buffer << "\n";
}

void Label_IC_Stmt::print_assembly(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << label << ":    \t";
	file_buffer << "\n";
}

//////// Code_For_Ast

Code_For_Ast::Code_For_Ast()
{
	// ics_list = *(new list<Icode_Stmt *>);
	// result_register = new Register_Descriptor;
}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg)
{
	ics_list = ic_l;
	result_register = reg;
}

void Code_For_Ast::append_ics(Icode_Stmt & ics)
{
	ics_list.push_back(&ics);
}

list<Icode_Stmt *> & Code_For_Ast::get_icode_list()
{
	return ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg()
{
	return result_register;
}

void Code_For_Ast::set_reg(Register_Descriptor * reg)
{
	result_register = reg;
}

Code_For_Ast & Code_For_Ast::operator=(const Code_For_Ast & rhs)
{
	result_register = rhs.result_register;
	ics_list = rhs.ics_list;
	return *this;
}
