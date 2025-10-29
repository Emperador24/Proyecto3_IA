#include "RedBayesiana.h"
#include <iostream>
#include <map>

/**
 * Programa principal para demostrar el funcionamiento de la Red Bayesiana
 * Ejemplo clásico: Red del Aspersor (Sprinkler)
 * 
 * Estructura:
 *     Lluvia
 *      /  \
 * Aspersor  PastoMojado
 *      \    /
 *    PastoMojado
 * 
 * Variables:
 * - Lluvia: ¿Está lloviendo?
 * - Aspersor: ¿El aspersor está encendido?
 * - PastoMojado: ¿El pasto está mojado?
 */

void mostrarMenu() {
    std::cout << "\n╔═══════════════════════════════════════════════════╗\n";
    std::cout << "║        SISTEMA DE RED BAYESIANA - MENÚ            ║\n";
    std::cout << "╚═══════════════════════════════════════════════════╝\n";
    std::cout << "1. Mostrar estructura de la red\n";
    std::cout << "2. Mostrar tablas de probabilidad\n";
    std::cout << "3. Inferencia: P(Lluvia=true | PastoMojado=true)\n";
    std::cout << "4. Inferencia: P(Aspersor=true | PastoMojado=true)\n";
    std::cout << "5. Inferencia: P(PastoMojado=true | Lluvia=false)\n";
    std::cout << "6. Inferencia personalizada\n";
    std::cout << "7. Salir\n";
    std::cout << "\nSeleccione una opción: ";
}

void inferenciaPersonalizada(RedBayesiana& red) {
    std::cout << "\n=== INFERENCIA PERSONALIZADA ===\n";
    std::cout << "Variables disponibles: Lluvia, Aspersor, PastoMojado\n\n";
    
    std::map<std::string, bool> consulta;
    std::map<std::string, bool> evidencia;
    
    // Obtener consulta
    std::cout << "Variable a consultar: ";
    std::string varConsulta;
    std::cin >> varConsulta;
    
    std::cout << "Valor (true/false): ";
    std::string valorStr;
    std::cin >> valorStr;
    bool valor = (valorStr == "true" || valorStr == "T" || valorStr == "1");
    consulta[varConsulta] = valor;
    
    // Obtener evidencia
    std::cout << "\n¿Agregar evidencia? (s/n): ";
    char respuesta;
    std::cin >> respuesta;
    
    while (respuesta == 's' || respuesta == 'S') {
        std::cout << "Variable de evidencia: ";
        std::string varEvidencia;
        std::cin >> varEvidencia;
        
        std::cout << "Valor (true/false): ";
        std::cin >> valorStr;
        valor = (valorStr == "true" || valorStr == "T" || valorStr == "1");
        evidencia[varEvidencia] = valor;
        
        std::cout << "¿Agregar más evidencia? (s/n): ";
        std::cin >> respuesta;
    }
    
    // Realizar inferencia
    red.inferenciaConTraza(consulta, evidencia);
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║      SISTEMA DE INFERENCIA CON REDES BAYESIANAS        ║\n";
    std::cout << "║     Ejemplo: Red del Aspersor (Sprinkler Network)      ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";
    
    // Crear red bayesiana
    RedBayesiana red;
    
    // Cargar estructura desde archivo
    std::cout << "Cargando estructura de la red...\n";
    if (!red.cargarEstructura("estructura.txt")) {
        std::cerr << "Error al cargar estructura. Verifique que exista estructura.txt\n";
        return 1;
    }
    
    // Cargar probabilidades desde archivo
    std::cout << "Cargando tablas de probabilidad...\n";
    if (!red.cargarProbabilidades("probabilidades.txt")) {
        std::cerr << "Error al cargar probabilidades. Verifique que exista probabilidades.txt\n";
        return 1;
    }
    
    std::cout << "\n¡Red Bayesiana cargada exitosamente!\n";
    
    // Menú interactivo
    int opcion;
    do {
        mostrarMenu();
        std::cin >> opcion;
        
        switch(opcion) {
            case 1:
                red.mostrarEstructura();
                break;
                
            case 2:
                red.mostrarTodasLasTablas();
                break;
                
            case 3: {
                // P(Lluvia=true | PastoMojado=true)
                std::map<std::string, bool> consulta = {{"Lluvia", true}};
                std::map<std::string, bool> evidencia = {{"PastoMojado", true}};
                red.inferenciaConTraza(consulta, evidencia);
                break;
            }
            
            case 4: {
                // P(Aspersor=true | PastoMojado=true)
                std::map<std::string, bool> consulta = {{"Aspersor", true}};
                std::map<std::string, bool> evidencia = {{"PastoMojado", true}};
                red.inferenciaConTraza(consulta, evidencia);
                break;
            }
            
            case 5: {
                // P(PastoMojado=true | Lluvia=false)
                std::map<std::string, bool> consulta = {{"PastoMojado", true}};
                std::map<std::string, bool> evidencia = {{"Lluvia", false}};
                red.inferenciaConTraza(consulta, evidencia);
                break;
            }
            
            case 6:
                inferenciaPersonalizada(red);
                break;
                
            case 7:
                std::cout << "\n¡Hasta luego!\n";
                break;
                
            default:
                std::cout << "\nOpción inválida. Intente nuevamente.\n";
        }
        
        if (opcion != 7) {
            std::cout << "\nPresione Enter para continuar...";
            std::cin.ignore();
            std::cin.get();
        }
        
    } while (opcion != 7);
    
    return 0;
}
