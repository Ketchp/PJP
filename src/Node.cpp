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

Identifier::Identifier(std::string identifier)
    : identifier{std::move(identifier)}
{}


Type::Type(T type)
    : type(type)
{}

std::shared_ptr<Type> Type::TypeVoid{};
std::shared_ptr<Type> Type::TypeInt{};
//std::shared_ptr<Type> Type::TypeReal{};

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
