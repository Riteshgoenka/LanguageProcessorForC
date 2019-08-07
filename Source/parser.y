%{
	#include <stdio.h>
	extern "C" void yyerror(char*);
	extern int yylex(void);	
	extern int yylineno;
	Symbol_Table* symbol_table = new Symbol_Table();
	int dummy_counter;
	string curr_proc_name;
	Procedure* curr_proc;
%}


%union {
	int integer_value;
	double double_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	Procedure * procedure;
	Data_Type data_type;
}

%token <integer_value> INTEGER_NUMBER
%token <double_value> DOUBLE_NUMBER
%token <string_value> NAME
%token ASSIGN
%token INTEGER
%token FLOAT
%token VOID
%token WHILE
%token DO
%token IF
%token ELSE
%token LT
%token LTE
%token GT
%token GTE
%token E
%token NE
%token AND
%token OR
%token NOT
%token PRINT
%token RETURN


%right '?'
%left OR
%left AND
%left NOT
%left E NE
%left LT LTE GT GTE
%left '+' '-'
%left '*' '/'
%right '('
%nonassoc THEN
%nonassoc ELSE

%type <symbol_table> optional_variable_declaration_list global_variable_declaration_list variable_declaration_list global_variable_declaration variable_declaration global_declaration declaration variable_list arg_list named_arg_list unnamed_arg_list named_arg
%type <procedure> procedure_definition func_declaration
%type <ast_list> statement_list actual_arg_list full_actual_arg_list
%type <ast> variable constant assignment_statement print_statement arithmetic_expression sequence statement relational_expression logical_expression selection_statement iteration_statement return_statement func_statement
%type <data_type> data_type
%type <string_value> func_name

%start program

%%

program								: global_variable_declaration_list procedure_definition_list
										{
											// program_object.set_procedure($2, yylineno);
										}
									| procedure_definition_list
										{
											// program_object.set_procedure($1, yylineno);
										}
									;

procedure_definition_list			: procedure_definition_list procedure_definition
										{

										}
									| procedure_definition
										{

										}
									;

procedure_definition				: data_type NAME '(' arg_list ')' '{' optional_variable_declaration_list 
										{
											// $4->append_list(*($7), yylineno);
											// symbol_table = $4;
											// symbol_table->append_list(*($7), yylineno);
											string* name = new string(*($2));
											if (*name != "main"){
												*name = *name + "_";
											}
											if (program_object.is_procedure_exists(*name)){
												Procedure* p = program_object.get_procedure_prototype(*name);
												if (p->get_return_type() != $1){
													cerr << "cs316: Error: return type in declaration and definition not matching\n";
													exit(1);
												}
											}
											
											($7)->set_table_scope(local);
											($4)->set_table_scope(formal);
											curr_proc = new Procedure($1, *name, yylineno);
											curr_proc->set_local_list(*($7));
											curr_proc->set_formal_param_list(*($4));
											program_object.set_proc_to_map(*name, curr_proc);
											curr_proc_name = *($2);
											if (*($2) != "main"){
												curr_proc_name = curr_proc_name + "_";
											}
										}		
									  statement_list '}'
										{

											$$ = curr_proc;
									  		($<procedure>$)->set_ast_list(*($9));
									  		string* name = new string(*($2));
											if (*name != "main"){
												*name = *name + "_";
											}
											program_object.set_proc_to_map(*name, $$);
										}
                         			;

optional_variable_declaration_list	: variable_declaration_list
									| 
										{
											$$ = new Symbol_Table();
										}
									;

global_variable_declaration_list	: global_variable_declaration
										{
											$$ = $1;
                                            ($$)->set_table_scope(global);
											program_object.set_global_table(*($$));
											// cout << program_object.variable_in_symbol_list_check("i1_") << "jflkdjf\n";
										}
									| global_variable_declaration_list global_variable_declaration
										{
											($1)->append_list(*($2), yylineno);
                                            $$ = $1;
                                            ($$)->set_table_scope(global);
											program_object.set_global_table(*($$));
										}
									;

variable_declaration_list			: variable_declaration
										{
											$$ = $1;
										}
									| variable_declaration_list variable_declaration
										{
											($1)->append_list(*($2), yylineno);
                                            $$ = $1;
										}
									;

variable_declaration				: declaration ';'
									;

declaration							: data_type variable_list
										{
											$$ = $2;
										}
                                    ;

global_variable_declaration			: global_declaration ';'
									;

global_declaration					: data_type variable_list
										{
											$$ = $2;
										}
									| func_declaration
										{
											$$ = new Symbol_Table();
										}
                                    ;

func_declaration					: data_type NAME '(' arg_list ')'
										{
											string* name = new string(*($2));
											if (*name != "main"){
												*name = *name + "_";
											}
											$$ = new Procedure($1, *name, yylineno);
											($$)->set_formal_param_list(*($4));
											program_object.set_proc_to_map(*name, $$);
										}
									;

data_type							: INTEGER 
										{
											$$ = int_data_type;
										}
									| FLOAT
										{
											$$ = double_data_type;
										}
									| VOID
										{
											$$ = void_data_type;
										}
									;

arg_list							: named_arg_list
										{

										}
									| unnamed_arg_list
										{

										}
									| 
										{
											$$ = new Symbol_Table();
										}
									;

named_arg_list						: named_arg_list ',' named_arg
										{
											($1)->append_list(*($3), yylineno);
										}
									| named_arg
										{
											$$ = $1;
										}
									;

named_arg 							: data_type NAME
										{
											string* name = new string(*($2) + "_");
                                    		Symbol_Table_Entry* ste = new Symbol_Table_Entry(*(name), $1, yylineno);
                                    		ste->set_symbol_scope(formal);
                                    		$$ = new Symbol_Table();
                                    		($$)->push_symbol(ste);
										}
									;

unnamed_arg_list					: unnamed_arg_list ',' data_type
										{
											dummy_counter++;
											string* name = new string("dummy" + to_string(dummy_counter));
                                    		Symbol_Table_Entry* ste = new Symbol_Table_Entry(*(name), $3, yylineno);
                                    		ste->set_symbol_scope(formal);
											($1)->push_symbol(ste);
										}
									| data_type
										{
											dummy_counter = 1;
											string* name = new string("dummy" + to_string(dummy_counter));
                                    		Symbol_Table_Entry* ste = new Symbol_Table_Entry(*(name), $1, yylineno);
                                    		ste->set_symbol_scope(formal);
                                    		$$ = new Symbol_Table();
                                    		($$)->push_symbol(ste);
										}
									;

variable_list                       : NAME
										{
											$1 = new string(*($1) + "_");
											// cout << *($1) << endl;
											$$ = new Symbol_Table();
                                    		Symbol_Table_Entry* ste = new Symbol_Table_Entry(*($1), $<data_type>0, yylineno);
                                    		// ste->set_symbol_scope(local);
                                    		($$)->push_symbol(ste);

                                    	}
                                    | variable_list ',' NAME
                                    	{
                                    		$3 = new string(*($3) + "_");
                                    		Symbol_Table_Entry* ste = new Symbol_Table_Entry(*($3), $<data_type>0, yylineno);
                                    		// ste->set_symbol_scope(local);
                                    		Symbol_Table* st = new Symbol_Table();
                                    		(st)->push_symbol(ste);
                                    		($1)->append_list(*st, yylineno);
                                    		$$ = $1;
                                    	}
									;

statement_list						: statement_list statement
										{
											($1)->push_back($2);
											$$ = $1;
										}
									| 
										{
											$$ = new list<Ast*>();
										}
									;

sequence							: statement_list
										{
											list<Ast*>::iterator it;
											if (($1)->begin() == ($1)->end()){
												cerr << "cs316: Error: Block of statements cannot be empty\n";
												exit(1);
											}
											$$ = new Sequence_Ast(yylineno);
											for (it = ($1)->begin(); it != ($1)->end(); it++){
												((Sequence_Ast*)$$)->ast_push_back(*it);
											}
										}
									;

statement 							: assignment_statement
										{
											$$ = $1;
										}
									| iteration_statement
										{
											$$ = $1;
										}
									| selection_statement
										{
											$$ = $1;
										}
									| print_statement
										{
											$$ = $1;
										}
									| return_statement
										{
											$$ = $1;
										}
									| func_statement
										{
                                            ((Call_Ast*)$1)->set_register(NULL);
											$$ = $1;
										}
									| '{' sequence '}'
										{
											$$ = $2;
										}
									;

assignment_statement				: variable ASSIGN arithmetic_expression ';'
										{
											$$ = new Assignment_Ast($1, $3, yylineno);
											$$->check_ast();
										}
									| variable ASSIGN func_statement
										{
											Register_Descriptor* rd = new Register_Descriptor();
											((Call_Ast*)$3)->set_register(rd);
											$$ = new Assignment_Ast($1, $3, yylineno);
											$$->check_ast();
										}
									;

func_statement 						: func_name '(' full_actual_arg_list ')' ';' 
										{
											$$ = new Call_Ast(*($1), yylineno);
											((Call_Ast*)$$)->set_actual_param_list(*($3));
											((Call_Ast*)$$)->check_actual_formal_param(program_object.get_procedure_prototype(*($1))->get_formal_param_list());
										}

full_actual_arg_list				: actual_arg_list
										{
											$$ = $1;
										}
									| 
										{
											$$ = new list<Ast*>;
										}

actual_arg_list 					: arithmetic_expression
										{
											$$ = new list<Ast*>;
											($$)->push_back($1);
										}
									| actual_arg_list ',' arithmetic_expression
										{
											$$ = $1;
											($$)->push_back($3);
										}


print_statement 					: PRINT variable ';'
										{
											$$ = new Print_Ast($2, yylineno);
											$$->check_ast();
										}

return_statement					: RETURN ';'
										{
											$$ = new Return_Ast(NULL, curr_proc_name, yylineno);
										}
									| RETURN arithmetic_expression ';'
										{
											$$ = new Return_Ast($2, curr_proc_name, yylineno);
										}

variable							: NAME
										{
											$1 = new string(*($1) + "_");
											if (curr_proc->variable_in_symbol_list_check(*($1))){
												$$ = new Name_Ast(*($1), curr_proc->get_symbol_table_entry(*($1)), yylineno);
											}
											else if (curr_proc->variable_in_formal_list_check(*($1))){
												$$ = new Name_Ast(*($1), curr_proc->get_formal_param_entry(*($1)), yylineno);
											}
											else if (program_object.variable_in_symbol_list_check(*($1))) {
												// cout << "global " << *($1) << endl;
												$$ = new Name_Ast(*($1), program_object.get_symbol_table_entry(*($1)), yylineno);
											}
											else {
												// cout << "undeclared " << *($1) << endl;
												fprintf(stderr, "cs316: Error: Line: %d: Variable has not been declared\n", yylineno);
												exit(1);
											}
										}
									;

func_name 							: NAME
										{
											$$ = new string(*($1) + "_");
											program_object.get_procedure_prototype(*($$));
										}

constant							: INTEGER_NUMBER
										{
											$$ = new Number_Ast<int>($1, int_data_type, yylineno);
										}
									| DOUBLE_NUMBER
										{
											$$ = new Number_Ast<double>($1, double_data_type, yylineno);
										}
									;

arithmetic_expression 				: constant
									| variable
									| arithmetic_expression '+' arithmetic_expression
										{
											$$ = new Plus_Ast($1, $3, yylineno);
											$$->check_ast();
										}
									| arithmetic_expression '-' arithmetic_expression
										{
											$$ = new Minus_Ast($1, $3, yylineno);
											$$->check_ast();
										}
									| arithmetic_expression '/' arithmetic_expression
										{
											$$ = new Divide_Ast($1, $3, yylineno);
											$$->check_ast();
										}
									| arithmetic_expression '*' arithmetic_expression
										{
											$$ = new Mult_Ast($1, $3, yylineno);
											$$->check_ast();
										}
									| '(' arithmetic_expression ')'
										{
											$$ = $2;
											$$->check_ast();
										}
									| '-' arithmetic_expression %prec '*'
										{
											$$ = new UMinus_Ast($2, NULL, yylineno);
											$$->check_ast();
										}
									| logical_expression '?' arithmetic_expression ':' arithmetic_expression %prec '?'
										{
											$$ = new Conditional_Expression_Ast($1, $3, $5, yylineno);
											$$->check_ast();
										}
									;

relational_expression				: arithmetic_expression LT arithmetic_expression
										{
											$$ = new Relational_Expr_Ast($1, less_than, $3, yylineno);
											$$->check_ast();
										}
									| arithmetic_expression LTE arithmetic_expression
										{
											$$ = new Relational_Expr_Ast($1, less_equalto, $3, yylineno);
											$$->check_ast();
										}
									| arithmetic_expression GT arithmetic_expression
										{
											$$ = new Relational_Expr_Ast($1, greater_than, $3, yylineno);
											$$->check_ast();
										}
									| arithmetic_expression GTE arithmetic_expression
										{
											$$ = new Relational_Expr_Ast($1, greater_equalto, $3, yylineno);
											$$->check_ast();
										}
									| arithmetic_expression E arithmetic_expression
										{
											$$ = new Relational_Expr_Ast($1, equalto, $3, yylineno);
											$$->check_ast();
										}
									| arithmetic_expression NE arithmetic_expression
										{
											$$ = new Relational_Expr_Ast($1, not_equalto, $3, yylineno);
											$$->check_ast();
										}
									;

logical_expression					: relational_expression
										{
											$$ = $1;
											$$->check_ast();
										}
									| logical_expression AND logical_expression
										{
											$$ = new Logical_Expr_Ast($1, _logical_and, $3, yylineno);
											$$->check_ast();
										}
									| logical_expression OR logical_expression
										{
											$$ = new Logical_Expr_Ast($1, _logical_or, $3, yylineno);
											$$->check_ast();
										}
									| NOT logical_expression
										{
											Number_Ast<int>* one = new Number_Ast<int>(1, int_data_type, yylineno);
											$$ = new Logical_Expr_Ast(one, _logical_not, $2, yylineno);
											$$->check_ast();
										}
									| '(' logical_expression ')'
										{
											$$ = $2;
											$$->check_ast();
										}

selection_statement					: IF '(' logical_expression ')' statement %prec THEN
										{
											$$ = new Selection_Statement_Ast($3, $5, NULL, yylineno);
											$$->check_ast();
										}
									| IF '(' logical_expression ')' statement ELSE statement 
										{
											$$ = new Selection_Statement_Ast($3, $5, $7, yylineno);
											$$->check_ast();
										}
									;

iteration_statement					: WHILE '(' logical_expression ')' statement
										{
											$$ = new Iteration_Statement_Ast($3, $5, yylineno, false);
											$$->check_ast();
										}
									| DO statement WHILE '(' logical_expression ')' ';'
										{
											$$ = new Iteration_Statement_Ast($5, $2, yylineno, true);
											$$->check_ast(); 
										}
									;

%%
