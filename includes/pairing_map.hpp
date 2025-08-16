#ifndef PAIRING_MAP_HPP
#define PAIRING_MAP_HPP

#include <initializer_list>
#include <unordered_map>
#include <utility>

#include "token.hpp"

template <typename T1, typename T2> class PairingMap
{
    public:
    PairingMap(std::initializer_list<std::pair<T1, T2>> init)
    : forward_map_(forward(init)), reverse_map_(reverse(init))
    {
    }

    const T2 &forward_at(const T1 &key) const
    {
        return forward_map_.at(key);
    }
    const T1 &reverse_at(const T2 &key) const
    {
        return reverse_map_.at(key);
    }

    private:
    static std::unordered_map<T1, T2>
    forward(const std::initializer_list<std::pair<T1, T2>> &init)
    {
        std::unordered_map<T1, T2> forward_map;
        for (const auto &pair : init) {
            forward_map[pair.first] = pair.second;
        }
        return forward_map;
    }
    static std::unordered_map<T2, T1>
    reverse(const std::initializer_list<std::pair<T1, T2>> &init)
    {
        std::unordered_map<T2, T1> reverse_map;
        for (const auto &pair : init) {
            reverse_map[pair.second] = pair.first;
        }
        return reverse_map;
    }

    const std::unordered_map<T1, T2> forward_map_;
    const std::unordered_map<T2, T1> reverse_map_;
};

const PairingMap<std::string, ConstituentToken>
TOKEN_DICT({{"int", ConstituentToken::TYPE_INT},
            {"float", ConstituentToken::TYPE_FLOAT},
            {"double", ConstituentToken::TYPE_DOUBLE},
            {"bool", ConstituentToken::TYPE_BOOL},
            {"char", ConstituentToken::TYPE_CHAR},
            {"long", ConstituentToken::TYPE_LONG},
            {"short", ConstituentToken::TYPE_SHORT},
            {"pointer", ConstituentToken::TYPE_POINTER},
            {"uint", ConstituentToken::TYPE_UINT},
            {"ulong", ConstituentToken::TYPE_ULONG},
            {"ushort", ConstituentToken::TYPE_USHORT},
            {"uchar", ConstituentToken::TYPE_UCHAR},
            {"string", ConstituentToken::TYPE_STRING},
            {"void", ConstituentToken::TYPE_VOID},
            {"let", ConstituentToken::VARIABLE_DECLARATION_LET},
            {"op", ConstituentToken::VARIABLE_DECLARATION_OP},
            {"fn", ConstituentToken::FUNCTION_DECLARATION_F},
            {"fn_", ConstituentToken::FUNCTION_DECLARATION_F_VOID},
            {"fn?", ConstituentToken::FUNCTION_DECLARATION_F_OPTIONAL},
            {"fn/", ConstituentToken::FUNCTION_DECLARATION_F_LAMBDA},
            {"ret", ConstituentToken::FUNCTION_RETURN},
            {"rev", ConstituentToken::FUNCTION_DECLARATION_F_VOID},
            {"if", ConstituentToken::CONDITIONAL_IF},
            {"else", ConstituentToken::CONDITIONAL_ELSE},
            {"else if", ConstituentToken::CONDITIONAL_ELSE_IF},
            {"match", ConstituentToken::PATTERN_MATCH},
            {"null", ConstituentToken::NULL_LITERAL},
            {":", ConstituentToken::COLON},
            {"'", ConstituentToken::SINGLE_QUOTE},
            {"\"", ConstituentToken::DOUBLE_QUOTE},
            {",", ConstituentToken::COMMA},
            {"[", ConstituentToken::BRACKET_OPEN},
            {"]", ConstituentToken::BRACKET_CLOSE},
            {"{", ConstituentToken::BRACE_OPEN},
            {"}", ConstituentToken::BRACE_CLOSE},
            {"(", ConstituentToken::PARENTHESES_OPEN},
            {")", ConstituentToken::PARENTHESES_CLOSE},
            {"//", ConstituentToken::COMMENT_LINE},
            {"/*", ConstituentToken::COMMENT_BLOCK_OPEN},
            {"*/", ConstituentToken::COMMENT_BLOCK_CLOSE},
            {";", ConstituentToken::NEWLINE},
            {"=", ConstituentToken::ASSIGNMENT},
            {"+", ConstituentToken::ARITHMETIC_ADD},
            {"-", ConstituentToken::ARITHMETIC_SUB},
            {"*", ConstituentToken::ARITHMETIC_MUL},
            {"/", ConstituentToken::ARITHMETIC_DIV},
            {"%", ConstituentToken::ARITHMETIC_MOD},
            {"==", ConstituentToken::COMPARISON_EQ},
            {"!=", ConstituentToken::COMPARISON_NEQ},
            {"<", ConstituentToken::COMPARISON_LT},
            {">", ConstituentToken::COMPARISON_GT},
            {"<=", ConstituentToken::COMPARISON_LTE},
            {">=", ConstituentToken::COMPARISON_GTE},
            {"&&", ConstituentToken::LOGICAL_AND},
            {"||", ConstituentToken::LOGICAL_OR},
            {"!", ConstituentToken::LOGICAL_NOT},
            {"&", ConstituentToken::BITWISE_AND},
            {"|", ConstituentToken::BITWISE_OR},
            {"^", ConstituentToken::BITWISE_XOR},
            {"~", ConstituentToken::BITWISE_NEG},
            {"<<", ConstituentToken::BITWISE_SL},
            {">>", ConstituentToken::BITWISE_SR},
            {"->", ConstituentToken::ARROW}});

#endif // PAIRING_MAP_HPP
