#include "Nodo.h"
#include <iostream>
#include <iomanip>
#include <sstream>

/**
 * Constructor: inicializa un nodo con su nombre
 */
Nodo::Nodo(const std::string& nom) : nombre(nom) {}

/**
 * Retorna el nombre del nodo
 */
std::string Nodo::getNombre() const {
    return nombre;
}

/**
 * Establece el dominio de valores posibles del nodo
 */
void Nodo::setDominio(const std::vector<std::string>& valores) {
    dominio = valores;
}

/**
 * Obtiene el dominio de valores
 */
std::vector<std::string> Nodo::getDominio() const {
    return dominio;
}

/**
 * Agrega un padre al nodo
 */
void Nodo::agregarPadre(std::shared_ptr<Nodo> padre) {
    padres.push_back(padre);
}

/**
 * Agrega un hijo al nodo
 */
void Nodo::agregarHijo(std::shared_ptr<Nodo> hijo) {
    hijos.push_back(hijo);
}

/**
 * Retorna el vector de padres
 */
std::vector<std::shared_ptr<Nodo>> Nodo::getPadres() const {
    return padres;
}

/**
 * Retorna el vector de hijos
 */
std::vector<std::shared_ptr<Nodo>> Nodo::getHijos() const {
    return hijos;
}

/**
 * Convierte un vector de valores en una clave string única
 * Ejemplo: ["none", "yes"] -> "none,yes"
 */
std::string Nodo::vectorAString(const std::vector<std::string>& valores) const {
    if (valores.empty()) return "";
    
    std::string resultado = valores[0];
    for (size_t i = 1; i < valores.size(); i++) {
        resultado += "," + valores[i];
    }
    return resultado;
}

/**
 * Establece la probabilidad condicional
 */
void Nodo::setProbabilidad(const std::vector<std::string>& valoresPadres,
                          const std::string& valorNodo,
                          double probabilidad) {
    std::string clave = vectorAString(valoresPadres);
    tablaProbabilidad[clave][valorNodo] = probabilidad;
}

/**
 * Obtiene la probabilidad P(nodo=valorNodo | valoresPadres)
 */
double Nodo::getProbabilidad(const std::string& valorNodo,
                            const std::vector<std::string>& valoresPadres) const {
    std::string clave = vectorAString(valoresPadres);
    
    auto it = tablaProbabilidad.find(clave);
    if (it != tablaProbabilidad.end()) {
        auto it2 = it->second.find(valorNodo);
        if (it2 != it->second.end()) {
            return it2->second;
        }
    }
    
    // Si no se encuentra, retornar probabilidad uniforme
    if (!dominio.empty()) {
        return 1.0 / dominio.size();
    }
    return 0.0;
}

/**
 * Verifica si el nodo no tiene padres
 */
bool Nodo::esRaiz() const {
    return padres.empty();
}

/**
 * Verifica si el nodo no tiene hijos
 */
bool Nodo::esHoja() const {
    return hijos.empty();
}

/**
 * Muestra la tabla de probabilidad en formato texto legible
 */
void Nodo::mostrarTablaProbabilidad() const {
    std::cout << "\n========================================\n";
    std::cout << "Tabla de Probabilidad: " << nombre << "\n";
    std::cout << "Dominio: {";
    for (size_t i = 0; i < dominio.size(); i++) {
        std::cout << dominio[i];
        if (i < dominio.size() - 1) std::cout << ", ";
    }
    std::cout << "}\n";
    std::cout << "========================================\n";
    
    if (padres.empty()) {
        // Nodo sin padres (raíz)
        std::cout << "P(" << nombre << ")\n";
        std::cout << "----------------------------------------\n";
        
        auto it = tablaProbabilidad.find("");
        if (it != tablaProbabilidad.end()) {
            for (const auto& par : it->second) {
                std::cout << std::setw(12) << par.first << " | " 
                         << std::fixed << std::setprecision(2) 
                         << par.second << "\n";
            }
        }
    } else {
        // Nodo con padres - mostrar encabezado
        for (const auto& padre : padres) {
            std::cout << std::setw(10) << padre->getNombre();
        }
        std::cout << " |";
        for (const auto& valor : dominio) {
            std::cout << std::setw(10) << valor;
        }
        std::cout << "\n";
        std::cout << std::string(10 * (padres.size() + dominio.size() + 1), '-') << "\n";
        
        // Mostrar cada fila de la tabla
        for (const auto& entrada : tablaProbabilidad) {
            // Separar la clave en valores individuales
            std::vector<std::string> valores;
            std::stringstream ss(entrada.first);
            std::string valor;
            while (std::getline(ss, valor, ',')) {
                valores.push_back(valor);
            }
            
            // Mostrar valores de padres
            for (const auto& v : valores) {
                std::cout << std::setw(10) << v;
            }
            std::cout << " |";
            
            // Mostrar probabilidades para cada valor del nodo
            for (const auto& d : dominio) {
                auto it = entrada.second.find(d);
                if (it != entrada.second.end()) {
                    std::cout << std::setw(10) << std::fixed 
                             << std::setprecision(2) << it->second;
                } else {
                    std::cout << std::setw(10) << "---";
                }
            }
            std::cout << "\n";
        }
    }
    std::cout << "========================================\n";
}