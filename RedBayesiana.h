#ifndef RED_BAYESIANA_H
#define RED_BAYESIANA_H

#include "Nodo.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

/**
 * Clase que representa una Red Bayesiana completa
 * Maneja la estructura de nodos conectados y permite realizar inferencias
 */
class RedBayesiana {
private:
    // Mapa de nodos: nombre -> puntero al nodo
    std::map<std::string, std::shared_ptr<Nodo>> nodos;
    
    // Nodos raíz de la red (sin padres)
    std::vector<std::shared_ptr<Nodo>> nodosRaiz;
    
    /**
     * Función auxiliar para mostrar estructura recursivamente
     * @param nodo Nodo actual a mostrar
     * @param visitados Set de nodos ya visitados
     * @param nivel Nivel de indentación
     */
    void mostrarEstructuraRecursiva(std::shared_ptr<Nodo> nodo, 
                                     std::map<std::string, bool>& visitados,
                                     int nivel) const;
    
    /**
     * Genera todas las combinaciones posibles de valores para un conjunto de variables
     * @param variables Lista de nombres de variables
     * @return Vector con todas las asignaciones posibles
     */
    std::vector<std::map<std::string, bool>> generarCombinaciones(
        const std::vector<std::string>& variables) const;
    
    /**
     * Calcula la probabilidad conjunta para una asignación completa de variables
     * @param asignacion Mapa variable->valor para todas las variables
     * @return Probabilidad conjunta P(asignacion)
     */
    double calcularProbabilidadConjunta(const std::map<std::string, bool>& asignacion) const;

public:
    /**
     * Constructor de la Red Bayesiana
     */
    RedBayesiana();
    
    /**
     * Carga la estructura de la red desde un archivo
     * Formato: cada línea contiene "NodoPadre NodoHijo"
     * @param nombreArchivo Ruta del archivo con la estructura
     * @return true si se cargó correctamente, false en caso contrario
     */
    bool cargarEstructura(const std::string& nombreArchivo);
    
    /**
     * Carga las tablas de probabilidad desde un archivo
     * @param nombreArchivo Ruta del archivo con las probabilidades
     * @return true si se cargó correctamente, false en caso contrario
     */
    bool cargarProbabilidades(const std::string& nombreArchivo);
    
    /**
     * Muestra la estructura de la red en formato texto
     * Recorre desde las raíces mostrando predecesores de cada nodo
     */
    void mostrarEstructura() const;
    
    /**
     * Muestra todas las tablas de probabilidad de la red
     */
    void mostrarTodasLasTablas() const;
    
    /**
     * Obtiene un nodo por su nombre
     * @param nombre Nombre del nodo a buscar
     * @return Puntero al nodo, o nullptr si no existe
     */
    std::shared_ptr<Nodo> obtenerNodo(const std::string& nombre) const;
    
    /**
     * Obtiene todos los nombres de nodos en la red
     * @return Vector con los nombres
     */
    std::vector<std::string> obtenerNombresNodos() const;
    
    /**
     * Realiza inferencia por enumeración
     * Calcula P(consulta | evidencia)
     * @param consulta Mapa con las variables de consulta y sus valores
     * @param evidencia Mapa con las variables de evidencia y sus valores observados
     * @return Probabilidad calculada
     */
    double inferencia(const std::map<std::string, bool>& consulta,
                      const std::map<std::string, bool>& evidencia);
    
    /**
     * Realiza inferencia con traza detallada del proceso
     * @param consulta Variables y valores a consultar
     * @param evidencia Variables y valores observados
     * @return Probabilidad calculada
     */
    double inferenciaConTraza(const std::map<std::string, bool>& consulta,
                              const std::map<std::string, bool>& evidencia);
};

#endif
