#ifndef LEXER_HPP
#define LEXER_HPP

#include <queue>
#include <unordered_set>

#include "rat_source.hpp"
#include "token.hpp"

extern const std::unordered_set<char> punctuators;
extern const std::unordered_set<std::string> keywords;
extern const std::unordered_set<std::string> operators;
extern const std::unordered_set<std::string> types;
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

  void advanceStringLiteral();
  void advanceCharLiteral();

  void dequePush(GenericToken type, const std::string &value,
                 const unsigned int &line_num, const unsigned int &col_num);

  void debugPrintln(const unsigned int &line_num);
  void debugLineCol(const unsigned int &line_num, const unsigned int &col_num);

  RatSource source_file_;
  std::queue<Token> tokens_;
};

#endif // LEXER_HPP
