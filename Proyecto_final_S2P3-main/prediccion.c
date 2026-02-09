#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "complementos.h"


// ------------------------------------------------------------------
int buscar_o_crear_zona(Zona zonas_arreglo[], const char *nombre)
{
    for (int i = 0; i < ZONAS; ++i) // Usa ZONAS de complementos.h
    {
        if (strcmp(zonas_arreglo[i].nombre, nombre) == 0) // ya existe
            return i;

        if (zonas_arreglo[i].nombre[0] == '\0') // casilla libre
        {
            strncpy(zonas_arreglo[i].nombre, nombre, MAX_NOMBRE - 1);
            zonas_arreglo[i].nombre[MAX_NOMBRE - 1] = '\0';
            return i;
        }
    }
    return -1; // sin espacio
}

// ------------------------------------------------------------------
// Funcion principal para la prediccion general
void prediccion_general(float prediccion[ZONAS][CONTA], int mostrar) {
    FILE *fh = fopen(ARCHIVO_HIST, "r");
    if (!fh) {
        printf("No se pudo abrir el archivo historico.\n");
        return;
    }

    // Zona zonas_arreglo[ZONAS] = {0};
    char linea[256];
    fgets(linea, sizeof linea, fh); // descartar cabecera

    while (fgets(linea, sizeof linea, fh)) {
        char zona_nom[MAX_NOMBRE];
        Registro r;
        if (sscanf(linea, "%19[^,],%f,%f,%f,%f,%f,%f,%f",
                   zona_nom, &r.co2, &r.so2, &r.no2, &r.pm25,
                   &r.temp, &r.viento, &r.humedad) != 8)
            continue;

        int idx = buscar_o_crear_zona(zonas_arreglo, zona_nom);
        if (idx == -1)
            continue;

        for (int k = HIST_DIAS - 1; k > 0; --k)
            zonas_arreglo[idx].hist[k] = zonas_arreglo[idx].hist[k - 1];
        zonas_arreglo[idx].hist[0] = r;

        if (zonas_arreglo[idx].dias_cargados < HIST_DIAS)
            zonas_arreglo[idx].dias_cargados++;
    }
    fclose(fh);

    // Pedir datos de clima actual al usuario
    Registro clima_act;
    puts("\n=== INGRESE CONDICIONES CLIMATICAS ACTUALES ===");

    printf("Temperatura (C): ");
    ingresar_decimal(&clima_act.temp);

    printf("Viento (m/s):     ");
    ingresar_decimal(&clima_act.viento);

    printf("Humedad (%%):      ");
    ingresar_decimal(&clima_act.humedad);

    // Prediccion promedio ponderado
    const float PESO[HIST_DIAS] = {0.5f, 0.3f, 0.2f};
    for (int i = 0; i < ZONAS && zonas_arreglo[i].nombre[0]; ++i) {
        float suma_peso = 0;
        float co2 = 0, so2 = 0, no2 = 0, pm = 0;
        float tp = 0, vp = 0, hp = 0;

        for (int d = 0; d < zonas_arreglo[i].dias_cargados; ++d) {
            float peso =  (HIST_DIAS - d);
            co2 += zonas_arreglo[i].hist[d].co2 * peso;
            so2 += zonas_arreglo[i].hist[d].so2 * peso;
            no2 += zonas_arreglo[i].hist[d].no2 * peso;
            pm += zonas_arreglo[i].hist[d].pm25 * peso;

            tp += zonas_arreglo[i].hist[d].temp * peso;
            vp += zonas_arreglo[i].hist[d].viento * peso;
            hp += zonas_arreglo[i].hist[d].humedad * peso;
            suma_peso += peso;
        }

        co2 /= suma_peso;
        so2 /= suma_peso;
        no2 /= suma_peso;
        pm /= suma_peso;
        tp /= suma_peso;
        vp /= suma_peso;
        hp /= suma_peso;

        float factor = 1.0f + ((clima_act.temp > tp) ? 0.05f : -0.05f);
        if (clima_act.viento >= 3.0f)
            factor -= 0.05f;
        if (clima_act.humedad >= 80.0f)
            pm *= 1.05f;

        prediccion[i][0] = co2 * factor;
        prediccion[i][1] = so2 * factor;
        prediccion[i][2] = no2 * factor;
        prediccion[i][3] = pm;
    }

    // Si mostrar==1, imprime y guarda resultados
    if (mostrar) {
        puts("\n====== PREDICCION 24 h ======");
        printf("%-10s | %10s %10s %10s %10s\n", "Zona", "CO2", "SO2", "NO2", "PM2.5");
        puts("--------------------------------------------------------------");
        for (int i = 0; i < ZONAS && zonas_arreglo[i].nombre[0]; ++i)
            printf("%-10s | %10.1f %10.1f %10.1f %10.1f\n",
                   zonas_arreglo[i].nombre,
                   prediccion[i][0], prediccion[i][1],
                   prediccion[i][2], prediccion[i][3]);

        FILE *fr = fopen(ARCHIVO_REPORTE, "w");
        if (fr) {
            fprintf(fr, "Zona,CO2,SO2,NO2,PM2.5\n");
            for (int i = 0; i < ZONAS && zonas_arreglo[i].nombre[0]; ++i)
                fprintf(fr, "%s,%.2f,%.2f,%.2f,%.2f\n",
                        zonas_arreglo[i].nombre,
                        prediccion[i][0], prediccion[i][1],
                        prediccion[i][2], prediccion[i][3]);
            fclose(fr);
        } else {
            perror(ARCHIVO_REPORTE);
        }
    }
}
