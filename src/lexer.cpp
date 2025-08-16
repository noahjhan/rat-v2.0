#include "lexer.hpp"

const std::unordered_set<char> punctuators = {':', '\'', '\"', '[', ']',
                                              '{', '}',  '(',  ')', ','};

const std::unordered_set<std::string> keywords = {
"let", "op", "if", "else", "fn", "fn_", "fn?", "fn/", "null", "ret", "rev"};

const std::unordered_set<std::string> operators = {
"=",  "+",  "-", "*", "/", "%", "==", "!=", "<",  ">",  "<=", ">=",
"&&", "||", "!", "&", "|", "^", "~",  "<<", ">>", "->", "=>"};

const std::unordered_set<std::string> types = {
"int",     "float", "double", "bool",   "char",  "long",  "short",
"pointer", "uint",  "ulong",  "ushort", "uchar", "string"};

const std::unordered_set<char> non_digits = {'u', 'i', 'f', 'd', 'l', 's', 'c', '.', '-'};

const std::unordered_set<char> escape_chars = {'n', 't',  'r', 'b',  'f',  'v',
                                               'a', '\\', '?', '\'', '\"', '0'};

const std::unordered_map<GenericToken, std::string> str_representation = {
{GenericToken::IDENTIFIER, "Identifier"},
{GenericToken::KEYWORD, "Keyword"},
{GenericToken::NUMERIC_LITERAL, "Numeric Literal"},
{GenericToken::STRING_LITERAL, "String Literal"},
{GenericToken::CHAR_LITERAL, "Char Literal"},
{GenericToken::PUNCTUATOR, "Punctuator"},
{GenericToken::OPERATOR, "Operator"},
{GenericToken::TYPE, "Type"},
{GenericToken::COMMENT_BLOCK, "Comment Block"},
{GenericToken::COMMENT_SEPARATOR, "Comment Separator"}};

const std::string comment_separator = "//";

Lexer::Lexer(const RatSource &source_file) : source_file_(source_file)
{
}

inline bool Lexer::isAcceptableIdentifier(const char &ch) const
{
    return std::isalnum(ch) || ch == '_';
}
inline bool Lexer::isAcceptableStringLiteral(const char &ch) const
{
    return std::isprint(ch) || std::isspace(ch);
}
inline bool Lexer::isAcceptableNumericLiteral(const char &ch) const
{
    return std::isdigit(ch) || (non_digits.find(ch) != non_digits.end());
}
inline bool Lexer::isAcceptableCharLiteral(const char &ch) const
{
    return isprint(ch);
}
inline bool Lexer::isAcceptableNumericSequence(const std::string &value) const
{
    static const std::regex pattern(R"((\d+)((\.)?(\d*)?(d|f)?|(u)?[icls]?))");
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

inline void Lexer::processPartial(const std::string &partial, const char &curr,
                                  const unsigned int &line_num,
                                  const unsigned int &col_num)
{
    bool is_numeric = false;
    bool is_identifier = (!std::isdigit(partial.front()));
    if (isAcceptableNumericSequence(partial)) {
        is_numeric = true;
    }
    for (const auto &ch : partial) {
        if (!isAcceptableIdentifier(ch)) {
            is_identifier = false;
        }
    }

    if (is_numeric && is_identifier) {
        debugLineCol(line_num, col_num);
        std::cerr << "received: '" << partial << '\'' << std::endl;
        debugPrintln(source_file_, line_num, col_num);
        throw std::invalid_argument("ambiguous token");
    }
    else if (is_identifier) {
        dequePush(GenericToken::IDENTIFIER, partial, line_num, col_num);
    }
    else if (is_numeric) {
        dequePush(GenericToken::NUMERIC_LITERAL, partial, line_num, col_num);
    }
    else {
        debugLineCol(line_num, col_num);
        std::cerr << "received: '" << partial << '\'' << std::endl;
        debugPrintln(source_file_, line_num, col_num);
        throw std::invalid_argument("unrecognized token");
    }

    if (curr == '\n') {
        dequePush(GenericToken::PUNCTUATOR, ";", 0, 0);
    }
}

bool Lexer::advanceToken()
{
    bool is_newline = source_file_.advanceWhitespace();
    if (is_newline) {
        // for now end line punctuator is 0 0
        dequePush(GenericToken::PUNCTUATOR, ";", 0, 0);
    }
    // test 2 lines of whitespace before

    const unsigned int line_num = source_file_.getLineNum();
    const unsigned int col_num = source_file_.getColNum();

    char curr = source_file_.readChar();
    std::string partial;

    if (curr == -1) {
        return false;
    }

    // process punctuators
    if (punctuators.find(curr) != punctuators.end()) {
        // single-quotes -> process as character literal
        if (curr == '\"') {
            source_file_.reverse();
            advanceStringLiteral();
            return true;
            // double-quotes -> process as string literal
        }
        else if (curr == '\'') {
            source_file_.reverse();
            advanceCharLiteral();
            return true;
            // else -> process as other punctuator
        }
        else {
            partial.push_back(curr);
            dequePush(GenericToken::PUNCTUATOR, partial, line_num, col_num);
            return true;
        }
    }

    while (!std::isspace(curr) && curr != -1) {
        if (punctuators.find(curr) != punctuators.end()) {
            source_file_.reverse();
            break;
        }
        partial.push_back(curr);

        // keywords
        if (keywords.find(partial) != keywords.end() &&
            !isAcceptableIdentifier(source_file_.peekChar())) {
            char peek = source_file_.peekChar();
            partial.push_back(peek);
            if (keywords.find(partial) == keywords.end()) {
                partial.pop_back();
            }
            else {
                curr = source_file_.readChar();
            }
            dequePush(GenericToken::KEYWORD, partial, line_num, col_num);
            return true;
        }

        // types
        if (types.find(partial) != types.end() &&
            !isAcceptableIdentifier(source_file_.peekChar())) {
            dequePush(GenericToken::TYPE, partial, line_num, col_num);
            return true;
        }

        // comments
        if (partial == "/" && source_file_.peekChar() == '/') {
            partial.push_back(source_file_.readChar());
            dequePush(GenericToken::COMMENT_SEPARATOR, partial, line_num, col_num);
            partial = source_file_.readLine();
            dequePush(GenericToken::COMMENT_BLOCK, partial, line_num, col_num);
            return true;
        }

        // operators
        if (operators.find(partial) != operators.end() &&
            !isAcceptableIdentifier(source_file_.peekChar())) {
            char peek = source_file_.peekChar();
            partial.push_back(peek);
            if (operators.find(partial) == operators.end()) {
                partial.pop_back();
            }
            else {
                curr = source_file_.readChar();
            }
            dequePush(GenericToken::OPERATOR, partial, line_num, col_num);
            return true;
        }

        curr = source_file_.readChar();
    }

    processPartial(partial, curr, line_num, col_num);
    return true;
}

void Lexer::advanceStringLiteral()
{
    const unsigned int line_num = source_file_.getLineNum();
    const unsigned int col_num = source_file_.getColNum();

    char curr = source_file_.readChar();
    std::string partial;
    while (isAcceptableStringLiteral(curr)) {
        partial.push_back(curr);
        curr = source_file_.readChar();
        if (curr == '\"') {
            // end of literal reached
            partial.push_back(curr);
            dequePush(GenericToken::STRING_LITERAL, partial, line_num, col_num);
            return;
        }
        if (curr == -1 || curr == '\n') {
            debugLineCol(line_num, col_num);
            debugPrintln(source_file_, line_num, col_num);
            throw std::invalid_argument(
            "error: string literals must be surrounded by double-quotes");
        }
    }
}
void Lexer::advanceCharLiteral()
{
    const unsigned int line_num = source_file_.getLineNum();
    const unsigned int col_num = source_file_.getColNum();
    char curr = source_file_.readChar();
    std::string partial;

    while (isAcceptableCharLiteral(curr)) {
        partial.push_back(curr);
        curr = source_file_.readChar();
        if (curr == '\'') {
            // end of literal reached
            partial.push_back(curr);
            if (source_file_.peekChar() == '\'') {
                partial.push_back(source_file_.readChar());
            }
            if ((partial.size() != 3 && partial.size() != 4) ||
                (partial.size() == 4 && partial[1] != '\\') ||
                (partial.size() == 3 && partial[1] == '\\') ||
                (escape_chars.find(partial[2]) == escape_chars.end())) {
                debugLineCol(line_num, col_num);
                std::cerr << "received: '" << partial << '\'' << std::endl;
                debugPrintln(source_file_, line_num, col_num);
                throw std::invalid_argument("syntax error: unrecognized char literal");
            }

            dequePush(GenericToken::CHAR_LITERAL, partial, line_num, col_num);
            return;
        }
        if (curr == -1) {
            // end of file reached
            debugLineCol(line_num, col_num);
            debugPrintln(source_file_, line_num, col_num);
            throw std::invalid_argument(
            "error: character literals must be surrounded by single-quotes");
        }
    }
}

void Lexer::dequePush(GenericToken type, const std::string &value,
                      const unsigned int &line_num, const unsigned int &col_num)
{
    if (value == ";" && (!tokens_.empty() && tokens_.back().value == ";")) {
        return;
    }
    Token t;
    t.type = type;
    t.value = value;
    t.line_num = line_num;
    t.col_num = col_num;
    tokens_.push_back(t);
}

void Lexer::debugPrinter(bool verbose)
{
    if (verbose) {
        std::cout << "DEBUG" << std::endl;
        for (const auto &t : tokens_) {
            std::string str = str_representation.at(t.type);
            std::cout << "token: " << str << std::endl;
            std::cout << "value: '" << t.value << '\'' << std::endl;
            std::cout << "line: " << t.line_num << std::endl;
            std::cout << "col: " << t.col_num << std::endl;
            std::cout << std::endl;
        }
    }
    else {
        for (const auto &t : tokens_) {
            std::cout << '\'' << t.value << '\'' << std::endl;
        }
    }
}

std::deque<Token> &Lexer::getTokens()
{
    return tokens_;
}
