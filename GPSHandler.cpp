/**PROGRAMA GPSHANDLER.CPP PARA BeagleBone Black
* Programa que mediante un objeto del tipo GPS que contiene Mutex, un buffer dedicado 
* y un objeto que contiene los datos obtenidos de RTKLIB enviados mediante sockets, 
* puede entregar la información de manera ordenada para su respectivo uso.
*
* Por Alex A. Turriza Suárez - 2017
**/

/*Declaración de Bibliotecas útiles*/
//#include <iostream> //Inicializado en gps.h
#include <sys/socket.h> //Usado por socket()
//#include <unistd.h> //Usado por fork(), ya inicializado en gps.h
//#include <errno.h> //Usado por errno, ya inicializado en gps.h
#include <netinet/in.h> //Declaración de IPPROTO_TCP y de inet_aton()
#include <arpa/inet.h> //Usado por inet_aton()
#include <csignal> //Usado por función signal()
#include <cstdlib> //Usado por función system()
//#include <cstdio> //Usado por perror, ya inicializado en gps.h
#include <thread> //Usado para manejar diferentes hilos
#include <fstream> //Usado para el archivo logGPS.txt
#include "gps.h" //Usado por objeto GPS
#include "lcd.h" //Usado para manejo de objeto de tipo LCD

/*Declaración de constantes*/
#ifndef _PUERTORTKLIB_
#define _PUERTORTKLIB_ 8989
#endif

#ifndef _DIRECHOST_
#define _DIRECHOST_ "127.0.0.1"
#endif

#ifndef _DIRECTRTK_
#define _DIRECTRTK_ "/home/debian/Alex_tesista/RTKLIB-rtklib_2.4.3/app/rtkrcv/gcc"
#endif

#ifndef _DELAYTIME_
#define _DELAYTIME_ 0.7 //Segundos, destinado a funcionar en función de los hertz del gps.
#endif

/*Declaración de punteros globales*/
BlackLib::BlackGPIO * ledForSignalHand; //Puntero global del led para funciones de manejo de señales. 
lcd * lcdForSignalHand; //Puntero globla de lcd para funciones de manejo de señales.
int * fdForSignalHand; //Puntero al descriptor del socket. Usado para cerrar la conexión en el manejador de señales.

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
	/*Función que actualiza el miembro mensaje, del objeto GPS. fd es el descriptor de donde obtendrá datos.*/
	while(true)
	{
		ugps->actualizaDatos(fd);
	}
}

void imprimeGPS(GPS * ugps, lcd * myLcd, std::ofstream * output) 
{
	/*Función que imprime los datos del objeto GPS a la LCD.*/
	std::string temp = "";
	while(true)
	{
		//cout << "Intentaré imprimir" << endl;
		if(ugps->fix())
		{
			myLcd->setColor(3); //GPS azul indica señal fix.
			ugps->lockMutex();
			temp += ugps->getLatitud().substr(0,7);
			temp += ", ";
			temp += ugps->getLongitud().substr(0,7);
			temp += "A:"; 
			temp += ugps->getAltitud().substr(0,4);
			temp += ", Sat: ";
			temp += ugps->getNumSat();
			myLcd->writeMessage(temp);
			#ifdef _SALIDA_
			* output << ugps->getLatitud() << ", " << ugps->getLongitud() << std::endl;
			#endif
			ugps->unlockMutex();
			temp.clear();
			usleep(_DELAYTIME_ * 1000000);
		}
		else
		{
			myLcd->setColor(2); //GPS verde indica señal no-fix.
			temp += "Waiting signal..";
			temp += "Ratio: ";
			temp += ugps->getRatio();
			myLcd->writeMessage(temp);
			temp.clear();
			usleep(_DELAYTIME_ * 1000000);
		}
	}
}

/**FIN DE MANEJO DE INFORMACIÓN GPS MEDIANTE THREADS**/

/**FUNCIONES PARA MANEJO DE PERIFÉRICOS Y SISTEMA DE LA BEAGLEBONE BLACK**/
void finalizacion(int param)
{
	/*Al entrar a esta función, se habrá indicando que el programa debe terminarse*/
	if(close(* fdForSignalHand)) //Cerramos conexión con el socket
	{
		perror("close() socket ERROR");
		exit(-1);
	}
	ledForSignalHand->setValue(low); //Apagamos led
	lcdForSignalHand->setColor(0); //Apagamos lcd
	lcdForSignalHand->displayOn(false, false);
	sleep(3); //Esperamos a salida del programa por apagar();
	exit(0);
}

void apagar()
{
	/*Función que lanza el apagado del sistema*/
	sleep(1); //¿Es necesario este sleep?
	system("shutdown -h now"); //Lanza la orden de apagado.
}

void pbPooling(BlackLib::BlackGPIO * bt1, BlackLib::BlackGPIO * bt2, BlackLib::BlackGPIO * led)
{
	/*Función que evalúa los estados de los pushbuttons anclados a la BeagleBone para determinar si se debe apagar o no el sistema*/
	int button1val, button2val;
	signal(SIGUSR1, finalizacion); //Se usará esta señal de usuario.
	signal(SIGINT, finalizacion); //En caso de Ctrl + C
	while(true)
	{
		button1val = atoi(bt1->getValue().c_str());
		button2val = atoi(bt1->getValue().c_str());
		
		if(!button2val)
		{
			led->setValue(low);
			usleep(300000);
			if(!button1val)
			{
				usleep(300000); //¿Puede ser retirado este usleep?
				std::thread(apagar).detach();
				raise(SIGUSR1);
				break;
			}
		}
		led->setValue(high);
	}
}

/**FIN DE FUNCIONES PARA MANEJO DE PERIFÉRICOS**/

int main()
{
	/**Declaración e inicialización de objetos y variables.**/
	GPS ubloxC94; //Objeto GPS. contiene Mutex, un objeto mensaje que contiene los datos.
	int cPID, fd; //Contendrán el identificador de proceso padre/hijo, y el descriptor de donde se obtendrán los datos de rtklib
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
		/*Inicialización de objetos que NO pueden duplicarse por el fork()*/
		lcd myLcd; //Objeto LCD. contiene métodos para manejar una lcd conectada a pines de la BeagleBone.
		std::ofstream * ptr = NULL;
		BlackLib::BlackGPIO led(BlackLib::GPIO_61, BlackLib::output, BlackLib::SecureMode);
		BlackLib::BlackGPIO button1(BlackLib::GPIO_3, BlackLib::input);
		BlackLib::BlackGPIO button2(BlackLib::GPIO_49, BlackLib::input);
		led.setValue(low);
		myLcd.setColor(1); //Colocamos la LCD roja indicando inicio del programa.
		/**Inicialización de los punteros globales para el manejador de señales**/
		ledForSignalHand = &led; //Puntero global del led para funciones de manejo de señales. 
		lcdForSignalHand = &myLcd; //Puntero globla de lcd para funciones de manejo de señales.
		fdForSignalHand = &fd;
		#ifdef _SALIDA_
		std::ofstream output("logGPS.txt");
		ptr = &output;
		#endif
		sleep(2); //Damos tiempo a que RTKLIB inicie correctamente.
		if((fd = openSocket()) >= 0)
		{
			if(!(conecta(_DIRECHOST_, _PUERTORTKLIB_, fd)))
			{
				//Conexión exitosa.
				led.setValue(high); //Encendemos led indicando conexión exitosa.
				//RUTINA DE MANEJO DE GPS
				std::thread(actualiza, &ubloxC94, fd).detach();
				std::thread(imprimeGPS, &ubloxC94, &myLcd, ptr).detach();
				//RUTINA DE MANEJO DE BOTONES
				std::thread(pbPooling, &button1, &button2, &led).join();
			}
		}
		if(close(fd))
		{
			perror("close() socket ERROR");
			return (-1);
		}
		#ifdef _SALIDA_
		output.close();
		#endif
	}
	return 0;
}
