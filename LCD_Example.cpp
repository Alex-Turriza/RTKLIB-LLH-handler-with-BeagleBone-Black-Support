#include <iostream>
#include "lcd.h"

void Demo(lcd * lq)
{
    lq->writeMessage("Bienvenido al demo de la LCD");
    lq->setColor(1);
    lq->writeMessage("Se muestra cursor y parpadea");
    lq->displayOn(true, true);
    sleep(3);
    lq->writeMessage("Se muestra cursor y no parpadea");
    lq->displayOn(true, false);
    sleep(3);
    lq->writeMessage("No hay cursor");
    lq->displayOn(false, false);
    sleep(3);
    lq->writeMessage("Vamos a escribir una frase...");
    sleep(2);
    lq->writeMessage("Hola mundo!");
    sleep(3);
    cout << "Display se apaga" << endl;
    lq->displayOff();
    sleep(3);
    lq->writeMessage("Display re-encendido");
    lq->displayOn(false, false);
    sleep(3);
    lq->writeMessage("Hola mundo!");
    cout << "Movemos la pantalla" << endl;
    for(int j = 0; j < 5; j++)
    {
        lq->shiftRight();
        sleep(1);
    }
    for(int k = 0; k < 5; k++)
    {
        lq->shiftLeft();
        sleep(1);
    }
    lq->writeMessage("De colores!");
    for(int i = 1; i < 8; i++)
    {
        lq->setColor(i);
        sleep(1);
    }
    lq->writeMessage("Se muestra cursor y parpadea");
    lq->displayOn(true, true);
    sleep(3);
    lq->writeMessage("cursor se mueve a Home");
    lq->cursorHome();
    sleep(3);
    cout << "Limpiamos la pantalla" << endl;
    lq->clearDisplay();
    sleep(3);
    cout << "Fin del demo" << endl;
}

int main()
{
    lcd myLcd;
    Demo(&myLcd);
    myLcd.setColor(1);
    myLcd.writeMessage("Yo no me llamo  Javier!");
    sleep(3);
    myLcd.writeMessage("alex_rt07@hotmail.com");
    sleep(3);
    return 0;
}
