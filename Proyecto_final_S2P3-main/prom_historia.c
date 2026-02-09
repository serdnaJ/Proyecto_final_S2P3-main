#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "complementos.h"

void ingreso_datos_procesar(FILE *f)
{
    int num = 0;
    int count = 0;
    char linea[256];
    fgets(linea, sizeof linea, f); // descarta cabecera
    while (fgets(linea, sizeof linea, f))
    {
        char zona_nom[20];
        Registro r;
        if (sscanf(linea, "%19[^,],%f,%f,%f,%f,%f,%f,%f",
                   zona_nom, &r.co2, &r.so2, &r.no2, &r.pm25,
                   &r.temp, &r.viento, &r.humedad) != 8)
            continue;

        int idx = buscar_o_crear_zona(zonas_arreglo, zona_nom);

        // más de 5 zonas_arreglo se ignora
        if (idx == -1)
            continue;

        // desplaza histórico y guarda día reciente, desplazando los más antiguos a la derecha
        for (int k = HIST_DIAS - 1; k > 0; k--)
            zonas_arreglo[idx].hist[k] = zonas_arreglo[idx].hist[k - 1];
        zonas_arreglo[idx].hist[0] = r;

        if (zonas_arreglo[idx].dias_cargados < HIST_DIAS)
            zonas_arreglo[idx].dias_cargados++;
    }
}

// Guardar en archivo
void calculo_promedio_historico(FILE *pResult, struct contaminante *limites)
{
    for (int i = 0; i < ZONAS; i++)
    {
        proContNoPond[i].pCo2 = 0.0;
        proContNoPond[i].pSo2 = 0.0;
        proContNoPond[i].pNo2 = 0.0;
        proContNoPond[i].pPm25 = 0.0;

        for (int j = 0; j < HIST_DIAS; j++)
        {
            proContNoPond[i].pCo2 += zonas_arreglo[i].hist[j].co2;
            proContNoPond[i].pSo2 += zonas_arreglo[i].hist[j].so2;
            proContNoPond[i].pNo2 += zonas_arreglo[i].hist[j].no2;
            proContNoPond[i].pPm25 += zonas_arreglo[i].hist[j].pm25;
        }
        proContNoPond[i].pCo2 /= HIST_DIAS;
        proContNoPond[i].pSo2 /= HIST_DIAS;
        proContNoPond[i].pNo2 /= HIST_DIAS;
        proContNoPond[i].pPm25 /= HIST_DIAS;

        fprintf(pResult, "Zona: %s\n", zonas_arreglo[i].nombre);
        fprintf(pResult, "CO2   promedio: %.2f ppm\n", proContNoPond[i].pCo2);
        if (proContNoPond[i].pCo2 < limites[0].limite)
        {
            fprintf(pResult, "Estado: Normal\n");
        }
        else if (proContNoPond[i].pCo2 == limites[0].limite)
        {
            fprintf(pResult, "Estado: Se encuentra en el limite\n");
        }
        else
        {
            fprintf(pResult, "Estado: Peligroso\n");
        }

        fprintf(pResult, "SO2   promedio: %.2f ug/m3\n", proContNoPond[i].pSo2);
        if (proContNoPond[i].pSo2 < limites[1].limite)
        {
            fprintf(pResult, "Estado: Normal\n");
        }
        else if (proContNoPond[i].pSo2 == limites[1].limite)
        {
            fprintf(pResult, "Estado: Se encuentra en el limite\n");
        }
        else
        {
            fprintf(pResult, "Estado: Peligroso\n");
        }

        fprintf(pResult, "NO2   promedio: %.2f ug/m3\n", proContNoPond[i].pNo2);
        if (proContNoPond[i].pNo2 < limites[2].limite)
        {
            fprintf(pResult, "Estado: Normal\n");
        }
        else if (proContNoPond[i].pNo2 == limites[2].limite)
        {
            fprintf(pResult, "Estado: Se encuentra en el limite\n");
        }
        else
        {
            fprintf(pResult, "Estado: Peligroso\n");
        }

        fprintf(pResult, "PM2.5 promedio: %.2f ug/m3\n", proContNoPond[i].pPm25);
        if (proContNoPond[i].pPm25 < limites[3].limite)
        {
            fprintf(pResult, "Estado: Normal\n");
        }
        else if (proContNoPond[i].pPm25 == limites[3].limite)
        {
            fprintf(pResult, "Estado: Se encuentra en el limite\n");
        }
        else
        {
            fprintf(pResult, "Estado: Peligroso\n");
        }

        fprintf(pResult, "---------------------------------\n");

        // Mostrar en terminal
        printf("Zona: %s\n", zonas_arreglo[i].nombre);
        printf("CO2   promedio: %.2f ppm\n", proContNoPond[i].pCo2);
        if (proContNoPond[i].pCo2 < limites[0].limite)
        {
            printf("Estado: Normal\n");
        }
        else if (proContNoPond[i].pCo2 == limites[0].limite)
        {
            printf("Estado: Se encuentra en el limite\n");
        }
        else
        {
            printf("Estado: Peligroso\n");
        }

        printf("SO2   promedio: %.2f ug/m3\n", proContNoPond[i].pSo2);
        if (proContNoPond[i].pSo2 < limites[1].limite)
        {
            printf("Estado: Normal\n");
        }
        else if (proContNoPond[i].pSo2 == limites[1].limite)
        {
            printf("Estado: Se encuentra en el limite\n");
        }
        else
        {
            printf("Estado: Peligroso\n");
        }

        printf("NO2   promedio: %.2f ug/m3\n", proContNoPond[i].pNo2);
        if (proContNoPond[i].pNo2 < limites[2].limite)
        {
            printf("Estado: Normal\n");
        }
        else if (proContNoPond[i].pNo2 == limites[2].limite)
        {
            printf("Estado: Se encuentra en el limite\n");
        }
        else
        {
            printf("Estado: Peligroso\n");
        }

        printf("PM2.5 promedio: %.2f ug/m3\n", proContNoPond[i].pPm25);
        if (proContNoPond[i].pPm25 < limites[3].limite)
        {
            printf("Estado: Normal\n");
        }
        else if (proContNoPond[i].pPm25 == limites[3].limite)
        {
            printf("Estado: Se encuentra en el limite\n");
        }
        else
        {
            printf("Estado: Peligroso\n");
        }

        printf("---------------------------------\n");
    }
}