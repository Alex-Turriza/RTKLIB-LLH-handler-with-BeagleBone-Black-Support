#include "gps.h"

int leeSckt(int fdSocket, void * buf, size_t bytes)
{
	int nbytes;
	if( (nbytes = read(fdSocket, buf, bytes)) < 0)
	{
		perror("read() ERROR:");
		return (-1);
	}
	return nbytes;
}

GPS::GPS()
{
	this->buf = "";
}

GPS::~GPS()
{
}

/*Conjunto de funciones get*/
std::string GPS::getFecha()
{
	return this->msj.getFecha();
}

std::string GPS::getHora()
{
	return this->msj.getHoraSat();
}

std::string GPS::getLatitud()
{
	return this->msj.getLat();
}

std::string GPS::getLongitud()
{
	return this->msj.getLon();
}

std::string GPS::getAltitud()
{
	return this->msj.getAlt();
}

std::string GPS::getNumSat()
{
	return this->msj.getNumSat();
}

std::string GPS::getRatio()
{
	return this->msj.getRatio();
}
	
/*Conjunto de funciones set*/
void GPS::setFecha(std::string f)
{
	this->msj.setFecha(f);
}

void GPS::setHora(std::string hs)
{
	this->msj.setHoraSat(hs);
}

void GPS::setLatitud(std::string lat)
{
	this->msj.setLat(lat);
}

void GPS::setLongitud(std::string lon)
{
	this->msj.setLon(lon);
}

void GPS::setAltitud(std::string alt)
{
	this->msj.setAlt(alt);
}

void GPS::setSolStat(std::string ss)
{
	this->msj.setSolStat(ss);
}

void GPS::setNumSat(std::string ns)
{
	this->msj.setNumSat(ns);
}

void GPS::setstDev(std::string * ST)
{
	this->msj.setstDev(ST);
}

void GPS::setAge(std::string ag)
{
	this->msj.setAge(ag);
}

void GPS::setRatio(std::string r)
{
	this->msj.setRatio(r);
}

void GPS::lockMutex()
{
	this->Mutex.lock();
}

void GPS::unlockMutex()
{
	this->Mutex.unlock();
}

/*Más funciones de interés*/
int GPS::actBuffer(int fd) //Función que actualizará el buffer del objeto GPS
{
	//this->buf = ""; //Descomentar sólo en caso de emergencia XD		
	//Recibimos el file descriptor fd del socket conectado a RTKLIB
	char buffer;
	while(true)
	{
		if(leeSckt(fd, &buffer, sizeof(buffer)) == sizeof(buffer))
		{
			if(buffer == '\n') //'\n' indica el final del mensaje para sistemas UNIX (LF 0x0A).
			{
				return 0;
			}
			else
				buf += buffer;
		}
	}
}

int GPS::actualizaDatos(int fd)
{
	this->actBuffer(fd);
	this->Mutex.lock();
	if(this->msj.actualizaMsj(this->buf))
	{
		this->Mutex.unlock();
		return -1; //Ha ocurrido un error o algo inesperado.
	}
	this->Mutex.unlock();
	buf = "";
	return 0;
}

int GPS::fix()
{
	//Regresa 1 si ratio > 2.5, regresa 0 en otro caso.
	return (stof(this->msj.getRatio()) > _FIXVALUE_);
}
