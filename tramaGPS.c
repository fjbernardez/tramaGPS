/*
Tratamiento de trama de información recibida por GPS_NMEA.
Validacion e interpretacion.

autor: Francisco Bernárdez
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******************** prototipos ********************/
char getCheckSum (void);
int validaCheckSum (char checkSumEsperado, char CheckSumCalculado);
void parceaTramaAscii(void);
void showData (void);
/***************************************************/

/******************** Trama en memoria para el proceso de la informacion ********************/
/* checkSum = 0x77 calculado en: https://www.scadacore.com/tools/programming-calculators/online-checksum-calculator */
char GPS_NMEA[] = {'$','G','P','G','G','A',',','1','2','3','5','1','9',',','4','8','0','7','.','0','3','8',',','N',',','0','1','1','3','1','.','0','0','0',',','E',',','1',0x77};
/********************************************************************************************/

/**************** Globales ****************/
typedef struct {

	// buffer para recepcion
	unsigned char buffer[39];
  // buffer horario
  unsigned char horario [7];
	//buffer Latitud
	float latitud;
  unsigned char bufferlatitud[11];
	//buffer Longitud
  float longitud;
  unsigned char bufferlongitud[11];

} TRAMA_GPS;

TRAMA_GPS tramaGPS;

int flagCheckSumValidado = 0;
/****************************************/

/*********** App principal ***********/
int  main() {

  int i;
  int checkSum = 0;

  printf("Array en memoria:\n");
  for (i = 0; i<39 ; i++) {
    tramaGPS.buffer[i] = GPS_NMEA[i];
    printf("%c",tramaGPS.buffer[i]);
  }

  printf("\ncheckSumCalculado = %d\n", getCheckSum() );

  if ( validaCheckSum (tramaGPS.buffer[39],checkSum) ) {
    flagCheckSumValidado=1;
    printf("Trama validada\n\n");
  }
  else {
    printf("Trama no valida\n\n");
  }

/*parcear la informacion*/
if (!flagCheckSumValidado) {
  printf("No se parcea trama recibida\n");
  return -1;
}

parceaTramaAscii();
showData ();

  return 0;
}
/**********************************/


/**************************** Funciones *****************************/
char getCheckSum (void) {

  char auxCheckSum = 0;
  int i;

  for (i = 0; i<38 ; i++) {
    auxCheckSum ^= tramaGPS.buffer[i];
  }

  return auxCheckSum;
}

int validaCheckSum (char checkSumEsperado, char CheckSumCalculado){

  int retorno = 0;

  if (checkSumEsperado == CheckSumCalculado) {
    retorno = 1;
  }

  return retorno;

}

void parceaTramaAscii(void) {

  char bufferlatitud[10];
  char bufferlongitud[10];

  memcpy( (void*)tramaGPS.horario, (void*)&tramaGPS.buffer[7],6);
  tramaGPS.horario[7]='\0';

  memcpy( (void*)tramaGPS.bufferlatitud, (void*)&tramaGPS.buffer[14],10);
  tramaGPS.bufferlatitud[11]='\0';

  memcpy( (void*)tramaGPS.bufferlongitud, (void*)&tramaGPS.buffer[26],10);
  tramaGPS.bufferlongitud[11]='\0';

  //preparo buffer para convertir a float
  memcpy( (void*)bufferlatitud,(void*)&tramaGPS.bufferlatitud, strlen((char*)tramaGPS.bufferlatitud)-2 );
  bufferlatitud[ strlen((char*)tramaGPS.bufferlatitud)-6 ]= ',';
  bufferlatitud[ strlen((char*)tramaGPS.bufferlatitud)-1 ]= '\0';
  //preparo buffer para convertir a float
  memcpy( (void*)bufferlongitud,(void*)&tramaGPS.bufferlongitud, strlen((char*)tramaGPS.bufferlongitud)-2 );
  bufferlongitud[ strlen((char*)tramaGPS.bufferlongitud)-6 ]= ',';
  bufferlongitud[ strlen((char*)tramaGPS.bufferlongitud)-1 ]= '\0';

  //conversion ascii to float
  tramaGPS.latitud = atof(bufferlatitud);
  tramaGPS.longitud = atof(bufferlongitud);
}

void showData (void) {
  printf("Informacion recibida: \n\n");
  printf("HORARIO:\t %s\n",tramaGPS.horario);
  printf("LATITUD:\t %.03f\n",tramaGPS.latitud );
  printf("LONGITUD:\t %.03f\n",tramaGPS.longitud);

}

/********************************************************************/
