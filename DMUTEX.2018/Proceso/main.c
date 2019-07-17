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
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int puerto_udp;

struct Process {
  
  char ident [80];
  int ports;

};

struct Mensg {

  char nameProcess [80];
  int clock [80];
  
};

int main(int argc, char* argv[]) {
  int port, i;
  char line[80], proc[80];
  struct Process nProcess[80];

  if(argc<2){
    fprintf(stderr, "Uso: proceso <ID>\n");
    return 1;
  }

  // Establece el modo buffer de entrada/salida a linea
  setvbuf(stdout, (char*)malloc(sizeof(char)*80),_IOLBF,80);
  setvbuf(stdin, (char*)malloc(sizeof(char)*80),_IOLBF,80);

//puerto_udp=1111; 
  /* Se determina el puerto UDP que corresponda.
                      Dicho puerto debe estar libre y quedará
                      reservado por dicho proceso.*/

  int UDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  // Comprobar si se crea correctamente??

  //fprintf(stdout,"%s: %d\n",argv[1],puerto_udp);
  int processI;
  

  struct sockaddr_in addressServer;
  bzero((char*)&addressServer, sizeof(addressServer));
  addressServer.sin_family = AF_INET;
  addressServer.sin_addr.s_addr = INADDR_ANY;
  addressServer.sin_port = htons(0);
  socklen_t len = sizeof(addressServer);
  bind(UDPSocket, (struct sockaddr *)&addressServer, sizeof(addressServer));
  getsockname(UDPSocket, (struct sockaddr *)&addressServer, &len);
  puerto_udp = ntohs(addressServer.sin_port);
  fprintf(stdout, "%s: %d\n", argv[1], puerto_udp);

  int  nProcs = 0;
  for(; fgets(line,80,stdin); ) {
    if(!strcmp(line,"START\n")){
      break;
    }
    sscanf(line,"%[^:]: %d",proc,&port);
    /* Habra que guardarlo en algun sitio */
    strcpy(nProcess[nProcs].ident, proc);
    nProcess[nProcs].ports = port;

    if(!strcmp(proc,argv[1])){ 
    /* Este proceso soy yo */ 
      processI = nProcs; 
      //posible aqui nProcs++;
    }
    nProcs++;
  }

  char actionProc[80];
  /* Inicializar Reloj */
  int reloj[nProcs];
  i = 0;
  while(i < nProcs){
    reloj[i] = 0;
    i++;
  }

  /* Procesar Acciones */
  
  while (fgets(line, 80, stdin)) {
    sscanf(line, "%s %s", actionProc, proc);

    if(!strcmp(actionProc,"GETCLOCK")){
      fprintf(stdout,"%s: ",nProcess[processI].ident);
      fprintf(stdout,"LC[");
      int j = 0;
      while(j < nProcs-1){
        fprintf(stdout,"%d,", reloj[j]);
        j++;
      }
      fprintf(stdout,"%d]\n", reloj[nProcs-1]);
    }

    if(!strcmp(actionProc, "EVENT")){
      reloj[processI]++;
      fprintf(stdout, "%s: TICK\n", nProcess[processI].ident);  
    }

    if (!strcmp(actionProc, "FINISH")) {
      break;
    }

    struct Mensg mensg;
    struct sockaddr_in UDPDestin;
    int reloj1 [nProcs];
    int k = 0;
    if(!strcmp(actionProc, "RECEIVE")){
      recv(UDPSocket, &mensg, sizeof(struct Mensg), 0);
      int o = 0;
      while(o < nProcs){
        reloj1[o] = mensg.clock[o];
        o++;
      }
      reloj[processI]++;
      while(k < nProcs) {
        if (k != processI) {
          if (reloj1[k] > reloj[k]) {
            reloj[k] = reloj1[k];
          }
        }
        k++;
      }
      fprintf(stdout,"%s: RECEIVE(MSG,%s)|TICK\n",nProcess[processI].ident, mensg.nameProcess);
    }

    if(!strcmp(actionProc, "MESSAGETO")){
      reloj[processI]++;
      fprintf(stdout, "%s: ", nProcess[processI].ident);
      int m = 0;
      int npuerto = port;
    
      while(m < nProcs){
        if(!strcmp(proc, nProcess[m].ident)){
          bzero((char*)&addressServer, sizeof(addressServer));
          addressServer.sin_family = AF_INET;
          addressServer.sin_addr.s_addr = INADDR_ANY;
          npuerto = nProcess[m].ports;
        } 
        m++;
      }

      int t = 0;
      while(t < nProcs){
        mensg.clock[t] = reloj[t];
        t++;
      }
      strcpy(mensg.nameProcess, argv[1]);
      UDPDestin.sin_family = AF_INET;
      UDPDestin.sin_addr.s_addr = INADDR_ANY;
      UDPDestin.sin_port = htons(npuerto);
      socklen_t tam_dir = sizeof(struct sockaddr_in);
      sendto(UDPSocket, &mensg, sizeof(struct Mensg), 0, (struct sockaddr *)&UDPDestin, len);
      fprintf(stdout, "TICK|SEND(MSG,%s)\n", proc);
    }
  }
  return 0;
}