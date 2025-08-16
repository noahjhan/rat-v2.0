#include "lexer.hpp"
#include "pairing_map.hpp"
#include "rat_source.hpp"

int main()
{
    try {
        std::string filename = "../data/empty.rat";
        RatSource source(filename);
        Lexer lexer(source);
        while (lexer.advanceToken())
            ;
        lexer.debugPrinter(true);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
