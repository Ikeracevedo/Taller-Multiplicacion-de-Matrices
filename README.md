Proyecto Taller: Multiplicación de Matrices Cuadradas
=====================================================

Este repositorio reúne varias implementaciones del problema clásico de multiplicación de matrices cuadradas utilizando dos algoritmos principales:

*   **Algoritmo Naive (Clásico)**
    
*   **Algoritmo de Strassen (Recursivo)**
    

Las implementaciones están desarrolladas en cuatro lenguajes diferentes para facilitar el análisis comparativo y la práctica:

*   Python
    
*   Java
    
*   C
    
*   C++
    

Contenido del Repositorio
-------------------------

### Carpetas / Archivos

*   **/python/**Código Python que genera matrices aleatorias y evalúa naive y Strassen, midiendo tiempo y memoria, con exportación a CSV.
    
*   **/java/**ArquiEjercicio.java: Implementa multiplicación bloqueada (naive optimizado) y Strassen. Mide rendimiento y memoria.
    
*   **/c/**Naive.c: Algoritmo ingenuo en C, solicita tamaño de matriz, imprime resultado (si es pequeño), reporta tiempo y memoria.Strassen.c: Implementación recursiva de Strassen en C con manejo manual de memoria y padding para tamaños no potencias de 2.
    
*   **/cpp/**Naive (1).cpp: Naive con vectores en C++, similar a C pero con vectores y chrono para medición de tiempo.Strassen.cpp: Strassen en C++ con vectores y medición con chrono, padding interno para potencia de 2.
    

Descripción General de los Algoritmos
-------------------------------------

*   **Naive:** Multiplicación clásica con tres bucles anidados. Sencillo pero con complejidad O(n³).
    
*   **Strassen:** Algoritmo que reduce multiplicaciones usando división recursiva y combinaciones lineales, más eficiente en grandes matrices.
    

Detalle por Lenguaje y Archivos
-------------------------------

### Java — ArquiEjercicio.java

*   Multiplicación bloqueada (optimización para caché).
    
*   Algoritmo de Strassen recursivo.
    
*   Matrices por defecto 128×128 (modificable en código).
    
*   Mide tiempo CPU y memoria usada.
    

**Compilar y ejecutar:**

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   bashjavac ArquiEjercicio.java  java ArquiEjercicio   `

### C — Naive.c y Strassen.c

*   Solicitan tamaño matriz al usuario.
    
*   Imprimen matrices si tamaño ≤ 10.
    
*   Miden tiempo CPU y memoria estimada.
    

**Compilar y ejecutar:**

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   bashgcc Naive.c -o naive  ./naive  gcc Strassen.c -o strassen  ./strassen   `

### Python — Código principal

*   Ejecuta pruebas en tamaños \[64, 128, 256, 512, 1024\].
    
*   Mide tiempo y memoria con time y tracemalloc.
    
*   Guarda resultados en resultados.csv.
    
*   Verifica que resultados naive y Strassen coincidan.
    

**Ejecutar:**

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   bashpython multiplicacion_matrices.py   `

### C++ — Naive (1).cpp y Strassen.cpp

*   Uso de std::vector.
    
*   Medición de tiempo con chrono.
    
*   Solicitan tamaño matriz.
    

**Compilar y ejecutar:**

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   bashg++ "Naive (1).cpp" -o naive_cpp  ./naive_cpp  g++ Strassen.cpp -o strassen_cpp  ./strassen_cpp   `

Cómo usar el repositorio
------------------------

1.  Clona o descarga el repositorio.
    
2.  Elige el lenguaje y algoritmo a probar.
    
3.  Sigue las instrucciones de compilación y ejecución descritas.
    
4.  Observa resultados en consola o archivos CSV (Python).
    
5.  Modifica parámetros (tamaño matriz, bloque, umbral Strassen) según necesidad.
    

Notas importantes
-----------------

*   Para matrices grandes, Strassen suele ser más eficiente.
    
*   El tamaño ideal para Strassen es potencia de 2 (el código hace padding si no lo es).
    
*   Evita imprimir matrices grandes para no saturar consola.
    
*   Ajusta tamaño de bloque en Java para optimizar.
    
*   El consumo de memoria puede ser alto para matrices grandes; asegúrate de tener RAM suficiente.
