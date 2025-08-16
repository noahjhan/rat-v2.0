#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include "lexer.hpp"
#include "pairing_map.hpp"

using namespace Node;

class Parser
{
    public:
    Parser(std::deque<Token> &tokens, RatSource &source_file);
    std::unique_ptr<AST> dispatch();

    std::unique_ptr<GenericExpr> recurseNumeric();
    std::unique_ptr<GenericExpr> recurseFactor();
    std::unique_ptr<GenericExpr> recurseTerm();
    std::unique_ptr<GenericExpr> recurseAdditive();
    std::unique_ptr<GenericExpr> recurseShift();
    std::unique_ptr<GenericExpr> recurseComparison();
    std::unique_ptr<GenericExpr> recurseLogical();
    std::unique_ptr<GenericExpr> recurseExpr();
    std::unique_ptr<GenericExpr> tokenToExpr();

    private:
    inline Token pop();
    inline Token peek();
    inline void restore(const Token &token);
    inline int numTokens() const;

    std::deque<Token> tokens_;
    RatSource source_file_;

    // symbol table to keep track of variables
    // stack of constituent tokens to keep track of return types ? perhaps
};

#endif // PARSER_HPP
