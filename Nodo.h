#ifndef NODO_H
#define NODO_H

#include <string>
#include <vector>
#include <map>
#include <memory>

/**
 * Clase que representa un Nodo en la Red Bayesiana
 * Soporta dominios de valores arbitrarios (no solo booleanos)
 */
class Nodo {
private:
    std::string nombre;                          // Nombre del nodo
    std::vector<std::shared_ptr<Nodo>> padres;  // Nodos predecesores
    std::vector<std::shared_ptr<Nodo>> hijos;   // Nodos sucesores
    std::vector<std::string> dominio;            // Valores posibles del nodo
    
    // Tabla de probabilidad condicional
    // La clave es una combinación de valores de los padres (como string)
    // El valor es un mapa: valor_nodo -> probabilidad
    std::map<std::string, std::map<std::string, double>> tablaProbabilidad;

public:
    /**
     * Constructor del nodo
     * @param nom Nombre del nodo
     */
    Nodo(const std::string& nom);
    
    /**
     * Obtiene el nombre del nodo
     */
    std::string getNombre() const;
    
    /**
     * Establece el dominio de valores del nodo
     * @param valores Vector con los valores posibles
     */
    void setDominio(const std::vector<std::string>& valores);
    
    /**
     * Obtiene el dominio de valores
     */
    std::vector<std::string> getDominio() const;
    
    /**
     * Agrega un nodo padre
     */
    void agregarPadre(std::shared_ptr<Nodo> padre);
    
    /**
     * Agrega un nodo hijo
     */
    void agregarHijo(std::shared_ptr<Nodo> hijo);
    
    /**
     * Obtiene la lista de padres
     */
    std::vector<std::shared_ptr<Nodo>> getPadres() const;
    
    /**
     * Obtiene la lista de hijos
     */
    std::vector<std::shared_ptr<Nodo>> getHijos() const;
    
    /**
     * Establece la probabilidad para una configuración específica
     * @param valoresPadres Valores de los padres como vector de strings
     * @param valorNodo Valor del nodo
     * @param probabilidad Probabilidad P(nodo=valorNodo|padres)
     */
    void setProbabilidad(const std::vector<std::string>& valoresPadres, 
                        const std::string& valorNodo, 
                        double probabilidad);
    
    /**
     * Obtiene la probabilidad dado el valor del nodo y valores de padres
     * @param valorNodo Valor del nodo
     * @param valoresPadres Valores de los nodos padres
     * @return Probabilidad P(nodo=valorNodo|padres)
     */
    double getProbabilidad(const std::string& valorNodo, 
                          const std::vector<std::string>& valoresPadres) const;
    
    /**
     * Verifica si el nodo es raíz
     */
    bool esRaiz() const;
    
    /**
     * Verifica si el nodo es hoja
     */
    bool esHoja() const;
    
    /**
     * Muestra la tabla de probabilidad en formato legible
     */
    void mostrarTablaProbabilidad() const;
    
private:
    /**
     * Convierte un vector de valores en una clave string
     */
    std::string vectorAString(const std::vector<std::string>& valores) const;
};

#endif