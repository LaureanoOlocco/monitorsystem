/**
 * @file metrics.h
 * @brief Funciones para obtener el uso de CPU, memoria, red y disco desde el sistema de archivos /proc.
 *
 * Este archivo contiene las definiciones de funciones que permiten obtener información
 * sobre el uso de recursos del sistema, incluyendo CPU, memoria, red y disco, a través
 * de la lectura de archivos en el sistema de archivos virtual /proc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h> // Añadido para statvfs
#include <unistd.h>

#define BUFFER_SIZE 256 /**< Tamaño del búfer utilizado para leer archivos. */

/**
 * @brief Obtiene el porcentaje de uso de memoria desde /proc/meminfo.
 *
 * Esta función lee los valores de memoria total y disponible desde /proc/meminfo
 * y calcula el porcentaje de uso de memoria.
 *
 * @return Uso de memoria como porcentaje (0.0 a 100.0), o -1.0 en caso de error.
 */
double get_memory_usage();

/**
 * @brief Obtiene el porcentaje de uso de CPU desde /proc/stat.
 *
 * Esta función lee los tiempos de CPU desde /proc/stat y calcula el porcentaje
 * de uso de CPU en un intervalo de tiempo.
 *
 * @return Uso de CPU como porcentaje (0.0 a 100.0), o -1.0 en caso de error.
 */
double get_cpu_usage();

/**
 * @brief Obtiene el porcentaje de uso de red desde /proc/net/dev.
 *
 * Esta función lee los bytes recibidos y transmitidos desde /proc/net/dev y
 * calcula el porcentaje de uso de red basado en la diferencia entre lecturas.
 *
 * @return Uso de red como porcentaje (0.0 a 100.0), o -1.0 en caso de error.
 */
double get_network_usage();

/**
 * @brief Obtiene el porcentaje de uso de disco desde /proc/diskstats y statvfs.
 *
 * Esta función lee la información de uso de disco desde /proc/diskstats y utiliza
 * statvfs para obtener información del sistema de archivos y calcular el porcentaje
 * de uso.
 *
 * @return Uso de disco como porcentaje (0.0 a 100.0), o -1.0 en caso de error.
 */
double get_disk_usage();
