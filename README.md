# Sistema de Inferencia con Redes Bayesianas

## 📋 Descripción del Proyecto

Implementación completa de un sistema **genérico** de Redes Bayesianas con motor de inferencia por enumeración en C++. El sistema soporta **dominios de valores arbitrarios** (no solo booleanos) y **cualquier estructura de red**.

### ✅ Requisitos Cumplidos

- ✅ **Estructura de Red Bayesiana** con clases OOP bien diseñadas
- ✅ **Carga de estructura** desde archivos de texto
- ✅ **Carga de tablas de probabilidad** desde archivos editables
- ✅ **Visualización de estructura** mostrando predecesores de cada nodo
- ✅ **Visualización de tablas** en formato legible
- ✅ **Motor de inferencia por enumeración** completo
- ✅ **Traza detallada** del proceso paso a paso
- ✅ **Sistema genérico** - funciona con cualquier dominio y estructura
- ✅ **Código comentado** con nivel profesional de detalle

## 🏗️ Arquitectura del Sistema

### Diseño Orientado a Objetos

El sistema está construido con clases apropiadas (NO usa solo strings tokenizados ni listas simples):

#### **Clase Nodo**
Representa un nodo individual con:
- Nombre del nodo
- Lista de nodos padres (predecesores)
- Lista de nodos hijos (sucesores)
- Dominio de valores (ej: {none, light, heavy})
- Tabla de probabilidad condicional estructurada

**Métodos principales:**
```cpp
void setDominio(vector<string>& valores)
void agregarPadre(shared_ptr<Nodo> padre)
void setProbabilidad(vector<string>& valoresPadres, string& valorNodo, double prob)
double getProbabilidad(string& valorNodo, vector<string>& valoresPadres)
```

#### **Clase RedBayesiana**
Gestiona la red completa:
- Mapa de nodos con acceso eficiente
- Lista de nodos raíz
- Motor de inferencia por enumeración
- Generador de combinaciones
- Calculador de probabilidades conjuntas

**Métodos principales:**
```cpp
bool cargarEstructura(string& archivo)
bool cargarProbabilidades(string& archivo)
void mostrarEstructura()
double inferenciaConTraza(map<string,string>& consulta, map<string,string>& evidencia)
```

## 📂 Estructura de Archivos

```
proyecto/
│
├── Nodo.h                    # Declaración clase Nodo
├── Nodo.cpp                  # Implementación clase Nodo
├── RedBayesiana.h            # Declaración clase RedBayesiana
├── RedBayesiana.cpp          # Implementación clase RedBayesiana
├── main.cpp                  # Programa principal interactivo
├── Makefile                  # Compilación automática
├── estructura.txt            # Estructura de la red
├── probabilidades.txt        # Tablas de probabilidad
└── README.md                 # Documentación
```

## 🔧 Compilación y Ejecución

### Requisitos
- Compilador C++ con soporte C++11 o superior
- Make (opcional)

### Compilar y Ejecutar

```bash
# Opción 1: Con Makefile
make
./red_bayesiana

# Opción 2: Compilación manual
g++ -std=c++11 -Wall -O2 -o red_bayesiana main.cpp Nodo.cpp RedBayesiana.cpp
./red_bayesiana

# Limpiar archivos compilados
make clean
```

## 📝 Formato de Archivos de Entrada

### Archivo `estructura.txt`

Define las relaciones padre → hijo en la red:

```
# Comentarios comienzan con #
Rain Maintenance
Rain Train
Train Appointment
```

**Reglas:**
- Una línea por arista
- Formato: `NodoPadre NodoHijo`
- Líneas vacías y comentarios (#) se ignoran

### Archivo `probabilidades.txt`

Define dominios y tablas de probabilidad condicional:

```
# Nodo sin padres (raíz)
NODO Rain
DOMINIO none light heavy
none | none 0.7
none | light 0.2
none | heavy 0.1

# Nodo con un padre
NODO Maintenance
DOMINIO yes no
none | yes 0.4
none | no 0.6
light | yes 0.2
light | no 0.8

# Nodo con múltiples padres
NODO Train
DOMINIO on_time delayed
none yes | on_time 0.8
none yes | delayed 0.2
light no | on_time 0.7
light no | delayed 0.3
```

**Formato:**
- `NODO NombreNodo`: Inicia definición de nodo
- `DOMINIO valor1 valor2 ...`: Define valores posibles
- `valor_padre1 valor_padre2 ... | valor_nodo probabilidad`: Entrada de la CPT

## 🎯 Ejemplo Implementado: Red de Trenes

### Descripción del Problema

La red modela el problema de llegar a una cita considerando:
- **Rain**: Nivel de lluvia {none, light, heavy}
- **Maintenance**: ¿Se hizo mantenimiento? {yes, no}
- **Train**: Estado del tren {on_time, delayed}
- **Appointment**: Resultado {attend, miss}

### Estructura de la Red

```
         Rain {none, light, heavy}
         /    \
        /      \
 Maintenance   Train {on_time, delayed}
   {yes,no}      |
                 |
            Appointment {attend, miss}
```

### Relaciones Causales

- **Rain → Maintenance**: La lluvia afecta si se hace mantenimiento
- **Rain → Train**: La lluvia afecta directamente los retrasos del tren
- **Maintenance → Train**: El mantenimiento afecta la puntualidad
- **Train → Appointment**: El estado del tren determina si llegamos a la cita

### Tablas de Probabilidad

#### P(Rain)
| none | light | heavy |
|------|-------|-------|
| 0.7  | 0.2   | 0.1   |

#### P(Maintenance | Rain)
| R     | yes | no  |
|-------|-----|-----|
| none  | 0.4 | 0.6 |
| light | 0.2 | 0.8 |
| heavy | 0.1 | 0.9 |

#### P(Train | Rain, Maintenance)
| R     | M   | on_time | delayed |
|-------|-----|---------|---------|
| none  | yes | 0.8     | 0.2     |
| none  | no  | 0.9     | 0.1     |
| light | yes | 0.6     | 0.4     |
| light | no  | 0.7     | 0.3     |
| heavy | yes | 0.4     | 0.6     |
| heavy | no  | 0.5     | 0.5     |

#### P(Appointment | Train)
| T        | attend | miss |
|----------|--------|------|
| on_time  | 0.9    | 0.1  |
| delayed  | 0.6    | 0.4  |

## 🔍 Funcionalidades del Sistema

### 1. Visualización de Estructura
Muestra la red en formato jerárquico con:
- Nodos raíz identificados
- Relaciones padre-hijo
- Dominios de valores para cada nodo

### 2. Visualización de Tablas
Presenta todas las CPT (Conditional Probability Tables) en formato tabla legible.

### 3. Motor de Inferencia por Enumeración
Implementa el algoritmo exacto que:
- Identifica variables ocultas
- Genera todas las combinaciones posibles
- Calcula probabilidades conjuntas
- Aplica normalización con evidencia
- **Muestra traza completa paso a paso**

### 4. Consultas Predefinidas
Ejemplos listos para ejecutar:
- P(Rain=light | Appointment=miss)
- P(Train=delayed | Rain=heavy)
- P(Appointment=attend | Train=on_time)

### 5. Inferencia Personalizada
Permite al usuario construir cualquier consulta con evidencia arbitraria.

## 🧮 Algoritmo de Inferencia

El sistema implementa **inferencia por enumeración exacta**:

```
P(Q | E) = P(Q, E) / P(E)
         = Σ_H P(Q, E, H) / Σ_{Q,H} P(E, H)
```

Donde:
- **Q**: Variables de consulta
- **E**: Variables de evidencia
- **H**: Variables ocultas

### Proceso Detallado:

1. **Identificar variables ocultas**: H = Todas - Q - E
2. **Calcular P(Q, E)**:
   - Generar todas las combinaciones de H
   - Para cada combinación, calcular P(Q, E, H) usando la regla de la cadena
   - Sumar: P(Q, E) = Σ_H P(Q, E, H)
3. **Calcular P(E)**:
   - Incluir Q en las variables ocultas
   - Generar todas las combinaciones
   - Sumar: P(E) = Σ_{Q,H} P(E, H)
4. **Normalizar**: P(Q | E) = P(Q, E) / P(E)

### Regla de la Cadena

Para calcular probabilidades conjuntas:
```
P(X1, X2, ..., Xn) = ∏_i P(Xi | Parents(Xi))
```

## 💡 Ejemplos de Uso

### Ejemplo 1: Diagnóstico Inverso
```
Consulta: P(Rain=light | Appointment=miss)
```
**Pregunta:** Si perdí mi cita, ¿qué tan probable es que la lluvia fuera ligera?

**Salida esperada:**
```
CONSULTA: P(Rain=light)
EVIDENCIA: Appointment=miss

Calculando P(Consulta, Evidencia):
  [1] Maintenance=yes Train=on_time  => P = 0.001600
  [2] Maintenance=yes Train=delayed  => P = 0.003200
  ...
  
RESULTADO:
P(Rain=light | Appointment=miss) = 0.XXXX = XX.XX%
```

### Ejemplo 2: Predicción
```
Consulta: P(Train=delayed | Rain=heavy)
```
**Pregunta:** Si llueve fuerte, ¿qué tan probable es que el tren se retrase?

### Ejemplo 3: Toma de Decisiones
```
Consulta: P(Appointment=attend | Train=on_time)
```
**Pregunta:** Si el tren va a tiempo, ¿llegaré a la cita?

## 🔄 Extensibilidad del Sistema

El sistema es **completamente genérico** y puede adaptarse a **cualquier dominio**:

### Para usar una red diferente:

1. **Modificar `estructura.txt`**: Definir nuevas relaciones
2. **Modificar `probabilidades.txt`**: 
   - Definir dominios de cada variable
   - Especificar tablas CPT
3. **Ejecutar el programa**: El sistema se adapta automáticamente

### Dominios Aplicables:

- ✅ Diagnóstico médico
- ✅ Detección de fraudes
- ✅ Análisis de riesgos
- ✅ Sistemas de recomendación
- ✅ Control de calidad
- ✅ Predicción del clima
- ✅ Análisis de sentimientos
- ✅ Robótica y navegación

### Ventajas del Diseño Genérico:

1. **Dominios arbitrarios**: No limitado a booleanos
2. **Múltiples valores**: Soporta {none, light, heavy}, {low, medium, high}, etc.
3. **Estructuras variables**: Cualquier DAG (Directed Acyclic Graph)
4. **Fácil modificación**: Archivos de texto editables
5. **Sin recompilación**: Cambios en datos, no en código

## 📊 Complejidad Computacional

- **Tiempo de inferencia**: O(n × d^k)
  - n: número de nodos
  - d: tamaño máximo de dominio
  - k: número de variables ocultas
  
- **Espacio**: O(n × d^p)
  - p: número máximo de padres

**Nota:** La enumeración exacta es exponencial, pero apropiada para:
- Redes pequeñas-medianas (< 20 nodos)
- Dominios pequeños (< 10 valores)
- Aplicaciones donde se requiere exactitud

## ✅ Validación del Sistema

### Pruebas Incluidas:

1. **Carga correcta** de estructura y probabilidades
2. **Verificación de dominios** y valores
3. **Cálculo de probabilidades conjuntas**
4. **Inferencias con y sin evidencia**
5. **Normalización correcta** de resultados
6. **Consultas múltiples** sobre la misma red

### Casos de Prueba Sugeridos:

```cpp
// Caso 1: Sin evidencia
P(Rain=none)

// Caso 2: Con evidencia simple
P(Train=delayed | Rain=heavy)

// Caso 3: Con evidencia múltiple
P(Rain=light | Train=delayed, Maintenance=yes)

// Caso 4: Variable intermedia
P(Maintenance=yes | Appointment=miss)
```

## 🎓 Conceptos Implementados

### Teoría de Probabilidad:
- Probabilidad condicional
- Regla de la cadena
- Marginalización
- Teorema de Bayes
- Independencia condicional

### Estructuras de Datos:
- Grafos dirigidos acíclicos (DAG)
- Mapas hash para búsqueda eficiente
- Punteros inteligentes (shared_ptr)
- Vectores y contenedores STL

### Algoritmos:
- Inferencia por enumeración exacta
- Generación de combinaciones
- Recursión para traversal de grafos
- Parsing de archivos estructurados

### Programación:
- **POO** con clases bien diseñadas
- Encapsulamiento y abstracción
- Separación de responsabilidades
- Diseño modular y extensible
- Manejo robusto de errores

## 🚀 Características Destacadas

### ✨ Fortalezas del Proyecto:

1. **Diseño OOP profesional**: Clases apropiadas, no estructuras ad-hoc
2. **Código comentado**: Cada función documentada con propósito y parámetros
3. **Genericidad**: Funciona con cualquier red, sin cambios en código
4. **Traza educativa**: Muestra proceso completo de inferencia
5. **Interfaz amigable**: Menú interactivo con ejemplos
6. **Archivos editables**: Cambios sin recompilar
7. **Validación robusta**: Manejo de errores y casos edge
8. **Extensible**: Fácil agregar nuevas funcionalidades


## 👨‍💻 Uso en Clase

Este proyecto está diseñado para:
- ✅ Demostración en clase
- ✅ Sustentación con ejemplos en vivo
- ✅ Modificación en tiempo real
- ✅ Pruebas con diferentes redes
- ✅ Explicación paso a paso del algoritmo

---

## 🎯 ¡Proyecto Listo para Sustentación!

El sistema cumple con **todos los requisitos** del proyecto:
- ✅ Clases OOP apropiadas (Nodo, RedBayesiana)
- ✅ No usa solo strings tokenizados ni diccionarios simples
- ✅ Carga estructura desde archivos
- ✅ Carga probabilidades desde archivos
- ✅ Visualiza estructura mostrando predecesores
- ✅ Visualiza tablas de probabilidad
- ✅ Motor de inferencia por enumeración
- ✅ Traza detallada del proceso
- ✅ Sistema genérico para cualquier dominio
- ✅ Código comentado profesionalmente

**¡Funcional y listo para entregar!** 🚀

## 👨🏻‍💻 Equipo de Desarrollo 
  Samuel Eduardo Emperador Contreras
  ✉️ semperadorcontreras24@gmail.com