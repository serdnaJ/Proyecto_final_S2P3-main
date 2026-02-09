#ifndef FUNCIONES_PRESENTE_H
#define FUNCIONES_PRESENTE_H


#define CONTA 4
#define ZONAS 5
#define HIST_DIAS 30
#define MAX_NOMBRE 20

// ----- Nombres y paths de archivos -----
#define ARCHIVO_ACTUAL      "tabla_actual.csv"      // nombre archivo de datos actuales
#define ARCHIVO_PREDICCION  "tabla_prediccion.csv"  // nombre archivo de datos de predicción
#define PATH_ACTUAL         "tabla_actual.csv"      // Path para datos actuales
#define PATH_PREDICCION     "tabla_prediccion.csv"  // Path para datos de predicción
#define ARCHIVO_HIST        "historico.csv"         // path para datos historicos
#define ARCHIVO_REPORTE     "predicciones.csv"      // path para reporte de predicciones
#define ARCHIVO_RESULTADO_HISTORIA "promedio_historia.csv" // path para resultado de promedios historicos


// desde prediccion.c
typedef struct
{
    float co2, so2, no2, pm25;
    float temp, viento, humedad;
} Registro;

typedef struct
{
    char nombre[MAX_NOMBRE];
    Registro hist[HIST_DIAS]; // Usa HIST_DIAS de complementos.h
    int dias_cargados;
    Registro pred; // resultado de la prediccion
} Zona;

struct PromediosNoPonderados
{
	float pCo2;
	float pSo2;
	float pNo2;
	float pPm25;
};

// ----- Estructuras y arreglos globales -----
struct contaminante {
    char nombre[30];
    int limite;
};

extern const char *recomendaciones[];
extern const char *zonas[];
extern Zona zonas_arreglo[ZONAS];
extern struct PromediosNoPonderados proContNoPond[ZONAS];

// ----- Ingreso de datos -----
void ingresar_datos(float nivel_actual[ZONAS][CONTA], struct contaminante *limites, int *n);

// ----- Validaciones -----
void ingresar_decimal(float *valor); // Solo acepta números >= 0
int validacion_decimales(const float *num_float);

// ----- Tablas y visualización -----
void mostrar_tabla(const float nivel_actual[ZONAS][CONTA], const struct contaminante *limites, int n, const char *titulo);
void mostrar_recomendaciones(const int *elevado, int n);
void mostrar_historico(const char *nombre_archivo);
void mostrar_predicciones(const char *nombre_archivo);

// ----- Comparaciones -----
void comparar_niveles(const float nivel_actual[ZONAS][CONTA], const struct contaminante *limites, int elevado[ZONAS][CONTA], int *n);

// ----- Guardado de datos -----
void guardar_tabla(const float nivel_actual[ZONAS][CONTA], const struct contaminante *limites, int n, const char *nombre_archivo, const char *titulo);

// ----- Utilidades de archivos -----
void vaciar_tabla(const char *nombre_archivo);
void leer_tabla(const char *nombre_archivo);

// ----- Menú principal y submenús -----
void opciones_presente(void);
void menu_datos_actuales(float nivel_actual[ZONAS][CONTA], struct contaminante *limites, int *n, int elevado[ZONAS][CONTA]);
void menu_prediccion(float prediccion[ZONAS][CONTA], struct contaminante *limites, int *n);
void menu_visualizacion(void);
void menu_vaciado(void);

// ----- Predicción -----
void prediccion_general(float prediccion[ZONAS][CONTA], int mostrar);

// ----- Auxiliares -----
int hay_datos_tabla(const float tabla[ZONAS][CONTA], int zonas, int conta);

// ----- Promedio histórico -----
void calculo_promedio_historico(FILE *pResult, struct contaminante *limites);
void ingreso_datos_procesar(FILE *f);
int buscar_o_crear_zona(Zona zonas_arreglo[], const char *nombre);

#endif