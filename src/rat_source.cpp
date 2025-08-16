#include "rat_source.hpp"

RatSource::RatSource(const std::string &filename) : filename_(filename) {
  if (!filename.ends_with(".rat")) {
    throw std::invalid_argument("Invalid filename extension: " + filename);
  }
  fs_.open(filename);
  if (!fs_.is_open()) {
    throw std::runtime_error("Unable to open file: " + filename);
  }
  line_num_ = prev_line_num_ = 1;
  col_num_ = prev_col_num_ = 1;
}

RatSource::~RatSource() { fs_.close(); }

RatSource::RatSource(const RatSource &other) {
  if (this != &other) {
    fs_.close();
    this->filename_ = other.filename_;
    this->fs_.open(this->filename_);
    this->line_num_ = other.line_num_;
    this->prev_line_num_ = other.prev_line_num_;
    this->col_num_ = other.col_num_;
    this->prev_col_num_ = other.prev_col_num_;
  }
}

RatSource &RatSource::operator=(const RatSource &other) {
  if (this != &other) {
    fs_.close();
    this->filename_ = other.filename_;
    this->fs_.open(this->filename_);
    this->line_num_ = other.line_num_;
    this->prev_line_num_ = other.prev_line_num_;
    this->col_num_ = other.col_num_;
    this->prev_col_num_ = other.prev_col_num_;
  }
  return *this;
}

void RatSource::close() { fs_.close(); }

void RatSource::reset() {
  fs_.clear();
  fs_.seekg(0, std::ios::beg);
  line_num_ = col_num_ = prev_line_num_ = prev_col_num_ = 1;
}

std::string RatSource::readLine() {

  resetCol();
  std::string line;
  if (std::getline(fs_, line)) {
    nextLine();
    return line;
  }
  return std::string();
}

char RatSource::readChar() {
  char ch;
  if (fs_.get(ch)) {
    nextCol();
    if (ch == '\n') {
      resetCol();
      nextLine();
    }
  }
  if (fs_.eof()) {
    return -1;
  }
  return ch;
}
char RatSource::peekChar() { return fs_.peek(); }

void RatSource::reverse() {
  if (!fs_.good()) {
    throw std::runtime_error("error: cannot reverse at current location");
  }
  fs_.unget();
  if (fs_.eof()) {
    throw std::runtime_error("error: cannot reverse at current location");
  }
  if (fs_.peek() == '\n') {
    prevLine();
  }
  prevCol();
}

bool RatSource::advanceWhitespace() {
  bool is_newline = false;
  char ch;
  if (!std::isspace(fs_.peek())) {
    return is_newline;
  }
  while (fs_.get(ch)) {
    nextCol();
    if (ch == '\n') {
      is_newline = true;
      resetCol();
      nextLine();
    }
    if (!std::isspace(ch)) {
      fs_.unget();
      prevCol();
      return is_newline;
    }
  }
  return false; // EOF
}
void RatSource::seekLine(const unsigned &idx) {
  std::string line;
  reset();
  while (std::getline(fs_, line)) {
    nextLine();
    if (line_num_ == idx) {
      col_num_ = 1;
      return;
    }
  }
  std::cerr << "file length: '" << line_num_ << "' requested '" << idx << '\''
            << std::endl;
  throw std::invalid_argument("error: line out of bounds");
}
unsigned int RatSource::getLineNum() const { return line_num_; }

unsigned int RatSource::getColNum() const { return col_num_; }

inline void RatSource::resetLine() {
  prev_line_num_ = line_num_;
  line_num_ = 1;
}

inline void RatSource::resetCol() {
  prev_col_num_ = col_num_;
  col_num_ = 1;
}

inline void RatSource::nextLine() {
  prev_line_num_ = line_num_;
  ++line_num_;
}

inline void RatSource::nextCol() {
  prev_col_num_ = col_num_;
  ++col_num_;
}

inline void RatSource::prevLine() {
  line_num_ = prev_line_num_;
  prev_line_num_ = (line_num_ > 1) ? line_num_ - 1 : 1;
}

inline void RatSource::prevCol() {
  col_num_ = prev_col_num_;
  prev_col_num_ = (col_num_ > 1) ? col_num_ - 1 : 1;
}
