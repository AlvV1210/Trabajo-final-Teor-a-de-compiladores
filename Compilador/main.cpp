#include <iostream>
#include <fstream>
#include <string>

#include "antlr4-runtime.h"
#include "RecipesLexer.h"
#include "RecipesParser.h"
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

    // 1. Crear input ANTLR desde archivo
    ANTLRInputStream input(stream);

    // 2. Lexer
    RecipesLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    // 3. Parser
    RecipesParser parser(&tokens);
    tree::ParseTree *tree = parser.program();

    // 4. Visitor 
    RecipesVisitorImpl visitor;
    visitor.visit(tree);

    // 5. Mostrar resultados del visitor
    cout << "\n===== RESULTADOS DEL COMPILADOR =====\n";
    cout << "Costo total calculado: " << visitor.totalCost << "\n";
    cout << "Calorías totales: " << visitor.totalCalories << "\n";

    cout << "Ingredientes registrados: \n";
    for (auto &p : visitor.ingredients) {
    cout << "  - " << p.first << " = " << p.second << "\n";
    }

    return 0;
}
