#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "rat_source.hpp"

void debugPrintln(RatSource &source_file, const unsigned int &line_num);
void debugLineCol(const unsigned int &line_num, const unsigned int &col_num);

#endif // DEBUG_HPP
