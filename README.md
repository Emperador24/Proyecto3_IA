# Sistema de Inferencia con Redes Bayesianas

## 📋 Descripción del Proyecto

Implementación completa de un sistema de Redes Bayesianas con motor de inferencia por enumeración en C++. El proyecto cumple con todos los requisitos solicitados:

- ✅ Estructura de Red Bayesiana con clases OOP
- ✅ Carga de estructura desde archivos
- ✅ Carga de tablas de probabilidad desde archivos
- ✅ Visualización de estructura y tablas
- ✅ Motor de inferencia por enumeración
- ✅ Traza detallada del proceso de inferencia
- ✅ Sistema genérico aplicable a cualquier dominio

## 🏗️ Arquitectura del Sistema

### Clases Principales

#### 1. **Clase Nodo** (`Nodo.h`, `Nodo.cpp`)
Representa un nodo individual en la red:
- **Atributos:**
  - Nombre del nodo
  - Lista de nodos padres (predecesores)
  - Lista de nodos hijos (sucesores)
  - Tabla de probabilidad condicional
  
- **Métodos principales:**
  - `agregarPadre()`: Establece relaciones de dependencia
  - `setProbabilidad()`: Configura probabilidades condicionales
  - `getProbabilidad()`: Obtiene P(nodo|padres)
  - `mostrarTablaProbabilidad()`: Visualización en formato texto

#### 2. **Clase RedBayesiana** (`RedBayesiana.h`, `RedBayesiana.cpp`)
Gestiona la red completa y el proceso de inferencia:
- **Funcionalidades:**
  - Carga de estructura desde archivo
  - Carga de probabilidades desde archivo
  - Visualización de la estructura de la red
  - Motor de inferencia por enumeración
  - Generación de traza del proceso

### 3. **Programa Principal** (`main.cpp`)
Interfaz interactiva para el usuario con menú de opciones.

## 📂 Estructura de Archivos

```
proyecto/
│
├── Nodo.h                    # Declaración de la clase Nodo
├── Nodo.cpp                  # Implementación de la clase Nodo
├── RedBayesiana.h            # Declaración de la clase RedBayesiana
├── RedBayesiana.cpp          # Implementación de la clase RedBayesiana
├── main.cpp                  # Programa principal con menú interactivo
├── Makefile                  # Archivo para compilación
├── estructura.txt            # Archivo de estructura de la red
├── probabilidades.txt        # Archivo de tablas de probabilidad
└── README.md                 # Este archivo
```

## 🔧 Compilación y Ejecución

### Requisitos
- Compilador C++ compatible con C++11 o superior (g++, clang++)
- Make (opcional, pero recomendado)

### Opción 1: Usando Makefile
```bash
# Compilar el proyecto
make

# Ejecutar
./bayesian_network

# O compilar y ejecutar directamente
make run

# Limpiar archivos compilados
make clean
```

### Opción 2: Compilación manual
```bash
# Compilar
g++ -std=c++11 -Wall -O2 -o bayesian_network main.cpp Nodo.cpp RedBayesiana.cpp

# Ejecutar
./bayesian_network
```

## 📝 Formato de Archivos de Entrada

### Archivo de Estructura (`estructura.txt`)

Define las relaciones padre-hijo en la red:

```
# Comentarios comienzan con #
NodoPadre NodoHijo
Lluvia Aspersor
Lluvia PastoMojado
Aspersor PastoMojado
```

Cada línea representa una arista dirigida: `Padre → Hijo`

### Archivo de Probabilidades (`probabilidades.txt`)

Define las tablas de probabilidad condicional:

```
# Para nodos raíz (sin padres)
NODO Lluvia
0.2

# Para nodos con padres
NODO Aspersor
false | 0.4
true | 0.01

# Para nodos con múltiples padres
NODO PastoMojado
false false | 0.0
false true | 0.9
true false | 0.8
true true | 0.99
```

**Formato:**
- `NODO NombreNodo`: Inicia la tabla de un nodo
- `valor_padre1 valor_padre2 ... | probabilidad`: Probabilidad condicional
- Las probabilidades representan P(Nodo=true | padres)

## 🎯 Ejemplo Implementado: Red del Aspersor

### Descripción del Problema

La red modela la siguiente situación:
- ¿Está lloviendo? (Lluvia)
- ¿El aspersor está encendido? (Aspersor)
- ¿El pasto está mojado? (PastoMojado)

### Estructura de la Red

```
        Lluvia (0.2)
         /    \
        /      \
   Aspersor   PastoMojado
        \      /
         \    /
      PastoMojado
```

### Probabilidades

- **P(Lluvia = true) = 0.2**

- **P(Aspersor = true | Lluvia)**
  - Si NO llueve: 0.4
  - Si llueve: 0.01

- **P(PastoMojado = true | Lluvia, Aspersor)**
  - Lluvia=F, Aspersor=F: 0.0
  - Lluvia=F, Aspersor=T: 0.9
  - Lluvia=T, Aspersor=F: 0.8
  - Lluvia=T, Aspersor=T: 0.99

## 🔍 Funcionalidades del Sistema

### 1. Visualización de Estructura
Muestra la red en formato árbol, indicando para cada nodo sus predecesores (padres).

### 2. Visualización de Tablas
Presenta todas las tablas de probabilidad condicional en formato legible.

### 3. Inferencia con Traza
Implementa el algoritmo de **enumeración exacta** que:
- Calcula P(Consulta | Evidencia)
- Muestra paso a paso el proceso:
  - Enumera todas las combinaciones de variables ocultas
  - Calcula probabilidades conjuntas
  - Aplica normalización

### 4. Consultas Predefinidas
- P(Lluvia=true | PastoMojado=true)
- P(Aspersor=true | PastoMojado=true)
- P(PastoMojado=true | Lluvia=false)

### 5. Inferencia Personalizada
Permite al usuario realizar cualquier consulta sobre la red.

## 🧮 Algoritmo de Inferencia por Enumeración

El motor implementa la fórmula:

```
P(Q | E) = P(Q, E) / P(E)
```

Donde:
- **Q**: Variables de consulta
- **E**: Variables de evidencia
- **H**: Variables ocultas (resto)

**Proceso:**
1. Identificar variables ocultas H = Todas - Q - E
2. Calcular P(Q, E) = Σ_H P(Q, E, H)
3. Calcular P(E) = Σ_{Q,H} P(Q, E, H)
4. Retornar P(Q | E) = P(Q, E) / P(E)

## 💡 Ejemplos de Uso

### Ejemplo 1: Inferencia Básica
```
Consulta: P(Lluvia=true | PastoMojado=true)
Resultado: 0.7079
```
**Interpretación:** Si observamos que el pasto está mojado, hay un 70.79% de probabilidad de que esté lloviendo.

### Ejemplo 2: Diagnóstico
```
Consulta: P(Aspersor=true | PastoMojado=true)
Resultado: 0.4298
```
**Interpretación:** Si el pasto está mojado, hay un 42.98% de probabilidad de que el aspersor esté encendido.

## 🔄 Extensibilidad del Sistema

El sistema es **completamente genérico** y puede adaptarse a cualquier dominio:

### Para usar una red diferente:

1. **Crear archivo de estructura** con las relaciones entre nodos
2. **Crear archivo de probabilidades** con las tablas CPT
3. **Ejecutar el programa** - automáticamente carga y procesa la nueva red

### Ejemplos de dominios aplicables:
- Diagnóstico médico
- Detección de fraudes
- Análisis de riesgos
- Sistemas de recomendación
- Control de procesos industriales

## 📊 Complejidad Computacional

- **Tiempo:** O(n × 2^k) donde k es el número de variables ocultas
- **Espacio:** O(n) para almacenar la red

**Nota:** La enumeración exacta es exponencial, pero apropiada para redes pequeñas-medianas.

## ✅ Validación del Sistema

El proyecto incluye casos de prueba que verifican:
- Carga correcta de estructura y probabilidades
- Cálculo preciso de probabilidades conjuntas
- Inferencias correctas con diferentes evidencias
- Normalización apropiada de resultados

## 🎓 Conceptos Implementados

- **Programación Orientada a Objetos** (POO)
- **Estructuras de datos** (grafos, mapas, vectores)
- **Algoritmos probabilísticos**
- **Inferencia bayesiana**
- **Manejo de archivos**
- **Diseño modular y extensible**

## 📚 Referencias

- Russell & Norvig, "Artificial Intelligence: A Modern Approach"
- Koller & Friedman, "Probabilistic Graphical Models"

## 👨‍💻 Autor

Proyecto desarrollado para el curso de Métodos Probabilísticos

---

**¡El sistema está listo para ser usado y sustentado!** 🚀
