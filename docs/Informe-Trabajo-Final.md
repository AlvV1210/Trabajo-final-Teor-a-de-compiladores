<p align="center">
  <img src="../imagenes/logo-upc.png" alt="Logo UPC" width="750"/>
</p>
<h2 align="center">FACULTAD DE INGENIERÍA</h2>
<h2 align="center">INFORME DEL TRABAJO FINAL</h2>

<h3 align="center">CURSO DE TEORÍA DE COMPILADORES – 1ACC0218</h3>

<h3 align="center">DISEÑO DE UN LENGUAJE DE PROGRAMACIÓN PARA RECETAS DE COCINA </h3>

<p align="center"><strong>Sección:</strong> 1733</p>


<h4 align="center">Alumnos:</h4>

<p align="center">
  • Leonel Alzamora Gonzales U20231C427 <br/>
  • Aaron Alvaro Felices U202315164 <br/>
  • Sebastian Timana Mendoza U202218729
</p>

<br/>

<p align="center"><strong>Diciembre 2025</strong></p>
<p style="page-break-after: always;"></p>

---

<h2 align="center"> CONTENIDO</h2>
---

# **1. Introducción**

El presente trabajo propone el diseño e implementación de *RecipeLang*, un lenguaje específico de dominio (DSL) orientado a la representación de recetas de cocina. Su propósito es describir de manera clara y estructurada los elementos que conforman un proceso culinario, como ingredientes, unidades, costos, tiempos, dependencias y pasos de preparación.

Lenguajes de propósito general como Python, Java o C++ pueden modelar recetas, pero requieren que el usuario implemente manualmente aspectos esenciales como la conversión de unidades, manejo de dependencias, cálculo de costos, estructura de pasos y tiempos de cocción. Esto incrementa la complejidad del proceso e introduce posibilidades de error, sobre todo para usuarios no especializados en programación.

RecipeLang busca resolver estos inconvenientes proporcionando un lenguaje sencillo, legible y especializado, permitiendo describir recetas de manera natural y obtener automáticamente cálculos relevantes como el costo total, tiempos de cocción y orden de ejecución de los pasos.

---

# **2. Problemática y Motivación**

La representación de recetas en lenguajes tradicionales presenta diversas dificultades:

- La descripción de ingredientes, unidades y cantidades debe codificarse manualmente.  
- Las dependencias entre pasos requieren estructuras adicionales (listas, grafos o manejo manual de índices).  
- El cálculo automático del costo total o duración requiere lógica adicional.  
- La sintaxis resulta poco intuitiva para usuarios ajenos a la informática.  
- El proceso completo es propenso a errores y demanda tiempo.

Surge así la necesidad de construir un lenguaje especializado que facilite la representación de recetas incluyendo:

- Declaración de ingredientes con cantidades y costos.  
- Definición intuitiva de pasos, tiempos y dependencias.  
- Cálculos automáticos asociados (costos, tiempos paralelos o secuenciales).  
- Validación semántica integrada.  

Este proyecto permite mostrar cómo un DSL enfocado en un dominio específico puede simplificar un proceso complejo y dar lugar a herramientas útiles para gastronomía, nutrición, gestión de restaurantes y procesamiento de datos de recetas.

---

# **3. Objetivos**

### **Objetivo General**
Diseñar e implementar un lenguaje de programación especializado en la representación estructurada de recetas de cocina, usando ANTLR4 para la gramática y LLVM IR para la generación de código intermedio.

### **Objetivos Específicos**
- Definir una gramática en ANTLR4 que represente ingredientes, unidades, cantidades, pasos, tiempos, dependencias y costos.  
- Implementar un frontend basado en ANTLR4 capaz de generar el árbol sintáctico y clases para su recorrido.  
- Implementar un backend en C++ encargado del análisis semántico y la generación de LLVM IR.  
- Permitir el cálculo automático de costos y tiempos totales.  
- Validar el compilador mediante pruebas con recetas reales definidas en archivos `.recipe`.  
- Construir una arquitectura modular basada en las etapas clásicas de un compilador.

---

# **4. Gramática en ANTLR4**

La gramática base del lenguaje se encuentra en el archivo:

`grammar/RecipeLang.g4`

Define la sintaxis para representar recetas con los siguientes elementos principales:

### **Componentes principales de la gramática**

- **recipeStmt** — Unidad principal 
- **rindeDecl** → declaración de ingredientes con cantidades, unidades y costo. 
- **tiempoPrepDecl** → Tiempo de preparación
- **tiempoCoccDecl** → Tiempo de cocción
- **tagsDecl** → Etiquetas
- **ingredientesBlock** → Lista de ingredientes
- **pasosBlock** → Lista de pasos
- **nutricionBlock**  → Información nutricional
 
### **Ejemplo representativo de sintaxis permitida**

```recipeLang
receta "Arroz con pollo" {
    rinde 4 porciones
    tiempo_preparacion 20 minutos
    tiempo_coccion 35 minutos

    tags: ["peruano", "casero"]

    ingredientes {
        ingrediente arroz cantidad 200g costo 3.5 calorias 250
        ingrediente pollo cantidad 500g costo 8.0 calorias 600
        ingrediente aceite cantidad 20ml costo 1.0 calorias 180
    }

    pasos {
        paso 1 descripcion "Hervir el agua"
        paso 2 descripcion "Cocinar el arroz" usa arroz depende_de 1
        paso 3 descripcion "Freír el pollo" usa pollo, aceite
    }

    nutricion {
        total_costo
        total_calorias
    }
}

}
```

# **5. Gramática Actualizada**

Durante el desarrollo del compilador se actualizaron y extendieron varias reglas de la gramática con el fin de soportar funcionalidades adicionales y mejorar la claridad del lenguaje. Las mejoras más importantes fueron:

✔ Nueva estructura
- Palabra clave receta como bloque raíz
- Metadatos detallados: rinde, tiempo de preparación, cocción
- Ingredientes con: cantidad, costo y calorías
- Pasos más semánticos: descripción, ingredientes usados, dependencias
- Sistema de tags estilo catálogo
- Sección nutricional automática

Estas mejoras permiten modelar recetas de mayor complejidad y garantizan un análisis semántico consistente en el backend.

---

# **6. Arquitectura del Compilador**

El compilador está dividido en tres capas principales, siguiendo la arquitectura clásica de compiladores:

- frontend/ → Código generado por ANTLR4 (lexer, parser, visitor)
- backend/ → Implementación en C++ para análisis semántico y generación IR
- grammar/ → Gramática RecipeLang.g4
- tests/ → Casos de prueba en .recipe
- dist/ → Archivos .ll generados


---

# **Flujo del Compilador**

## **1. Lexer / Parser (ANTLR4)**
- Toma el archivo `.recipe`.
- Genera el árbol sintáctico (parse tree).

## **2. Generación del AST  (`frontend/generated/`)**
- ANTLR produce clases en C++ para recorrer el AST.
- Se construyen estructuras para representar ingredientes, pasos, unidades y dependencias.

## **3. Análisis Semántico (`backend/`)**
Durante esta etapa se verifican:

- ✔ Unidades válidas  
- ✔ Cálculos de costos  
- ✔ Dependencias correctas entre pasos  
- ✔ Detección de paralelismo  

## **4. Codegen (LLVM IR)**
El backend produce un archivo `.ll` que contiene:

- Variables para cada ingrediente  
- Funciones para cada paso  
- Operaciones aritméticas para costos y tiempos  
- Estructura secuencial o paralela según la receta  

## **5. Salida (.ll)**
- El usuario puede compilar el archivo IR con herramientas LLVM.
- Permite optimizar o generar un ejecutable final.

# **7. Plan de Validación**

## **Estrategia utilizada**

### **1. Casos de prueba creados en `/tests`**
- Incluyen ejemplos reales de recetas (paella, pollo, etc.).
- Permiten probar sintaxis, semántica y generación de código.

---

## **2. Validación Sintáctica**
Cada archivo `.recipe` se ingresa al compilador y se verifica:

- Que cumpla la gramática.
- Que el árbol sintáctico se genere correctamente.

---

## **3. Validación Semántica**
Se revisan los siguientes posibles errores:

- Ingredientes duplicados  
- Unidades no válidas  
- Dependencias inexistentes  
- Ciclos entre pasos  

---

## **4. Validación del Backend**
Cada caso de prueba verifica que:

- ✔ Se genere correctamente el archivo LLVM IR  
- ✔ Los costos se acumulen correctamente  
- ✔ Los tiempos paralelos se calculen de forma adecuada  



# **8. Resultados de la Validación**

Tras aplicar el plan de validación, se obtuvieron los siguientes resultados:

---

### ✔ **Casos válidos procesados correctamente**

- El compilador reconoció todas las estructuras del lenguaje.  
- Las recetas en `tests/` generaron correctamente sus archivos `.ll`.  
- Los cálculos de costos coincidieron con los valores estimados.  
- El manejo de paralelismo funcionó correctamente.  

---

### ✔ **Manejo satisfactorio de errores semánticos**

Los errores fueron detectados y reportados correctamente:

- Declaración repetida de ingredientes.  
- Dependencias hacia pasos no definidos.  
- Unidades inválidas.  
- Tiempos negativos o inconsistentes.  

---

### ✔ **Backend funcional y estable**

- Los archivos `.ll` fueron compatibles con LLVM.  
- Reprodujeron la estructura semántica del programa original de manera correcta.  

---

# **9. Conclusiones**

- RecipeLang demuestra que un DSL puede simplificar notablemente la representación de procesos culinarios complejos.  
- ANTLR4 permitió crear una gramática robusta, clara y fácil de extender.  
- LLVM IR ofrece una base sólida para futuras optimizaciones o generación de ejecutables.  
- La arquitectura modular permite extender fácilmente RecipeLang (subrecetas, conversión de unidades, cálculo nutricional).  
- El proceso de validación evidencia un manejo correcto de recetas válidas y errores semánticos.

---

# **10. Guía de Instalación**

## **INSTALAR EXTENSION PARA VS CODE**

  <img src="../imagenes/extension.png" alt="Logo UPC" width="450"/>
  
## **1. Descargar imagen Docker**
```bash
docker pull petermontalvo09/llvm:1.0.0
```
## **2. INICIALIZAR EL CONTENEDOR DESDE DOCKER**

  <img src="../imagenes/iniciar-contenedor-docker.png" alt="Logo UPC" width="650"/>
  
## **3. Iniciar el contenedor**

```bash
docker start -ai llvm_2502
```
## **4. Instalar dependencias**

```bash
pacman -Syu --noconfirm
pacman -S --noconfirm jdk-openjdk wget
```
## **5. Instalar ANTLR**

```bash
wget https://www.antlr.org/download/antlr-4.13.2-complete.jar -O /usr/local/lib/antlr-4.13.1-complete.jar
echo 'export CLASSPATH=".:/usr/local/lib/antlr-4.13.1-complete.jar:$CLASSPATH"' >> ~/.bashrc
echo 'alias antlr4="java -jar /usr/local/lib/antlr-4.13.1-complete.jar"' >> ~/.bashrc
echo 'alias grun="java org.antlr.v4.gui.TestRig"' >> ~/.bashrc
source ~/.bashrc
cd /tmp
```
## **6. Clonar el repositorio**

```bash
git clone https://github.com/AlvV1210/Trabajofinal-Teoria-de-compiladores.git
```
## **7. Instalación de ANTLR4 C++ Runtime**

```bash
cd antlr4/runtime/Cpp

mkdir build && cd build

cmake .. -DCMAKE_INSTALL_PREFIX=/usr

make -j$(nproc)

sudo make install

cd /tmp/antlr4/runtime/Cpp

sudo mkdir -p /usr/include/antlr4-runtime

sudo cp /tmp/antlr4/runtime/Cpp/runtime/src/*.h /usr/include/antlr4-runtime/

# sudo cp -r /tmp/antlr4/runtime/Cpp/runtime/src/antlr4-runtime /usr/include/

sudo mkdir -p /usr/include/antlr4-runtime/support

sudo cp -r /tmp/antlr4/runtime/Cpp/runtime/src/support/* /usr/include/antlr4-runtime/support/

sudo mkdir -p /usr/include/antlr4-runtime

sudo cp -r /tmp/antlr4/runtime/Cpp/runtime/src/* /usr/include/antlr4-runtime/

```
## **8. Creas el espacio de trabajo*
<img src="../imagenes/crear-espacio-trabajo.png" alt="Logo UPC" width="450"/>
- En VSCODE
  <img src="../imagenes/espacio-trabajo-1.png" alt="Logo UPC" width="650"/>
  <img src="../imagenes/espacio-trabajo-1.png" alt="Logo UPC" width="650"/>
- DARLE A OPEN FOLDER Y ELEGIR EL WORKSPACE
  <img src="../imagenes/seleccion-workspace.png" alt="Logo UPC" width="650"/>
- DENTRO DE LA CARPETA HACER CLICK EN CLONE REPOSITORY
  <img src="../imagenes/clonar-repositorio.png" alt="Logo UPC" width="650"/>
  ELEGIR EL NOMBRE QUE QUIERAS PARA LA CARPETA DEL REPOSITORIO (YO LE PUSE recipeLang)

- LUEGO SE LE VUELVE A DAR ABRIR FOLDER Y ESTA VEZ ELEGIMOS EL QUE TENGA EL NOMBRE QUE ELEGIMOS DENTRO DEL WORKSPACE
  <img src="../imagenes/abrir-carpeta-repositorio.png" alt="Logo UPC" width="650"/>
- DENTRO DE LA CARPETA ABRIR UNA NUEVA TERMINAL Y CREAR LA CARPETA BUILD DONDE SE VA A CREAR EL COMPILADOR 
  <img src="../imagenes/mkdir-build.png" alt="Logo UPC" width="450"/>
  <img src="../imagenes/cd build.png" alt="Logo UPC" width="450"/>
## **8. Compilar el proyecto (dentro de build)**
```bash
rm -rf *
cmake ..
make
```
## **8. Ejecutar RecipeLang**
```bash
./recipec ../tests/paella.recipe
```





