/**
* Clase que contiene de forma desglosada la información obtenida de RTKLIB
* que se obtiene en el formato LLH. 
*
*Por Alex A. Turriza Suárez    -    2017**/

#ifndef LLH_H_
#define LLH_H_

#include <iostream>
#include "sdev.h"

//Para más información consulte Apéndice B página 101 del manual de RTKLIB 2.4.2
class mensaje
{
	std::string fecha; //Indica año, mes y día yyyy/mm/dd.
	std::string horaSat; //Indica hora, minuto y segundos HH:MM:SS.SSS.
	std::string lat; //Latitud.
	std::string lon; //Longitud.
	std::string alt; //Altitud.
	std::string solStat; //Quality flag.1-> Fix || 2 -> Float || 3 -> Reservado || 4 -> DGPS || 5 -> Single. 
	std::string numSat; //Numero de satélites VÁLIDOS para estimar la solución.
	stDev staDev; //Objeto que contiene las desviaciones estándar.
	std::string age; //Diferencia entre relojes de Rover - Base.
	std::string ratio; //Ratio.

     public:
	mensaje();
	~mensaje();
	
	/*FUNCIONES DE TIPO GET*/
	std::string getFecha();
	std::string getHoraSat();
	std::string getLat();
	std::string getLon();
	std::string getAlt();
	std::string getSolStat();
	std::string getNumSat();
	stDev getstDev();
	std::string getAge();
	std::string getRatio();

	/*FUNCIONES DE TIPO SET*/
	void setFecha(std::string f);
	void setHoraSat(std::string hs);
	void setLat(std::string lt);
	void setLon(std::string ln);
	void setAlt(std::string at);
	void setSolStat(std::string ss);
	void setNumSat(std::string ns);
	int setstDev(std::string * stdev);
	void setAge(std::string ag);
	void setRatio(std::string r);

	/*OTRAS FUNCIONES ÚTILES*/
	int actualizaMsj(std::string buffer);
};
#endif /* LLH_H_ */
