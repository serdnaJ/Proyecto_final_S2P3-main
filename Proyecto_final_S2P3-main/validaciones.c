#include <stdio.h>
#include <string.h>
#include <math.h>
#include "complementos.h"

void ingresar_decimal(float *valor) {
    char buffer[100];
    char extra;
    int valido = 0;
    do {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error de entrada. Intente de nuevo:\n");
            continue;
        }
        // Valida que solo haya un número (entero o decimal) y nada más, y que sea >= 0
        if (sscanf(buffer, "%f %c", valor, &extra) == 1 && *valor >= 0) {
            valido = 1;
        } else {
            printf("Entrada invalida. Por favor, solo numeros (enteros o decimales) mayores o iguales a 0\nIngrese nuevamente:");
        }
    } while (!valido);
}

int validacion_decimales(const float *num_float)
{
    int num = (int)floor(*num_float);
    return num;
}

// Función auxiliar para verificar si hay datos en la tabla
int hay_datos_tabla(const float tabla[ZONAS][CONTA], int zonas, int conta)
{
    for (int z = 0; z < zonas; z++)
        for (int i = 0; i < conta; i++)
            if (tabla[z][i] != 0)
                return 1; // Hay datos
    return 0; // No hay datos
}