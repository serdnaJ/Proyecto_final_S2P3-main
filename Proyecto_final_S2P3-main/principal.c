#include <stdio.h>
#include "complementos.h"

Zona zonas_arreglo[ZONAS] = {0};
struct PromediosNoPonderados proContNoPond[ZONAS] = {0};

int main()
{
    puts("===============================================================");
    puts("  SISTEMA INTEGRAL DE GESTION Y PREDICCION DE CONTAMINACION DEL AIRE");
    puts("===============================================================\n");
    puts("                   Bienvenido al sistema\n");
    FILE *f = fopen(ARCHIVO_HIST, "r");
    if(!f)
    {
        puts("Error al abrir el archivo de datos históricos. Asegúrese de que el archivo exista.");
        return 1; // Salir si no se puede abrir el archivo
    }
    ingreso_datos_procesar(f);
    opciones_presente(); // Llama al menú principal actualizado
    puts("\nGracias por usar el sistema. Hasta pronto.");
    return 0;
}