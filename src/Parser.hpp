#pragma once
#include <iostream>
#include <algorithm>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>


#include "Lexer.hpp"
#include "Node.hpp"



class Parser {
public:
    Parser();

    bool parse();
    std::ostream &print_lexer_output(std::ostream &os) const;
    std::shared_ptr<llvm::Module> generate();  // generate
    std::ostream &format_ast(std::ostream &os) const;
private:
    std::shared_ptr<llvm::Module> _generate();
    static void _check_unused(const Function &mila, bool is_global);

    Token get_token();

    template<bool required = true>
    Token match(TokenType accept) {
        attempted_matches.emplace(accept);
        if(current_token.type == accept) {
//            std::cerr << "Matched: " << current_token << std::endl;
            return get_token();
        }
        return NO_MATCH<required>();
    }

    template<bool required = true>
    Token match(std::initializer_list<TokenType> accept) {
        for(const auto &type: accept) {
            attempted_matches.emplace(type);
            if(current_token.type == type) {
//                std::cerr << "Matched: " << current_token << std::endl;
                return get_token();
            }
        }
        return NO_MATCH<required>();
    }

    Token try_match(TokenType accept);
    [[nodiscard]] bool peek(TokenType accept);
    [[nodiscard]] bool peek(std::initializer_list<TokenType> accept);

    template<bool required>
    Token NO_MATCH();

    [[noreturn]] static void error(const std::string &message) {
        std::cerr << COLOR::as_red(message) << std::endl;
        throw_error("");
    }

    [[noreturn]] void no_match(const std::string &message) {
        lexer.blame_token(std::cerr, current_token);
        std::cerr << COLOR::as_red(message) << std::endl;

        if(attempted_matches.size() > 1)
            std::cerr << COLOR::as_red("Attempted matches: ");
        else
            std::cerr << COLOR::as_red("Attempted to match: ");

        std::cerr << COLOR::RED
                  << attempted_matches
                  << COLOR::RESET
                  << std::endl;

        throw_error("");
    }

    [[noreturn]] static void throw_error(std::string value);

    std::unique_ptr<Mila> parse_mila();

    // Parses local variables for function
    void parse_variables(Function &function);
    void parse_variables_var(Function &function);
    void parse_variables_const(Function &function);

    [[nodiscard]] std::vector<Identifier> parse_identifier_list();

    // Parses function declaration / definition
    void parse_function(Function &function);

    void parse_implementation(Function &function);

    std::unique_ptr<Expression> parse_expression();
    std::unique_ptr<WhileStatement> parse_while_statement();
    std::unique_ptr<ForStatement> parse_for_statement();
    std::unique_ptr<IfStatement> parse_if_statement();
    std::unique_ptr<ExitStatement> parse_exit_statement();
    std::unique_ptr<ContinueStatement> parse_continue_statement();
    std::unique_ptr<BreakStatement> parse_break_statement();
    std::unique_ptr<Statements> parse_statements();
    std::unique_ptr<Statements> parse_scope();
    std::unique_ptr<Statement> parse_statement();
    std::unique_ptr<Expression> parse_L9();
    std::unique_ptr<Expression> parse_L9_(std::unique_ptr<Expression> lhs);
    std::unique_ptr<Expression> parse_L8();
    std::unique_ptr<Expression> parse_L8_(std::unique_ptr<Expression> lhs);
    std::unique_ptr<Expression> parse_L7();
    std::unique_ptr<Expression> parse_L7_(std::unique_ptr<Expression> lhs);
    std::unique_ptr<Expression> parse_L6();
    std::unique_ptr<Expression> parse_L6_(std::unique_ptr<Expression> lhs);
    std::unique_ptr<Expression> parse_L5();
    std::unique_ptr<Expression> parse_L5_(std::unique_ptr<Expression> lhs);
    std::unique_ptr<Expression> parse_L4();
    std::unique_ptr<Expression> parse_L4_(std::unique_ptr<Expression> lhs);
    std::unique_ptr<Expression> parse_L3();
    std::unique_ptr<Expression> parse_L3_(std::unique_ptr<Expression> lhs);
    std::unique_ptr<Expression> parse_L2();
    std::unique_ptr<Expression> parse_L2_();
    std::unique_ptr<Expression> parse_L1();
    std::unique_ptr<Expression> parse_L1_(std::unique_ptr<Expression> lhs);
    std::unique_ptr<Type> parse_type();
    std::unique_ptr<Type> parse_trivial_type();
    std::unique_ptr<TypeArray> parse_type_array();
    parameter_list_T parse_parameters();
    void parse_parameter(parameter_list_T &parameters);
    std::vector<std::unique_ptr<Expression>> parse_arguments();
    std::unique_ptr<LiteralExpression> parse_literal_expression();
    Mila_int_T parse_literal_integer();
    std::unique_ptr<Expression> parse_subscript();

    std::set<TokenType> attempted_matches;

    std::shared_ptr<llvm::LLVMContext> mila_context;   // llvm context
    std::shared_ptr<llvm::IRBuilder<>> mila_builder;   // llvm builder
    std::shared_ptr<llvm::Module> mila_module;         // llvm module

    Lexer lexer;
    Token current_token;
    std::unique_ptr<Mila> program;
    std::shared_ptr<SymbolTable> current_table;  // context in which variables are looked for
};
