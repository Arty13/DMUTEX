/* DMUTEX (2009) Sistemas Operativos Distribuidos
 * Código de Apoyo
 *
 * ESTE CÓDIGO DEBE COMPLETARLO EL ALUMNO:
 *    - Para desarrollar las funciones de mensajes, reloj y
 *      gestión del bucle de tareas se recomienda la implementación
 *      de las mismas en diferentes ficheros.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int puerto_udp;

int main(int argc, char* argv[])
{
  int port;
  char line[80],proc[80];

  if(argc<2)
  {
    fprintf(stderr,"Uso: proceso <ID>\n");
    return 1;
  }

  /* Establece el modo buffer de entrada/salida a línea */
  setvbuf(stdout,(char*)malloc(sizeof(char)*80),_IOLBF,80);
  setvbuf(stdin,(char*)malloc(sizeof(char)*80),_IOLBF,80);

  puerto_udp=1111; /* Se determina el puerto UDP que corresponda.
                      Dicho puerto debe estar libre y quedará
                      reservado por dicho proceso. */

  fprintf(stdout,"%s: %d\n",argv[1],puerto_udp);

  for(;fgets(line,80,stdin);)
  {
    if(!strcmp(line,"START\n"))
      break;

    sscanf(line,"%[^:]: %d",proc,&port);
    /* Habra que guardarlo en algun sitio */

    if(!strcmp(proc,argv[1]))
    { /* Este proceso soy yo */ }
  }

  /* Inicializar Reloj */

  /* Procesar Acciones */

  return 0;
}
