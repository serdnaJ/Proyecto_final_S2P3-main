#include <stdio.h>
#include "complementos.h"

void opciones_presente()
{
    struct contaminante limites[CONTA] = {
        {"PM", 15},
        {"NO2", 25},
        {"SO2", 40},
        {"CO2", 750}};

    float nivel_actual[ZONAS][CONTA] = {0};
    float prediccion[ZONAS][CONTA] = {0};
    int elevado[ZONAS][CONTA] = {0};
    int n = CONTA;
    float opcion_float;
    int opcion;

    do
    {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1. Datos actuales\n");
        printf("2. Prediccion de contaminacion\n");
        printf("3. Mostrar y guardar promedio de contaminacion en historia\n");
        printf("4. Visualizacion de tablas\n");
        printf("5. Vaciar archivos\n");
        printf("6. Salir del sistema\n");

        do
        {
            printf("Seleccione una opcion (1-6): ");
            ingresar_decimal(&opcion_float);
            opcion = validacion_decimales(&opcion_float);
            if (opcion < 1 || opcion > 6)
            {
                printf("Opcion no valida. Intente de nuevo.\n");
            }
        } while (opcion < 1 || opcion > 6);

        switch (opcion)
        {
        case 1:
            menu_datos_actuales(nivel_actual, limites, &n, elevado);
            break;
        case 2:
            menu_prediccion(prediccion, limites, &n);
            break;

        case 3:
        {

            FILE *f = fopen(ARCHIVO_RESULTADO_HISTORIA, "a");
            if (f)
            {
                calculo_promedio_historico(f, limites);
                fclose(f);
            }
            else
            {
                printf("No se pudo abrir el archivo de resultados.\n");
            }
            break;
        }

        case 4:
            menu_visualizacion();
            break;
        case 5:
            menu_vaciado();
            break;
        case 6:
            printf("Saliendo del sistema...\n");
            break;
        }
    } while (opcion != 6);

    printf("\n\n");
}

// Submenú de datos actuales
// Submenú de datos actuales
void menu_datos_actuales(float nivel_actual[ZONAS][CONTA], struct contaminante *limites, int *n, int elevado[ZONAS][CONTA]) {
    float opcion_float;
    int opcion;
    do {
        printf("\n--- MENU DATOS ACTUALES ---\n");
        printf("1. Ingresar datos actuales y comparar con limites\n");
        printf("2. Ver tabla de datos actuales\n");
        printf("3. Volver al menu principal\n");
        do {
            printf("Seleccione una opcion (1-3): ");
            ingresar_decimal(&opcion_float);
            opcion = validacion_decimales(&opcion_float);
            if (opcion < 1 || opcion > 3) {
                printf("Opcion no valida. Intente de nuevo.\n");
            }
        } while (opcion < 1 || opcion > 3);

        switch (opcion) {
        case 1:
            ingresar_datos(nivel_actual, limites, n);
            comparar_niveles(nivel_actual, limites, elevado, n);
            for (int z = 0; z < ZONAS; z++) {
                printf("\nZona: %s\n", zonas[z]);
                mostrar_recomendaciones(elevado[z], *n);
            }
            guardar_tabla(nivel_actual, limites, *n, PATH_ACTUAL, "Nivel Actual");
            printf("Datos actuales guardados correctamente.\n");
            break;
        case 2:
            leer_tabla(PATH_ACTUAL);
            break;
        }
    } while (opcion != 3);
}

// Submenú de predicción
void menu_prediccion(float prediccion[ZONAS][CONTA], struct contaminante *limites, int *n) {
    float opcion_float;
    int opcion;
    int elevado[ZONAS][CONTA] = {0};
    do {
        printf("\n--- MENU PREDICCION ---\n");
        printf("1. Ingresar datos para prediccion y guardar tabla\n");
        printf("2. Ver tabla de datos de prediccion comparadas con limites\n");
        printf("3. Mostrar alertas y recomendaciones por zonas (usando datos de prediccion)\n");
        printf("4. Volver al menu principal\n");
        do {
            printf("Seleccione una opcion (1-4): ");
            ingresar_decimal(&opcion_float);
            opcion = validacion_decimales(&opcion_float);
            if (opcion < 1 || opcion > 4) {
                printf("Opcion no valida. Intente de nuevo.\n");
            }
        } while (opcion < 1 || opcion > 4);

        switch (opcion) {
        case 1:
            prediccion_general(prediccion, 0); // Calcula y llena el arreglo SIN mostrar
            guardar_tabla(prediccion, limites, *n, PATH_PREDICCION, "Prediccion");
            printf("Prediccion calculada y guardada correctamente.\n");
            break;
        case 2:
            leer_tabla(PATH_PREDICCION);
            break;
        case 3:
            comparar_niveles(prediccion, limites, elevado, n);
            for (int z = 0; z < ZONAS; z++) {
                printf("\nZona: %s\n", zonas[z]);
                mostrar_recomendaciones(elevado[z], *n);
            }
            break;
        }
    } while (opcion != 4);
}

// Submenú de visualización
void menu_visualizacion(void)
{
    float opcion_float;
    int opcion;
    float prediccion[ZONAS][CONTA] = {0};
    do
    {
        printf("\n--- MENU VISUALIZACION ---\n");
        printf("1. Ver tabla de datos actuales\n");
        printf("2. Ver tabla de datos de prediccion\n");
        printf("3. Ver tabla de datos historicos\n");
        printf("4. Ver tabla de predicciones\n");
        printf("5. Volver al menu principal\n");
        do
        {
            printf("Seleccione una opcion (1-5): ");
            ingresar_decimal(&opcion_float);
            opcion = validacion_decimales(&opcion_float);
            if (opcion < 1 || opcion > 5)
            {
                printf("Opcion no valida. Intente de nuevo.\n");
            }
        } while (opcion < 1 || opcion > 5);

        switch (opcion)
        {
        case 1:
            leer_tabla(PATH_ACTUAL);
            break;
        case 2:
            leer_tabla(PATH_PREDICCION);
            break;
        case 3:
            mostrar_historico(ARCHIVO_HIST);
            break;
        case 4:
            mostrar_predicciones(ARCHIVO_REPORTE);
            break;
        }
    } while (opcion != 5);
}

// Submenú de vaciado de archivos
void menu_vaciado(void)
{
    float opcion_float;
    int opcion;
    do
    {
        printf("\n--- MENU VACIADO DE ARCHIVOS ---\n");
        printf("1. Vaciar archivo de datos actuales\n");
        printf("2. Vaciar archivo de datos de prediccion\n");
        printf("3. Vaciar archivo de datos promedio historico\n");
        printf("4. Volver al menu principal\n");
        do
        {
            printf("Seleccione una opcion (1-4): ");
            ingresar_decimal(&opcion_float);
            opcion = validacion_decimales(&opcion_float);
            if (opcion < 1 || opcion > 4)
            {
                printf("Opcion no valida. Intente de nuevo.\n");
            }
        } while (opcion < 1 || opcion > 4);

        switch (opcion)
        {
        case 1:
            vaciar_tabla(PATH_ACTUAL);
            printf("Archivo de datos actuales vaciado.\n");
            break;
        case 2:
            vaciar_tabla(PATH_PREDICCION);
            printf("Archivo de datos de prediccion vaciado.\n");
            break;
        case 3:
            vaciar_tabla(ARCHIVO_RESULTADO_HISTORIA);
            printf("Archivo de promedios historicos vaciado.\n");
            break;
        }
    } while (opcion != 4);
}
