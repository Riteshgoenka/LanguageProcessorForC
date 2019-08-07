#include "ast.hh"

template class Number_Ast<double>;
template class Number_Ast<int>;

///////// AST

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){}

///////// ASSIGNMENT AST

Code_For_Ast & Assignment_Ast::compile()
{
	Code_For_Ast rhs_cfa = rhs->compile();
	Code_For_Ast lhs_cfa = lhs->create_store_stmt(rhs_cfa.get_reg());

	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	ic_l.splice(ic_l.end(), rhs_cfa.get_icode_list());
	ic_l.splice(ic_l.end(), lhs_cfa.get_icode_list());

	Code_For_Ast* res = new Code_For_Ast(ic_l, NULL);

	rhs_cfa.get_reg()->reset_use_for_expr_result();

	return *res;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{

}

///////// NAME AST

Code_For_Ast & Name_Ast::compile()
{
	Code_For_Ast* res = new Code_For_Ast();

	Register_Descriptor* reg; 
	if (get_data_type() == int_data_type){
		reg = machine_desc_object.get_new_register<int_reg>();
	}
	else if (get_data_type() == double_data_type){
		reg = machine_desc_object.get_new_register<float_reg>();
	}
	res->set_reg(reg);

	Register_Addr_Opd* result = new Register_Addr_Opd(reg);
	Mem_Addr_Opd* opd1 = new Mem_Addr_Opd(*variable_symbol_entry);
	Move_IC_Stmt* stmt;
	if (get_data_type() == int_data_type){
		stmt = new Move_IC_Stmt(load, opd1, result);
	}
	else if (get_data_type() == double_data_type){
		stmt = new Move_IC_Stmt(load_d, opd1, result);
	}
	res->append_ics(*stmt);

	return *res;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	Code_For_Ast* res = new Code_For_Ast();

	// res->set_reg(store_register);
	// variable_symbol_entry->set_register(machine_desc_object.spim_register_table[fp]);

	Register_Addr_Opd* opd1 = new Register_Addr_Opd(store_register);
	Mem_Addr_Opd* result = new Mem_Addr_Opd(*variable_symbol_entry);
	Move_IC_Stmt* stmt;
	if (get_data_type() == int_data_type){
		stmt = new Move_IC_Stmt(store, opd1, result);
	}
	else if (get_data_type() == double_data_type){
		stmt = new Move_IC_Stmt(store_d, opd1, result);
	}
	res->append_ics(*stmt);

	return *res;
}

///////// NUMBER AST

template<typename T>
Code_For_Ast & Number_Ast<T>::compile()
{
	Code_For_Ast* res = new Code_For_Ast();

	Register_Descriptor* reg; 
	if (get_data_type() == int_data_type){
		reg = machine_desc_object.get_new_register<int_reg>();
	}
	else if (get_data_type() == double_data_type){
		reg = machine_desc_object.get_new_register<float_reg>();
	}
	res->set_reg(reg);

	Register_Addr_Opd* result = new Register_Addr_Opd(reg);
	Const_Opd<T>* opd1 = new Const_Opd<T>(constant);
	Move_IC_Stmt* stmt;
	if (get_data_type() == int_data_type){
		stmt = new Move_IC_Stmt(imm_load, opd1, result);
	}
	else if (get_data_type() == double_data_type){
		stmt = new Move_IC_Stmt(imm_load_d, opd1, result);
	}
	res->append_ics(*stmt);

	return *res;
}

template<typename T>
Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra)
{
	
}

///////// PLUS AST

Code_For_Ast & Plus_Ast::compile()
{
	Code_For_Ast lhs_cfa = lhs->compile();
	Code_For_Ast rhs_cfa = rhs->compile();

	Register_Descriptor* reg; 
	if (get_data_type() == int_data_type){
		reg = machine_desc_object.get_new_register<int_reg>();
	}
	else if (get_data_type() == double_data_type){
		reg = machine_desc_object.get_new_register<float_reg>();
	}
	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* stmt;
	if (get_data_type() == int_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(add, opd1, opd2, result);
	}
	else if (get_data_type() == double_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(add_d, opd1, opd2, result);
	}

	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	ic_l.splice(ic_l.end(), lhs_cfa.get_icode_list());
	ic_l.splice(ic_l.end(), rhs_cfa.get_icode_list());
	ic_l.push_back(stmt);

	Code_For_Ast* res = new Code_For_Ast(ic_l, reg);

	// reg->set_use_for_expr_result();
	lhs_cfa.get_reg()->reset_use_for_expr_result();
	rhs_cfa.get_reg()->reset_use_for_expr_result();

	return *res;
}

Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{

}

///////// MINUS AST

Code_For_Ast & Minus_Ast::compile()
{
	Code_For_Ast lhs_cfa = lhs->compile();
	Code_For_Ast rhs_cfa = rhs->compile();

	Register_Descriptor* reg; 
	if (get_data_type() == int_data_type){
		reg = machine_desc_object.get_new_register<int_reg>();
	}
	else if (get_data_type() == double_data_type){
		reg = machine_desc_object.get_new_register<float_reg>();
	}
	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* stmt;
	if (get_data_type() == int_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(sub, opd1, opd2, result);
	}
	else if (get_data_type() == double_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(sub_d, opd1, opd2, result);
	}

	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	ic_l.splice(ic_l.end(), lhs_cfa.get_icode_list());
	ic_l.splice(ic_l.end(), rhs_cfa.get_icode_list());
	ic_l.push_back(stmt);

	Code_For_Ast* res = new Code_For_Ast(ic_l, reg);

	// reg->set_use_for_expr_result();
	lhs_cfa.get_reg()->reset_use_for_expr_result();
	rhs_cfa.get_reg()->reset_use_for_expr_result();

	return *res;
}

Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{

}

///////// DIVIDE AST

Code_For_Ast & Divide_Ast::compile()
{
	Code_For_Ast lhs_cfa = lhs->compile();
	Code_For_Ast rhs_cfa = rhs->compile();

	Register_Descriptor* reg; 
	if (get_data_type() == int_data_type){
		reg = machine_desc_object.get_new_register<int_reg>();
	}
	else if (get_data_type() == double_data_type){
		reg = machine_desc_object.get_new_register<float_reg>();
	}
	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* stmt;
	if (get_data_type() == int_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(divd, opd1, opd2, result);
	}
	else if (get_data_type() == double_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(div_d, opd1, opd2, result);
	}

	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	ic_l.splice(ic_l.end(), lhs_cfa.get_icode_list());
	ic_l.splice(ic_l.end(), rhs_cfa.get_icode_list());
	ic_l.push_back(stmt);

	Code_For_Ast* res = new Code_For_Ast(ic_l, reg);

	// reg->set_use_for_expr_result();
	lhs_cfa.get_reg()->reset_use_for_expr_result();
	rhs_cfa.get_reg()->reset_use_for_expr_result();

	return *res;
}

Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	
}

///////// MULT AST

Code_For_Ast & Mult_Ast::compile()
{
	Code_For_Ast lhs_cfa = lhs->compile();
	Code_For_Ast rhs_cfa = rhs->compile();

	Register_Descriptor* reg; 
	if (get_data_type() == int_data_type){
		reg = machine_desc_object.get_new_register<int_reg>();
	}
	else if (get_data_type() == double_data_type){
		reg = machine_desc_object.get_new_register<float_reg>();
	}
	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* stmt;
	if (get_data_type() == int_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(mult, opd1, opd2, result);
	}
	else if (get_data_type() == double_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(mult_d, opd1, opd2, result);
	}

	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	ic_l.splice(ic_l.end(), lhs_cfa.get_icode_list());
	ic_l.splice(ic_l.end(), rhs_cfa.get_icode_list());
	ic_l.push_back(stmt);

	Code_For_Ast* res = new Code_For_Ast(ic_l, reg);

	// reg->set_use_for_expr_result();
	lhs_cfa.get_reg()->reset_use_for_expr_result();
	rhs_cfa.get_reg()->reset_use_for_expr_result();

	return *res;
}

Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	
}

///////// UNARY MINUS AST

Code_For_Ast & UMinus_Ast::compile()
{
	Code_For_Ast lhs_cfa = lhs->compile();

	Register_Descriptor* reg; 
	if (get_data_type() == int_data_type){
		reg = machine_desc_object.get_new_register<int_reg>();
	}
	else if (get_data_type() == double_data_type){
		reg = machine_desc_object.get_new_register<float_reg>();
	}
	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* stmt;
	if (get_data_type() == int_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(uminus, opd1, NULL, result);
	}
	else if (get_data_type() == double_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(uminus_d, opd1, NULL, result);
	}

	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	ic_l.splice(ic_l.end(), lhs_cfa.get_icode_list());
	ic_l.push_back(stmt);

	Code_For_Ast* res = new Code_For_Ast(ic_l, reg);

	// reg->set_use_for_expr_result();
	lhs_cfa.get_reg()->reset_use_for_expr_result();

	return *res;
}

Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{

}

///////// CONDITIONAL EXPRESSION AST

Code_For_Ast & Conditional_Expression_Ast::compile()
{


	Code_For_Ast cond_cfa = cond->compile();
	string label1 = "label" + to_string(labelCounter);
	labelCounter++;
	Code_For_Ast lhs_cfa = lhs->compile();
	string label2 = "label" + to_string(labelCounter);
	labelCounter++;
	Register_Addr_Opd* opd = new Register_Addr_Opd(cond_cfa.get_reg());

	Control_Flow_IC_Stmt* stmt;
	Tgt_Op id = ((Compute_IC_Stmt*)(cond_cfa.get_icode_list().back()))->get_inst_op_of_ics().get_op();
	Register_Addr_Opd* rao = (Register_Addr_Opd*)(cond_cfa.get_icode_list().back())->get_result();
	bool to_free = true;
	if (id == seq_d || id == sle_d || id == slt_d){
		if (rao == NULL){
			stmt = new Control_Flow_IC_Stmt(bc1f, opd, NULL, label1);
		}
		else{
			stmt = new Control_Flow_IC_Stmt(bc1t, opd, NULL, label1);
		}
		to_free = false;
	}
	else{
		stmt = new Control_Flow_IC_Stmt(beq, opd, NULL, label1);
	}

	Control_Flow_IC_Stmt* j_stmt;
	j_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label2);

	Label_IC_Stmt* label1_stmt;
	Label_IC_Stmt* label2_stmt;
	label1_stmt = new Label_IC_Stmt(label, label1);
	label2_stmt = new Label_IC_Stmt(label, label2);

	Compute_IC_Stmt* or1_stmt;
	Compute_IC_Stmt* or2_stmt;
	Register_Addr_Opd* or_opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
	Register_Addr_Opd* or_zero = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);

	
	lhs_cfa.get_reg()->reset_use_for_expr_result();
	Code_For_Ast rhs_cfa = rhs->compile();
	Register_Addr_Opd* or_opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());

	rhs_cfa.get_reg()->reset_use_for_expr_result();

	Register_Descriptor* reg; 
	if (get_data_type() == int_data_type){
		reg = machine_desc_object.get_new_register<int_reg>();
	}
	else if (get_data_type() == double_data_type){
		reg = machine_desc_object.get_new_register<float_reg>();
	}
	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	or1_stmt = new Compute_IC_Stmt(or_t, or_opd1, or_zero, result);
	or2_stmt = new Compute_IC_Stmt(or_t, or_opd2, or_zero, result);

	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	ic_l.splice(ic_l.end(), cond_cfa.get_icode_list());
	ic_l.push_back(stmt);
	ic_l.splice(ic_l.end(), lhs_cfa.get_icode_list());
	ic_l.push_back(or1_stmt);
	ic_l.push_back(j_stmt);
	ic_l.push_back(label1_stmt);
	ic_l.splice(ic_l.end(), rhs_cfa.get_icode_list());
	ic_l.push_back(or2_stmt);
	ic_l.push_back(label2_stmt);

	Code_For_Ast* res = new Code_For_Ast(ic_l, reg);

	// reg->set_use_for_expr_result();
	if (to_free)
		cond_cfa.get_reg()->reset_use_for_expr_result();

	return *res;
}

// Code_For_Ast & Conditional_Expression_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
// {

// }

///////// RELATIONAL EXPRESSION AST

Code_For_Ast & Relational_Expr_Ast::compile()
{
	if (lhs_condition->get_data_type() == int_data_type){
		Code_For_Ast lhs_cfa = lhs_condition->compile();
		Code_For_Ast rhs_cfa = rhs_condition->compile();

		Register_Descriptor* reg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd* result = new Register_Addr_Opd(reg);

		Compute_IC_Stmt* stmt;
		if (rel_op == less_equalto){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(sle, opd1, opd2, result);
		}
		else if (rel_op == less_than){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(slt, opd1, opd2, result);
		}
		else if (rel_op == greater_than){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(sgt, opd1, opd2, result);
		}
		else if (rel_op == greater_equalto){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(sge, opd1, opd2, result);
		}
		else if (rel_op == equalto){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(seq, opd1, opd2, result);
		}
		else if (rel_op == not_equalto){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(sne, opd1, opd2, result);
		}

		list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
		ic_l.splice(ic_l.end(), lhs_cfa.get_icode_list());
		ic_l.splice(ic_l.end(), rhs_cfa.get_icode_list());
		ic_l.push_back(stmt);

		Code_For_Ast* res = new Code_For_Ast(ic_l, reg);

		// reg->set_use_for_expr_result();
		lhs_cfa.get_reg()->reset_use_for_expr_result();
		rhs_cfa.get_reg()->reset_use_for_expr_result();

		return *res;
	}
	else if (lhs_condition->get_data_type() == double_data_type){
		Code_For_Ast lhs_cfa = lhs_condition->compile();
		Code_For_Ast rhs_cfa = rhs_condition->compile();

		Compute_IC_Stmt* stmt;
		Register_Addr_Opd* z = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);
		if (rel_op == less_equalto){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(sle_d, opd1, opd2, NULL);
		}
		else if (rel_op == less_than){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(slt_d, opd1, opd2, NULL);
		}
		else if (rel_op == greater_than){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(sle_d, opd1, opd2, z);
		}
		else if (rel_op == greater_equalto){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(slt_d, opd1, opd2, z);
		}
		else if (rel_op == equalto){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(seq_d, opd1, opd2, NULL);
		}
		else if (rel_op == not_equalto){
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
			Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
			stmt = new Compute_IC_Stmt(seq_d, opd1, opd2, z);
		}
		list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
		ic_l.splice(ic_l.end(), lhs_cfa.get_icode_list());
		ic_l.splice(ic_l.end(), rhs_cfa.get_icode_list());
		ic_l.push_back(stmt);

		Code_For_Ast* res = new Code_For_Ast(ic_l, NULL);

		// reg->set_use_for_expr_result();
		lhs_cfa.get_reg()->reset_use_for_expr_result();
		rhs_cfa.get_reg()->reset_use_for_expr_result();

		return *res;
	}
}

// Code_For_Ast & Relational_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
// {

// }

///////// LOGICAL EXPRESSION AST

Code_For_Ast & Logical_Expr_Ast::compile()
{
	Code_For_Ast lhs_cfa = lhs_op->compile();
	Code_For_Ast rhs_cfa = rhs_op->compile();

	Register_Descriptor* reg = machine_desc_object.get_new_register<int_reg>();
	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* stmt;
	if (bool_op == _logical_not){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(not_t, opd2, opd1, result);
	}
	else if (bool_op == _logical_or){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(or_t, opd1, opd2, result);
	}
	else if (bool_op == _logical_and){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(lhs_cfa.get_reg());
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(rhs_cfa.get_reg());
		stmt = new Compute_IC_Stmt(and_t, opd1, opd2, result);
	}

	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	ic_l.splice(ic_l.end(), lhs_cfa.get_icode_list());
	ic_l.splice(ic_l.end(), rhs_cfa.get_icode_list());
	ic_l.push_back(stmt);

	Code_For_Ast* res = new Code_For_Ast(ic_l, reg);

	// reg->set_use_for_expr_result();
	lhs_cfa.get_reg()->reset_use_for_expr_result();
	rhs_cfa.get_reg()->reset_use_for_expr_result();

	return *res;
}

// Code_For_Ast & Logical_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
// {

// }

///////// SELECTION STATEMENT AST

Code_For_Ast & Selection_Statement_Ast::compile()
{
	Code_For_Ast cond_cfa = cond->compile();
	Code_For_Ast then_cfa = then_part->compile();
	
	string label1 = "label" + to_string(labelCounter);
	labelCounter++;
	
	Register_Addr_Opd* opd = new Register_Addr_Opd(cond_cfa.get_reg());
	Control_Flow_IC_Stmt* stmt;
	Tgt_Op id = ((Compute_IC_Stmt*)(cond_cfa.get_icode_list().back()))->get_inst_op_of_ics().get_op();
	Register_Addr_Opd* rao = (Register_Addr_Opd*)(cond_cfa.get_icode_list().back())->get_result();
	bool to_free = true;
	if (id == seq_d || id == sle_d || id == slt_d){
		if (rao == NULL){
			stmt = new Control_Flow_IC_Stmt(bc1f, opd, NULL, label1);
		}
		else{
			stmt = new Control_Flow_IC_Stmt(bc1t, opd, NULL, label1);
		}
		to_free = false;
	}
	else{
		stmt = new Control_Flow_IC_Stmt(beq, opd, NULL, label1);
	}

	Label_IC_Stmt* label1_stmt;
	label1_stmt = new Label_IC_Stmt(label, label1);

	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	ic_l.splice(ic_l.end(), cond_cfa.get_icode_list());
	ic_l.push_back(stmt);
	ic_l.splice(ic_l.end(), then_cfa.get_icode_list());

	if (else_part != NULL) {
		Code_For_Ast else_cfa = else_part->compile();

		string label2 = "label" + to_string(labelCounter);
		labelCounter++;
		Label_IC_Stmt* label2_stmt;
		label2_stmt = new Label_IC_Stmt(label, label2);

		Control_Flow_IC_Stmt* j_stmt;
		j_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label2);

		ic_l.push_back(j_stmt);
		ic_l.push_back(label1_stmt);
		ic_l.splice(ic_l.end(), else_cfa.get_icode_list());
		ic_l.push_back(label2_stmt);
	}
	else {
		ic_l.push_back(label1_stmt);
	}

	Code_For_Ast* res = new Code_For_Ast(ic_l, NULL);

	if (to_free){
		cond_cfa.get_reg()->reset_use_for_expr_result();
	}
	
	return *res;
}

// Code_For_Ast & Selection_Statement_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
// {

// }

///////// ITERATION STATEMENT AST

Code_For_Ast & Iteration_Statement_Ast::compile()
{
	Code_For_Ast cond_cfa = cond->compile();

	string label1 = "label" + to_string(labelCounter);
	labelCounter++;
	string label2 = "label" + to_string(labelCounter);
	labelCounter++;

	Code_For_Ast body_cfa = body->compile();

	Register_Addr_Opd* opd = new Register_Addr_Opd(cond_cfa.get_reg());

	Control_Flow_IC_Stmt* stmt;
	Tgt_Op id = ((Compute_IC_Stmt*)(cond_cfa.get_icode_list().back()))->get_inst_op_of_ics().get_op();
	Register_Addr_Opd* rao = (Register_Addr_Opd*)(cond_cfa.get_icode_list().back())->get_result();
	bool to_free = true;
	if (id == seq_d || id == sle_d || id == slt_d){
		if (rao == NULL){
			stmt = new Control_Flow_IC_Stmt(bc1f, opd, NULL, label1);
		}
		else{
			stmt = new Control_Flow_IC_Stmt(bc1t, opd, NULL, label1);
		}
		to_free = false;
	}
	else{
		stmt = new Control_Flow_IC_Stmt(bne, opd, NULL, label1);
	}

	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	if(!(is_do_form)) {
		Control_Flow_IC_Stmt* j_stmt;
		j_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label2);
		ic_l.push_back(j_stmt);
	}
	Label_IC_Stmt* label1_stmt;
	Label_IC_Stmt* label2_stmt;
	label1_stmt = new Label_IC_Stmt(label, label1);
	label2_stmt = new Label_IC_Stmt(label, label2);

	ic_l.push_back(label1_stmt);
	ic_l.splice(ic_l.end(), body_cfa.get_icode_list());
	ic_l.push_back(label2_stmt);
	ic_l.splice(ic_l.end(), cond_cfa.get_icode_list());
	ic_l.push_back(stmt);

	Code_For_Ast* res = new Code_For_Ast(ic_l, NULL);

	if (to_free)
		cond_cfa.get_reg()->reset_use_for_expr_result();

	return *res;
}

// Code_For_Ast & Iteration_Statement_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
// {

// }

///////// SEQUENCE AST

Code_For_Ast & Sequence_Ast::compile()
{
	list<Ast *>::iterator it;
	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	for (it = statement_list.begin(); it != statement_list.end(); it++){
		Code_For_Ast temp = (*it)->compile();
		ic_l.splice(ic_l.end(), temp.get_icode_list());
	}
	Code_For_Ast* res = new Code_For_Ast(ic_l, NULL);
	sa_icode_list = ic_l;
	return *res;
}

// Code_For_Ast & Sequence_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
// {

// }

Code_For_Ast & Print_Ast::compile()
{

	Move_IC_Stmt* stmt1;
	Move_IC_Stmt* stmt2;
	Print_IC_Stmt* stmt3 = new Print_IC_Stmt();
	if (var->get_data_type() == int_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[v0]);
		Const_Opd<int>* opd2 = new Const_Opd<int>(1);
		Register_Addr_Opd* opd3 = new Register_Addr_Opd(machine_desc_object.spim_register_table[a0]);
		Mem_Addr_Opd* opd4 = new Mem_Addr_Opd(var->get_symbol_entry());
		stmt1 = new Move_IC_Stmt(imm_load, opd2, opd1);
		stmt2 = new Move_IC_Stmt(load, opd4, opd3);
	}
	else if (var->get_data_type() == double_data_type){
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[v0]);
		Const_Opd<int>* opd2 = new Const_Opd<int>(3);
		Register_Addr_Opd* opd3 = new Register_Addr_Opd(machine_desc_object.spim_register_table[f12]);
		Mem_Addr_Opd* opd4 = new Mem_Addr_Opd(var->get_symbol_entry());
		stmt1 = new Move_IC_Stmt(imm_load, opd2, opd1);
		stmt2 = new Move_IC_Stmt(load_d, opd4, opd3);
	}
	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	ic_l.push_back(stmt1);
	ic_l.push_back(stmt2);
	ic_l.push_back(stmt3);
	Code_For_Ast* res = new Code_For_Ast(ic_l, NULL);
	return *res;
}

///////// CALL AST

Code_For_Ast & Call_Ast::compile()
{
	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	Code_For_Ast* res;

	list<Symbol_Table_Entry*> ste_list = program_object.get_procedure_prototype(procedure_name)->get_formal_param_list().get_table();
	list<Ast*>::reverse_iterator it;
	list<Symbol_Table_Entry*>::reverse_iterator ste_it;
	int size = 0;
	for (it = actual_param_list.rbegin(), ste_it = ste_list.rbegin(); it != actual_param_list.rend(), ste_it != ste_list.rend(); it++, ste_it++){
		Code_For_Ast cfa = (*it)->compile();
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(cfa.get_reg());
		string* name = new string((*ste_it)->get_variable_name());
		Symbol_Table_Entry* newste = new Symbol_Table_Entry(*name, (*ste_it)->get_data_type(), (*ste_it)->get_lineno(), sp_ref);
		newste->set_symbol_scope(formal);
		newste->set_start_offset(-1 * size);
		Mem_Addr_Opd* result = new Mem_Addr_Opd(*(newste));
		Move_IC_Stmt* stmt;
		if ((*it)->get_data_type() == int_data_type){
			stmt = new Move_IC_Stmt(store, opd1, result);
			size += 4;
		}
		else if ((*it)->get_data_type() == double_data_type){
			stmt = new Move_IC_Stmt(store_d, opd1, result);
			size += 8;
		}
		newste->set_end_offset(-1 * size);
		ic_l.splice(ic_l.end(), cfa.get_icode_list());
		ic_l.push_back(stmt);
		cfa.get_reg()->reset_use_for_expr_result();
	}
	Control_Flow_IC_Stmt* call_stmt;
	call_stmt = new Control_Flow_IC_Stmt(jal, NULL, NULL, procedure_name, size);
	ic_l.push_back(call_stmt);
    Register_Descriptor* reg = NULL; 
	if (get_data_type() == int_data_type){
		reg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);
		Register_Addr_Opd* result = new Register_Addr_Opd(reg);
		Move_IC_Stmt* mstmt;
		mstmt = new Move_IC_Stmt(mov, opd1, result);
		ic_l.push_back(mstmt);
		res = new Code_For_Ast(ic_l, reg);
	}
	else if (get_data_type() == double_data_type){
		reg = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);
		Register_Addr_Opd* result = new Register_Addr_Opd(reg);
		Move_IC_Stmt* mstmt;
		mstmt = new Move_IC_Stmt(move_d, opd1, result);
		ic_l.push_back(mstmt);
		res = new Code_For_Ast(ic_l, reg);
	}
	else{
		res = new Code_For_Ast(ic_l, NULL);
	}
    if (reg != NULL && return_value_reg == NULL){
        reg->reset_use_for_expr_result();
    }
	return *res;
}

Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{

}

///////// RETURN AST

Code_For_Ast & Return_Ast::compile()
{
	Control_Flow_IC_Stmt* stmt;
	stmt = new Control_Flow_IC_Stmt(ret_inst, NULL, NULL, "epilogue_" + proc_name);
	list<Icode_Stmt*> ic_l = *(new list<Icode_Stmt*>);
	Code_For_Ast* res;
	Register_Descriptor* reg = NULL;
	if (get_data_type() == int_data_type){
		Code_For_Ast cfa = return_value->compile();
		reg = machine_desc_object.spim_register_table[v1];
		Register_Addr_Opd* result = new Register_Addr_Opd(reg);
		Move_IC_Stmt* mstmt;
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(cfa.get_reg());
		mstmt = new Move_IC_Stmt(mov, opd1, result);
		ic_l.splice(ic_l.end(), cfa.get_icode_list());
		ic_l.push_back(mstmt);
		cfa.get_reg()->reset_use_for_expr_result();
	}
	else if (get_data_type() == double_data_type){
		Code_For_Ast cfa = return_value->compile();
		reg = machine_desc_object.spim_register_table[f0];
		Register_Addr_Opd* result = new Register_Addr_Opd(reg);
		Move_IC_Stmt* mstmt;
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(cfa.get_reg());
		mstmt = new Move_IC_Stmt(move_d, opd1, result);
		ic_l.splice(ic_l.end(), cfa.get_icode_list());
		ic_l.push_back(mstmt);
		cfa.get_reg()->reset_use_for_expr_result();
	}
	ic_l.push_back(stmt);
	res = new Code_For_Ast(ic_l, reg);
	return *res;
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{

}
