#ifndef RED_BAYESIANA_H
#define RED_BAYESIANA_H

#include "Nodo.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

/**
 * Clase que representa una Red Bayesiana completa
 * Soporta dominios de valores arbitrarios y cualquier estructura de red
 */
class RedBayesiana {
private:
    // Mapa de nodos: nombre -> puntero al nodo
    std::map<std::string, std::shared_ptr<Nodo>> nodos;
    
    // Nodos raíz de la red (sin padres)
    std::vector<std::shared_ptr<Nodo>> nodosRaiz;
    
    /**
     * Función auxiliar para mostrar estructura recursivamente
     */
    void mostrarEstructuraRecursiva(std::shared_ptr<Nodo> nodo, 
                                     std::map<std::string, bool>& visitados,
                                     int nivel) const;
    
    /**
     * Genera todas las combinaciones posibles de valores para variables
     * @param variables Lista de pares (nombre_variable, nodo)
     * @return Vector con todas las asignaciones posibles
     */
    std::vector<std::map<std::string, std::string>> generarCombinaciones(
        const std::vector<std::pair<std::string, std::shared_ptr<Nodo>>>& variables) const;
    
    /**
     * Calcula la probabilidad conjunta para una asignación completa
     * @param asignacion Mapa variable->valor para todas las variables
     * @return Probabilidad conjunta P(asignacion)
     */
    double calcularProbabilidadConjunta(
        const std::map<std::string, std::string>& asignacion) const;

public:
    /**
     * Constructor de la Red Bayesiana
     */
    RedBayesiana();
    
    /**
     * Carga la estructura de la red desde un archivo
     * Formato: cada línea "NodoPadre NodoHijo"
     */
    bool cargarEstructura(const std::string& nombreArchivo);
    
    /**
     * Carga las tablas de probabilidad desde un archivo
     * Formato mejorado para soportar múltiples valores
     */
    bool cargarProbabilidades(const std::string& nombreArchivo);
    
    /**
     * Muestra la estructura de la red en formato texto
     */
    void mostrarEstructura() const;
    
    /**
     * Muestra todas las tablas de probabilidad
     */
    void mostrarTodasLasTablas() const;
    
    /**
     * Obtiene un nodo por su nombre
     */
    std::shared_ptr<Nodo> obtenerNodo(const std::string& nombre) const;
    
    /**
     * Obtiene todos los nombres de nodos
     */
    std::vector<std::string> obtenerNombresNodos() const;
    
    /**
     * Realiza inferencia por enumeración con traza
     * @param consulta Mapa variable -> valor a consultar
     * @param evidencia Mapa variable -> valor observado
     * @return Probabilidad calculada P(consulta | evidencia)
     */
    double inferenciaConTraza(const std::map<std::string, std::string>& consulta,
                              const std::map<std::string, std::string>& evidencia);
    
    /**
     * Realiza inferencia sin mostrar traza detallada
     */
    double inferencia(const std::map<std::string, std::string>& consulta,
                     const std::map<std::string, std::string>& evidencia);
};

#endif