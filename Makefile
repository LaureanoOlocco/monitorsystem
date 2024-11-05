# Makefile para compilar el programa de métricas

# Compilador
CC = gcc

# Flags del compilador
CFLAGS = -Iinclude

# Librerías
LIBS = -lprom -pthread -lpromhttp -lcjson

# Archivos fuente
SOURCES = src/main.c src/expose_metrics.c src/metrics.c

# Nombre del ejecutable
TARGET = metrics

# Regla por defecto
all: $(TARGET)

# Regla para compilar el programa
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

# Regla para limpiar los archivos compilados
clean:
	rm -f $(TARGET)

# Regla para recompilar todo
rebuild: clean all