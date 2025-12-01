#ifndef RECIPES_VISITOR_IMPL_H
#define RECIPES_VISITOR_IMPL_H

#include "RecipeLangBaseVisitor.h"
#include <map>
#include <string>
#include <vector>

struct Ingredient {
    std::string amount;
    double cost = 0.0;
    int calories = 0;
};

struct Step {
    int number = 0;
    std::string description;
    std::vector<std::string> uses;
    std::vector<int> depends;
};

struct Recipe {
    std::string name;
    int rinde = 0;
    int tiempo_preparacion = 0;
    int tiempo_coccion = 0;
    std::vector<std::string> tags;
    std::map<std::string, Ingredient> ingredients;
    std::vector<Step> steps;
    double totalCost = 0.0;
    int totalCalories = 0;
};

class RecipesVisitorImpl : public RecipeLangBaseVisitor {
public:
    Recipe current;

    // Visitadores
    virtual antlrcpp::Any visitRecipeStmt(RecipeLangParser::RecipeStmtContext *ctx) override;
    virtual antlrcpp::Any visitRindeDecl(RecipeLangParser::RindeDeclContext *ctx) override;
    virtual antlrcpp::Any visitTiempoPrepDecl(RecipeLangParser::TiempoPrepDeclContext *ctx) override;
    virtual antlrcpp::Any visitTiempoCoccDecl(RecipeLangParser::TiempoCoccDeclContext *ctx) override;

    virtual antlrcpp::Any visitTagsDecl(RecipeLangParser::TagsDeclContext *ctx) override;
    virtual antlrcpp::Any visitTag(RecipeLangParser::TagContext *ctx) override;

    virtual antlrcpp::Any visitIngredientStmt(RecipeLangParser::IngredientStmtContext *ctx) override;
    virtual antlrcpp::Any visitIngredientesBlock(RecipeLangParser::IngredientesBlockContext *ctx) override;

    virtual antlrcpp::Any visitStepStmt(RecipeLangParser::StepStmtContext *ctx) override;
    virtual antlrcpp::Any visitPasosBlock(RecipeLangParser::PasosBlockContext *ctx) override;

    virtual antlrcpp::Any visitNutricionBlock(RecipeLangParser::NutricionBlockContext *ctx) override;
    virtual antlrcpp::Any visitTotalCostStmt(RecipeLangParser::TotalCostStmtContext *ctx) override;
    virtual antlrcpp::Any visitTotalCaloriesStmt(RecipeLangParser::TotalCaloriesStmtContext *ctx) override;
};
#endif