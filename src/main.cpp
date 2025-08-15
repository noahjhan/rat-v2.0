#include "lexer.hpp"
#include "rat_source.hpp"

int main() {
  std::string filename = "../data/empty.rat";
  RatSource source(filename);
  Lexer lexer(source);
  return 0;
}
