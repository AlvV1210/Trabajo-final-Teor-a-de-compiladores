#include "RecipesVisitorImpl.h"
#include <iostream>

using namespace std;

antlrcpp::Any RecipesVisitorImpl::visitRecipeStmt(RecipesParser::RecipeStmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any RecipesVisitorImpl::visitIngredientStmt(RecipesParser::IngredientStmtContext *ctx) {

    string name = ctx->ID()->getText();

    // Guardamos solo 1 unidad (puedo mejorarlo si quieres)
    ingredients[name] = 1.0;

    // costPerUnit existe?
    if (ctx->FLOAT() != nullptr) {
        double cost = stod(ctx->FLOAT()->getText());
        totalCost += cost;
    }

    // calories existe?
    if (ctx->INT() != nullptr) {
        double cal = stod(ctx->INT()->getText());
        totalCalories += cal;
    }

    return nullptr;
}

antlrcpp::Any RecipesVisitorImpl::visitTotalCostStmt(RecipesParser::TotalCostStmtContext *ctx) {
    cout << "Total Cost: " << totalCost << endl;
    return nullptr;
}

antlrcpp::Any RecipesVisitorImpl::visitTotalCaloriesStmt(RecipesParser::TotalCaloriesStmtContext *ctx) {
    cout << "Total Calories: " << totalCalories << endl;
    return nullptr;
}