# Makefile para compilar el proyecto de Red Bayesiana

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = red_bayesiana
OBJS = main.o Nodo.o RedBayesiana.o

# Regla principal
all: $(TARGET)

# Compilar el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "Compilación exitosa! Ejecute con: ./$(TARGET)"

# Compilar archivos objeto
main.o: main.cpp RedBayesiana.h Nodo.h
	$(CXX) $(CXXFLAGS) -c main.cpp

RedBayesiana.o: RedBayesiana.cpp RedBayesiana.h Nodo.h
	$(CXX) $(CXXFLAGS) -c RedBayesiana.cpp

Nodo.o: Nodo.cpp Nodo.h
	$(CXX) $(CXXFLAGS) -c Nodo.cpp

# Limpiar archivos compilados
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Archivos limpiados"

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Ayuda
help:
	@echo "Comandos disponibles:"
	@echo "  make        - Compila el proyecto"
	@echo "  make clean  - Elimina archivos compilados"
	@echo "  make run    - Compila y ejecuta el programa"
	@echo "  make help   - Muestra esta ayuda"

.PHONY: all clean run help
