/**Clase que contendrá un objeto del tipo mensaje e implementará entre sus métodos los algoritmos 
  *para un correcto manejo de los datos que contiene.
  *
  *Por Alex A. Turriza Suárez   -   2017**/

#ifndef GPS_H_
#define GPS_H_

#include "llh.h" //Objeto mensaje 
#include <unistd.h> //Usado por read()
#include <errno.h> //Usado por errno
#include <cstdio> //Usado por perror
#include <mutex> //Usado por objeto de tipo Mutex

#define _FIXVALUE_ 2.5 //Valor parámetro para indicar si señal del GPS es fiable o no.

int leeSckt(int fdSocket, void * buf, size_t bytes);

class GPS
{
	mensaje msj;
	std::string buf;
	std::mutex Mutex;
   public:
	GPS();

	~GPS();

	/*Conjunto de funciones get*/
	std::string getFecha();
	std::string getHora();
	std::string getLatitud();
	std::string getLongitud();
	std::string getAltitud();
	std::string getNumSat();
	std::string getRatio();
	
	/*Conjunto de funciones set*/
	void setFecha(std::string f);
	void setHora(std::string hs);
	void setLatitud(std::string lat);
	void setLongitud(std::string lon);
	void setAltitud(std::string alt);
	void setSolStat(std::string ss);
	void setNumSat(std::string ns);
	void setstDev(std::string * ST);
	void setAge(std::string ag);
	void setRatio(std::string r);
	void lockMutex();
	void unlockMutex();

	/*Más funciones de interés*/
	int actBuffer(int fd); //Función que actualizará el buffer del objeto GPS.
	int actualizaDatos(int fd); //Función que actualiza el objeto Mensaje. Contiene a actBuffer y actualización del objeto Mensaje en uno solo.
	int fix(); //Indicará gran certeza de datos.
};

#endif //GPS_H_
