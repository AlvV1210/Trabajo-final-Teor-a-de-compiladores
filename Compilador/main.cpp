#include <iostream>
#include <fstream>
#include <string>

#include "antlr4-runtime.h"
#include "RecipeLangLexer.h"
#include "RecipeLangParser.h"
#include "RecipesVisitorImpl.h"

using namespace antlr4;
using namespace std;

int main(int argc, const char* argv[]) {

    if (argc < 2) {
        cerr << "Uso: ./compilador archivo.recipe\n";
        return 1;
    }

    string filename = argv[1];
    ifstream stream(filename);

    if (!stream.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << "\n";
        return 1;
    }

    ANTLRInputStream input(stream);
    RecipeLangLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    RecipeLangParser parser(&tokens);
    tree::ParseTree *tree = parser.program();

    // Aquí el visitor imprime
    RecipesVisitorImpl visitor;
    visitor.visit(tree);

    return 0;
}