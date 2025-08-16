#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <variant>
#include <vector>

#include "token.hpp"

namespace Node
{

struct AST;
struct FunctionDecl;
struct VariableDecl;
struct ConditionalStatement;
struct ReturnStatement;
struct GenericExpr;
struct BinaryExpr;
struct UnaryExpr;
struct NumericLiteral;
struct StringLiteral;
struct Identifier;
struct FunctionCall;

using AST_VARIANT = std::variant<FunctionDecl, VariableDecl, ConditionalStatement,
                                 ReturnStatement, GenericExpr>;

using EXPRESSION_VARIANT =
std::variant<GenericExpr, BinaryExpr, UnaryExpr, NumericLiteral, StringLiteral,
             Identifier, FunctionCall>;

struct AST {
    std::shared_ptr<AST_VARIANT> curr;
    std::shared_ptr<AST> next;
};

struct FunctionDecl {
    ConstituentToken function_type;
    std::string value;
    unsigned int line_num;
    unsigned int col_num;

    std::vector<std::unique_ptr<Identifier>> parameters;
    ConstituentToken return_type; // return_type
    std::unique_ptr<AST> body;
    // parameters will be stored in the symbol table
};

struct VariableDecl {
    ConstituentToken type;
    std::string value;
    unsigned int line_num;
    unsigned int col_num;

    std::unique_ptr<GenericExpr> expr;
};

struct ConditionalStatement {
    ConstituentToken type; // statement type
    std::string value;
    unsigned int line_num;
    unsigned int col_num;

    std::unique_ptr<GenericExpr> expr;
    std::unique_ptr<ConditionalStatement> next; // if - else if - else
    std::unique_ptr<AST> body;
};

struct ReturnStatement {
    ConstituentToken type;
    std::string value;
    unsigned int line_num;
    unsigned int col_num;

    std::unique_ptr<GenericExpr> expr;
};

struct GenericExpr {
    std::unique_ptr<EXPRESSION_VARIANT> expr;
};

struct BinaryExpr {
    ConstituentToken op;
    std::unique_ptr<EXPRESSION_VARIANT> lhs;
    std::unique_ptr<EXPRESSION_VARIANT> rhs;
};

struct UnaryExpr {
    ConstituentToken op;
    std::unique_ptr<EXPRESSION_VARIANT> expr;
};

struct NumericLiteral {
    ConstituentToken type;
    std::string value;
    unsigned int line_num;
    unsigned int col_num;
};

struct StringLiteral {
    ConstituentToken type;
    std::string value;
    unsigned int line_num;
    unsigned int col_num;
};

struct Identifier {
    ConstituentToken type;
    std::string value;
    unsigned int line_num;
    unsigned int col_num;
};

struct FunctionCall {
    ConstituentToken type;
    std::string value;
    unsigned int line_num;
    unsigned int col_num;

    std::vector<std::unique_ptr<GenericExpr>> parameters;
};

} // namespace Node
#endif // AST_HPP
