/**
 * @file expose_metrics.h
 * @brief Programa para leer el uso de CPU, memoria, red y disco y exponerlos como métricas de Prometheus.
 *
 * Este archivo contiene las definiciones de las funciones utilizadas para actualizar y exponer
 * métricas de uso del sistema a Prometheus a través de un servidor HTTP.
 */

#include "metrics.h"
#include <errno.h>
#include <prom.h>
#include <promhttp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Para sleep

#define BUFFER_SIZE 256 /**< Tamaño del búfer utilizado para leer archivos. */

/**
 * @brief Actualiza la métrica de uso de CPU.
 *
 * Esta función actualiza la métrica de uso de CPU para reflejar el estado actual.
 */
void update_cpu_gauge();

/**
 * @brief Actualiza la métrica de uso de memoria.
 *
 * Esta función actualiza la métrica de uso de memoria para reflejar el estado actual.
 */
void update_memory_gauge();

/**
 * @brief Actualiza la métrica de uso de red.
 *
 * Esta función actualiza la métrica de uso de red para reflejar el estado actual.
 */
void update_network_gauge();

/**
 * @brief Actualiza la métrica de uso de disco.
 *
 * Esta función actualiza la métrica de uso de disco para reflejar el estado actual.
 */
void update_disk_gauge();

/**
 * @brief Función del hilo para exponer las métricas vía HTTP en el puerto 8000.
 *
 * Esta función se ejecuta en un hilo separado y expone las métricas recopiladas
 * a través de un servidor HTTP en el puerto 8000.
 *
 * @param arg Argumento no utilizado.
 * @return NULL
 */
void* expose_metrics(void* arg);

/**
 * @brief Inicializa mutex y métricas.
 *
 * Esta función inicializa el mutex necesario para la sincronización de acceso
 * a las métricas y configura las métricas iniciales.
 */
void init_metrics();

/**
 * @brief Destructor de mutex.
 *
 * Esta función libera los recursos asociados al mutex utilizado en la aplicación.
 */
void destroy_mutex();
