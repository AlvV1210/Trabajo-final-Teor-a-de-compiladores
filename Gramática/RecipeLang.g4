grammar RecipeLang;

// Raíz
program : recipeStmt+ EOF ;

// =============================
// DECLARACIÓN DE RECETA
// =============================
recipeStmt
    : 'receta' STRING '{'
        rindeDecl
        tiempoPrepDecl
        tiempoCoccDecl
        tagsDecl
        ingredientesBlock
        pasosBlock
        nutricionBlock
      '}'
    ;

// Metadatos
rindeDecl          : 'rinde' INT ('porciones' | 'porcion')? ;
tiempoPrepDecl     : 'tiempo_preparacion' INT ('minutos' | 'minuto')? ;
tiempoCoccDecl     : 'tiempo_coccion' INT ('minutos' | 'minuto')? ;

// Tags
tagsDecl : 'tags' ':' '[' tag (',' tag)* ']' ;
tag      : ID | STRING ;

// Ingredientes
ingredientesBlock : 'ingredientes' '{' ingredientStmt* '}' ;

ingredientStmt
    : 'ingrediente' ID
      'cantidad' AMOUNT
      'costo'     FLOAT
      'calorias'  INT
    ;

// Pasos
pasosBlock : 'pasos' '{' stepStmt* '}' ;

stepStmt
    : 'paso' INT
      'descripcion' STRING
      ('usa' ID (',' ID)*)?
      ('depende_de' INT (',' INT)*)?
    ;

// Nutrición
nutricionBlock
    : 'nutricion' '{'
        totalCostStmt?
        totalCaloriesStmt?
      '}'
    ;

totalCostStmt     : 'total_costo' ;
totalCaloriesStmt : 'total_calorias' ;

// =============================
// LÉXICO
// =============================
ID      : [a-zA-Z_][a-zA-Z_0-9]* ;
FLOAT   : [0-9]+ '.' [0-9]* ;
INT     : [0-9]+ ;
AMOUNT  : [0-9]+[a-zA-Z]+ ;
STRING  : '"' (~["\r\n])* '"' ;

WS      : [ \t\r\n]+ -> skip ;
