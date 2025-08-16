#include "parser.hpp"

using namespace Node;

Parser::Parser(std::deque<Token> &tokens, RatSource &source_file)
: tokens_(tokens), source_file_(source_file)
{
    //     symbol_table_.enterScope();
}

std::unique_ptr<AST> Parser::dispatch()
{
    if (tokens_.empty()) {
        //     symbol_table_.exitScope();
        return std::unique_ptr<AST>();
    }

    while (peek().value == ";") {
        pop();
    }

    if (tokens_.empty()) {
        //     symbol_table_.exitScope();
        return std::unique_ptr<AST>();
    }
    else if (peek().value == "{") {
        //     symbol_table_.enterScope();
        pop();
        return dispatch();
    }
    else if (peek().value == "}") {
        //    symbol_table_.exitScope();
        pop();
        return std::unique_ptr<AST>();
    }

    auto createASTNode = [&](auto parsingFunction,
                             const std::string error_message) -> std::unique_ptr<AST> {
        auto ptr = parsingFunction();
        if (!ptr) {
            throw std::invalid_argument(error_message);
        }
        auto ast = std::make_unique<AST>();
        ast->curr = std::make_unique<AST_VARIANT>(std::move(*ptr));
        if (!tokens_.empty() && peek().value != ";" && peek().value != "else") {
            throw std::invalid_argument("expected newline");
        }
        ast->next = dispatch();
        return ast;
    };

    return std::unique_ptr<AST>();
}

Token Parser::pop()
{
    if (tokens_.empty()) {
        throw std::invalid_argument("out of tokens, cannot pop");
    }
    auto token = tokens_.front();
    tokens_.pop_front();
    return token;
}

Token Parser::peek()
{
    if (tokens_.empty()) {
        throw std::invalid_argument("out of tokens, cannot peek");
    }
    return tokens_.front();
}

void Parser::restore(const Token &token)
{
    tokens_.push_front(token);
}

int Parser::numTokens() const
{
    return tokens_.size();
}
