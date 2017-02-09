#include "sdev.h"

int borraString(std::string * str, int valor)
{
	//Función que recibe un puntero a un string. Borra el string desde el principio del mismo hasta un determinado offset, el rango borrado es: [0, valor]. Si posee espacios en blanco al inicio del nuevo string, los elimina.
	if(str->erase(str->begin(), str->begin() + (++valor)) == str->end())
	{
		return -1;
	}
	while(str->at(0) == ' ')
	{
		str->erase(str->begin(), str->begin() + 1);
	}
	return 0;
}

stDev::stDev()
{
	this->north = this->east = this->up = this->northEast = this->eastUp = this->upNorth = "0.0";
}

stDev::~stDev()
{
}

/*FUNCIONES DE TIPO GET*/
std::string stDev::getNorth()
{
	return this->north;
}

std::string stDev::getEast()
{
	return this->east;
}

std::string stDev::getUp()
{
	return this->up;
}

std::string stDev::getNorthEast()
{
	return this->northEast;
}

std::string stDev::getEastUp()
{
	return this->eastUp;
}

std::string stDev::getUpNorth()
{
	return this->upNorth;
}

/*FUNCIONES DE TIPO SET*/
void stDev::setNorth(std::string n)
{
	this->north = n;
}

void stDev::setEast(std::string e)
{
	this->east = e;;
}

void stDev::setUp(std::string u)
{
	this->up = u;
}

void stDev::setNorthEast(std::string ne)
{
	this->northEast = ne;
}

void stDev::setEastUp(std::string eu)
{
	this->eastUp = eu;
}

void stDev::setUpNorth(std::string un)
{
	this->upNorth = un;
}

/*FUNCIÓN QUE ACTUALIZA LAS DESVIACIONES ESTÁNDAR A PARTIR DE UN STRING QUE INICIA
 *CON LA DESVIACIÓN ESTÁNDAR NORTE EN LLH*/

int stDev::actualizaStDev(std::string * dato)
{
	int value;
	int iteracion = 0;
	do
	{
		if((value = dato->find_first_of(" ")) == dato->npos)
		{
			//Hemos alcanzado la última palabra
			//Este caso no debería ocurrir en esta clase, bajo mensaje LLH
			setUpNorth(* dato);
			return -1;
		}
		switch(iteracion++)
		{
			case 0:
				setNorth(dato->substr(0, value));
				if(borraString(dato, value))
					return -1;
				break;
			case 1:
				setEast(dato->substr(0, value));
				if(borraString(dato, value))
					return -1;
				break;
			case 2:
				setUp(dato->substr(0, value));
				if(borraString(dato, value))
					return -1;
				break;
			case 3:
				setNorthEast(dato->substr(0, value));
				if(borraString(dato, value))
					return -1;
				break;
			case 4:
				setEastUp(dato->substr(0, value));
				if(borraString(dato, value))
					return -1;
				break;
			case 5:
				setUpNorth(dato->substr(0, value));
				if(borraString(dato, value))
					return -1;
				return 0;
			default:
				return -1;
		}
	}
	while(true);
}
