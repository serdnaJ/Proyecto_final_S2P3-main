#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "complementos.h"

// Definición de arreglos globales
const char *recomendaciones[] = {
    "Reducir trafico vehicular",
    "Cierre temporal de industrias",
    "Suspender actividades al aire libre para minimizar exposicion",
    "Promover el uso de transporte publico",
    "Aumentar areas verdes"};

const char *zonas[] = {
    "Beijing",
    "Shanghai",
    "Shenzhen",
    "Guangzhou",
    "Tianjin"};

// Ingreso de datos actuales
void ingresar_datos(float nivel_actual[ZONAS][CONTA], struct contaminante *limites, int *n)
{
    extern const char *zonas[];
    for (int z = 0; z < ZONAS; z++)
    {
        printf("Ingrese los datos para la zona %d - %s:\n", z + 1, zonas[z]);
        for (int j = 0; j < *n; j++)
        {
            printf("Ingrese el nivel actual de %s: ", limites[j].nombre);
            ingresar_decimal(&nivel_actual[z][j]);
        }
        printf("\n"); // Espacio después de cada zona
    }
}

// Comparación de niveles con límites
void comparar_niveles(const float nivel_actual[ZONAS][CONTA], const struct contaminante *limites, int elevado[ZONAS][CONTA], int *n)
{
    extern const char *zonas[];
    for (int z = 0; z < ZONAS; z++)
    {
        printf("Zona: %s\n", zonas[z]);
        for (int i = 0; i < *n; i++)
        {
            printf("Contaminante: %s\n", limites[i].nombre);
            printf("Nivel actual: %.2f\n", nivel_actual[z][i]);
            if (nivel_actual[z][i] >= limites[i].limite)
            {
                printf("ALERTA: El nivel de %s supera el limite permitido de %d.\n", limites[i].nombre, limites[i].limite);
                elevado[z][i] = 1;
            }
            else
            {
                printf("El nivel de %s esta dentro del limite permitido.\n", limites[i].nombre);
                elevado[z][i] = 0;
            }
            printf("\n");
        }
    }
}

// Mostrar recomendaciones según niveles elevados
void mostrar_recomendaciones(const int *elevado, int n)
{
    printf("Recomendaciones:\n");
    if (elevado[0] && elevado[1])
    {
        printf("El nivel de PM y NO2 estan elevados.\n");
        printf("%s\n%s\n%s\n", recomendaciones[0], recomendaciones[2], recomendaciones[3]);
    }
    else if (elevado[1])
    {
        printf("El nivel de NO2 esta elevado.\n");
        printf("%s\n", recomendaciones[1]);
    }
    else if (elevado[0] && elevado[3])
    {
        printf("El nivel de PM y CO2 estan elevados.\n");
        printf("%s\n", recomendaciones[4]);
    }
    else
    {
        printf("No se requieren recomendaciones especiales.\n");
    }
}

// Mostrar tabla de datos (actuales o predicción)
void mostrar_tabla(const float nivel_actual[ZONAS][CONTA], const struct contaminante *limites, int n, const char *titulo)
{
    extern const char *zonas[];

    // Obtener fecha y hora actual
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char fecha_hora[64];
    strftime(fecha_hora, sizeof(fecha_hora), "%Y-%m-%d %H:%M:%S", tm_info);

    // Espacio antes de la tabla y mostrar fecha/hora
    printf("\n\n\nTabla mostrada el: %s\n\n", fecha_hora);

    printf("%-15s %-15s %-15s %-15s %-15s\n", "Zona", "Contaminante", titulo, "Limite", "Excede Limite");
    printf("-----------------------------------------------------------------------------------------------\n");
    for (int z = 0; z < ZONAS; z++)
    {
        for (int i = 0; i < n; i++)
        {
            printf("%-15s %-15s %-15.2f %-15d %-15s\n",
                   zonas[z],
                   limites[i].nombre,
                   nivel_actual[z][i],
                   limites[i].limite,
                   (nivel_actual[z][i] >= limites[i].limite) ? "SI" : "NO");
        }
        // Línea separadora después de cada zona
        printf("-----------------------------------------------------------------------------------------------\n");
    }
    // Espacio extra al final de la tabla
    printf("\n\n");
}
