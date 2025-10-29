#ifndef NODO_H
#define NODO_H

#include <string>
#include <vector>
#include <map>
#include <memory>

/**
 * Clase que representa un Nodo en la Red Bayesiana
 * Cada nodo tiene un nombre, padres, hijos y una tabla de probabilidad condicional
 */
class Nodo {
private:
    std::string nombre;                          // Nombre del nodo
    std::vector<std::shared_ptr<Nodo>> padres;  // Nodos predecesores
    std::vector<std::shared_ptr<Nodo>> hijos;   // Nodos sucesores
    
    // Tabla de probabilidad condicional
    // La clave es una combinación de valores de los padres
    // El valor es la probabilidad P(Nodo=true | Padres)
    std::map<std::vector<bool>, double> tablaProbabilidad;
    
    std::vector<std::string> nombresValores;     // Valores posibles (true, false)

public:
    /**
     * Constructor del nodo
     * @param nom Nombre del nodo
     */
    Nodo(const std::string& nom);
    
    /**
     * Obtiene el nombre del nodo
     * @return Nombre del nodo
     */
    std::string getNombre() const;
    
    /**
     * Agrega un nodo padre (predecesor)
     * @param padre Puntero al nodo padre
     */
    void agregarPadre(std::shared_ptr<Nodo> padre);
    
    /**
     * Agrega un nodo hijo (sucesor)
     * @param hijo Puntero al nodo hijo
     */
    void agregarHijo(std::shared_ptr<Nodo> hijo);
    
    /**
     * Obtiene la lista de padres
     * @return Vector de punteros a nodos padres
     */
    std::vector<std::shared_ptr<Nodo>> getPadres() const;
    
    /**
     * Obtiene la lista de hijos
     * @return Vector de punteros a nodos hijos
     */
    std::vector<std::shared_ptr<Nodo>> getHijos() const;
    
    /**
     * Establece la probabilidad para una configuración específica de padres
     * @param valoresPadres Valores booleanos de los padres
     * @param probabilidad Probabilidad condicional P(nodo=true|padres)
     */
    void setProbabilidad(const std::vector<bool>& valoresPadres, double probabilidad);
    
    /**
     * Obtiene la probabilidad dado el valor del nodo y valores de padres
     * @param valorNodo Valor del nodo (true/false)
     * @param valoresPadres Valores de los nodos padres
     * @return Probabilidad P(nodo=valorNodo|padres)
     */
    double getProbabilidad(bool valorNodo, const std::vector<bool>& valoresPadres) const;
    
    /**
     * Verifica si el nodo es raíz (no tiene padres)
     * @return true si es raíz, false en caso contrario
     */
    bool esRaiz() const;
    
    /**
     * Verifica si el nodo es hoja (no tiene hijos)
     * @return true si es hoja, false en caso contrario
     */
    bool esHoja() const;
    
    /**
     * Obtiene la tabla de probabilidad completa
     * @return Mapa con la tabla de probabilidad
     */
    std::map<std::vector<bool>, double> getTablaProbabilidad() const;
    
    /**
     * Muestra la tabla de probabilidad en formato legible
     */
    void mostrarTablaProbabilidad() const;
};

#endif
