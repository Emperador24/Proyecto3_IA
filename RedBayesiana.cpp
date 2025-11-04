#include "RedBayesiana.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

/**
 * Constructor: inicializa una red bayesiana vacía
 */
RedBayesiana::RedBayesiana() {}

/**
 * Carga la estructura de la red desde un archivo
 * Formato: cada línea "NodoPadre NodoHijo"
 */
bool RedBayesiana::cargarEstructura(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se puede abrir " << nombreArchivo << "\n";
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
    
    std::cout << "✓ Estructura cargada: " << nodos.size() << " nodos, "
              << nodosRaiz.size() << " raíces\n";
    
    return true;
}

/**
 * Carga las tablas de probabilidad desde un archivo
 * Formato mejorado:
 * NODO NombreNodo
 * DOMINIO valor1 valor2 valor3...
 * valor_padre1 valor_padre2... | valor_nodo prob
 */
bool RedBayesiana::cargarProbabilidades(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se puede abrir " << nombreArchivo << "\n";
        return false;
    }
    
    std::string linea;
    std::shared_ptr<Nodo> nodoActual = nullptr;
    int lineaNum = 0;
    
    while (std::getline(archivo, linea)) {
        lineaNum++;
        
        // Ignorar líneas vacías o comentarios
        if (linea.empty() || linea[0] == '#') continue;
        
        // Eliminar espacios al inicio y final
        size_t inicio = linea.find_first_not_of(" \t\r\n");
        size_t fin = linea.find_last_not_of(" \t\r\n");
        if (inicio == std::string::npos) continue;
        linea = linea.substr(inicio, fin - inicio + 1);
        
        std::istringstream iss(linea);
        std::string palabra;
        iss >> palabra;
        
        // Nueva definición de nodo
        if (palabra == "NODO") {
            std::string nombreNodo;
            iss >> nombreNodo;
            nodoActual = obtenerNodo(nombreNodo);
            if (!nodoActual) {
                std::cerr << "Línea " << lineaNum << " - Advertencia: Nodo no encontrado en estructura: " 
                         << nombreNodo << "\n";
                std::cerr << "Asegúrese de definir primero la estructura en estructura.txt\n";
            }
        }
        // Definición de dominio
        else if (palabra == "DOMINIO" && nodoActual) {
            std::vector<std::string> dominio;
            std::string valor;
            while (iss >> valor) {
                dominio.push_back(valor);
            }
            nodoActual->setDominio(dominio);
            
            // Verificación: El dominio debe tener al menos 2 valores
            if (dominio.size() < 2) {
                std::cerr << "Línea " << lineaNum << " - Advertencia: Dominio con menos de 2 valores para " 
                         << nodoActual->getNombre() << "\n";
            }
        }
        // Línea de probabilidad
        else if (nodoActual && nodoActual->getDominio().size() > 0) {
            // Retroceder para leer toda la línea
            iss.clear();
            iss.seekg(0);
            
            std::vector<std::string> valoresPadres;
            std::string valorNodo;
            double probabilidad;
            
            // Buscar el separador '|'
            size_t posPipe = linea.find('|');
            
            if (posPipe != std::string::npos) {
                // Hay separador '|': formato "valores_padres | valor_nodo prob"
                std::string parteIzq = linea.substr(0, posPipe);
                std::string parteDer = linea.substr(posPipe + 1);
                
                // Leer valores de padres
                std::istringstream issIzq(parteIzq);
                std::string valor;
                while (issIzq >> valor) {
                    valoresPadres.push_back(valor);
                }
                
                // Leer valor del nodo y probabilidad
                std::istringstream issDer(parteDer);
                if (issDer >> valorNodo >> probabilidad) {
                    // Verificar que el número de valores de padres coincida
                    if (nodoActual->getPadres().size() != valoresPadres.size()) {
                        std::cerr << "Línea " << lineaNum << " - Error: Número de valores de padres ("
                                 << valoresPadres.size() << ") no coincide con número de padres ("
                                 << nodoActual->getPadres().size() << ") para " 
                                 << nodoActual->getNombre() << "\n";
                        continue;
                    }
                    
                    // Verificar que valorNodo está en el dominio
                    auto dominio = nodoActual->getDominio();
                    if (std::find(dominio.begin(), dominio.end(), valorNodo) == dominio.end()) {
                        std::cerr << "Línea " << lineaNum << " - Advertencia: Valor '" << valorNodo 
                                 << "' no está en el dominio de " << nodoActual->getNombre() << "\n";
                    }
                    
                    nodoActual->setProbabilidad(valoresPadres, valorNodo, probabilidad);
                }
            } else {
                // Sin separador '|': formato para nodos raíz "valor probabilidad"
                if (nodoActual->getPadres().empty()) {
                    if (iss >> valorNodo >> probabilidad) {
                        // Verificar que valorNodo está en el dominio
                        auto dominio = nodoActual->getDominio();
                        if (std::find(dominio.begin(), dominio.end(), valorNodo) == dominio.end()) {
                            std::cerr << "Línea " << lineaNum << " - Advertencia: Valor '" << valorNodo 
                                     << "' no está en el dominio de " << nodoActual->getNombre() << "\n";
                        }
                        
                        nodoActual->setProbabilidad({}, valorNodo, probabilidad);
                    }
                } else {
                    std::cerr << "Línea " << lineaNum << " - Error: Falta separador '|' para nodo con padres\n";
                }
            }
        } else if (nodoActual && nodoActual->getDominio().size() == 0) {
            std::cerr << "Línea " << lineaNum << " - Error: Debe definir DOMINIO antes de las probabilidades para " 
                     << nodoActual->getNombre() << "\n";
        }
    }
    
    archivo.close();
    
    // Validar que todos los nodos tienen dominio y probabilidades completas
    bool todasCompletas = true;
    for (const auto& par : nodos) {
        auto nodo = par.second;
        if (nodo->getDominio().empty()) {
            std::cerr << "Advertencia: Nodo '" << nodo->getNombre() 
                     << "' no tiene dominio definido\n";
            todasCompletas = false;
        }
    }
    
    if (todasCompletas) {
        std::cout << "✓ Probabilidades cargadas exitosamente\n";
    } else {
        std::cout << "⚠ Probabilidades cargadas con advertencias\n";
    }
    
    return true;
}

/**
 * Muestra la estructura de la red
 */
void RedBayesiana::mostrarEstructura() const {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║    ESTRUCTURA DE LA RED BAYESIANA      ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    
    std::map<std::string, bool> visitados;
    
    for (const auto& raiz : nodosRaiz) {
        mostrarEstructuraRecursiva(raiz, visitados, 0);
    }
    
    std::cout << "\nTotal de nodos: " << nodos.size() << "\n";
}

/**
 * Función auxiliar recursiva para mostrar estructura
 */
void RedBayesiana::mostrarEstructuraRecursiva(std::shared_ptr<Nodo> nodo,
                                               std::map<std::string, bool>& visitados,
                                               int nivel) const {
    if (visitados[nodo->getNombre()]) return;
    visitados[nodo->getNombre()] = true;
    
    // Indentación
    for (int i = 0; i < nivel; i++) {
        std::cout << "  ";
    }
    
    std::cout << "└─ " << nodo->getNombre();
    
    // Mostrar padres
    auto padres = nodo->getPadres();
    if (!padres.empty()) {
        std::cout << " [Padres: ";
        for (size_t i = 0; i < padres.size(); i++) {
            std::cout << padres[i]->getNombre();
            if (i < padres.size() - 1) std::cout << ", ";
        }
        std::cout << "]";
    } else {
        std::cout << " (RAÍZ)";
    }
    
    // Mostrar dominio
    auto dominio = nodo->getDominio();
    if (!dominio.empty()) {
        std::cout << " {";
        for (size_t i = 0; i < dominio.size(); i++) {
            std::cout << dominio[i];
            if (i < dominio.size() - 1) std::cout << ", ";
        }
        std::cout << "}";
    }
    
    std::cout << "\n";
    
    // Recursión en hijos
    for (const auto& hijo : nodo->getHijos()) {
        mostrarEstructuraRecursiva(hijo, visitados, nivel + 1);
    }
}

/**
 * Muestra todas las tablas de probabilidad
 */
void RedBayesiana::mostrarTodasLasTablas() const {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║       TABLAS DE PROBABILIDAD           ║\n";
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
 * Genera todas las combinaciones de valores para un conjunto de variables
 */
std::vector<std::map<std::string, std::string>> RedBayesiana::generarCombinaciones(
    const std::vector<std::pair<std::string, std::shared_ptr<Nodo>>>& variables) const {
    
    std::vector<std::map<std::string, std::string>> resultado;
    
    if (variables.empty()) {
        resultado.push_back(std::map<std::string, std::string>());
        return resultado;
    }
    
    // Caso base: una variable
    if (variables.size() == 1) {
        auto dominio = variables[0].second->getDominio();
        for (const auto& valor : dominio) {
            std::map<std::string, std::string> combinacion;
            combinacion[variables[0].first] = valor;
            resultado.push_back(combinacion);
        }
        return resultado;
    }
    
    // Caso recursivo: combinar primera variable con el resto
    std::vector<std::pair<std::string, std::shared_ptr<Nodo>>> resto(
        variables.begin() + 1, variables.end());
    auto combinacionesResto = generarCombinaciones(resto);
    
    auto dominio = variables[0].second->getDominio();
    for (const auto& valor : dominio) {
        for (const auto& combResto : combinacionesResto) {
            std::map<std::string, std::string> combinacion = combResto;
            combinacion[variables[0].first] = valor;
            resultado.push_back(combinacion);
        }
    }
    
    return resultado;
}

/**
 * Calcula la probabilidad conjunta P(todas las variables)
 * Usa la regla de la cadena: P(X1,...,Xn) = ∏ P(Xi | Parents(Xi))
 */
double RedBayesiana::calcularProbabilidadConjunta(
    const std::map<std::string, std::string>& asignacion) const {
    
    double probabilidad = 1.0;
    
    // Para cada nodo, multiplicar P(nodo | padres)
    for (const auto& par : nodos) {
        std::shared_ptr<Nodo> nodo = par.second;
        std::string nombreNodo = nodo->getNombre();
        
        // Verificar que la variable esté en la asignación
        if (asignacion.find(nombreNodo) == asignacion.end()) {
            std::cerr << "Error: Variable " << nombreNodo << " no encontrada en asignación\n";
            return 0.0;
        }
        
        std::string valorNodo = asignacion.at(nombreNodo);
        
        // Obtener valores de los padres
        std::vector<std::string> valoresPadres;
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
 * Realiza inferencia sin traza
 */
double RedBayesiana::inferencia(const std::map<std::string, std::string>& consulta,
                                const std::map<std::string, std::string>& evidencia) {
    return inferenciaConTraza(consulta, evidencia);
}

/**
 * Realiza inferencia por enumeración con traza detallada
 * Calcula P(consulta | evidencia)
 */
double RedBayesiana::inferenciaConTraza(
    const std::map<std::string, std::string>& consulta,
    const std::map<std::string, std::string>& evidencia) {
    
    std::cout << "\n╔═══════════════════════════════════════════════════╗\n";
    std::cout << "║      PROCESO DE INFERENCIA POR ENUMERACIÓN        ║\n";
    std::cout << "╚═══════════════════════════════════════════════════╝\n\n";
    
    // Mostrar consulta
    std::cout << "CONSULTA: P(";
    bool primero = true;
    for (const auto& par : consulta) {
        if (!primero) std::cout << ", ";
        std::cout << par.first << "=" << par.second;
        primero = false;
    }
    std::cout << ")\n\n";
    
    // Mostrar evidencia
    std::cout << "EVIDENCIA: ";
    if (evidencia.empty()) {
        std::cout << "Ninguna";
    } else {
        primero = true;
        for (const auto& par : evidencia) {
            if (!primero) std::cout << ", ";
            std::cout << par.first << "=" << par.second;
            primero = false;
        }
    }
    std::cout << "\n\n";
    
    // Identificar variables ocultas
    std::vector<std::pair<std::string, std::shared_ptr<Nodo>>> variablesOcultas;
    for (const auto& par : nodos) {
        std::string nombre = par.first;
        if (consulta.find(nombre) == consulta.end() &&
            evidencia.find(nombre) == evidencia.end()) {
            variablesOcultas.push_back({nombre, par.second});
        }
    }
    
    std::cout << "VARIABLES OCULTAS: ";
    if (variablesOcultas.empty()) {
        std::cout << "Ninguna";
    } else {
        for (size_t i = 0; i < variablesOcultas.size(); i++) {
            std::cout << variablesOcultas[i].first;
            if (i < variablesOcultas.size() - 1) std::cout << ", ";
        }
    }
    std::cout << "\n\n";
    
    // Generar combinaciones de variables ocultas
    auto combinacionesOcultas = generarCombinaciones(variablesOcultas);
    
    std::cout << "─────────────────────────────────────────────────────\n";
    std::cout << "     Calculando P(Consulta, Evidencia):\n";
    std::cout << "─────────────────────────────────────────────────────\n\n";
    
    double probConsultaYEvidencia = 0.0;
    int iteracion = 1;
    
    // Sumar sobre todas las combinaciones de variables ocultas
    for (const auto& combinacionOculta : combinacionesOcultas) {
        // Crear asignación completa
        std::map<std::string, std::string> asignacionCompleta;
        
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
        
        std::cout << "  [" << std::setw(2) << iteracion++ << "] ";
        for (const auto& par : combinacionOculta) {
            std::cout << par.first << "=" << par.second << " ";
        }
        std::cout << " => P = " << std::fixed << std::setprecision(6) << prob << "\n";
        
        probConsultaYEvidencia += prob;
    }
    
    std::cout << "\nΣ P(Consulta, Evidencia, Ocultas) = " << std::fixed 
              << std::setprecision(6) << probConsultaYEvidencia << "\n\n";
    
    // Si hay evidencia, calcular P(Evidencia) para normalizar
    if (!evidencia.empty()) {
        std::cout << "─────────────────────────────────────────────────────\n";
        std::cout << "        Calculando P(Evidencia):\n";
        std::cout << "─────────────────────────────────────────────────────\n\n";
        
        // Variables ocultas ahora incluyen también la consulta
        std::vector<std::pair<std::string, std::shared_ptr<Nodo>>> todasVariablesOcultas = variablesOcultas;
        for (const auto& par : consulta) {
            todasVariablesOcultas.push_back({par.first, obtenerNodo(par.first)});
        }
        
        auto todasCombinaciones = generarCombinaciones(todasVariablesOcultas);
        double probEvidencia = 0.0;
        iteracion = 1;
        
        for (const auto& combinacion : todasCombinaciones) {
            std::map<std::string, std::string> asignacionCompleta;
            
            // Agregar evidencia
            for (const auto& par : evidencia) {
                asignacionCompleta[par.first] = par.second;
            }
            
            // Agregar todas las demás variables
            for (const auto& par : combinacion) {
                asignacionCompleta[par.first] = par.second;
            }
            
            double prob = calcularProbabilidadConjunta(asignacionCompleta);
            
            std::cout << "  [" << std::setw(2) << iteracion++ << "] ";
            for (const auto& par : combinacion) {
                std::cout << par.first << "=" << par.second << " ";
            }
            std::cout << " => P = " << std::fixed << std::setprecision(6) << prob << "\n";
            
            probEvidencia += prob;
        }
        
        std::cout << "\nΣ P(Evidencia, Ocultas) = " << std::fixed 
                  << std::setprecision(6) << probEvidencia << "\n\n";
        
        // Calcular probabilidad condicional
        double resultado = probConsultaYEvidencia / probEvidencia;
        
        std::cout << "═════════════════════════════════════════════════════\n";
        std::cout << "║                    RESULTADO                      ║\n";
        std::cout << "═════════════════════════════════════════════════════\n\n";
        std::cout << "P(Consulta | Evidencia) = P(Consulta, Evidencia) / P(Evidencia)\n";
        std::cout << "                        = " << std::fixed << std::setprecision(6)
                  << probConsultaYEvidencia << " / " << probEvidencia << "\n";
        std::cout << "                        = " << std::fixed << std::setprecision(4) 
                  << resultado << "\n";
        std::cout << "                        = " << std::fixed << std::setprecision(2)
                  << (resultado * 100) << "%\n\n";
        
        return resultado;
    } else {
        // Sin evidencia
        std::cout << "═════════════════════════════════════════════════════\n";
        std::cout << "║             RESULTADO (sin evidencia)             ║\n";
        std::cout << "═════════════════════════════════════════════════════\n\n";
        std::cout << "P(Consulta) = " << std::fixed << std::setprecision(6) 
                  << probConsultaYEvidencia << "\n";
        std::cout << "            = " << std::fixed << std::setprecision(2)
                  << (probConsultaYEvidencia * 100) << "%\n\n";
        
        return probConsultaYEvidencia;
    }
}