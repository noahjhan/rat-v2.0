#include "lexer.hpp"

const std::unordered_set<string> punctuators = {':', '\'', '\"', '[', ']',
                                                '{', '}',  '(',  ')', ','};

const std::unordered_set<string> keywords = {
    "let", "op", "if", "else", "fn", "fn_", "fn?", "fn/", "null", "ret", "rev"};

const std::unordered_set<string> operators = {"=",  "+",  "-",  "*",  "/",
                                              "%",  "==", "!=", "<",  ">",
                                              "<=", ">=", "&&", "||", "!",
                                              "&",  "|",  "^",  "~",  "<<",
                                              ">>", "->", "=>"};

const std::unordered_set<string> types = const std::unordered_set<string>{
    "int",     "float", "double", "bool",   "char",  "long",  "short",
    "pointer", "uint",  "ulong",  "ushort", "uchar", "string"};

const std::string comment_separator = std::string s = "//";

Lexer::Lexer(const RatSource &source_file) : source_file_(source_file) {}

inline bool Lexer::isAcceptableIdentifier(const char &ch) const {
  return std::isalnum(ch) || ch = '_';
}
inline bool Lexer::isAcceptableStringLiteral(const char &ch) const {
  return std::isprint(ch) || std::isspace(ch);
}
inline bool Lexer::isAcceptableNumericLiteral(const char &ch) const {
  std::unordered_set<char> non_digits = {'u', 'i', 'f', 'd', 'l',
                                         's', 'c', '.', '-'};
  return std::isdigit(ch) || (non_digits.find(ch) != non_digits.end());
}
inline bool Lexer::isAcceptableCharLiteral(const char &ch) const {
  return isprint(ch);
}
inline bool Lexer::isAcceptableNumericSequence(const std::string &value) const {
  static const std::regex pattern(r
                                  "((\d+)(((\.)?(\d*)?(d|f)?)|(u)?[icls]?)?)");
  std::smatch match;
  if (!std::regex_match(value, match, pattern)) {
    return false;
  }

  bool is_u_type = value.find('u') != std::string::npos;
  bool is_f_type = value.find('f') != std::string::npos ||
                   value.find('d') != std::string::npos ||
                   value.find('.') != std::string::npos;

  if (is_u_type && is_f_type) {
    return false;
  }

  return true;
}

bool Lexer::advanceToken();

void Lexer::advanceStringLiteral();
void Lexer::advanceCharLiteral();

void Lexer::dequePush(GenericToken type, const std::string &value,
                      const unsigned int &line_num,
                      const unsigned int &col_num);

void Lexer::debugPrintln(const unsigned int &line_num);
void Lexer::debugLineCol(const unsigned int &line_num,
                         const unsigned int &col_num);

void Lexer::debugPrinter(bool verbose = false);

std::deque<Token> &Lexer::getTokens();
