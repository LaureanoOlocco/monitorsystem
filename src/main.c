/**
 * @file main.c
 * @brief Entry point of the system
 *
 * Este archivo contiene la función principal del programa, que inicializa
 * las métricas del sistema, crea un hilo para exponer esas métricas a través
 * de HTTP y entra en un bucle principal que actualiza las métricas periódicamente.
 */

#include "expose_metrics.h"
#include <pthread.h> // Necesario para el uso de hilos
#include <stdbool.h>
#include <stdio.h>  // Necesario para printf y fprintf
#include <stdlib.h> // Necesario para EXIT_FAILURE y EXIT_SUCCESS
#include <unistd.h> // Necesario para sleep y getcwd
#include <cjson/cJSON.h>

#define CONFIG_FILE "../lib/monitor/config.json"
#define SLEEP_TIME 1          /**< Tiempo en segundos entre actualizaciones de métricas. */
volatile bool running = true; /**< Variable que controla el estado de ejecución del programa. */


// Variables para configuraciones leídas desde el archivo JSON
int sample_interval = 1;
bool collect_cpu = true;
bool collect_memory = true;
bool collect_network = true;
bool collect_disk = true;

bool load_config()
{
    //printf("Intentando abrir el archivo de configuración: %s\n", CONFIG_FILE);
    FILE *file = fopen(CONFIG_FILE, "r");
    if (!file)
    {
        fprintf(stderr, "No se pudo abrir el archivo de configuración %s\n", CONFIG_FILE);
        return false;
    }
    else
    {
        printf("Archivo de configuración abierto correctamente: %s\n", CONFIG_FILE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = malloc(length + 1);
    if (!data)
    {
        fprintf(stderr, "Error de memoria\n");
        fclose(file);
        return false;
    }

    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0';

    cJSON *config_json = cJSON_Parse(data);
    free(data);

    if (!config_json)
    {
        fprintf(stderr, "Error al analizar el JSON de configuración\n");
        return false;
    }

    // Extrae las configuraciones desde el JSON
    cJSON *interval = cJSON_GetObjectItem(config_json, "sample_interval");
    cJSON *cpu = cJSON_GetObjectItem(config_json, "collect_cpu");
    cJSON *memory = cJSON_GetObjectItem(config_json, "collect_memory");
    cJSON *network = cJSON_GetObjectItem(config_json, "collect_network");
    cJSON *disk = cJSON_GetObjectItem(config_json, "collect_disk");

    if (interval) sample_interval = interval->valueint;
    if (cpu) collect_cpu = cJSON_IsTrue(cpu);
    if (memory) collect_memory = cJSON_IsTrue(memory);
    if (network) collect_network = cJSON_IsTrue(network);
    if (disk) collect_disk = cJSON_IsTrue(disk);

    cJSON_Delete(config_json);
    return true;
}


/**
 * @brief Punto de entrada del programa.
 *
 * Esta función inicializa las métricas del sistema y crea un hilo para
 * exponer estas métricas a través de HTTP. Luego, entra en un bucle
 * que actualiza las métricas cada segundo mientras la variable `running`
 * sea verdadera.
 *
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Arreglo de cadenas de caracteres que representan los argumentos de línea de comandos.
 * @return EXIT_SUCCESS si el programa se ejecuta correctamente, o EXIT_FAILURE en caso de error.
 */
int main(int argc, char* argv[])
{   
        printf("\n"
           "╔══════════════════════════════════════════════╗\n"
           "║             Monitor del Sistema              ║\n"
           "║──────────────────────────────────────────────║\n"
           "║  Iniciando monitoreo de:                     ║\n"
           "║  • CPU                                       ║\n"
           "║  • Memoria                                   ║\n"
           "║  • Red                                       ║\n"
           "║  • Disco                                     ║\n"
           "║                                              ║\n"
           "║  Servidor HTTP iniciado en puerto 8000       ║\n"
           "╚══════════════════════════════════════════════╝\n\n");


        // Lee la configuración desde el archivo JSON
    if (!load_config())
    {
        fprintf(stderr, "Error al cargar la configuración. Usando valores predeterminados.\n");
    }

    // Inicializar las métricas
    init_metrics();

    // Creamos un hilo para exponer las métricas vía HTTP
    pthread_t tid;
    if (pthread_create(&tid, NULL, expose_metrics, NULL) != 0)
    {
        fprintf(stderr, "Error al crear el hilo del servidor HTTP\n");
        return EXIT_FAILURE;
    }

    while (running)
    {
        if (collect_cpu) update_cpu_gauge();
        if (collect_memory) update_memory_gauge();
        if (collect_network) update_network_gauge();
        if (collect_disk) update_disk_gauge();
        sleep(sample_interval);
    }

    return EXIT_SUCCESS; /**< Finaliza el programa con éxito. */
}
