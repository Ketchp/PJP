#include "Parser.hpp"
#include "first_follow.hpp"

#include <map>


Parser::Parser()
    : current_token{lexer.get_token()}
{}


template<>
Token Parser::NO_MATCH<true>() {
    throw_error("todo");
}

template<>
Token Parser::NO_MATCH<false>() {
    return Token::NO_MATCH();
}


std::unique_ptr<Mila> Parser::parse_mila() {
    match(TokenType::TOK_PROGRAM);

    auto program_name =
            std::get<Mila_string_T>(std::move(
                            match(TokenType::TOK_IDENTIFIER).data
            ));

    auto mila = std::make_unique<Mila>(program_name);
    current_table = mila->function_type->local_variables;

    {
        Identifier writeln{"writeln"};

        current_table->add_function(
                std::make_shared<Function>(
                    Type::INT(),
                    writeln,
                    parameter_list_T{{Type::INT(), Identifier{"x"}}},
                    current_table
                )
        );
    }
    {
        Identifier write_str{"write_str"};

        current_table->add_function(
                std::make_shared<Function>(
                        Type::INT(),
                        write_str,
                        parameter_list_T{{Type::STRING(), Identifier{"x"}}},
                        current_table
                )
        );
    }
    {
        Identifier readln{"readln"};

        current_table->add_function(
                std::make_shared<Function>(
                        Type::INT(),
                        readln,
                        parameter_list_T{{Type::VOID(), Identifier{"x"}}},
                        current_table
                )
        );
    }
    {
        Identifier dec{"dec"};

        current_table->add_function(
                std::make_shared<Function>(
                        Type::VOID(),
                        dec,
                        parameter_list_T{{Type::INT(), Identifier{"x"}}},
                        current_table
                )
        );
    }

    match(TokenType::TOK_SEMICOLON);

    while(true) {
        if(peek({TokenType::TOK_VAR, TokenType::TOK_CONST})) {
            parse_variables(*mila);
        }
        else if(peek({TokenType::TOK_FUNCTION, TokenType::TOK_PROCEDURE})) {
            parse_function(*mila);
        }
        else if(peek(TokenType::TOK_BEGIN)) {
            parse_implementation(*mila);
            match(TokenType::TOK_DOT);
        }
        else {
            match(TokenType::TOK_EOF);
            if(mila->implementation == nullptr)
                throw_error("todo");
            return mila;
        }
    }
}




std::unique_ptr<Statements> Parser::parse_scope() {
    if(try_match(TokenType::TOK_BEGIN)) {
        auto statements = parse_statements();
        match(TokenType::TOK_END);
        return statements;
    }

    return std::make_unique<Statements>(parse_expression());
}

std::unique_ptr<WhileStatement> Parser::parse_while_statement() {
    match(TokenType::TOK_WHILE);
    auto condition = parse_expression();
    match(TokenType::TOK_DO);
    auto body = parse_scope();

    return std::make_unique<WhileStatement>(std::move(condition),
                                            std::move(body));
}

std::unique_ptr<ForStatement> Parser::parse_for_statement() {
    match(TokenType::TOK_FOR);
    auto start = parse_expression();
    auto direction = match({TokenType::TOK_TO, TokenType::TOK_DOWNTO}).type;
    auto stop = parse_expression();
    match(TokenType::TOK_DO);
    auto body = parse_scope();

    return std::make_unique<ForStatement>(std::move(start),
                                          std::move(stop),
                                          std::move(body),
                                          direction);
}



std::unique_ptr<IfStatement> Parser::parse_if_statement() {
    match(TokenType::TOK_IF);
    auto condition = parse_expression();
    match(TokenType::TOK_THEN);
    auto body_if = parse_scope();

    if(try_match(TokenType::TOK_ELSE))
        return std::make_unique<IfStatement>(std::move(condition),
                                             std::move(body_if),
                                             parse_scope());
    return std::make_unique<IfStatement>(std::move(condition),
                                         std::move(body_if));
}

std::unique_ptr<ExitStatement> Parser::parse_exit_statement() {
    match(TokenType::TOK_EXIT);
    if(current_table->variables.contains(RETURN_VALUE_IDENTIFIER))
        return std::make_unique<ExitStatement>(&current_table->variables.at(RETURN_VALUE_IDENTIFIER));
    return std::make_unique<ExitStatement>();
}


std::unique_ptr<Statement> Parser::parse_statement() {
    if(peek(TokenType::TOK_WHILE))
        return parse_while_statement();
    if(peek(TokenType::TOK_IF))
        return parse_if_statement();
    if(peek(TokenType::TOK_FOR))
        return parse_for_statement();
    if(peek(TokenType::TOK_EXIT))
        return parse_exit_statement();
    if(peek(first::Expression))
        return parse_expression();
    Parser::throw_error("TODO");
}

std::unique_ptr<Statements> Parser::parse_statements() {
    Statements::statement_vector_T statements;

    do {
        if(peek(first::Statement))
            statements.emplace_back(parse_statement());
        else
            break;
    } while(try_match(TokenType::TOK_SEMICOLON));

    return std::make_unique<Statements>(std::move(statements));
}

void Parser::parse_variables(Function &function) {
    if(peek(TokenType::TOK_VAR))
        parse_variables_var(function);
    else if(peek(TokenType::TOK_CONST))
        parse_variables_const(function);
    else
        throw_error("TODO");
}

void Parser::parse_variables_var(Function &function) {
    match(TokenType::TOK_VAR);

    do {
        auto identifiers = parse_identifier_list();

        match(TokenType::TOK_COLON);

        function.function_type->local_variables->add_var(identifiers, parse_type());

        match(TokenType::TOK_SEMICOLON);
    } while(peek(TokenType::TOK_IDENTIFIER));
}

void Parser::parse_variables_const(Function &function) {
    match(TokenType::TOK_CONST);

    do {
        auto identifiers = parse_identifier_list();

        match(TokenType::TOK_EQUAL);

        try {
            Mila_variant_T value = parse_expression()->get_value();

            function.function_type->local_variables->add_const(identifiers, value);
        } catch (const std::bad_cast &) {
            throw_error("TODO");
        }

        match(TokenType::TOK_SEMICOLON);
    } while(peek(TokenType::TOK_IDENTIFIER));
}


std::vector<Identifier> Parser::parse_identifier_list() {
    std::vector<Identifier> identifiers;

    do {
        identifiers.emplace_back(match(TokenType::TOK_IDENTIFIER));
    } while(try_match(TokenType::TOK_COMMA));

    return identifiers;
}

// function param is not function we parse but parent function of function we are going to parse
void Parser::parse_function(Function &function) {
    bool is_procedure = false;

    if(try_match(TokenType::TOK_PROCEDURE))
        is_procedure = true;
    else
        match(TokenType::TOK_FUNCTION);

    auto function_name = Identifier(match(TokenType::TOK_IDENTIFIER));  // function_name

    auto parameters = parse_parameters();  // (id1: type; id2: type; ...)

    std::shared_ptr<Type> return_type;

    if(is_procedure) {
        return_type = Type::VOID();
    }
    else {
        match(TokenType::TOK_COLON);
        return_type = parse_type();
    }

    match(TokenType::TOK_SEMICOLON);

    auto new_function = std::make_shared<Function>(
            return_type,
            function_name,
            parameters,
            current_table
    );

    new_function = function.function_type->local_variables->add_function(new_function);

    // forward definition
    if(try_match(TokenType::TOK_FORWARD)) {
        match(TokenType::TOK_SEMICOLON);
        return;
    }

    if(peek({TokenType::TOK_VAR, TokenType::TOK_CONST})) {
        parse_variables(*new_function);
    }

    // set local variables symbol variables
    current_table = new_function->function_type->local_variables;

    parse_implementation(*new_function);

    // revert to global variables
    current_table = current_table->global_table;

    match(TokenType::TOK_SEMICOLON);
}

void Parser::parse_implementation(Function &function) {
    match(TokenType::TOK_BEGIN);
    function.implementation = parse_statements();
    match(TokenType::TOK_END);
}


// Integer | Real | String
std::unique_ptr<Type> Parser::parse_trivial_type() {
    return std::make_unique<Type>(match(first::trivial_type).type);
}

// array [<LIT_INT> .. <LIT_INT>] of <TYPE>
std::unique_ptr<TypeArray> Parser::parse_type_array() {
    match(TokenType::TOK_ARRAY);
    match(TokenType::TOK_SQUARE_BRACE_L);
    auto start = parse_literal_integer();
    match(TokenType::TOK_DOT_DOT);
    auto end = parse_literal_integer();
    match(TokenType::TOK_SQUARE_BRACE_R);
    match(TokenType::TOK_OF);

    return std::make_unique<TypeArray>(parse_trivial_type(), start, end);
}

Mila_int_T Parser::parse_literal_integer() {
    Mila_int_T negative = 1;
    if(try_match(TokenType::TOK_MINUS))
        negative = -1;

    return negative * std::get<Mila_int_T>(match(TokenType::TOK_LITERAL_INT).data);
}

std::unique_ptr<Type> Parser::parse_type() {
    if(peek(TokenType::TOK_ARRAY))
        return parse_type_array();
    return parse_trivial_type();
}

void Parser::parse_parameter(parameter_list_T &parameters) {
    auto identifier = match(TokenType::TOK_IDENTIFIER);
    match(TokenType::TOK_COLON);
    parameters.emplace_back(parse_type(), identifier);
}

parameter_list_T Parser::parse_parameters() {
    parameter_list_T parameters;

    match(TokenType::TOK_BRACE_L);

    if(peek(TokenType::TOK_IDENTIFIER))
        parse_parameter(parameters);

    while(try_match(TokenType::TOK_SEMICOLON))
        parse_parameter(parameters);

    match(TokenType::TOK_BRACE_R);

    return parameters;
}

std::vector<std::unique_ptr<Expression>> Parser::parse_arguments() {
    std::vector<std::unique_ptr<Expression>> arguments;

    match(TokenType::TOK_BRACE_L);
    if(try_match(TokenType::TOK_BRACE_R))
        return {};

    do {
        arguments.emplace_back(parse_expression());
    } while(try_match(TokenType::TOK_COMMA));

    match(TokenType::TOK_BRACE_R);
    return arguments;
}


std::unique_ptr<LiteralExpression> Parser::parse_literal_expression() {
    return std::make_unique<LiteralExpression>(match({
        TokenType::TOK_LITERAL_STRING,
        TokenType::TOK_LITERAL_INT,
        TokenType::TOK_LITERAL_REAL,
    }));
}


std::unique_ptr<Expression> Parser::parse_subscript() {
    match(TokenType::TOK_SQUARE_BRACE_L);
    auto index = parse_L9();
    match(TokenType::TOK_SQUARE_BRACE_R);
    return index;
}


std::unique_ptr<Expression> Parser::parse_expression() {
    return parse_L9();
}

std::unique_ptr<Expression> Parser::parse_L9() {
    return parse_L9_(parse_L8());
}

std::unique_ptr<Expression> Parser::parse_L8() {
    return parse_L8_(parse_L7());
}

std::unique_ptr<Expression> Parser::parse_L7() {
    return parse_L7_(parse_L6());
}

std::unique_ptr<Expression> Parser::parse_L6() {
    return parse_L6_(parse_L5());
}

std::unique_ptr<Expression> Parser::parse_L5() {
    return parse_L5_(parse_L4());
}

std::unique_ptr<Expression> Parser::parse_L4() {
    return parse_L4_(parse_L3());
}

std::unique_ptr<Expression> Parser::parse_L3() {
    return parse_L3_(parse_L2());
}

std::unique_ptr<Expression> Parser::parse_L2() {
    if(try_match(TokenType::TOK_PLUS))
        return std::make_unique<UnaryExpression<UnaryOp::PLUS>>(parse_L2());
    if(try_match(TokenType::TOK_MINUS))
        return std::make_unique<UnaryExpression<UnaryOp::MINUS>>(parse_L2());
    if(try_match(TokenType::TOK_NOT))
        return std::make_unique<UnaryExpression<UnaryOp::NOT>>(parse_L2());
    if(peek(TokenType::TOK_BRACE_L))
        return parse_L2_();
    if(peek(literal_or_identifier))
        return parse_L1();
    Parser::throw_error(std::to_string(__LINE__) + " TODO");
}

std::unique_ptr<Expression> Parser::parse_L1() {
    if(auto token = try_match(TokenType::TOK_IDENTIFIER))
        return parse_L1_(std::make_unique<IdentifierExpression>(Identifier{token}, current_table.get()));
    if(peek(literal))
        return parse_L1_(parse_literal_expression());
    Parser::throw_error("TODO");
}


std::unique_ptr<Expression> Parser::parse_L9_(std::unique_ptr<Expression> lhs) {
    if(peek(follow::L9_))
        return lhs;

    match(TokenType::TOK_ASSIGN);
    return std::make_unique<BinaryExpression<BinaryOp::ASSIGN>>(
            std::move(lhs),
            parse_L9()
    );
}

std::unique_ptr<Expression> Parser::parse_L8_(std::unique_ptr<Expression> lhs) {
    if(peek(follow::L8_))
        return lhs;

    match(TokenType::TOK_OR);
    return parse_L8_(
            std::make_unique<BinaryExpression<BinaryOp::OR>>(
                    std::move(lhs),
                    parse_L7()
            )
    );
}


std::unique_ptr<Expression> Parser::parse_L7_(std::unique_ptr<Expression> lhs) {
    if(peek(follow::L7_))
        return lhs;

    match(TokenType::TOK_AND);
    return parse_L7_(
            std::make_unique<BinaryExpression<BinaryOp::AND>>(
                    std::move(lhs),
                    parse_L6()
            )
    );
}


std::unique_ptr<Expression> Parser::parse_L6_(std::unique_ptr<Expression> lhs) {
    if(peek(follow::L6_))
        return lhs;

    auto op = match(first::L6_).type;
    auto rhs = parse_L5();

    if(op == TokenType::TOK_EQUAL)
        lhs = std::make_unique<BinaryExpression<BinaryOp::EQUAL>>(std::move(lhs), std::move(rhs));
    else if(op == TokenType::TOK_NOT_EQUAL)
        lhs = std::make_unique<BinaryExpression<BinaryOp::NOT_EQUAL>>(std::move(lhs), std::move(rhs));
    else unreachable();

    return parse_L6_(std::move(lhs));
}


std::unique_ptr<Expression> Parser::parse_L5_(std::unique_ptr<Expression> lhs) {
    if(peek(follow::L5_))
        return lhs;

    auto op = match(first::L5_).type;
    auto rhs = parse_L4();

    if(op == TokenType::TOK_LESS)
        lhs = std::make_unique<BinaryExpression<BinaryOp::LESS>>(std::move(lhs), std::move(rhs));
    else if(op == TokenType::TOK_LESS_EQUAL)
        lhs = std::make_unique<BinaryExpression<BinaryOp::LESS_EQUAL>>(std::move(lhs), std::move(rhs));
    else if(op == TokenType::TOK_GREATER)
        lhs = std::make_unique<BinaryExpression<BinaryOp::GREATER>>(std::move(lhs), std::move(rhs));
    else if(op == TokenType::TOK_GREATER_EQUAL)
        lhs = std::make_unique<BinaryExpression<BinaryOp::GREATER_EQUAL>>(std::move(lhs), std::move(rhs));
    else unreachable();

    return parse_L5_(std::move(lhs));
}

std::unique_ptr<Expression> Parser::parse_L4_(std::unique_ptr<Expression> lhs) {
    if(peek(follow::L4_))
        return lhs;

    auto op = match(first::L4_).type;
    auto rhs = parse_L3();

    if(op == TokenType::TOK_PLUS)
        lhs = std::make_unique<BinaryExpression<BinaryOp::PLUS>>(std::move(lhs), std::move(rhs));
    else if(op == TokenType::TOK_MINUS)
        lhs = std::make_unique<BinaryExpression<BinaryOp::MINUS>>(std::move(lhs), std::move(rhs));
    else unreachable();

    return parse_L4_(std::move(lhs));
}

std::unique_ptr<Expression> Parser::parse_L3_(std::unique_ptr<Expression> lhs) {
    if(peek(follow::L3_))
        return lhs;

    auto op = match(first::L3_).type;
    auto rhs = parse_L2();

    if(op == TokenType::TOK_STAR)
        lhs = std::make_unique<BinaryExpression<BinaryOp::STAR>>(std::move(lhs), std::move(rhs));
    else if(op == TokenType::TOK_DIV)
        lhs = std::make_unique<BinaryExpression<BinaryOp::DIV>>(std::move(lhs), std::move(rhs));
    else if(op == TokenType::TOK_MOD)
        lhs = std::make_unique<BinaryExpression<BinaryOp::MOD>>(std::move(lhs), std::move(rhs));
    else unreachable();

    return parse_L3_(std::move(lhs));
}




std::unique_ptr<Expression> Parser::parse_L2_() {
    match(TokenType::TOK_BRACE_L);  // L2_ always begins with "(type)L2" or "(L9)"

    if(peek(first::type)) {
        auto cast_type = parse_type();
        match(TokenType::TOK_BRACE_R);

        return std::make_unique<CastExpression>(std::move(cast_type), parse_L2());
    }
    if(peek(first::L2_parenthesis)) {
        auto lhs = parse_L9();
        match(TokenType::TOK_BRACE_R);

        return parse_L1_(std::move(lhs));
    }

    Parser::throw_error("TODO");
}


std::unique_ptr<Expression> Parser::parse_L1_(std::unique_ptr<Expression> lhs) {
    if(peek(follow::L1_))
        return lhs;

    if(peek(TokenType::TOK_BRACE_L))
        return parse_L1_(std::make_unique<CallExpression>(
                                 std::move(lhs),
                                 parse_arguments()
                         )
        );

    if(peek(TokenType::TOK_SQUARE_BRACE_L))
        return parse_L1_(std::make_unique<BinaryExpression<BinaryOp::SUBSCRIPT>>(
                                 std::move(lhs),
                                 parse_subscript()
                         )
        );

    Parser::throw_error("TODO");
}





std::shared_ptr<llvm::Module> Parser::generate() {
    try {
        return _generate();
    } catch (GeneratorError &err) {
        std::cerr << err.what() << std::endl;
    }

    return nullptr;
}


static std::set<std::string> builtins{
    // external fce.c
    "writeln",
    "write",
    "write_str",
    "readln",

    // main function
    "main",

    // helper functions
    "dec",
};

bool is_builtin_function(const Identifier &id) {
    return builtins.contains(id.identifier);
}

std::shared_ptr<llvm::Module> Parser::_generate()
{
    mila_context = std::make_shared<llvm::LLVMContext>();
    mila_builder = std::make_shared<llvm::IRBuilder<>>(*mila_context);
    mila_module = std::make_shared<llvm::Module>(program->program_name, *mila_context);

    // create writeln function
    {
      std::vector<llvm::Type*> Ints(1, llvm::Type::getInt32Ty(*mila_context));
      llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(*mila_context), Ints, false);
      llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "writeln", *mila_module);
      for (auto & Arg : F->args())
          Arg.setName("x");
    }
    // create write_str function
    {
        std::vector<llvm::Type*> char_ptr(1, llvm::Type::getInt8PtrTy(*mila_context));
        llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(*mila_context), char_ptr, false);
        llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "write_str", *mila_module);
        for (auto & Arg : F->args())
            Arg.setName("x");
    }
    // create readln function
    {
        std::vector<llvm::Type*> IntPtr(1, llvm::PointerType::get(llvm::Type::getInt32Ty(*mila_context), 0));
        llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(*mila_context), IntPtr, false);
        llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "readln", *mila_module);
        for (auto & Arg : F->args())
            Arg.setName("x");
    }

    // create function definitions
    // create global variables / constants
    for(auto &[name, var_info]: program->function_type->local_variables->variables) {
        switch (var_info.symbol_class) {
            case SYMBOL_CLASS::PARAM:
            case SYMBOL_CLASS::RETURN_VALUE:
                throw GeneratorError{"Should not happen"};

            case SYMBOL_CLASS::VAR:
                var_info.global_variable = new llvm::GlobalVariable{
                        *mila_module,
                        var_info.type->typegen(*mila_builder),
                        false,
                        llvm::GlobalValue::CommonLinkage,
                        nullptr,
                        name.identifier
                };
                if(var_info.type->is_array())
                    var_info.global_variable->setInitializer(
                            llvm::ConstantAggregateZero::get(var_info.global_variable->getValueType())
                    );
                else if(var_info.type->is_int())
                    var_info.global_variable->setInitializer(
                            llvm::ConstantInt::get(llvm::Type::getInt32Ty(*mila_context), 0)
                    );
                else if(var_info.type->is_real())
                    var_info.global_variable->setInitializer(
                            llvm::ConstantFP::get(llvm::Type::getDoubleTy(*mila_context), 0)
                    );
                else if(var_info.type->is_string())
                    var_info.global_variable->setInitializer(
                            llvm::ConstantPointerNull::get(llvm::Type::getInt8PtrTy(*mila_context))
                    );
                else
                    throw GeneratorError{"Unsupported type for global var."};

                break;
            case SYMBOL_CLASS::CONST:
                var_info.value = std::visit(overloaded{
                    [](auto&) -> Value {throw GeneratorError{"Bad type."};},
                    [&](Mila_int_T v) -> Value {
                        return llvm::ConstantInt::get(
                                mila_builder->getContext(),
                                llvm::APInt(
                                    Mila_int_T_bits,
                                    v,
                                    Mila_int_T_signed
                            )
                        );},
                    [&](Mila_real_T v)-> Value {
                        return llvm::ConstantFP::get(
                                mila_builder->getContext(),
                                llvm::APFloat(v)
                        );
                    }
                }, var_info.initial_value);
                break;
            case SYMBOL_CLASS::FUNCTION:
                if(!is_builtin_function(name))
                    std::dynamic_pointer_cast<FunctionType>(var_info.type)->typegen_function(*mila_builder, *mila_module);
                break;
        }
    }

    for(auto &[name, function]:
            program->function_type->local_variables->functions) {
        if(!is_builtin_function(name))
            function->codegen(*mila_builder, *mila_module);
    }


    // create main function
    {
        llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(*mila_context), false);
        llvm::Function *MainFunction = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "main", *mila_module);

        // block
        llvm::BasicBlock *BB = llvm::BasicBlock::Create(*mila_context, "entry", MainFunction);
        mila_builder->SetInsertPoint(BB);

        program->implementation->codegen(*mila_builder, *mila_module);

        // return 0
        mila_builder->CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*mila_context), 0));
    }

    return mila_module;
}
