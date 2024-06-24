#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <list>
#include <iostream>

#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Function.h"

#include "Token.hpp"

#ifndef unreachable
#define unreachable() (__builtin_unreachable())
#endif


// https://stackoverflow.com/questions/26377430/how-to-perform-a-dynamic-cast-with-a-unique-ptr
//template <typename To, typename From>
//std::unique_ptr<To> dynamic_unique_cast(std::unique_ptr<From>&& p) {
//    if (To* cast = dynamic_cast<To*>(p.get()))
//    {
//        std::unique_ptr<To> result(cast);
//        p.release();
//        return result;
//    }
//    throw std::bad_cast{};
//}


// helper type for the visitor
template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};


using MvT = Mila_variant_T;

using Value = llvm::Value *;


class ParserError: std::exception {
public:
    explicit ParserError(std::string msg);

    [[nodiscard]] const char * what() const noexcept override;
private:
    std::string err;
};

class TypeError: std::exception {
public:
    explicit TypeError(std::string msg)
        : err{std::move(msg)}
    {};

    [[nodiscard]] const char * what() const noexcept override {
        return err.c_str();
    }
private:
    std::string err;
};

class GeneratorError: std::exception {
public:
    explicit GeneratorError(std::string msg)
        : err{std::move(msg)}
    {};

    [[nodiscard]] const char * what() const noexcept override {
        return err.c_str();
    }
private:
    std::string err;
};


void parser_warning(const std::string &message);



struct Identifier {
    explicit Identifier(std::string identifier);
    explicit Identifier(Token token)
        : identifier(std::get<std::string>(std::move(token.data)))
    {}

    friend auto operator<=>(const Identifier&, const Identifier&) = default;

    std::string identifier;
};

const Identifier RETURN_VALUE_IDENTIFIER{"__return_value__"};



struct Type {
    enum class T {
        UNKNOWN_T,
        VOID_T,
        INT_T,
        REAL_T,
        STRING_T,
        ARRAY_T,
        FUNCTION_T,
    };

    explicit Type(T type);

    explicit Type(TokenType token_type)
        : Type(_token_to_type(token_type))
    {}

    Type(const Type &other) = default;
    Type &operator=(const Type &other) = default;

    virtual ~Type() = default;

    static T _token_to_type(TokenType token_type) {
        if(token_type == TokenType::TOK_INTEGER)
            return T::INT_T;
        if(token_type == TokenType::TOK_REAL)
            return T::REAL_T;
        if(token_type == TokenType::TOK_STRING)
            return T::STRING_T;
        return T::UNKNOWN_T;
    }

    static std::unique_ptr<Type> from_value(const Mila_variant_T &value) {
        if(std::holds_alternative<Mila_int_T>(value))
            return std::make_unique<Type>(T::INT_T);
        else if(std::holds_alternative<Mila_real_T>(value))
            return std::make_unique<Type>(T::REAL_T);
        else if(std::holds_alternative<Mila_string_T>(value))
            return std::make_unique<Type>(T::STRING_T);
        else
            return std::make_unique<Type>(T::UNKNOWN_T);
    }

    bool operator==(const Type &other) const;

    [[nodiscard]] bool is_void() const {
        return type == T::VOID_T;
    }

    [[nodiscard]] bool is_int() const {
        return type == T::INT_T;
    }

    [[nodiscard]] bool is_real() const {
        return type == T::REAL_T;
    }

    [[nodiscard]] bool is_string() const {
        return type == T::STRING_T;
    }

    [[nodiscard]] bool is_numeric() const {
        return is_int() || is_real();
    }

    [[nodiscard]] bool is_array() const {
        return type == T::ARRAY_T;
    }

    [[nodiscard]] bool is_function() const {
        return type == T::FUNCTION_T;
    }

    [[nodiscard]] virtual llvm::Type *typegen(llvm::IRBuilder<> &builder) const {
        return typegen_trivial(builder);
    }

    [[nodiscard]] llvm::Type *typegen_trivial(llvm::IRBuilder<> &builder) const {
        switch (type) {
            case T::UNKNOWN_T:
                throw GeneratorError{"Could not generate type for UNKNOWN."};
            case T::VOID_T:
                return builder.getVoidTy();
            case T::INT_T:
                return builder.getIntNTy(Mila_int_T_bits);
            case T::REAL_T:
                return builder.getDoubleTy();
            case T::STRING_T:
                return builder.getInt8PtrTy();
            case T::ARRAY_T:
            case T::FUNCTION_T:
                throw GeneratorError{"Not trivial type."};
        }
        return nullptr;
    }

    static std::shared_ptr<Type> VOID();
    static std::shared_ptr<Type> INT();
    static std::shared_ptr<Type> STRING();

    T type;

    static std::shared_ptr<Type> TypeVoid;
    static std::shared_ptr<Type> TypeInt;
    static std::shared_ptr<Type> TypeString;
//    static std::shared_ptr<Type> TypeReal;
};



struct FunctionType;
struct Function;

struct SymbolTable {
    struct SymbolInfo {
        enum class Symbol_class {
            CONST,
            VAR,
            PARAM,
            RETURN_VALUE,
            FUNCTION
        };

        std::string identifier;
        std::shared_ptr<Type> type;
        Symbol_class symbol_class{};
        Mila_variant_T initial_value{};  // for const initial initial_value
        llvm::Value *value = nullptr;
        llvm::AllocaInst *store = nullptr;
        llvm::GlobalVariable *global_variable = nullptr;
    };

    void add_var(
            const std::vector<Identifier> &identifiers,
            const std::shared_ptr<Type> &type
    ) {
        for(const auto &identifier: identifiers)
            add_var(identifier, type);
    }

    void add_var(
            const Identifier &identifier,
            std::shared_ptr<Type> type
    ) {
        if(variables.contains(identifier))
            throw ParserError{"Redeclaration of variable."};

        variables.emplace(identifier, SymbolInfo{
                .identifier = identifier.identifier,
                .type = std::move(type),
                .symbol_class = SymbolInfo::Symbol_class::VAR,
        });
    }

    void add_const(
            const std::vector<Identifier> &identifiers,
            const Mila_variant_T &value
    ) {
        std::shared_ptr<Type> type = Type::from_value(value);

        if(type->type == Type::T::UNKNOWN_T)
            throw ParserError("Value must be constant.");

        for(const auto &identifier: identifiers)
            add_const(identifier, value, type);
    }

    void add_const(
            const Identifier &identifier,
            const Mila_variant_T &value,
            std::shared_ptr<Type> type
    ) {
        if(variables.contains(identifier))
            throw ParserError{"Redeclaration with const."};

        variables.emplace(identifier, SymbolInfo{
                .identifier = identifier.identifier,
                .type = std::move(type),
                .symbol_class = SymbolInfo::Symbol_class::CONST,
                .initial_value = value,
        });
    }


    std::shared_ptr<Function> add_function(
            const std::shared_ptr<Function> &function
    );

    void add_return_var(
            const std::shared_ptr<Type> &type
    ) {
        assert(!variables.contains(RETURN_VALUE_IDENTIFIER));

        variables.emplace(RETURN_VALUE_IDENTIFIER, SymbolInfo{
                .identifier = RETURN_VALUE_IDENTIFIER.identifier,
                .type = type,
                .symbol_class = SymbolInfo::Symbol_class::RETURN_VALUE,
        });
    }

    void add_param(
            const Identifier &identifier,
            const std::shared_ptr<Type> &type
    ) {
        if(variables.contains(identifier))
            throw ParserError{"Parameters must have unique names."};

        variables.emplace(identifier, SymbolInfo{
                .identifier = identifier.identifier,
                .type = type,
                .symbol_class = SymbolInfo::Symbol_class::PARAM,
        });
    }

    static void _check_forward_definition_match(
            const std::shared_ptr<Type> &declaration,
            const std::shared_ptr<FunctionType> &definition
    );

    std::map<Identifier, SymbolInfo> variables;
    std::map<Identifier, std::shared_ptr<Function>> functions;
    std::shared_ptr<SymbolTable> global_table;
};

using SYMBOL_CLASS = SymbolTable::SymbolInfo::Symbol_class;


struct TypeArray: public Type {
    TypeArray(
            std::shared_ptr<Type> element,
            Mila_int_T start,
            Mila_int_T stop
    ) : Type{T::ARRAY_T},
        element_type{(std::move(element))},
        array_start{start},
        array_end{stop}
    {}


    [[nodiscard]] llvm::Type *typegen(llvm::IRBuilder<> &builder) const override {
        return typegen_array(builder);
    }

    [[nodiscard]] llvm::ArrayType *typegen_array(llvm::IRBuilder<> &builder) const {
        return llvm::ArrayType::get(
                element_type->typegen(builder),
                array_end - array_start + 1
        );
    }

    std::shared_ptr<Type> element_type;
    Mila_int_T array_start, array_end;
};


struct DescentData {
    std::vector<std::pair<llvm::BasicBlock *, llvm::BasicBlock *>> continue_break_BBs;
};



struct Statement {
    virtual ~Statement() = default;
    virtual llvm::Value *codegen(llvm::IRBuilder<> &, llvm::Module &, DescentData &) const = 0;
    virtual llvm::Value *get_address(llvm::IRBuilder<> &, const llvm::Module &) const {
        throw GeneratorError{"Get address not supported on this stmt"};
    }
    [[nodiscard]] virtual llvm::Value *get_store(llvm::IRBuilder<> &, llvm::Module &, DescentData &) const {
        throw GeneratorError{"No AllocaInst."};
    };
};


struct Statements: public Statement {
    using statement_vector_T = std::vector<std::unique_ptr<Statement>>;

    explicit Statements(statement_vector_T statements)
        : statements(std::move(statements))
    {}

    explicit Statements(std::unique_ptr<Statement> statement) {
        statements.emplace_back(std::move(statement));
    }

    llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &module, DescentData &descent) const override {
        for(const auto &statement: statements)
            statement->codegen(builder, module, descent);
        return nullptr;
    };

    statement_vector_T statements;
};

using parameter_list_T = std::vector<std::pair<std::shared_ptr<Type>, Identifier>>;

struct FunctionType: public Type {
    FunctionType(
            const std::shared_ptr<Type> &return_type,
            Identifier name,
            parameter_list_T parameters,
            const std::shared_ptr<SymbolTable> &global_table
    ) : Type{T::FUNCTION_T},
        return_type{return_type},
        function_name{std::move(name)},
        parameter_types{std::move(parameters)},
        local_variables{std::make_shared<SymbolTable>()}
    {
        local_variables->global_table = global_table;

        if(!return_type->is_void() && function_name.identifier != "main")
            local_variables->add_return_var(return_type);

        for(const auto &[type, identifier]: parameter_types) {
            local_variables->add_param(identifier, type);
        }
    }

    llvm::Function *typegen_function(
            llvm::IRBuilder<> &builder,
            llvm::Module &module
    ) const {
        std::vector<llvm::Type *> llvm_parameter_types;

        for(const auto &[parameter_type, _]: parameter_types)
            llvm_parameter_types.emplace_back(parameter_type->typegen_trivial(builder));

        llvm::Type *llvm_return_type = return_type->typegen_trivial(builder);

        llvm::FunctionType *function_type = llvm::FunctionType::get(llvm_return_type,
                                                                    llvm_parameter_types,
                                                                    false);

        llvm::Function *llvm_function = llvm::Function::Create(
                function_type,
                llvm::Function::ExternalLinkage,
                function_name.identifier,
                module
        );

        size_t arg_idx = 0;
        for(auto &arg: llvm_function->args()) {
            const auto &identifier = parameter_types[arg_idx++].second;
            local_variables->variables.at(identifier).value = &arg;
            arg.setName(identifier.identifier);
        }

        return llvm_function;
    }

    std::shared_ptr<Type> return_type;
    Identifier function_name;
    parameter_list_T parameter_types;
    std::shared_ptr<SymbolTable> local_variables;
};

struct Function {
    explicit Function(
        const std::shared_ptr<FunctionType> &function_type
    ) : function_type{function_type}
    {}

    Function(
            const std::shared_ptr<Type> &return_type,
            Identifier name,
            parameter_list_T parameters,
            const std::shared_ptr<SymbolTable> &global_table
    ) : Function(std::make_shared<FunctionType>(
            return_type,
            std::move(name),
            std::move(parameters),
            global_table)
    ) {}

    llvm::Function *codegen(
            llvm::IRBuilder<> &builder,
            llvm::Module &module
    ) const {
        auto name = function_type->function_name.identifier;
        llvm::Function *llvm_function = module.getFunction(function_type->function_name.identifier);

        if(llvm_function == nullptr)
            throw GeneratorError{"Function " + name + " not found in module."};

        auto &context = builder.getContext();

        llvm::BasicBlock *entry_block = llvm::BasicBlock::Create(context, "entry", llvm_function);
        builder.SetInsertPoint(entry_block);

        if(!function_type->return_type->is_void())
            function_type->local_variables->variables.at(RETURN_VALUE_IDENTIFIER).store =
                    builder.CreateAlloca(llvm_function->getReturnType(), nullptr, "return_store");

        for(auto &[identifier, sym_info]: function_type->local_variables->variables) {
            if(sym_info.symbol_class == SYMBOL_CLASS::PARAM || sym_info.symbol_class == SYMBOL_CLASS::VAR) {
                sym_info.store = builder.CreateAlloca(
                        sym_info.type->typegen_trivial(builder),
                        nullptr,
                        identifier.identifier + "_store"
                );
            }

            if(sym_info.symbol_class == SYMBOL_CLASS::PARAM)  // copy params to store
                builder.CreateStore(sym_info.value, sym_info.store);
        }

        {
            DescentData temp;
            implementation->codegen(builder, module, temp);
        }

        if(function_type->return_type->is_void())
            builder.CreateRetVoid();
        else {
            auto return_value = builder.CreateLoad(
                        llvm_function->getReturnType(),
                        function_type->local_variables->variables[RETURN_VALUE_IDENTIFIER].store,
                        "return_value"
                    );

            builder.CreateRet(return_value);
        }

        llvm::verifyFunction(*llvm_function);

        return llvm_function;
    }

    std::shared_ptr<FunctionType> function_type;
    std::unique_ptr<Statements> implementation;
};

struct Expression: public Statement {
    [[nodiscard]] virtual Mila_variant_T get_value() const = 0;
    [[nodiscard]] virtual std::shared_ptr<Type> get_type() const = 0;

    llvm::Value *codegen(llvm::IRBuilder<> &, llvm::Module &, DescentData &descent) const override = 0;
};


struct LiteralExpression: public Expression {
    explicit LiteralExpression(Mila_variant_T value)
        : value{std::move(value)}
    {}

    explicit LiteralExpression(Token token)
        : value{(std::move(token.data))}
    {}

    [[nodiscard]] Mila_variant_T get_value() const override {
        return value;
    }

    [[nodiscard]] std::shared_ptr<Type> get_type() const override {
        return std::make_shared<Type>(*Type::from_value(value));
    }

    [[nodiscard]] llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &module, DescentData &) const override {
        return std::visit(overloaded{
                [](std::monostate) -> Value {throw GeneratorError{"Literal does not contain initial_value."};},
                [&](Mila_int_T int_value) -> Value {
                    return llvm::ConstantInt::get(
                            builder.getContext(),
                            llvm::APInt(Mila_int_T_bits, int_value, Mila_int_T_signed)
                    );
                },
                [&](Mila_real_T real_value) -> Value {
                    return llvm::ConstantFP::get(
                            builder.getContext(),
                            llvm::APFloat(real_value)
                    );
                },
                [&](const Mila_string_T &str) -> Value {
                    auto *const_string = llvm::ConstantDataArray::getString(
                            builder.getContext(), str, true
                    );

                    auto *global_string = new llvm::GlobalVariable(
                            module,
                            const_string->getType(),
                            true, // isConstant
                            llvm::GlobalValue::PrivateLinkage,
                            const_string,
                            "str_literal"
                    );

                    return builder.CreatePointerCast(
                            global_string,
                            llvm::Type::getInt8PtrTy(builder.getContext()),
                            "str_addr"
                    );
                }
        }, value);
    }

    Mila_variant_T value;
};


struct IdentifierExpression: public Expression {
    explicit IdentifierExpression(const Identifier &identifier, SymbolTable *table) {
        if(table->variables.contains(RETURN_VALUE_IDENTIFIER))
            return_info = &table->variables.at(RETURN_VALUE_IDENTIFIER);

        while(table != nullptr) {
            if(table->variables.contains(identifier)) {
                identifier_info = &table->variables.at(identifier);
                return;
            }

            table = table->global_table.get();
        }

        throw ParserError{"Unknown symbol: " + identifier.identifier};
    }

    [[nodiscard]] Mila_variant_T get_value() const override {
        if(return_info)
            return return_info->initial_value;
        return identifier_info->initial_value;
    }

    [[nodiscard]] std::shared_ptr<Type> get_type() const override {
        return identifier_info->type;
    }

    [[nodiscard]] llvm::Value *get_store(llvm::IRBuilder<> &, llvm::Module &, DescentData &) const override {
        if(identifier_info->symbol_class == SYMBOL_CLASS::CONST)
            throw GeneratorError{"Can not store to CONST"};
        if(identifier_info->symbol_class == SYMBOL_CLASS::FUNCTION)
            return return_info->store;

        if(identifier_info->store)
            return identifier_info->store;
        return identifier_info->global_variable;
    }

    [[nodiscard]] llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &, DescentData &) const override {
        switch (identifier_info->symbol_class) {
            case SYMBOL_CLASS::RETURN_VALUE:
            case SYMBOL_CLASS::PARAM:
            case SYMBOL_CLASS::VAR:
                if(identifier_info->type->is_array()) {
                    auto arr_t = std::dynamic_pointer_cast<TypeArray>(identifier_info->type);
//                    if(arr_t->array_start != 0) {
//                        std::vector<llvm::Value *> indices{
//                                llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 0),
//                                llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), -arr_t->array_start),
//                        };
//
//                        return builder.CreateGEP(
//                                arr_t->typegen(builder),
//                                identifier_info->global_variable,
//                                indices,
//                                "elem_ptr"
//                        );
//                    }
                    return identifier_info->global_variable;
                }

                if(identifier_info->global_variable)
                    return builder.CreateLoad(identifier_info->type->typegen_trivial(builder),
                                              identifier_info->global_variable,
                                              "load_global");
                return builder.CreateLoad(identifier_info->type->typegen_trivial(builder),
                                          identifier_info->store,
                                          "load_var");
            case SYMBOL_CLASS::CONST:
                return identifier_info->value;
            case SYMBOL_CLASS::FUNCTION:
                return builder.CreateLoad(return_info->type->typegen_trivial(builder),
                                          return_info->store,
                                          "load_return");
        }
        return nullptr;
    }

    llvm::Value *get_address(llvm::IRBuilder<> &, const llvm::Module &) const override {
        return identifier_info->global_variable;
    }

    SymbolTable::SymbolInfo *identifier_info;
    SymbolTable::SymbolInfo *return_info = nullptr;
};

enum class UnaryOp {
    PLUS,
    MINUS,
    NOT,
};


template<UnaryOp OPERATION>
struct UnaryExpression: public Expression {
    explicit UnaryExpression(std::unique_ptr<Expression> child)
        : child{std::move(child)}
    {
        _check_type();
    }

    void _check_type() {
        auto child_type = child->get_type();

        if(child_type->is_int())  // INT is valid for all operations
            return;

        if(child_type->is_real()) {
            if constexpr (OPERATION == UnaryOp::PLUS || OPERATION == UnaryOp::MINUS)
                return;
            throw ParserError{"Can not use not on real number."};  // maybe, add implicit conversion
        }

        throw TypeError{"Unary operation with wrong type."};
    }

    [[nodiscard]] Mila_variant_T get_value() const override {
        if constexpr(OPERATION == UnaryOp::PLUS)
            return child->get_value();
        if constexpr (OPERATION == UnaryOp::MINUS)
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [](Mila_int_T v) -> MvT {return -v;},
                    [](Mila_real_T v) -> MvT {return -v;},
                    [](auto) -> MvT {throw GeneratorError{"Operation not supported."};},
            }, child->get_value());
        return std::visit(overloaded{
            [](std::monostate) -> MvT { return {}; },
            [](Mila_int_T v) -> MvT {return !v;},
            [](auto) -> MvT {throw GeneratorError{"Operation not supported."};},
        }, child->get_value());
    }

    [[nodiscard]] std::shared_ptr<Type> get_type() const override {
        if constexpr (OPERATION == UnaryOp::NOT)
            return Type::INT();
        return child->get_type();
    }

    [[nodiscard]] llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &module, DescentData &descent) const override {
        if constexpr (OPERATION == UnaryOp::PLUS) {
            return child->codegen(builder, module, descent);
        }
        if constexpr (OPERATION == UnaryOp::MINUS) {
            auto child_t = child->get_type();
            if(child_t->is_int()) {
                return builder.CreateSub(
                        llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 0),
                        child->codegen(builder, module, descent)
                );
            } else if(child_t->is_real()) {
                return builder.CreateFSub(
                        llvm::ConstantFP::get(llvm::Type::getDoubleTy(builder.getContext()), 0),
                        child->codegen(builder, module, descent)
                );
            }
        }
        if constexpr (OPERATION == UnaryOp::NOT) {
            if(child->get_type()->is_int())
                return builder.CreateZExt(
                        builder.CreateICmpNE(
                                child->codegen(builder, module, descent),
                            llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 0),
                            "not"
                        ),
                        llvm::Type::getInt32Ty(builder.getContext()),
                        "int_ext"
                );
        }
        throw GeneratorError{"Type not supported for unary operation"};
    }

    std::unique_ptr<Expression> child;
};


struct CastExpression: public Expression {
    CastExpression(
            std::shared_ptr<Type> cast_type,
            std::unique_ptr<Expression> expression
    ):
        type{std::move(cast_type)},
        child{std::move(expression)}
    {}

    [[nodiscard]] Mila_variant_T get_value() const override {
        return {};  // todo
    }

    [[nodiscard]] std::shared_ptr<Type> get_type() const override {
        return type;
    }

    [[nodiscard]] llvm::Value *codegen(llvm::IRBuilder<> &, llvm::Module &, DescentData &) const override {
        // todo
        return nullptr;
    }

    std::shared_ptr<Type> type;
    std::shared_ptr<Expression> child;
};


enum class BinaryOp {
    ASSIGN,
    OR,
    AND,
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    PLUS,
    MINUS,
    STAR,
    DIV,
    MOD,
    SUBSCRIPT,
};

template<BinaryOp OPERATION>
struct BinaryExpression: public Expression {
    explicit BinaryExpression(std::unique_ptr<Expression> lhs,
                              std::unique_ptr<Expression> rhs)
        : lhs{std::move(lhs)},
          rhs{std::move(rhs)}
    {
        _check_type();
    }

    void _check_type() {
        auto l_type = lhs->get_type();
        auto r_type = rhs->get_type();

        if(l_type->is_function())
            l_type = std::dynamic_pointer_cast<FunctionType>(l_type)->return_type;

        if(r_type->is_function())
            r_type = std::dynamic_pointer_cast<FunctionType>(r_type)->return_type;

        if constexpr (OPERATION == BinaryOp::ASSIGN) {
            if(*l_type != *r_type)
                throw TypeError{"TODO"};  // todo implicit conversion for real / int types
        }
        else if constexpr (
                OPERATION == BinaryOp::OR ||
                OPERATION == BinaryOp::AND ||
                OPERATION == BinaryOp::EQUAL ||
                OPERATION == BinaryOp::NOT_EQUAL ||
                OPERATION == BinaryOp::MOD
        ) {
            if(!l_type->is_int() || !r_type->is_int())
                throw TypeError{"TODO"};
        }
        else if constexpr (
                OPERATION == BinaryOp::LESS ||
                OPERATION == BinaryOp::LESS_EQUAL ||
                OPERATION == BinaryOp::GREATER ||
                OPERATION == BinaryOp::GREATER_EQUAL ||
                OPERATION == BinaryOp::PLUS ||
                OPERATION == BinaryOp::MINUS ||
                OPERATION == BinaryOp::STAR ||
                OPERATION == BinaryOp::DIV
        ) {
            // todo implicit conversion for these operations
            if(!l_type->is_numeric() || !r_type->is_numeric() || *l_type != *r_type)
                throw TypeError{"TODO"};
        }
        else if constexpr (OPERATION == BinaryOp::SUBSCRIPT) {
            if(!l_type->is_array() || !r_type->is_int())
                throw TypeError{"TODO"};
        }
        else
            throw ParserError{"TODO"};  // unreachable
    }

    [[nodiscard]] Mila_variant_T get_value() const override {
        /* template
         *
         return std::visit(overloaded{
              [](std::monostate) -> MvT { return {}; },
              []() -> MvT {},
              [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
         }, lhs->get_value());
         *
         */
        if constexpr(OPERATION == BinaryOp::ASSIGN) {
            throw GeneratorError{"Get value not supported for assignment operation."};
        }
        if constexpr(OPERATION == BinaryOp::SUBSCRIPT) {
            throw GeneratorError{"Get value not supported for subscript operation."};
        }
        if constexpr(OPERATION == BinaryOp::OR) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v || std::get<Mila_int_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::AND) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v && std::get<Mila_int_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::EQUAL) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v == std::get<Mila_int_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::NOT_EQUAL) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v != std::get<Mila_int_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::LESS) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v < std::get<Mila_int_T>(rhs->get_value());},
                    [&](Mila_real_T v) -> MvT { return v < std::get<Mila_real_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::LESS_EQUAL) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v <= std::get<Mila_int_T>(rhs->get_value());},
                    [&](Mila_real_T v) -> MvT { return v <= std::get<Mila_real_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::GREATER) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v > std::get<Mila_int_T>(rhs->get_value());},
                    [&](Mila_real_T v) -> MvT { return v > std::get<Mila_real_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::GREATER_EQUAL) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v >= std::get<Mila_int_T>(rhs->get_value());},
                    [&](Mila_real_T v) -> MvT { return v >= std::get<Mila_real_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::PLUS) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v + std::get<Mila_int_T>(rhs->get_value());},
                    [&](Mila_real_T v) -> MvT { return v + std::get<Mila_real_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::MINUS) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v - std::get<Mila_int_T>(rhs->get_value());},
                    [&](Mila_real_T v) -> MvT { return v - std::get<Mila_real_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::STAR) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v * std::get<Mila_int_T>(rhs->get_value());},
                    [&](Mila_real_T v) -> MvT { return v * std::get<Mila_real_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::DIV) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v / std::get<Mila_int_T>(rhs->get_value());},
                    [&](Mila_real_T v) -> MvT { return v / std::get<Mila_real_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        if constexpr(OPERATION == BinaryOp::MOD) {
            return std::visit(overloaded{
                    [](std::monostate) -> MvT { return {}; },
                    [&](Mila_int_T v) -> MvT { return v % std::get<Mila_int_T>(rhs->get_value());},
                    [](auto) -> MvT { throw GeneratorError{"Bad type"}; },
            }, lhs->get_value());
        }
        throw GeneratorError{"Get value not supported for operation."};
    }
    [[nodiscard]] std::shared_ptr<Type> get_type() const override {
        if constexpr (
                OPERATION == BinaryOp::ASSIGN ||
                OPERATION == BinaryOp::PLUS ||
                OPERATION == BinaryOp::MINUS ||
                OPERATION == BinaryOp::STAR ||
                OPERATION == BinaryOp::DIV
        ) {
            return lhs->get_type();
        }
        if constexpr (
                OPERATION == BinaryOp::OR ||
                OPERATION == BinaryOp::AND ||
                OPERATION == BinaryOp::EQUAL ||
                OPERATION == BinaryOp::NOT_EQUAL ||
                OPERATION == BinaryOp::LESS ||
                OPERATION == BinaryOp::LESS_EQUAL ||
                OPERATION == BinaryOp::GREATER ||
                OPERATION == BinaryOp::GREATER_EQUAL ||
                OPERATION == BinaryOp::MOD
        ) {
            return Type::INT();
        }
        if constexpr (OPERATION == BinaryOp::SUBSCRIPT) {
            return std::dynamic_pointer_cast<TypeArray>(lhs->get_type())->element_type;
        }
        return std::make_shared<Type>(Type::T::UNKNOWN_T);
    }

    [[nodiscard]] llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &module, DescentData &descent) const override {
        if constexpr(OPERATION == BinaryOp::ASSIGN) {
            auto lhs_store = lhs->get_store(builder, module, descent);
            auto rhs_code = rhs->codegen(builder, module, descent);

            builder.CreateStore(rhs_code, lhs_store);

            return rhs_code;
        }

        auto lhs_code = lhs->codegen(builder, module, descent);
        auto rhs_code = rhs->codegen(builder, module, descent);
        auto lhs_type = lhs->get_type();
        auto rhs_type = rhs->get_type();

        if(lhs_type->is_function())
            lhs_type = std::dynamic_pointer_cast<FunctionType>(lhs_type)->return_type;

        if(rhs_type->is_function())
            rhs_type = std::dynamic_pointer_cast<FunctionType>(rhs_type)->return_type;

        if constexpr(OPERATION == BinaryOp::OR) {
            if(lhs_type->is_int())
                return builder.CreateOr(lhs_code, rhs_code, "or_tmp");
        }
        if constexpr(OPERATION == BinaryOp::AND) {
            if(lhs_type->is_int())
                return builder.CreateAnd(lhs_code, rhs_code, "and_tmp");
        }
        if constexpr(OPERATION == BinaryOp::EQUAL) {
            if(lhs_type->is_int())
                return builder.CreateICmpEQ(lhs_code, rhs_code, "eq_tmp");
            if(lhs_type->is_real())
                return builder.CreateFCmpOEQ(lhs_code, rhs_code, "eq_tmp");
        }
        if constexpr(OPERATION == BinaryOp::NOT_EQUAL) {
            if(lhs_type->is_int())
                return builder.CreateICmpNE(lhs_code, rhs_code, "ne_tmp");
            if(lhs_type->is_real())
                return builder.CreateFCmpONE(lhs_code, rhs_code, "ne_tmp");
        }
        if constexpr(OPERATION == BinaryOp::LESS) {
            if(lhs_type->is_int())
                return builder.CreateICmpSLT(lhs_code, rhs_code, "lt_tmp");
            if(lhs_type->is_real())
                return builder.CreateFCmpOLT(lhs_code, rhs_code, "lt_tmp");
        }
        if constexpr(OPERATION == BinaryOp::LESS_EQUAL) {
            if(lhs_type->is_int())
                return builder.CreateICmpSLE(lhs_code, rhs_code, "le_tmp");
            if(lhs_type->is_real())
                return builder.CreateFCmpOLE(lhs_code, rhs_code, "le_tmp");
        }
        if constexpr(OPERATION == BinaryOp::GREATER) {
            if(lhs_type->is_int())
                return builder.CreateICmpSGT(lhs_code, rhs_code, "gt_tmp");
            if(lhs_type->is_real())
                return builder.CreateFCmpOGT(lhs_code, rhs_code, "gt_tmp");
        }
        if constexpr(OPERATION == BinaryOp::GREATER_EQUAL) {
            if(lhs_type->is_int())
                return builder.CreateICmpSGE(lhs_code, rhs_code, "ge_tmp");
            if(lhs_type->is_real())
                return builder.CreateFCmpOGE(lhs_code, rhs_code, "ge_tmp");
        }
        if constexpr(OPERATION == BinaryOp::PLUS) {
            if(lhs_type->is_int())
                return builder.CreateAdd(lhs_code, rhs_code, "add_tmp");
            if(lhs_type->is_real())
                return builder.CreateFAdd(lhs_code, rhs_code, "add_tmp");
        }
        if constexpr(OPERATION == BinaryOp::MINUS) {
            if(lhs_type->is_int())
                return builder.CreateSub(lhs_code, rhs_code, "sub_tmp");
            if(lhs_type->is_real())
                return builder.CreateFSub(lhs_code, rhs_code, "sub_tmp");
        }
        if constexpr(OPERATION == BinaryOp::STAR) {
            if(lhs_type->is_int())
                return builder.CreateMul(lhs_code, rhs_code, "mul_tmp");
            if(lhs_type->is_real())
                return builder.CreateFMul(lhs_code, rhs_code, "mul_tmp");
        }
        if constexpr(OPERATION == BinaryOp::DIV) {
            if(lhs_type->is_int())
                return builder.CreateExactSDiv(lhs_code, rhs_code, "div_tmp");
            if(lhs_type->is_real())
                return builder.CreateFDiv(lhs_code, rhs_code, "div_tmp");
        }
        if constexpr(OPERATION == BinaryOp::MOD) {
            if(lhs_type->is_int())
                return builder.CreateSRem(lhs_code, rhs_code, "rem_tmp");
        }
        if constexpr(OPERATION == BinaryOp::SUBSCRIPT)
            return builder.CreateLoad(
                    get_type()->typegen_trivial(builder),
                    get_store(builder, module, descent),
                    "dereference"
            );

        throw GeneratorError{"Operation not supported for type."};
    }

    [[nodiscard]] llvm::Value *get_store(llvm::IRBuilder<> &builder, llvm::Module &module, DescentData &descent) const override {
        if constexpr (OPERATION == BinaryOp::SUBSCRIPT) {
            auto array_type = lhs->get_type()->typegen(builder);
            auto *idx = rhs->codegen(builder, module, descent);

            auto *lhs_id = dynamic_cast<IdentifierExpression *>(lhs.get());
            auto *lhs_arr = dynamic_cast<TypeArray *>(lhs_id->identifier_info->type.get());
            auto arr_start = lhs_arr->array_start;

            if(arr_start != 0) {
                idx = builder.CreateSub(
                        idx,
                        llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), arr_start),
                        "arr_start"
                );
            }

            std::vector<llvm::Value *> indices{
                    llvm::ConstantInt::get(llvm::Type::getInt32Ty(builder.getContext()), 0),
                    idx
            };

            return builder.CreateGEP(
                    array_type,
                    lhs->codegen(builder, module, descent),
                    indices,
                    "elem_ptr"
            );
        }
        if constexpr (OPERATION == BinaryOp::ASSIGN)
            return lhs->get_store(builder, module, descent);

        throw GeneratorError{"get_store not supported for operation."};
    }

    std::unique_ptr<Expression> lhs, rhs;
};


struct CallExpression: public Expression {
    CallExpression(
            const std::shared_ptr<Expression> &function,
            std::vector<std::unique_ptr<Expression>> arguments
    ):
        function{function},
        arguments{std::move(arguments)}
    {
        _check_type();
    }

    void _check_type() {

    }

    [[nodiscard]] Mila_variant_T get_value() const override {
        throw GeneratorError{"get_value not supported for compile time call."};
    }

    [[nodiscard]] std::shared_ptr<Type> get_type() const override {
        return std::dynamic_pointer_cast<FunctionType>(function->get_type())->return_type;
    }

    [[nodiscard]] llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &module, DescentData &descent) const override {
        const auto &fn_name =
                std::dynamic_pointer_cast<IdentifierExpression>(function)->identifier_info->identifier;

        if(fn_name == "dec")
            return _dec_codegen(builder, module, descent);

        llvm::Function *fn = module.getFunction(fn_name);

        if(fn == nullptr)
            throw GeneratorError{"Function not defined."};

        std::vector<Value> args_code;
        if(fn_name == "readln") {
            if(arguments.size() != 1)
                throw GeneratorError{"Wrong number of arguments."};
            args_code.emplace_back(arguments[0]->get_address(builder, module));
        }
        else
            for(const auto &arg: arguments)
                args_code.emplace_back(arg->codegen(builder, module, descent));

        if(fn->getReturnType()->isVoidTy())
            return builder.CreateCall(fn, args_code);
        return builder.CreateCall(fn, args_code, "call_tmp");
    }

    [[nodiscard]] llvm::Value *_dec_codegen(
            llvm::IRBuilder<> &builder,
            llvm::Module &module,
            DescentData &descent
    ) const {
        if(arguments.size() != 1)
            throw GeneratorError{"Wrong number of arguments."};

        auto &arg = arguments.front();
        auto argT = arg->get_type();

        if(argT->is_int()) {
            auto llvmT = argT->typegen_trivial(builder);
            Value v = builder.CreateLoad(llvmT, arg->get_store(builder, module, descent), "dec_load");
            v = builder.CreateSub(v, llvm::ConstantInt::get(llvmT, 1), "dec");
            builder.CreateStore(v, arg->get_store(builder, module, descent), "dec_store");
            return nullptr;
        }
        throw GeneratorError{"Function 'dec' supported only for integers."};
    }

    std::shared_ptr<Expression> function;
    std::vector<std::unique_ptr<Expression>> arguments;
};


struct IfStatement: public Statement {
    IfStatement(
            std::unique_ptr<Expression> condition,
            std::unique_ptr<Statements> body_if,
            std::unique_ptr<Statements> body_else = nullptr
    ):
        condition{std::move(condition)},
        body_if{std::move(body_if)},
        body_else{std::move(body_else)}
    {}

    [[nodiscard]] llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &module, DescentData &descent) const override {
        auto &context = builder.getContext();
        auto condition_code = condition->codegen(builder, module, descent);

        condition_code = builder.CreateICmpNE(
                condition_code,
                llvm::ConstantInt::get(
                    context,
                    llvm::APInt(1, 0, Mila_int_T_signed)
                ),
                "if_cond"
        );

        // Create blocks for the then and else cases.  Insert the 'then' block at the
        // end of the parent.

        auto parent = builder.GetInsertBlock()->getParent();

        auto if_BB = llvm::BasicBlock::Create(context, "body", parent);
        auto else_BB = llvm::BasicBlock::Create(context, "else", parent);
        auto after_BB = llvm::BasicBlock::Create(context, "after", parent);

        builder.CreateCondBr(condition_code, if_BB, else_BB);

        // Emit then value.
        builder.SetInsertPoint(if_BB);
        body_if->codegen(builder, module, descent);
        builder.CreateBr(after_BB);

        // Emit else block.
        builder.SetInsertPoint(else_BB);

        if(body_else)
            body_else->codegen(builder, module, descent);

        builder.CreateBr(after_BB);

        builder.SetInsertPoint(after_BB);

        return nullptr;
    }

    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statements> body_if;
    std::unique_ptr<Statements> body_else;
};

struct WhileStatement: public Statement {
    WhileStatement(
            std::unique_ptr<Expression> condition,
            std::unique_ptr<Statements> body
    ):
        condition{std::move(condition)},
        body{std::move(body)}
    {}

    [[nodiscard]] llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &module, DescentData &descent) const override {
        auto &context = builder.getContext();
        auto *parent = builder.GetInsertBlock()->getParent();

        auto condition_BB = llvm::BasicBlock::Create(context, "cond", parent);
        auto body_BB = llvm::BasicBlock::Create(context, "body", parent);
        auto after_BB = llvm::BasicBlock::Create(context, "after", parent);

        builder.CreateBr(condition_BB);

        builder.SetInsertPoint(condition_BB);
        auto condition_code = condition->codegen(builder, module, descent);

        condition_code = builder.CreateICmpNE(
                condition_code,
                llvm::ConstantInt::get(
                        context,
                        llvm::APInt(1, 0, Mila_int_T_signed)
                ),
                "if_cond"
        );

        // Create blocks for the then and else cases.  Insert the 'then' block at the
        // end of the parent.
        builder.CreateCondBr(condition_code, body_BB, after_BB);

        builder.SetInsertPoint(body_BB);

        descent.continue_break_BBs.emplace_back(condition_BB, after_BB);
        body->codegen(builder, module, descent);
        descent.continue_break_BBs.pop_back();

        builder.CreateBr(condition_BB);

        builder.SetInsertPoint(after_BB);

        return nullptr;
    }

    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statements> body;
};

struct ForStatement: public Statement {
    ForStatement(
            std::unique_ptr<Expression> start,
            std::unique_ptr<Expression> stop,
            std::unique_ptr<Statements> body,
            TokenType direction
    ):
        init{std::move(start)},
        stop_condition{std::move(stop)},
        body{std::move(body)},
        increment{direction == TokenType::TOK_TO}
    {}

    [[nodiscard]] llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &module, DescentData &descent) const override {
        auto &context = builder.getContext();
        auto *parent = builder.GetInsertBlock()->getParent();

        auto body_BB = llvm::BasicBlock::Create(context, "body", parent);
        auto stop_BB = llvm::BasicBlock::Create(context, "stop", parent);
        auto increment_BB = llvm::BasicBlock::Create(context, "inc", parent);
        auto after_BB = llvm::BasicBlock::Create(context, "after", parent);

        // for loop start
        init->codegen(builder, module, descent);

        // immediately jump to body,
        // body always executed at least once
        builder.CreateBr(body_BB);

        auto iter_type = init->get_type()->typegen_trivial(builder);
        auto iter_store = init->get_store(builder, module, descent);

        // build body
        {
            builder.SetInsertPoint(body_BB);

            // set targets for continue/break statements
            descent.continue_break_BBs.emplace_back(stop_BB, after_BB);
            body->codegen(builder, module, descent);
            descent.continue_break_BBs.pop_back();

            builder.CreateBr(stop_BB);
        }

        // checks stop condition
        {
            builder.SetInsertPoint(stop_BB);
            auto iter_value = builder.CreateLoad(
                    iter_type,
                    iter_store,
                    "iter_value"
            );

            auto stop_value = stop_condition->codegen(builder, module, descent);

            auto stop = builder.CreateICmpEQ(iter_value, stop_value, "stop_cond");

            builder.CreateCondBr(stop, after_BB, increment_BB);
        }

        // increment / decrement
        {
            builder.SetInsertPoint(increment_BB);

            auto iter_value = builder.CreateLoad(
                    iter_type,
                    iter_store,
                    "iter_value"
            );

            auto step = llvm::ConstantInt::get(
                    builder.getContext(),
                    llvm::APInt(Mila_int_T_bits, (increment ? 1 : -1), Mila_int_T_signed)
            );

            builder.CreateStore(
                    builder.CreateAdd(iter_value, step, "step"),
                    iter_store
            );

            builder.CreateBr(body_BB);
        }

        builder.SetInsertPoint(after_BB);
        return nullptr;
    }

    std::unique_ptr<Expression> init;
    std::unique_ptr<Expression> stop_condition;
    std::unique_ptr<Statements> body;
    bool increment;
};

struct ExitStatement: public Statement {
    explicit ExitStatement(SymbolTable::SymbolInfo *info = nullptr)
        : return_info{info}
    {}

    [[nodiscard]] llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &, DescentData &) const override {
        if(return_info) {
            auto return_value = builder.CreateLoad(
                    return_info->type->typegen_trivial(builder),
                    return_info->store,
                    "return_value"
            );

            builder.CreateRet(return_value);
        }
        else
            builder.CreateRetVoid();

        return nullptr;
    }

    SymbolTable::SymbolInfo *return_info;
};

struct ContinueStatement: public Statement {
    llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &, DescentData &descent) const override {
        auto &context = builder.getContext();
        auto *parent = builder.GetInsertBlock()->getParent();

        auto break_BB = llvm::BasicBlock::Create(context, "after_break", parent);

        builder.CreateBr(descent.continue_break_BBs.back().second);

        builder.SetInsertPoint(break_BB);
        builder.CreateUnreachable();  // block after break is unreachable

        return nullptr;
    }
};

struct BreakStatement: public Statement {
    llvm::Value *codegen(llvm::IRBuilder<> &builder, llvm::Module &, DescentData &descent) const override {
        auto &context = builder.getContext();
        auto *parent = builder.GetInsertBlock()->getParent();

        auto continue_BB = llvm::BasicBlock::Create(context, "after_continue", parent);

        builder.CreateBr(descent.continue_break_BBs.back().first);

        builder.SetInsertPoint(continue_BB);
        builder.CreateUnreachable();  // block after continue is unreachable

        return nullptr;
    }
};

struct Mila: public Function {
    explicit Mila(std::string name)
        : Function{
            std::make_shared<FunctionType>(
                    Type::INT(),
                    Identifier{"main"},
                    parameter_list_T{},
                    nullptr
            )
          },
          program_name{std::move(name)}
    {}

    std::string program_name;
};



