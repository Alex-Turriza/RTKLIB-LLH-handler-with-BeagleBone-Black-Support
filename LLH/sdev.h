/**
* Clase que contiene de forma desglosada las desviaciones estándar y componentes
* de la matriz de covarianza del sistema proporcionado por RTKLIB.
*
*
*Por Alex A. Turriza Suárez    -    2017**/

#ifndef SDEV_H_
#define SDEV_H_

#include <string>

int borraString(std::string * str, int valor);

/*Más información acerca de los miembros de la clase en Apéndice B página 102 Manual de RTKLIB 2.4.2*/
class stDev
{
	std::string north; //componente Norte (N) de la desviación estándar (m).
	std::string east; //componente Este (E) de la desviación estándar (m).
	std::string up; //componente Arriba (U) de la desviación estándar (m).
	std::string northEast; //raíz cuadrada del valor absoluto del componente NE de la matriz de covarianza estimada.
	std::string eastUp; //raíz cuadrada del valor absoluto del componente EU de la matriz de covarianza estimada.
	std::string upNorth; //raíz cuadrada del valor absoluto del componente UN de la matriz de covarianza estimada.
	
    public:
	stDev();
	~stDev();

	/*FUNCIONES DE TIPO GET*/
	std::string getNorth();
	std::string getEast();
	std::string getUp();
	std::string getNorthEast();
	std::string getEastUp();
	std::string getUpNorth();

	/*FUNCIONES DE TIPO SET*/
	void setNorth(std::string n);
	void setEast(std::string e);
	void setUp(std::string u);
	void setNorthEast(std::string ne);
	void setEastUp(std::string eu);
	void setUpNorth(std::string un);

	/*FUNCIÓN QUE ACTUALIZA LAS DESVIACIONES ESTÁNDAR A PARTIR DE UN STRING QUE INICIA
	 *CON LA DESVIACIÓN ESTÁNDAR NORTH EN LLH Y FINALIZA CON LA COMPONENTE UPNORTH DE LA 
	MATRIZ DE COVARIANZA*/
	int actualizaStDev(std::string * dato);
};
#endif /*SDEV_H_*/
