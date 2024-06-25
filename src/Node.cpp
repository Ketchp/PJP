#include "Node.hpp"

#include <ranges>


ParserError::ParserError(std::string msg)
    :err{std::move(msg)}
{}

[[nodiscard]] const char *ParserError::what() const noexcept {
    return err.c_str();
}

void parser_warning(const std::string &message) {
    std::cerr << "Parser warning: " << message << '\n';
}

// escapes C string to Mila source code string including ''
std::string escape_string(const std::string &original) {
    std::string result;
    result.reserve(original.size() + 2);

    result += '\'';

    // 0abfnrtv\'"
    for(auto c: original)
        if(c == '\0')
            result += "\\0";
        else if(c == '\a')
            result += "\\a";
        else if(c == '\b')
            result += "\\b";
        else if(c == '\f')
            result += "\\f";
        else if(c == '\n')
            result += "\\n";
        else if(c == '\r')
            result += "\\r";
        else if(c == '\t')
            result += "\\t";
        else if(c == '\v')
            result += "\\v";
        else if(c == '\\')
            result += "\\\\";
        else if(c == '\'')
            result += "\\'";
        else if(c == '"')   // does not need to be escaped but done for consistency with lexer
            result += "\\\"";
        else if(isprint(c))
            result += c;
        else
            result += std::format("\\x{:02x}", c);

    result += '\'';
    return result;
}

Identifier::Identifier(std::string identifier)
    : identifier{std::move(identifier)}
{}

std::ostream &operator<<(std::ostream &os, const Identifier &name) {
    return os << name.identifier;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Identifier> &names) {
    if(!names.empty()) {
        for(size_t idx = 0; idx < names.size() - 1; idx++)
            os << names[idx] << ", ";
        os << names.back();
    }
    return os;
}

Type::Type(T type)
    : type(type)
{}

std::shared_ptr<Type> Type::TypeVoid{};
std::shared_ptr<Type> Type::TypeInt{};
std::shared_ptr<Type> Type::TypeReal{};
std::shared_ptr<Type> Type::TypeString{};

std::shared_ptr<Type> Type::VOID() {
    if(!TypeVoid)
        TypeVoid = std::make_shared<Type>(Type::T::VOID_T);
    return TypeVoid;
}

std::shared_ptr<Type> Type::INT() {
    if(!TypeInt)
        TypeInt = std::make_shared<Type>(Type::T::INT_T);
    return TypeInt;
}

std::shared_ptr<Type> Type::REAL() {
    if(!TypeReal)
        TypeReal = std::make_shared<Type>(Type::T::REAL_T);
    return TypeReal;
}

std::shared_ptr<Type> Type::STRING() {
    if(!TypeString)
        TypeString = std::make_shared<Type>(Type::T::STRING_T);
    return TypeString;
}

auto operator<=>(const Type &lhs, const Type &rhs) {
    if(lhs.is_array() && rhs.is_array()) {
        const auto &lhs_arr = dynamic_cast<const TypeArray &>(lhs);
        const auto &rhs_arr = dynamic_cast<const TypeArray &>(rhs);

        return std::vector{lhs_arr.array_start, lhs_arr.array_end} <=>
               std::vector{rhs_arr.array_start, rhs_arr.array_end};
    }
    return lhs.type <=> rhs.type;
}


std::ostream &operator<<(std::ostream &os, const param_pair_T &param) {
    FormatConf conf;
    const auto &[type, name] = param;

    os << name << ": ";
    type->print(os, conf);

    return os;
}

// todo check if printing '(  )' wouldn't be better here
std::ostream &operator<<(std::ostream &os, const parameter_list_T &params) {
    if(!params.empty()) {
        FormatConf conf; // todo
        for(size_t idx = 0; idx < params.size() - 1; idx++)
            os << params[idx] << "; ";
        os << params.back();
    }
    return os;
}

std::shared_ptr<Function> SymbolTable::add_function(
        const std::shared_ptr<Function> &function
) {
    // check needed for forward definitions
    auto &identifier = function->function_type->function_name;

    if(variables.contains(identifier)) {
        _check_forward_definition_match(variables[identifier].type, function->function_type);
    } else {
        variables.emplace(identifier, SymbolInfo{
                .identifier = identifier.identifier,
                .type = function->function_type,
                .symbol_class = SymbolInfo::Symbol_class::FUNCTION,
        });

        functions.emplace(identifier, function);
    }

    return functions.at(identifier);
}

void SymbolTable::_check_forward_definition_match(
        const std::shared_ptr<Type> &old_declaration,
        const std::shared_ptr<FunctionType> &definition)
{
    if(!old_declaration->is_function())
        throw ParserError{"Redefinition of non function."};

    auto declaration = std::dynamic_pointer_cast<FunctionType>(old_declaration);

    auto f1p = declaration->parameter_types;
    auto f2p = definition->parameter_types;

    if(f1p.size() != f2p.size())
        throw ParserError{"Functions have different number of parameters."};

    for(auto [it1, it2] = std::pair{f1p.begin(), f2p.begin()};
        it1 != f1p.end();
        it1++, it2++) {
        auto [type1, identifier1] = *it1;
        auto [type2, identifier2] = *it2;

        if(identifier1 != identifier2)
            parser_warning("Different parameter name from definition.");

        if(*type1 != *type2)
            throw TypeError{"Redeclaration of function with different types"};
    }

    if(*declaration->return_type != *definition->return_type)
        throw TypeError{"Different return type."};
}


std::ostream &SymbolTable::print_forward(std::ostream &os, FormatConf &conf) const {
    for(const auto &[_name, func]: functions)
        if(!builtins.contains(_name.identifier))
            func->print_forward(os, conf) << '\n';

    if(!functions.empty())
        os << '\n';
    return os;
}

std::ostream &SymbolTable::print_implementations(std::ostream &os, FormatConf &conf) const {
    for(const auto &[_name, func]: functions)
        if(!builtins.contains(_name.identifier))
            func->print_implementation(os, conf) << '\n';

    if(!functions.empty())
        os << '\n';
    return os;
}

auto operator<=>(const std::shared_ptr<Type> &lhs, const std::shared_ptr<Type> &rhs) {
    return *lhs <=> *rhs;
}


std::ostream &SymbolTable::print_var_const(std::ostream &os, FormatConf &conf) const {
    using name_list_T = std::vector<Identifier>;
    std::map<std::shared_ptr<Type>, name_list_T> vars;
    std::map<Mila_variant_T, name_list_T> consts;

    for(const auto &[identifier, sym_info]: variables) {
        switch (sym_info.symbol_class) {
            case SYMBOL_CLASS::PARAM:
            case SYMBOL_CLASS::FUNCTION:
            case SYMBOL_CLASS::RETURN_VALUE:
                continue;  // skip non var/const

            case SYMBOL_CLASS::VAR:
                if(sym_info.type->is_trivial() || sym_info.type->is_array())
                    vars[sym_info.type].emplace_back(identifier);
                else
                    throw PrinterError{"Non trivial type or array as variable."};
                break;
            case SYMBOL_CLASS::CONST:
                consts[sym_info.initial_value].emplace_back(identifier);
        }
    }

    {
        bool const_printed = false;
        for (const auto &[value, names]: consts) {
            os << conf.indent;
            os << (const_printed ? "      " : "const ") << names << " = ";
            const_printed = true;
            std::visit(overloaded{
                    [](std::monostate) { throw PrinterError{"No value in constant"}; },
                    [&](auto v) { os << v; },
                    [&](const std::string &v) { os << '\'' << v << '\''; },
            }, value);
            os << ";\n";
        }
        if (const_printed && conf.indent.empty()) os << '\n';
    }

    {
        bool var_printed = false;
        for (const auto &[type, names]: vars) {
            os << conf.indent;
            os << (var_printed ? "    " : "var ") << names << " : ";
            var_printed = true;
            type->print(os, conf);
            os << ";\n";
        }
        if (var_printed && conf.indent.empty()) os << '\n';
    }

    return os;
}


bool Type::operator==(const Type &other) const {
    if(type == T::UNKNOWN_T)
        return false;

    if(type == T::ARRAY_T && other.type == T::ARRAY_T)
        // cast should succeed if Type is correctly constructed

        try {
            const auto &lhs = dynamic_cast<const TypeArray &>(*this);
            const auto &rhs = dynamic_cast<const TypeArray &>(other);

            return lhs.element_type == rhs.element_type &&
                    lhs.array_start == rhs.array_start &&
                    lhs.array_end == rhs.array_end;
        }
        catch(const std::bad_cast &) {
            return false;
        }

    return type == other.type;
}
