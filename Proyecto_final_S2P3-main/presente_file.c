#include <stdio.h>
#include <time.h>
#include <string.h>
#include "complementos.h"

// Vacía el archivo especificado (lo deja en blanco)
void vaciar_tabla(const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo) {
        fclose(archivo); // lo cierra despues de vaciarlo con modo w
        if (strcmp(nombre_archivo, PATH_ACTUAL) == 0) {
            printf("%s vaciado correctamente.\n", ARCHIVO_ACTUAL);
        } else if (strcmp(nombre_archivo, PATH_PREDICCION) == 0) {
            printf("%s vaciado correctamente.\n", ARCHIVO_PREDICCION);
        }
    } else {
        if (strcmp(nombre_archivo, PATH_ACTUAL) == 0) {
            printf("No se pudo vaciar %s.\n", ARCHIVO_ACTUAL);
        } else if (strcmp(nombre_archivo, PATH_PREDICCION) == 0) {
            printf("No se pudo vaciar %s.\n", ARCHIVO_PREDICCION);
        }
    }
}

// Lee y muestra el contenido del archivo especificado
void leer_tabla(const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        if (strcmp(nombre_archivo, PATH_ACTUAL) == 0) {
            printf("Archivo %s inexistente\n", ARCHIVO_ACTUAL);
        } else if (strcmp(nombre_archivo, PATH_PREDICCION) == 0) {
            printf("Archivo %s inexistente\n", ARCHIVO_PREDICCION);
        }

        fclose(archivo);
        return;
    }
    int c = fgetc(archivo);
    if (c == EOF) {
        if (strcmp(nombre_archivo, PATH_ACTUAL) == 0) {
            printf("Archivo vacio: %s\n", ARCHIVO_ACTUAL);
        } else if (strcmp(nombre_archivo, PATH_PREDICCION) == 0) {
            printf("Archivo vacio: %s\n", ARCHIVO_PREDICCION);
        }
        fclose(archivo);
        return;
    }
    rewind(archivo); // Mover el puntero al inicio del archivo

    // Leer cabecera
    char linea[256];
    if (fgets(linea, sizeof(linea), archivo)) {
        printf("\n%-20s %-12s %-15s %-15s %-8s %-12s\n", "Fecha", "Zona", "Contaminante", "Valor", "Limite", "Excede");
        printf("------------------------------------------------------------------------------------------\n");
    }

    // Leer y mostrar cada fila alineada
    while (fgets(linea, sizeof(linea), archivo)) {
        char fecha[64], zona[32], cont[32], valor[32], lim[32], exc[32];
        // El formato depende de cómo guardas el CSV, aquí se asume el formato con comillas para los primeros tres campos
        if (sscanf(linea, "\"%63[^\"]\",\"%31[^\"]\",\"%31[^\"]\",%31[^,],%31[^,],%31[^\n]", fecha, zona, cont, valor, lim, exc) == 6) {
            printf("%-20s %-12s %-15s %-15s %-8s %-12s\n", fecha, zona, cont, valor, lim, exc);
        }
    }
    fclose(archivo);
    printf("\n");
}

// Guarda la tabla en el archivo especificado, formato CSV
void guardar_tabla(const float nivel_actual[ZONAS][CONTA], const struct contaminante *limites, int n, const char *nombre_archivo, const char *titulo) {
    extern const char *zonas[];
    FILE *archivo;
    int archivo_vacio = 0;

    // Obtener fecha y hora actual
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char fecha_hora[64];
    strftime(fecha_hora, sizeof(fecha_hora), "%Y-%m-%d %H:%M:%S", tm_info);

    // Verifica si el archivo existe y está vacío
    archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        archivo_vacio = 1;
    } else {
        int c = fgetc(archivo);
        if (c == EOF) {
            archivo_vacio = 1;
        }
        fclose(archivo);
    }

    // Abre el archivo en modo append (lo crea si no existe)
    archivo = fopen(nombre_archivo, "a");
    if (!archivo) {
        printf("No se pudo abrir el archivo para guardar la tabla.\n");
        return;
    }

    // Escribe el encabezado solo si el archivo está vacío
    if (archivo_vacio) {
        fprintf(archivo, "Fecha, Zona, Contaminante, %s, Limite, Excede Limite\n", titulo);
    }

    // Escribe los datos por zona y contaminante en formato CSV
    for (int z = 0; z < ZONAS; z++) {
        for (int i = 0; i < n; i++) {
            fprintf(archivo, "\"%s\",\"%s\",\"%s\",%.2f,%d,%s\n",
                fecha_hora,
                zonas[z],
                limites[i].nombre,
                nivel_actual[z][i],
                limites[i].limite,
                (nivel_actual[z][i] >= limites[i].limite) ? "SI" : "NO");
        }
    }
    fclose(archivo);
}

void mostrar_historico(const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        printf("Archivo %s inexistente\n", nombre_archivo);
        return;
    }
    char linea[256];
    // Leer cabecera
    if (fgets(linea, sizeof(linea), archivo)) {
        printf("\n%-12s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n", 
            "Zona", "CO2", "SO2", "NO2", "PM2.5", "Temp", "Viento", "Humedad");
        printf("--------------------------------------------------------------------------\n");
    }
    // Leer y mostrar cada fila alineada
    while (fgets(linea, sizeof(linea), archivo)) {
        char zona[32];
        float co2, so2, no2, pm25, temp, viento, humedad;
        if (sscanf(linea, "%31[^,],%f,%f,%f,%f,%f,%f,%f", zona, &co2, &so2, &no2, &pm25, &temp, &viento, &humedad) == 8) {
            printf("%-12s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f\n",
                zona, co2, so2, no2, pm25, temp, viento, humedad);
        }
    }
    fclose(archivo);
    printf("\n");
}

void mostrar_predicciones(const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        printf("Archivo %s inexistente\n", nombre_archivo);
        return;
    }
    char linea[256];
    // Leer cabecera
    if (fgets(linea, sizeof(linea), archivo)) {
        printf("\n%-12s %-10s %-10s %-10s %-10s\n", "Zona", "CO2", "SO2", "NO2", "PM2.5");
        printf("-------------------------------------------------------------\n");
    }
    // Leer y mostrar cada fila alineada
    while (fgets(linea, sizeof(linea), archivo)) {
        char zona[32];
        float co2, so2, no2, pm25;
        if (sscanf(linea, "%31[^,],%f,%f,%f,%f", zona, &co2, &so2, &no2, &pm25) == 5) {
            printf("%-12s %-10.2f %-10.2f %-10.2f %-10.2f\n", zona, co2, so2, no2, pm25);
        }
    }
    fclose(archivo);
    printf("\n");
}