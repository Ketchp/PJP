```
mila   = header
         { var_or_const_declaration | function_declaration_or_definition }
         main
         { function_definition } .

header = "program" identifier ";" .
main   = begin_body_end "." .

// variables and constants
var_or_const_declaration = var_declaration | const_declaration .
var_declaration    = "var"   var_declaration_line   {var_declaration_line} .
const_declaration  = "const" const_declaration_line { const_declaration_line } .
var_declaration_line   = identifier {"," identifier} ":" type ";" .
const_declaration_line = identifier {"," identifier} "=" literal ";" .

// functions / procedures
function_declaration_or_definition = function_declaration | function_definition .
function_declaration = function_head "forward" ";" .
function_definition = function_head ";" [var_declaration] begin_body_end ";" .
function_head = "function"  identifier "(" [ function_parameters ] ")" ":" trivial_type ";" |
                "procedure" identifier "(" [ function_parameters ] ")"                  ";" .
function_parameters = function_parameter {"," function_parameter}
function_parameter = identifier ":" type

// statements
begin_body_end = "begin" { statement } "end" .
statement     = while_statement | if_statement | for_statement | expression ";" .
while_statement = "while" expression "do"   scope .
if_statement    = "if"    expression "then" scope [ else scope ] .
for_statement   = "for" assignment ( "to" | "downto" ) expression "do" scope .
scope = ( begin_body_end | expression ) ";" .


// expressions
expression = L9

L9 = L8 | L8 ":=" L9 .
L8 = L7 | L8 "or" L7 .
L7 = L6 | L7 "and" L6 .
L6 = L5 | L6 ( "=" | "<>" ) L5 .
L5 = L4 | L5 ( "<" | ">" | "<=" | ">=" ) L4 .
L4 = L3 | L4 ( "+" | "-" ) L3 .
L3 = L2 | L3 ( "*" | "div" | "mod" ) L2 .
L2 = L1 | ( "+" | "-" | "not" | "(" type ")" ) L2 .
L1 = "(" L9 ")" |
     identifier |
     literal    |
     L1 "(" [ L9 { "," L9 } ] ")" |  // todo
     L1 "[" L9 "]" .

// types
type = trivial_type | array_type .
array_type = "array" "[" int_literal ".." int_literal "]" "of" trivial_type .
trivial_type = "Integer" | "Real" | "String" .

// miscelanious
literal        = string_literal | number_literal
number_literal = int_literal | real_literal

decimal_digit           = "0" - "9" .
letter                  = ("a" - "z") | ("A" - "Z") .
identifier_symbol_first = letter | "_" .
identifier_symbol       = identifier_symbol_first | decimal_digit .

identifier = identifier_symbol_first {identifier_symbol} .



```


```
Operator precedence variables

+--------------+--------------+-----------------+-----------------+
|  Precedence  |   Operator   |   Description   |  Associativity  |
+--------------+--------------+-----------------+-----------------+
|      L1      | ()           | function call   | Left-to-right   |
|              | []           | subscript       |                 |
+--------------+--------------+-----------------+-----------------+
|              | +, -         | unary +/-       |                 |
|      L2      | not          | logical not     | Right-to-left   |
|              | (type)       | type cast       |                 |
+--------------+--------------+-----------------+-----------------+
|              | *            | multiplication  |                 |
|      L3      | div          | division        |                 |
|              | mod          | modulo          |                 |
+--------------+--------------+-----------------+                 |
|      L4      | +, -         | plus, minus     |                 |
+--------------+--------------+-----------------+                 |
|      L5      | <, >, <=, >= | comparison      | Left-to-right   |
+--------------+--------------+-----------------+                 |
|      L6      | =            | equality        |                 |
|              | <>           | not equality    |                 |
+--------------+--------------+-----------------+                 |
|      L7      | and          | logical and     |                 |
+--------------+--------------+-----------------+                 |
|      L8      | or           | logical or      |                 |
+--------------+--------------+-----------------+-----------------+
|      L9      | :=           | assignment      | Right-to-left   |
+--------------+--------------+-----------------+-----------------+


```
