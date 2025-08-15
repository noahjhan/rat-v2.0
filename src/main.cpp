#include "rat_source.hpp"

int main() {
  try {
    const std::string filename = "../data/empty.rat";
    RatSource source(filename);
    while (source.peekChar() != EOF) {
      source.readChar();
      source.reverse();
      unsigned int line = source.getLineNum();
      unsigned int col = source.getColNum();
      std::cout << source.readChar() << " - line: " << line << " col: " << col
                << std::endl;
      source.advanceWhitespace();
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
