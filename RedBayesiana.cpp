#include "RedBayesiana.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

/**
 * Constructor: inicializa una red bayesiana vacía
 */
RedBayesiana::RedBayesiana() {}

/**
 * Carga la estructura de la red desde un archivo
 * Cada línea: "NodoPadre NodoHijo" indica que NodoPadre -> NodoHijo
 */
bool RedBayesiana::cargarEstructura(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir archivo de estructura: " << nombreArchivo << "\n";
        return false;
    }
    
    std::string linea;
    while (std::getline(archivo, linea)) {
        // Ignorar líneas vacías o comentarios
        if (linea.empty() || linea[0] == '#') continue;
        
        std::istringstream iss(linea);
        std::string nombrePadre, nombreHijo;
        
        if (iss >> nombrePadre >> nombreHijo) {
            // Crear o obtener nodo padre
            if (nodos.find(nombrePadre) == nodos.end()) {
                nodos[nombrePadre] = std::make_shared<Nodo>(nombrePadre);
            }
            
            // Crear o obtener nodo hijo
            if (nodos.find(nombreHijo) == nodos.end()) {
                nodos[nombreHijo] = std::make_shared<Nodo>(nombreHijo);
            }
            
            // Establecer relación padre-hijo
            nodos[nombreHijo]->agregarPadre(nodos[nombrePadre]);
            nodos[nombrePadre]->agregarHijo(nodos[nombreHijo]);
        }
    }
    
    archivo.close();
    
    // Identificar nodos raíz
    for (const auto& par : nodos) {
        if (par.second->esRaiz()) {
            nodosRaiz.push_back(par.second);
        }
    }
    
    std::cout << "Estructura cargada: " << nodos.size() << " nodos, "
              << nodosRaiz.size() << " raíces.\n";
    
    return true;
}

/**
 * Carga las tablas de probabilidad desde un archivo
 * Formato:
 * NODO NombreNodo
 * valores_padres | probabilidad
 */
bool RedBayesiana::cargarProbabilidades(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir archivo de probabilidades: " << nombreArchivo << "\n";
        return false;
    }
    
    std::string linea;
    std::shared_ptr<Nodo> nodoActual = nullptr;
    
    while (std::getline(archivo, linea)) {
        // Ignorar líneas vacías o comentarios
        if (linea.empty() || linea[0] == '#') continue;
        
        std::istringstream iss(linea);
        std::string palabra;
        iss >> palabra;
        
        // Nueva tabla de nodo
        if (palabra == "NODO") {
            std::string nombreNodo;
            iss >> nombreNodo;
            nodoActual = obtenerNodo(nombreNodo);
            if (!nodoActual) {
                std::cerr << "Nodo no encontrado: " << nombreNodo << "\n";
            }
        }
        // Línea de probabilidad
        else if (nodoActual) {
            // Retroceder para leer toda la línea
            iss.clear();
            iss.seekg(0);
            
            std::vector<bool> valoresPadres;
            double probabilidad;
            
            // Si el nodo no tiene padres
            if (nodoActual->getPadres().empty()) {
                iss >> probabilidad;
                nodoActual->setProbabilidad(valoresPadres, probabilidad);
            }
            // Si tiene padres, leer valores hasta el '|'
            else {
                std::string token;
                while (iss >> token && token != "|") {
                    if (token == "true" || token == "T" || token == "1") {
                        valoresPadres.push_back(true);
                    } else if (token == "false" || token == "F" || token == "0") {
                        valoresPadres.push_back(false);
                    }
                }
                iss >> probabilidad;
                nodoActual->setProbabilidad(valoresPadres, probabilidad);
            }
        }
    }
    
    archivo.close();
    std::cout << "Probabilidades cargadas exitosamente.\n";
    return true;
}

/**
 * Muestra la estructura de la red recorriendo desde las raíces
 */
void RedBayesiana::mostrarEstructura() const {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║    ESTRUCTURA DE LA RED BAYESIANA     ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    
    std::map<std::string, bool> visitados;
    
    for (const auto& raiz : nodosRaiz) {
        mostrarEstructuraRecursiva(raiz, visitados, 0);
    }
}

/**
 * Función auxiliar recursiva para mostrar la estructura
 */
void RedBayesiana::mostrarEstructuraRecursiva(std::shared_ptr<Nodo> nodo,
                                               std::map<std::string, bool>& visitados,
                                               int nivel) const {
    if (visitados[nodo->getNombre()]) return;
    visitados[nodo->getNombre()] = true;
    
    // Indentación según el nivel
    for (int i = 0; i < nivel; i++) {
        std::cout << "  ";
    }
    
    // Mostrar nodo
    std::cout << "└─ " << nodo->getNombre();
    
    // Mostrar padres (predecesores)
    auto padres = nodo->getPadres();
    if (!padres.empty()) {
        std::cout << " (Padres: ";
        for (size_t i = 0; i < padres.size(); i++) {
            std::cout << padres[i]->getNombre();
            if (i < padres.size() - 1) std::cout << ", ";
        }
        std::cout << ")";
    } else {
        std::cout << " (RAÍZ)";
    }
    std::cout << "\n";
    
    // Recursión en hijos
    for (const auto& hijo : nodo->getHijos()) {
        mostrarEstructuraRecursiva(hijo, visitados, nivel + 1);
    }
}

/**
 * Muestra todas las tablas de probabilidad de la red
 */
void RedBayesiana::mostrarTodasLasTablas() const {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║     TABLAS DE PROBABILIDAD            ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
    
    for (const auto& par : nodos) {
        par.second->mostrarTablaProbabilidad();
    }
}

/**
 * Obtiene un nodo por su nombre
 */
std::shared_ptr<Nodo> RedBayesiana::obtenerNodo(const std::string& nombre) const {
    auto it = nodos.find(nombre);
    if (it != nodos.end()) {
        return it->second;
    }
    return nullptr;
}

/**
 * Obtiene todos los nombres de nodos
 */
std::vector<std::string> RedBayesiana::obtenerNombresNodos() const {
    std::vector<std::string> nombres;
    for (const auto& par : nodos) {
        nombres.push_back(par.first);
    }
    return nombres;
}

/**
 * Genera todas las combinaciones de valores booleanos para un conjunto de variables
 */
std::vector<std::map<std::string, bool>> RedBayesiana::generarCombinaciones(
    const std::vector<std::string>& variables) const {
    
    std::vector<std::map<std::string, bool>> combinaciones;
    int numCombinaciones = 1 << variables.size(); // 2^n combinaciones
    
    for (int i = 0; i < numCombinaciones; i++) {
        std::map<std::string, bool> combinacion;
        for (size_t j = 0; j < variables.size(); j++) {
            // Usar bits para generar combinaciones
            combinacion[variables[j]] = (i & (1 << j)) != 0;
        }
        combinaciones.push_back(combinacion);
    }
    
    return combinaciones;
}

/**
 * Calcula la probabilidad conjunta P(todas las variables)
 * Usa la regla de la cadena: P(X1,...,Xn) = ∏ P(Xi | Parents(Xi))
 */
double RedBayesiana::calcularProbabilidadConjunta(
    const std::map<std::string, bool>& asignacion) const {
    
    double probabilidad = 1.0;
    
    // Para cada nodo, multiplicar P(nodo | padres)
    for (const auto& par : nodos) {
        std::shared_ptr<Nodo> nodo = par.second;
        std::string nombreNodo = nodo->getNombre();
        bool valorNodo = asignacion.at(nombreNodo);
        
        // Obtener valores de los padres
        std::vector<bool> valoresPadres;
        for (const auto& padre : nodo->getPadres()) {
            valoresPadres.push_back(asignacion.at(padre->getNombre()));
        }
        
        // Multiplicar probabilidad condicional
        double prob = nodo->getProbabilidad(valorNodo, valoresPadres);
        probabilidad *= prob;
    }
    
    return probabilidad;
}

/**
 * Realiza inferencia sin mostrar traza
 */
double RedBayesiana::inferencia(const std::map<std::string, bool>& consulta,
                                const std::map<std::string, bool>& evidencia) {
    return inferenciaConTraza(consulta, evidencia);
}

/**
 * Realiza inferencia por enumeración con traza detallada
 * Calcula P(consulta | evidencia) usando la fórmula:
 * P(Q|E) = P(Q,E) / P(E)
 */
double RedBayesiana::inferenciaConTraza(const std::map<std::string, bool>& consulta,
                                        const std::map<std::string, bool>& evidencia) {
    
    std::cout << "\n╔═══════════════════════════════════════════════════╗\n";
    std::cout << "║      PROCESO DE INFERENCIA POR ENUMERACIÓN        ║\n";
    std::cout << "╚═══════════════════════════════════════════════════╝\n\n";
    
    // Mostrar consulta y evidencia
    std::cout << "Consulta: ";
    for (const auto& par : consulta) {
        std::cout << "P(" << par.first << "=" << (par.second ? "true" : "false") << ")";
    }
    std::cout << "\n\nEvidencia: ";
    if (evidencia.empty()) {
        std::cout << "Ninguna";
    } else {
        for (const auto& par : evidencia) {
            std::cout << par.first << "=" << (par.second ? "true" : "false") << " ";
        }
    }
    std::cout << "\n\n";
    
    // Identificar variables ocultas (no están en consulta ni evidencia)
    std::vector<std::string> variablesOcultas;
    for (const auto& par : nodos) {
        std::string nombre = par.first;
        if (consulta.find(nombre) == consulta.end() &&
            evidencia.find(nombre) == evidencia.end()) {
            variablesOcultas.push_back(nombre);
        }
    }
    
    std::cout << "Variables ocultas a enumerar: ";
    if (variablesOcultas.empty()) {
        std::cout << "Ninguna";
    } else {
        for (const auto& var : variablesOcultas) {
            std::cout << var << " ";
        }
    }
    std::cout << "\n\n";
    
    // Generar todas las combinaciones de variables ocultas
    auto combinacionesOcultas = generarCombinaciones(variablesOcultas);
    
    std::cout << "─────────────────────────────────────────────────────\n";
    std::cout << "Calculando P(Consulta, Evidencia):\n";
    std::cout << "─────────────────────────────────────────────────────\n\n";
    
    double probConsultaYEvidencia = 0.0;
    int iteracion = 1;
    
    // Sumar sobre todas las combinaciones de variables ocultas
    for (const auto& combinacionOculta : combinacionesOcultas) {
        // Crear asignación completa
        std::map<std::string, bool> asignacionCompleta;
        
        // Agregar consulta
        for (const auto& par : consulta) {
            asignacionCompleta[par.first] = par.second;
        }
        
        // Agregar evidencia
        for (const auto& par : evidencia) {
            asignacionCompleta[par.first] = par.second;
        }
        
        // Agregar variables ocultas
        for (const auto& par : combinacionOculta) {
            asignacionCompleta[par.first] = par.second;
        }
        
        // Calcular probabilidad conjunta
        double prob = calcularProbabilidadConjunta(asignacionCompleta);
        
        std::cout << "Iteración " << iteracion++ << ": ";
        for (const auto& par : combinacionOculta) {
            std::cout << par.first << "=" << (par.second ? "T" : "F") << " ";
        }
        std::cout << "=> P = " << std::fixed << std::setprecision(4) << prob << "\n";
        
        probConsultaYEvidencia += prob;
    }
    
    std::cout << "\nP(Consulta, Evidencia) = " << std::fixed 
              << std::setprecision(4) << probConsultaYEvidencia << "\n\n";
    
    // Si hay evidencia, calcular P(Evidencia) para normalizar
    if (!evidencia.empty()) {
        std::cout << "─────────────────────────────────────────────────────\n";
        std::cout << "Calculando P(Evidencia):\n";
        std::cout << "─────────────────────────────────────────────────────\n\n";
        
        // Variables ocultas incluyen también la consulta ahora
        std::vector<std::string> todasVariablesOcultas = variablesOcultas;
        for (const auto& par : consulta) {
            todasVariablesOcultas.push_back(par.first);
        }
        
        auto todasCombinaciones = generarCombinaciones(todasVariablesOcultas);
        double probEvidencia = 0.0;
        iteracion = 1;
        
        for (const auto& combinacion : todasCombinaciones) {
            std::map<std::string, bool> asignacionCompleta;
            
            // Agregar evidencia
            for (const auto& par : evidencia) {
                asignacionCompleta[par.first] = par.second;
            }
            
            // Agregar todas las demás variables
            for (const auto& par : combinacion) {
                asignacionCompleta[par.first] = par.second;
            }
            
            double prob = calcularProbabilidadConjunta(asignacionCompleta);
            
            std::cout << "Iteración " << iteracion++ << ": ";
            for (const auto& par : combinacion) {
                std::cout << par.first << "=" << (par.second ? "T" : "F") << " ";
            }
            std::cout << "=> P = " << std::fixed << std::setprecision(4) << prob << "\n";
            
            probEvidencia += prob;
        }
        
        std::cout << "\nP(Evidencia) = " << std::fixed 
                  << std::setprecision(4) << probEvidencia << "\n\n";
        
        // Calcular probabilidad condicional
        double resultado = probConsultaYEvidencia / probEvidencia;
        
        std::cout << "═════════════════════════════════════════════════════\n";
        std::cout << "RESULTADO:\n";
        std::cout << "P(Consulta | Evidencia) = P(Consulta, Evidencia) / P(Evidencia)\n";
        std::cout << "                        = " << probConsultaYEvidencia 
                  << " / " << probEvidencia << "\n";
        std::cout << "                        = " << std::fixed 
                  << std::setprecision(4) << resultado << "\n";
        std::cout << "═════════════════════════════════════════════════════\n\n";
        
        return resultado;
    } else {
        // Sin evidencia, la respuesta es directamente la probabilidad calculada
        std::cout << "═════════════════════════════════════════════════════\n";
        std::cout << "RESULTADO (sin evidencia):\n";
        std::cout << "P(Consulta) = " << std::fixed 
                  << std::setprecision(4) << probConsultaYEvidencia << "\n";
        std::cout << "═════════════════════════════════════════════════════\n\n";
        
        return probConsultaYEvidencia;
    }
}
