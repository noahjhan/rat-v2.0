#ifndef LEXER_HPP
#define LEXER_HPP

#include <deque>
#include <regex>
#include <unordered_map>
#include <unordered_set>

#include "debug.hpp"
#include "rat_source.hpp"
#include "token.hpp"

extern const std::unordered_set<char> punctuators;
extern const std::unordered_set<std::string> keywords;
extern const std::unordered_set<std::string> operators;
extern const std::unordered_set<std::string> types;
extern const std::unordered_set<char> non_digits;
extern const std::unordered_set<char> escape_chars;
extern const std::unordered_map<GenericToken, std::string> str_representation;
extern const std::string comment_separator;

class Lexer {
public:
  Lexer(const RatSource &source_file);
  bool advanceToken();
  void debugPrinter(bool verbose = false);
  std::deque<Token> &getTokens();

private:
  inline bool isAcceptableIdentifier(const char &ch) const;
  inline bool isAcceptableStringLiteral(const char &ch) const;
  inline bool isAcceptableNumericLiteral(const char &ch) const;
  inline bool isAcceptableCharLiteral(const char &ch) const;
  inline bool isAcceptableNumericSequence(const std::string &value) const;

  inline void processPartial(const std::string &partial, const char &curr,
                             const unsigned int &line_num,
                             const unsigned int &col_num);

  void advanceStringLiteral();
  void advanceCharLiteral();

  void dequePush(GenericToken type, const std::string &value,
                 const unsigned int &line_num, const unsigned int &col_num);

  RatSource source_file_;
  std::deque<Token> tokens_;
};

#endif // LEXER_HPP
