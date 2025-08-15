#ifndef RAT_SOURCE_HPP
#define RAT_SOURCE_HPP

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

class RatSource {
public:
  RatSource(const std::string &filename);
  ~RatSource();
  RatSource(const RatSource &other);
  RatSource &operator=(const RatSource &other);
  void close();
  void reset();
  std::string readLine();
  char readChar();
  char peekChar();
  void reverse();
  bool advanceWhitespace();
  void seekLine(const unsigned &idx);
  unsigned int getLineNum() const;
  unsigned int getColNum() const;

private:
  std::string filename_;
  std::ifstream fs_;
  unsigned int line_num_;
  unsigned int col_num_;
  unsigned int prev_line_num_;
  unsigned int prev_col_num_;

  inline void resetLine();
  inline void resetCol();
  inline void nextLine();
  inline void nextCol();
  inline void prevLine();
  inline void prevCol();
};

#endif // RAT_SOURCE_HPP
