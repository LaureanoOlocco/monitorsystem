#include "metrics.h"

double get_memory_usage()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long total_mem = 0, free_mem = 0;

    // Abrir el archivo /proc/meminfo
    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/meminfo");
        return -1.0;
    }

    // Leer los valores de memoria total y disponible
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (sscanf(buffer, "MemTotal: %llu kB", &total_mem) == 1)
        {
            continue; // MemTotal encontrado
        }
        if (sscanf(buffer, "MemAvailable: %llu kB", &free_mem) == 1)
        {
            break; // MemAvailable encontrado, podemos dejar de leer
        }
    }

    fclose(fp);

    // Verificar si se encontraron ambos valores
    if (total_mem == 0 || free_mem == 0)
    {
        fprintf(stderr, "Error al leer la información de memoria desde /proc/meminfo\n");
        return -1.0;
    }

    // Calcular el porcentaje de uso de memoria
    double used_mem = total_mem - free_mem;
    double mem_usage_percent = (used_mem / total_mem) * 100.0;

    return mem_usage_percent;
}

double get_cpu_usage()
{
    static unsigned long long prev_total = 0, prev_idle = 0;
    unsigned long long total, idle, user, nice, system, iowait, irq, softirq, steal;
    double cpu_usage;

    FILE* fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/stat");
        return -1.0;
    }

    if (fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle, &iowait, &irq, &softirq,
               &steal) != 8)
    {
        fclose(fp);
        return -1.0;
    }
    fclose(fp);

    total = user + nice + system + idle + iowait + irq + softirq + steal;
    unsigned long long total_diff = total - prev_total;
    unsigned long long idle_diff = idle - prev_idle;

    if (total_diff == 0)
    {
        // No ha habido cambios, devolver el último valor válido o 0
        return (prev_total > 0) ? ((prev_total - prev_idle) * 100.0 / prev_total) : 0.0;
    }

    cpu_usage = (1.0 - ((double)idle_diff / total_diff)) * 100.0;

    prev_total = total;
    prev_idle = idle;

    return cpu_usage;
}

double get_network_usage()
{
    static unsigned long long prev_bytes = 0;
    unsigned long long bytes = 0;
    char line[BUFFER_SIZE];
    FILE* fp = fopen("/proc/net/dev", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/net/dev");
        return -1.0;
    }

    // Saltar las dos primeras líneas
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp))
    {
        char* colon = strchr(line, ':');
        if (colon)
        {
            unsigned long long rx_bytes, tx_bytes;
            sscanf(colon + 1, "%llu %*u %*u %*u %*u %*u %*u %*u %llu", &rx_bytes, &tx_bytes);
            bytes += rx_bytes + tx_bytes;
        }
    }
    fclose(fp);

    if (prev_bytes == 0)
    {
        prev_bytes = bytes;
        return 0.0; // Primera lectura
    }

    double usage_kb = (bytes - prev_bytes) / 1024.0; // Convertir a KB
    prev_bytes = bytes;

    return usage_kb;
}

double get_disk_usage()
{
    struct statvfs stat;

    if (statvfs("/", &stat) != 0)
    {
        perror("Error al obtener estadísticas del sistema de archivos");
        return -1.0;
    }

    unsigned long long total_space = stat.f_blocks * stat.f_frsize;
    unsigned long long free_space = stat.f_bfree * stat.f_frsize;
    unsigned long long used_space = total_space - free_space;

    // Calcular el porcentaje de uso
    double usage_percent = ((double)used_space / total_space) * 100.0;

    return usage_percent;
}
