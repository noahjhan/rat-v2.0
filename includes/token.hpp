#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum class GenericToken {
  /// generic
  IDENTIFIER, // user-input [a-zA-Z0-9_]
  KEYWORD,
  NUMERIC_LITERAL, // expect a lot of conversions
  STRING_LITERAL,
  CHAR_LITERAL,
  PUNCTUATOR,
  OPERATOR,
  TYPE
};

struct Token {
  GenericToken type;
  std::string value;
  unsigned int line_num;
  unsigned int col_num;
};

#endif // TOKEN_HPP
