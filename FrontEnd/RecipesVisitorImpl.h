#ifndef RECIPES_VISITOR_IMPL_H
#define RECIPES_VISITOR_IMPL_H

#include "RecipesBaseVisitor.h"
#include <map>
#include <string>

class RecipesVisitorImpl : public RecipesBaseVisitor {
public:
    double totalCost = 0.0;
    double totalCalories = 0.0;
    std::map<std::string, double> ingredients;

    virtual antlrcpp::Any visitRecipeStmt(RecipesParser::RecipeStmtContext *ctx) override;
    virtual antlrcpp::Any visitTotalCostStmt(RecipesParser::TotalCostStmtContext *ctx) override;
    virtual antlrcpp::Any visitTotalCaloriesStmt(RecipesParser::TotalCaloriesStmtContext *ctx) override;
    virtual antlrcpp::Any visitIngredientStmt(RecipesParser::IngredientStmtContext *ctx) override;
};

#endif