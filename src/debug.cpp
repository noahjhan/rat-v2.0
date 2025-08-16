#include "debug.hpp"

void debugPrintln(RatSource &source_file, const unsigned int &line_num,
                  const unsigned int &col_num)
{
    source_file.seekLine(line_num);
    std::cerr << line_num << " | " << source_file.readLine() << '\n';
    std::cerr << std::string(col_num + 3, ' ') << '^' << std::endl;
}

void debugLineCol(const unsigned int &line_num, const unsigned int &col_num)
{
    std::cerr << "at line: " << line_num << ", col: " << col_num << std::endl;
}
