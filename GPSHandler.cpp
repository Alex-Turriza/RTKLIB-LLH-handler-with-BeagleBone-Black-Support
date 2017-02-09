/**PROGRAMA GPSHANDLER.CPP PARA BeagleBone Black
* Programa que mediante un objeto del tipo GPS que contiene Mutex, un buffer dedicado 
* y un objeto que contiene los datos obtenidos de RTKLIB enviados mediante sockets, 
* puede entregar la información de manera ordenada para su respectivo uso.
*
* Por Alex A. Turriza Suárez - 2017
**/

//#include <iostream> //Inicializado en gps.h
#include <sys/socket.h> //Usado por socket()
//#include <unistd.h> //Usado por fork(), ya inicializado en gps.h
//#include <errno.h> //Usado por errno, ya inicializado en gps.h
#include <netinet/in.h> //Declaración de IPPROTO_TCP y de inet_aton()
#include <arpa/inet.h> //Usado por inet_aton()
//#include <cstdio> //Usado por perror, ya inicializado en gps.h
#include <thread> //Usado para manejar diferentes hilos
#include "gps.h" //Usado por objeto GPS
#include "lcd.h" //Usado para manejo de objeto de tipo LCD

#ifndef _PUERTORTKLIB_
#define _PUERTORTKLIB_ 8989
#endif

#ifndef _DIRECHOST_
#define _DIRECHOST_ "127.0.0.1"
#endif

#ifndef _DIRECTRTK_
#define _DIRECTRTK_ "/home/alexrt07/RTKLIB-rtklib_2.4.3/app/rtkrcv/gcc"
#endif

#ifndef _DELAYTIME_
#define _DELAYTIME_ 0.7 //Segundos, destinado a funcionar con los hertz de un gps.
#endif

/**CONJUNTO DE FUNCIONES DEDICADAS AL MANEJO DEL SOCKET QUE CONECTARÁ A RTKLIB**/
int openSocket()
{
	/*Función que crea un socket y devuelve el descriptor*/
	int filedes;
	if((filedes = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		perror("socket() ERROR");
		return -1;
	}
	return filedes;
}

int conecta(char * Direccion, int puerto, int sockfd)
{
	/*Función que conecta un socket a una dirección dada y un determinado puerto*/
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(puerto);
	if(!(inet_aton(Direccion, (struct in_addr *) &addr.sin_addr.s_addr)))
	{
		std::cout << "inet_aton() ERROR: Dirección no válida" << std::endl;
		return (-1);
	}
	if(connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
	{
		perror("connect() ERROR");
		return (-1);
	}
	return 0;
}
/**FIN DE CONJUNTO DE FUNCIONES PARA MANEJO DE SOCKET**/

/**FUNCIONES PARA MANEJO DE INFORMACIÓN DEL GPS MEDIANTE THREADS**/
void actualiza(GPS * ugps, int fd)
{
	while(true)
	{
		ugps->actualizaDatos(fd);
	}
}

void imprimeGPS(GPS * ugps, lcd * myLcd)
{
	std::string temp = "";
	while(true)
	{
		if(ugps->fix())
		{
			myLcd->setColor(3); //GPS azul indica señal fix.
			ugps->lockMutex();
			temp += "Lat:"; 
			temp += ugps->getLatitud().substr(0,7);
			temp += "   Lon: ";
			temp += ugps->getLongitud().substr(0,7);
			temp += " Alt: "; 
			temp += ugps->getAltitud().substr(0,4);
			temp += "Sat: ";
			temp += ugps->getNumSat();
			myLcd->writeMessage(temp);
			ugps->unlockMutex();
			temp.clear();
			usleep(_DELAYTIME_ * 1000000);
		}
		else
		{
			myLcd->setColor(1); //GPS verde indica señal no-fix.
			temp += "Esperando señal ";
			temp += "Ratio: ";
			temp += ugps->getRatio();
			myLcd->writeMessage(temp);
			temp.clear();
			usleep(_DELAYTIME_ * 1000000);
		}
	}
}

int main()
{
	GPS ubloxC94;
	int cPID, fd;
	lcd myLcd;
	myLcd.setColor(1); //Colocamos la LCD roja indicando inicio del programa.
	if(!(cPID = fork()))
	{
		//Inicia rutina del programa hijo. Aquí se invoca a RTKLIB-RTKRCV.
		if(chdir(_DIRECTRTK_))
		{
			perror("chdir() ERROR:");
			return (-1);
		}
		else
		{
			//Ejecutar RTKLIB rtkrcv
			//if(execl("rtkrcv", "rtkrcv", "-s", "-o", "uuuu.conf", NULL))
			//{
			//	perror("Execl() ERROR:");
			//	return (-1);
			//}
		}
	}
	else
	{
		sleep(2); //Damos tiempo a que RTKLIB inicie correctamente.
		if((fd = openSocket()) >= 0)
		{
			if(!(conecta(_DIRECHOST_, _PUERTORTKLIB_, fd)))
			{
				//Conexión exitosa.
				myLcd.setColor(2); //Colocamos la LCD verde indicando conexión a socket exitosa.
				myLcd.writeMessage("Conexion exitosa");
				sleep(1);

				//RUTINA DE MANEJO DE GPS
				std::thread(actualiza, &ubloxC94, fd).detach();
				std::thread(imprimeGPS, &ubloxC94, &myLcd).join();
			}
		}
		if(close(fd))
		{
			perror("close() socket ERROR");
			return (-1);
		}
	}
	return 0;
}
