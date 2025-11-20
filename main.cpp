#include "RedBayesiana.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <iomanip>

/**
 * Programa principal para Red Bayesiana Genérica
 * Sistema totalmente interactivo - funciona con CUALQUIER ejemplo
 * Grupo 1
 */

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar() {
    std::cout << "\nPresione Enter para continuar...";
    std::cin.ignore();
    std::cin.get();
}

void mostrarMenu() {
    std::cout << "\n╔═══════════════════════════════════════════════════╗\n";
    std::cout << "║     SISTEMA DE RED BAYESIANA - MENÚ PRINCIPAL     ║\n";
    std::cout << "╚═══════════════════════════════════════════════════╝\n";
    std::cout << "1. Visualizar estructura de la red\n";
    std::cout << "2. Visualizar tablas de probabilidad\n";
    std::cout << "3. Realizar inferencia (consulta personalizada)\n";
    std::cout << "4. Ver información de variables\n";
    std::cout << "5. Inferencia rápida (sin traza detallada)\n";
    std::cout << "6. Cargar otra red (cambiar archivos)\n";
    std::cout << "7. Ayuda\n";
    std::cout << "8. Salir\n";
    std::cout << "\nSeleccione una opción: ";
}

void mostrarInformacionVariables(RedBayesiana& red) {
    std::cout << "\n╔═══════════════════════════════════════════════════╗\n";
    std::cout << "║        INFORMACIÓN DE VARIABLES DE LA RED         ║\n";
    std::cout << "╚═══════════════════════════════════════════════════╝\n\n";
    
    auto nombresNodos = red.obtenerNombresNodos();
    
    std::cout << "Total de variables: " << nombresNodos.size() << "\n\n";
    
    for (const auto& nombre : nombresNodos) {
        auto nodo = red.obtenerNodo(nombre);
        if (nodo) {
            std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
            std::cout << "Variable: " << nombre << "\n";
            
            // Mostrar dominio
            std::cout << "Dominio: {";
            auto dominio = nodo->getDominio();
            for (size_t i = 0; i < dominio.size(); i++) {
                std::cout << dominio[i];
                if (i < dominio.size() - 1) std::cout << ", ";
            }
            std::cout << "}\n";
            
            // Mostrar padres
            auto padres = nodo->getPadres();
            std::cout << "Padres: ";
            if (padres.empty()) {
                std::cout << "Ninguno (es raíz)";
            } else {
                for (size_t i = 0; i < padres.size(); i++) {
                    std::cout << padres[i]->getNombre();
                    if (i < padres.size() - 1) std::cout << ", ";
                }
            }
            std::cout << "\n";
            
            // Mostrar hijos
            auto hijos = nodo->getHijos();
            std::cout << "Hijos: ";
            if (hijos.empty()) {
                std::cout << "Ninguno (es hoja)";
            } else {
                for (size_t i = 0; i < hijos.size(); i++) {
                    std::cout << hijos[i]->getNombre();
                    if (i < hijos.size() - 1) std::cout << ", ";
                }
            }
            std::cout << "\n";
        }
    }
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
}

void mostrarAyuda() {
    std::cout << "\n╔═══════════════════════════════════════════════════╗\n";
    std::cout << "║                    AYUDA DEL SISTEMA              ║\n";
    std::cout << "╚═══════════════════════════════════════════════════╝\n\n";
    
    std::cout << "CÓMO USAR ESTE SISTEMA:\n\n";
    
    std::cout << "1. VISUALIZAR ESTRUCTURA:\n";
    std::cout << "   Muestra cómo están conectados los nodos de la red.\n";
    std::cout << "   Útil para entender las relaciones de dependencia.\n\n";
    
    std::cout << "2. VISUALIZAR TABLAS:\n";
    std::cout << "   Muestra todas las tablas de probabilidad condicional.\n";
    std::cout << "   Cada nodo tiene su tabla con todas las probabilidades.\n\n";
    
    std::cout << "3. REALIZAR INFERENCIA:\n";
    std::cout << "   Permite calcular probabilidades condicionales.\n";
    std::cout << "   Ejemplo: P(LluviaIntensa=si | PastoMojado=si)\n\n";
    
    std::cout << "4. VER INFORMACIÓN:\n";
    std::cout << "   Lista todas las variables con sus dominios,\n";
    std::cout << "   padres e hijos. Útil antes de hacer inferencias.\n\n";
    
    std::cout << "5. INFERENCIA RÁPIDA:\n";
    std::cout << "   Igual que la opción 3, pero sin mostrar todos\n";
    std::cout << "   los pasos intermedios. Más rápido para redes grandes.\n\n";
    
    std::cout << "6. CARGAR OTRA RED:\n";
    std::cout << "   Te permite cambiar a otra red bayesiana sin\n";
    std::cout << "   reiniciar el programa.\n\n";
    
    std::cout << "FORMATO DE INFERENCIA:\n";
    std::cout << "- Consulta: La(s) variable(s) cuya probabilidad quieres calcular\n";
    std::cout << "- Evidencia: Lo que ya sabes (variables observadas)\n";
    std::cout << "- Resultado: P(consulta | evidencia)\n\n";
    
    std::cout << "TIPS:\n";
    std::cout << "• Los nombres de variables distinguen mayúsculas/minúsculas\n";
    std::cout << "• Escribe los valores exactamente como aparecen en el dominio\n";
    std::cout << "• Puedes consultar múltiples variables a la vez\n";
    std::cout << "• La evidencia es opcional (puedes no poner ninguna)\n\n";
}

void inferenciaPersonalizada(RedBayesiana& red, bool conTraza = true) {
    std::cout << "\n╔═══════════════════════════════════════════════════╗\n";
    std::cout << "║              INFERENCIA PERSONALIZADA             ║\n";
    std::cout << "╚═══════════════════════════════════════════════════╝\n\n";
    
    // Mostrar variables disponibles
    std::cout << "VARIABLES DISPONIBLES:\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    auto nombresNodos = red.obtenerNombresNodos();
    for (const auto& nombre : nombresNodos) {
        auto nodo = red.obtenerNodo(nombre);
        if (nodo) {
            std::cout << "• " << nombre << " ∈ {";
            auto dominio = nodo->getDominio();
            for (size_t i = 0; i < dominio.size(); i++) {
                std::cout << dominio[i];
                if (i < dominio.size() - 1) std::cout << ", ";
            }
            std::cout << "}\n";
        }
    }
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    std::map<std::string, std::string> consulta;
    std::map<std::string, std::string> evidencia;
    
    // ========== CONSULTA ==========
    std::cout << "╔══════════════ PASO 1: CONSULTA ═══════════════╗\n";
    std::cout << "¿Qué variable(s) desea consultar?\n\n";
    std::cout << "¿Cuántas variables? ";
    int numConsulta;
    std::cin >> numConsulta;
    
    if (numConsulta <= 0 || numConsulta > (int)nombresNodos.size()) {
        std::cout << "\n❌ Número inválido de variables.\n";
        return;
    }
    
    for (int i = 0; i < numConsulta; i++) {
        std::cout << "\n--- Variable de consulta " << (i+1) << " ---\n";
        std::cout << "Nombre de la variable: ";
        std::string variable;
        std::cin >> variable;
        
        auto nodo = red.obtenerNodo(variable);
        if (!nodo) {
            std::cout << "❌ Error: Variable '" << variable << "' no encontrada.\n";
            std::cout << "Variables disponibles: ";
            for (const auto& n : nombresNodos) {
                std::cout << n << " ";
            }
            std::cout << "\n";
            i--;
            continue;
        }
        
        std::cout << "Dominio de " << variable << ": {";
        auto dominio = nodo->getDominio();
        for (size_t j = 0; j < dominio.size(); j++) {
            std::cout << dominio[j];
            if (j < dominio.size() - 1) std::cout << ", ";
        }
        std::cout << "}\n";
        
        std::cout << "Valor deseado: ";
        std::string valor;
        std::cin >> valor;
        
        // Validar que el valor esté en el dominio
        if (std::find(dominio.begin(), dominio.end(), valor) == dominio.end()) {
            std::cout << "⚠️  Advertencia: '" << valor << "' no está en el dominio.\n";
            std::cout << "¿Continuar de todas formas? (s/n): ";
            char resp;
            std::cin >> resp;
            if (resp != 's' && resp != 'S') {
                i--;
                continue;
            }
        }
        
        consulta[variable] = valor;
    }
    
    // ========== EVIDENCIA ==========
    std::cout << "\n╔══════════════ PASO 2: EVIDENCIA ══════════════╗\n";
    std::cout << "¿Qué variables ha observado (evidencia)?\n";
    std::cout << "(Presione 0 si no tiene evidencia)\n\n";
    std::cout << "¿Cuántas variables de evidencia? ";
    int numEvidencia;
    std::cin >> numEvidencia;
    
    if (numEvidencia < 0 || numEvidencia >= (int)nombresNodos.size()) {
        std::cout << "\n❌ Número inválido de variables.\n";
        return;
    }
    
    for (int i = 0; i < numEvidencia; i++) {
        std::cout << "\n--- Variable de evidencia " << (i+1) << " ---\n";
        std::cout << "Nombre de la variable: ";
        std::string variable;
        std::cin >> variable;
        
        // No puede ser una variable de consulta
        if (consulta.find(variable) != consulta.end()) {
            std::cout << "❌ Error: '" << variable << "' ya está en la consulta.\n";
            i--;
            continue;
        }
        
        auto nodo = red.obtenerNodo(variable);
        if (!nodo) {
            std::cout << "❌ Error: Variable '" << variable << "' no encontrada.\n";
            i--;
            continue;
        }
        
        std::cout << "Dominio de " << variable << ": {";
        auto dominio = nodo->getDominio();
        for (size_t j = 0; j < dominio.size(); j++) {
            std::cout << dominio[j];
            if (j < dominio.size() - 1) std::cout << ", ";
        }
        std::cout << "}\n";
        
        std::cout << "Valor observado: ";
        std::string valor;
        std::cin >> valor;
        
        // Validar que el valor esté en el dominio
        if (std::find(dominio.begin(), dominio.end(), valor) == dominio.end()) {
            std::cout << "⚠️  Advertencia: '" << valor << "' no está en el dominio.\n";
            std::cout << "¿Continuar de todas formas? (s/n): ";
            char resp;
            std::cin >> resp;
            if (resp != 's' && resp != 'S') {
                i--;
                continue;
            }
        }
        
        evidencia[variable] = valor;
    }
    
    // ========== CONFIRMAR ==========
    std::cout << "\n╔═══════════════ CONFIRMACIÓN ══════════════════╗\n";
    std::cout << "Consulta: P(";
    bool primero = true;
    for (const auto& par : consulta) {
        if (!primero) std::cout << ", ";
        std::cout << par.first << "=" << par.second;
        primero = false;
    }
    std::cout << ")";
    
    if (!evidencia.empty()) {
        std::cout << " | ";
        primero = true;
        for (const auto& par : evidencia) {
            if (!primero) std::cout << ", ";
            std::cout << par.first << "=" << par.second;
            primero = false;
        }
    }
    std::cout << "\n";
    std::cout << "╚═══════════════════════════════════════════════╝\n\n";
    
    std::cout << "¿Desea proceder con esta inferencia? (s/n): ";
    char confirmar;
    std::cin >> confirmar;
    
    if (confirmar == 's' || confirmar == 'S') {
        // Realizar inferencia
        if (conTraza) {
            red.inferenciaConTraza(consulta, evidencia);
        } else {
            double resultado = red.inferencia(consulta, evidencia);
            std::cout << "\n╔═══════════════════════════════════════════════════╗\n";
            std::cout << "║                    RESULTADO                      ║\n";
            std::cout << "╚═══════════════════════════════════════════════════╝\n\n";
            std::cout << "Probabilidad = " << std::fixed << std::setprecision(6) << resultado << "\n";
            std::cout << "             = " << std::fixed << std::setprecision(2) << (resultado * 100) << "%\n\n";
        }
    } else {
        std::cout << "\n❌ Inferencia cancelada.\n";
    }
}

bool cargarRed(RedBayesiana& red, std::string& nombreEstructura, std::string& nombreProbabilidades) {
    std::cout << "\n╔═══════════════════════════════════════════════════╗\n";
    std::cout << "║              CARGAR RED BAYESIANA                 ║\n";
    std::cout << "╚═══════════════════════════════════════════════════╝\n\n";
    
    std::cout << "Nombre del archivo de estructura [" << nombreEstructura << "]: ";
    std::string nuevaEstructura;
    std::getline(std::cin, nuevaEstructura);
    if (!nuevaEstructura.empty()) {
        nombreEstructura = nuevaEstructura;
    }
    
    std::cout << "Nombre del archivo de probabilidades [" << nombreProbabilidades << "]: ";
    std::string nuevasProbabilidades;
    std::getline(std::cin, nuevasProbabilidades);
    if (!nuevasProbabilidades.empty()) {
        nombreProbabilidades = nuevasProbabilidades;
    }
    
    std::cout << "\nCargando " << nombreEstructura << "...\n";
    if (!red.cargarEstructura(nombreEstructura)) {
        std::cerr << "\n❌ Error al cargar estructura.\n";
        return false;
    }
    
    std::cout << "Cargando " << nombreProbabilidades << "...\n";
    if (!red.cargarProbabilidades(nombreProbabilidades)) {
        std::cerr << "\n❌ Error al cargar probabilidades.\n";
        return false;
    }
    
    std::cout << "\n✓ Red cargada exitosamente!\n";
    return true;
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║   SISTEMA DE INFERENCIA CON REDES BAYESIANAS          ║\n";
    std::cout << "║   Versión Genérica - Compatible con cualquier red     ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";
    
    RedBayesiana red;
    std::string archivoEstructura = "estructura.txt";
    std::string archivoProbabilidades = "probabilidades.txt";
    
    // Cargar red inicial
    if (!cargarRed(red, archivoEstructura, archivoProbabilidades)) {
        std::cerr << "\n❌ No se pudo cargar la red inicial.\n";
        std::cerr << "Asegúrese de tener los archivos:\n";
        std::cerr << "  - estructura.txt\n";
        std::cerr << "  - probabilidades.txt\n";
        return 1;
    }
    
    pausar();
    
    // Menú interactivo
    int opcion;
    do {
        limpiarPantalla();
        mostrarMenu();
        std::cin >> opcion;
        std::cin.ignore(); // Limpiar buffer
        
        switch(opcion) {
            case 1:
                red.mostrarEstructura();
                pausar();
                break;
                
            case 2:
                red.mostrarTodasLasTablas();
                pausar();
                break;
                
            case 3:
                inferenciaPersonalizada(red, true);
                pausar();
                break;
                
            case 4:
                mostrarInformacionVariables(red);
                pausar();
                break;
                
            case 5:
                inferenciaPersonalizada(red, false);
                pausar();
                break;
                
            case 6: {
                RedBayesiana nuevaRed;
                if (cargarRed(nuevaRed, archivoEstructura, archivoProbabilidades)) {
                    red = nuevaRed;
                }
                pausar();
                break;
            }
                
            case 7:
                mostrarAyuda();
                pausar();
                break;
                
            case 8:
                std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
                std::cout << "║         ¡Gracias por usar el sistema!                  ║\n";
                std::cout << "║         Red Bayesiana - Inferencia por Enumeración     ║\n";
                std::cout << "╚════════════════════════════════════════════════════════╝\n\n";
                break;
                
            default:
                std::cout << "\n❌ Opción inválida. Por favor, seleccione 1-8.\n";
                pausar();
        }
        
    } while (opcion != 8);
    
    return 0;
}