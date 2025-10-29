#include "Nodo.h"
#include <iostream>
#include <iomanip>

/**
 * Constructor: inicializa un nodo con su nombre
 */
Nodo::Nodo(const std::string& nom) : nombre(nom) {
    nombresValores = {"false", "true"};
}

/**
 * Retorna el nombre del nodo
 */
std::string Nodo::getNombre() const {
    return nombre;
}

/**
 * Agrega un padre al nodo y registra este nodo como hijo del padre
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
 * Establece la probabilidad condicional para una configuración de padres
 * La probabilidad siempre es P(Nodo=true | configuración de padres)
 */
void Nodo::setProbabilidad(const std::vector<bool>& valoresPadres, double probabilidad) {
    tablaProbabilidad[valoresPadres] = probabilidad;
}

/**
 * Obtiene la probabilidad dado el valor del nodo y los valores de sus padres
 * Retorna P(Nodo=valorNodo | valoresPadres)
 */
double Nodo::getProbabilidad(bool valorNodo, const std::vector<bool>& valoresPadres) const {
    // Si no tiene padres (nodo raíz), la probabilidad está directamente almacenada
    if (padres.empty()) {
        auto it = tablaProbabilidad.find(std::vector<bool>());
        if (it != tablaProbabilidad.end()) {
            return valorNodo ? it->second : (1.0 - it->second);
        }
        return 0.5; // Valor por defecto
    }
    
    // Buscar la probabilidad en la tabla
    auto it = tablaProbabilidad.find(valoresPadres);
    if (it != tablaProbabilidad.end()) {
        // Si el valor del nodo es true, retornar la probabilidad
        // Si es false, retornar 1 - probabilidad
        return valorNodo ? it->second : (1.0 - it->second);
    }
    
    return 0.0; // Si no se encuentra, retornar 0
}

/**
 * Verifica si el nodo no tiene padres (es raíz)
 */
bool Nodo::esRaiz() const {
    return padres.empty();
}

/**
 * Verifica si el nodo no tiene hijos (es hoja)
 */
bool Nodo::esHoja() const {
    return hijos.empty();
}

/**
 * Retorna la tabla de probabilidad completa
 */
std::map<std::vector<bool>, double> Nodo::getTablaProbabilidad() const {
    return tablaProbabilidad;
}

/**
 * Muestra la tabla de probabilidad en formato texto legible
 * Incluye los nombres de los padres y las probabilidades
 */
void Nodo::mostrarTablaProbabilidad() const {
    std::cout << "\n========================================\n";
    std::cout << "Tabla de Probabilidad para: " << nombre << "\n";
    std::cout << "========================================\n";
    
    if (padres.empty()) {
        // Nodo sin padres (raíz)
        std::cout << "P(" << nombre << " = true) = " << std::fixed 
                  << std::setprecision(2) << tablaProbabilidad.begin()->second << "\n";
        std::cout << "P(" << nombre << " = false) = " << std::fixed 
                  << std::setprecision(2) << (1.0 - tablaProbabilidad.begin()->second) << "\n";
    } else {
        // Nodo con padres
        // Mostrar encabezado con nombres de padres
        for (const auto& padre : padres) {
            std::cout << std::setw(12) << padre->getNombre();
        }
        std::cout << " | P(" << nombre << "=true)\n";
        std::cout << "----------------------------------------\n";
        
        // Mostrar cada fila de la tabla
        for (const auto& entrada : tablaProbabilidad) {
            for (bool valor : entrada.first) {
                std::cout << std::setw(12) << (valor ? "true" : "false");
            }
            std::cout << " | " << std::fixed << std::setprecision(2) 
                      << entrada.second << "\n";
        }
    }
    std::cout << "========================================\n";
}
