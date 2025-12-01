#include "RecipesVisitorImpl.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Helper to strip quotes from STRING tokens
static string stripQuotes(const string &s) {
    if (s.size() >= 2 && s.front() == '\"' && s.back() == '\"') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

antlrcpp::Any RecipesVisitorImpl::visitRecipeStmt(RecipeLangParser::RecipeStmtContext *ctx) {
    // recipeStmt : 'receta' STRING '{' ... '}'
    if (ctx->STRING() != nullptr) {
        current = Recipe();
        current.name = stripQuotes(ctx->STRING()->getText());
    } else {
        current = Recipe();
    }

    // Visit children to fill current
    visitChildren(ctx);

    // Print formatted output
    cout << "===== RECETA COMPILADA =====" << endl;
    cout << "Nombre: " << current.name << endl;
    cout << "Rinde: " << current.rinde << " porciones" << endl;
    cout << "Tiempo preparación: " << current.tiempo_preparacion << " minutos" << endl;
    cout << "Tiempo cocción: " << current.tiempo_coccion << " minutos" << endl;
    cout << endl;

    cout << "Tags:" << endl;
    for (auto &t : current.tags) {
        cout << " - " << t << endl;
    }
    cout << endl;

    cout << "Ingredientes:" << endl;
    for (auto &p : current.ingredients) {
        const Ingredient &ing = p.second;
        cout << " - " << p.first << ": " << ing.amount
             << " | costo " << ing.cost
             << " | calorías " << ing.calories << endl;
    }
    cout << endl;

    cout << "Pasos:" << endl;
    for (const auto &s : current.steps) {
        cout << s.number << ". " << s.description;
        if (!s.uses.empty()) {
            cout << " (usa: ";
            for (size_t i = 0; i < s.uses.size(); ++i) {
                if (i) cout << ", ";
                cout << s.uses[i];
            }
            cout << ")";
        }
        if (!s.depends.empty()) {
            cout << " | depende de: ";
            for (size_t i = 0; i < s.depends.size(); ++i) {
                if (i) cout << ", ";
                cout << s.depends[i];
            }
        }
        cout << endl;
    }
    cout << endl;

    cout << "Nutrición:" << endl;
    cout << " - total costo: " << current.totalCost << endl;
    cout << " - total calorías: " << current.totalCalories << endl;
    cout << endl;

    return nullptr;
}

antlrcpp::Any RecipesVisitorImpl::visitRindeDecl(RecipeLangParser::RindeDeclContext *ctx) {
    if (ctx->INT() != nullptr) {
        current.rinde = stoi(ctx->INT()->getText());
    }
    return nullptr;
}

antlrcpp::Any RecipesVisitorImpl::visitTiempoPrepDecl(RecipeLangParser::TiempoPrepDeclContext *ctx) {
    if (ctx->INT() != nullptr) {
        current.tiempo_preparacion = stoi(ctx->INT()->getText());
    }
    return nullptr;
}

antlrcpp::Any RecipesVisitorImpl::visitTiempoCoccDecl(RecipeLangParser::TiempoCoccDeclContext *ctx) {
    if (ctx->INT() != nullptr) {
        current.tiempo_coccion = stoi(ctx->INT()->getText());
    }
    return nullptr;
}

antlrcpp::Any RecipesVisitorImpl::visitTagsDecl(RecipeLangParser::TagsDeclContext *ctx) {
    auto tags = ctx->tag();
    for (size_t i = 0; i < tags.size(); ++i) {
        string t = tags[i]->getText();
        current.tags.push_back(stripQuotes(t));
    }
    return nullptr;
}

antlrcpp::Any RecipesVisitorImpl::visitTag(RecipeLangParser::TagContext *ctx) {
    // Not used directly (handled in visitTagsDecl), keep for completeness.
    return nullptr;
}

antlrcpp::Any RecipesVisitorImpl::visitIngredientStmt(RecipeLangParser::IngredientStmtContext *ctx) {
    // ingredientStmt : 'ingrediente' ID 'cantidad' AMOUNT 'costo' FLOAT 'calorias' INT ;
    string name = ctx->ID()->getText();
    string amount = ctx->AMOUNT()->getText();
    double cost = 0.0;
    int calories = 0;

    if (ctx->FLOAT() != nullptr) {
        cost = stod(ctx->FLOAT()->getText());
    }
    if (ctx->INT() != nullptr) {
        calories = stoi(ctx->INT()->getText());
    }

    Ingredient ing;
    ing.amount = amount;
    ing.cost = cost;
    ing.calories = calories;

    current.ingredients[name] = ing;
    current.totalCost += cost;
    current.totalCalories += calories;

    return nullptr;
}

antlrcpp::Any RecipesVisitorImpl::visitIngredientesBlock(RecipeLangParser::IngredientesBlockContext *ctx) {
    // visit children (ingredientStmt*)
    return visitChildren(ctx);
}

antlrcpp::Any RecipesVisitorImpl::visitStepStmt(RecipeLangParser::StepStmtContext *ctx) {
    // stepStmt : 'paso' INT 'descripcion' STRING ('usa' ID (',' ID)*)? ('depende_de' INT (',' INT)*)? ;
    Step s;
    if (ctx->INT().size() > 0) {
        // first INT is step number
        s.number = stoi(ctx->INT(0)->getText());
    }
    if (ctx->STRING() != nullptr) {
        s.description = stripQuotes(ctx->STRING()->getText());
    }
    // IDs from 'usa'
    for (auto idCtx : ctx->ID()) {
        s.uses.push_back(idCtx->getText());
    }
    // depende_de INTs: any INTs after the first belong to depends
    for (size_t i = 1; i < ctx->INT().size(); ++i) {
        s.depends.push_back(stoi(ctx->INT(i)->getText()));
    }

    current.steps.push_back(s);
    return nullptr;
}

antlrcpp::Any RecipesVisitorImpl::visitPasosBlock(RecipeLangParser::PasosBlockContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any RecipesVisitorImpl::visitNutricionBlock(RecipeLangParser::NutricionBlockContext *ctx) {
    // children may include totalCostStmt and totalCaloriesStmt
    return visitChildren(ctx);
}

antlrcpp::Any RecipesVisitorImpl::visitTotalCostStmt(RecipeLangParser::TotalCostStmtContext *ctx) {
    // we already computed totalCost during ingredient visits; print handled in visitRecipeStmt
    return nullptr;
}

antlrcpp::Any RecipesVisitorImpl::visitTotalCaloriesStmt(RecipeLangParser::TotalCaloriesStmtContext *ctx) {
    return nullptr;
}
