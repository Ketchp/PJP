```
Mila -> Header Mila_
Mila_ -> Var_or_const_declaration           Mila_
Mila_ -> Function_declaration_or_definition Mila_
Mila_ -> Main

Header -> tok_PROGRAM tok_identifier tok_SEMICOLON
Main -> Begin_body_end tok_doT Function_definitions

Var_or_const_declaration -> Var_declaration
Var_or_const_declaration -> Const_declaration

Var_declaration       -> tok_VAR Var_declaration_line Var_declaration_lines
Var_declaration_lines -> 
Var_declaration_lines -> Var_declaration_line Var_declaration_lines

Const_declaration       -> tok_CONST Const_declaration_line Const_declaration_lines
Const_declaration_lines -> 
Const_declaration_lines -> Const_declaration_line Const_declaration_lines

Var_declaration_line     -> tok_identifier Comma_identifiers Var_declaration_line_end
Var_declaration_line_end -> tok_COLON Type tok_SEMICOLON

Const_declaration_line     -> tok_identifier Comma_identifiers Const_declaration_line_end
Const_declaration_line_end -> tok_equal Literal tok_SEMICOLON

Comma_identifiers -> 
Comma_identifiers -> tok_comma tok_identifier Comma_identifiers


Function_definitions -> 
Function_definitions -> Function_definition Function_definitions

Function_definition -> Function_head tok_SEMICOLON Function_definition_



Function_declaration_or_definition -> Function_head Function_declaration_or_definition_


Function_declaration_or_definition_ -> tok_FORWARD tok_SEMICOLON
Function_declaration_or_definition_ -> tok_SEMICOLON Function_definition_ 

Function_definition_ -> Var_declaration Begin_body_end tok_SEMICOLON
Function_definition_ -> Begin_body_end tok_SEMICOLON

Function_head -> tok_FUNCTION  tok_identifier tok_BRACE_L Function_parameters tok_BRACE_R tok_COLON Trivial_type tok_SEMICOLON
Function_head -> tok_PROCEDURE tok_identifier tok_BRACE_L Function_parameters tok_BRACE_R                        tok_SEMICOLON

Function_parameters -> 
Function_parameters -> Function_parameter Function_parameters_
Function_parameters_ -> 
Function_parameters_ -> tok_comma Function_parameters_
Function_parameter -> tok_identifier tok_COLON Type


Begin_body_end -> tok_BEGIN Statements tok_END
Statements -> 
Statements -> Statement Statements_
Statements_ ->
Statements_ -> tok_SEMICOLON Statements

Statement -> While_statement
Statement -> If_statement
Statement -> For_statement
Statement -> Expression
Statement -> tok_exit


While_statement -> tok_while Expression tok_do Scope

If_statement    -> tok_if    Expression tok_THEN Scope Else_statement tok_endif
Else_statement  -> 
Else_statement  -> tok_ELSE Scope

For_statement    -> tok_FOR    Expression For_statement_
For_statement_   -> tok_TO     Expression tok_do Scope
For_statement_   -> tok_downto Expression tok_do Scope

Scope -> Begin_body_end
Scope -> Statement

Expression -> L9

L9  -> L8 L9_
L9_ -> 
L9_ -> tok_assign L9

L8  -> L7 L8_ 
L8_ ->
L8_ -> tok_or L7 L8_

L7  -> L6 L7_
L7_ ->
L7_ -> tok_and L6 L7_

L6  -> L5 L6_
L6_ ->
L6_ -> tok_equal    L5 L6_
L6_ -> tok_not_equal L5 L6_

L5  -> L4 L5_
L5_ -> 
L5_ -> tok_LESS          L4 L5_
L5_ -> tok_GREATER       L4 L5_
L5_ -> tok_less_equal    L4 L5_
L5_ -> tok_greater_equal L4 L5_

L4  -> L3 L4_
L4_ -> 
L4_ -> tok_plus  L3 L4_
L4_ -> tok_minus L3 L4_

L3  -> L2 L3_
L3_ ->
L3_ -> tok_STAR L2 L3_
L3_ -> tok_DIV  L2 L3_
L3_ -> tok_MOD  L2 L3_

L2 -> L1
L2 -> tok_plus  L2
L2 -> tok_minus L2
L2 -> tok_NOT   L2
L2 -> tok_BRACE_L L2_
L2_ -> Type tok_BRACE_R L2
L2_ -> L9 tok_BRACE_R L1_

L1  -> tok_identifier L1_
L1  -> tok_literal L1_
L1_ ->
L1_ -> tok_BRACE_L Arguments tok_BRACE_R L1_
L1_ -> tok_SQUARE_BRACE_L L9 tok_SQUARE_BRACE_R L1_

Arguments  ->
Arguments  -> L9 Arguments_
Arguments_ ->
Arguments_ -> tok_comma L9 Arguments_

Type -> Trivial_type
Type -> Array_type
Array_type -> tok_array tok_SQUARE_BRACE_L tok_lit_int tok_dotdot tok_lit_int tok_SQUARE_BRACE_R tok_OF Trivial_type
Trivial_type -> tok_INTEGER
Trivial_type -> tok_REAL
Trivial_type -> tok_STRING




// miscelanious
literal        = string_literal | number_literal
number_literal = int_literal | real_literal

decimal_digit           = "0" - "9" .
letter                  = ("a" - "z") | ("A" - "Z") .
identifier_symbol_first = letter | "_" .
identifier_symbol       = identifier_symbol_first | decimal_digit .

identifier = identifier_symbol_first {identifier_symbol} .

```
