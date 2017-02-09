#include<iostream>
#include"BlackGPIO.h"
#include <unistd.h>

using namespace BlackLib;
using namespace std;

void writeLCD(digitalValue rs, digitalValue db7, digitalValue db6, digitalValue db5, digitalValue db4,
              BlackGPIO RS, BlackGPIO DB7, BlackGPIO DB6, BlackGPIO DB5, BlackGPIO DB4)
{
     RS.setValue(rs);
     DB7.setValue(db7);
     DB6.setValue(db6);
     DB5.setValue(db5);
     DB4.setValue(db4);
}

void go(BlackGPIO E)
{
    E.setValue(low);
    usleep(30000); //30 ms
    E.setValue(high);
}

int main()
{
    BlackGPIO red(GPIO_66, output, SecureMode);
    BlackGPIO green(GPIO_69, output, SecureMode);
    BlackGPIO blue(GPIO_45, output, SecureMode);
    BlackGPIO RS(GPIO_67, output, SecureMode);
    BlackGPIO Enable(GPIO_68, output, SecureMode);
    BlackGPIO DB7(GPIO_44, output, SecureMode);
    BlackGPIO DB6(GPIO_26, output, SecureMode);
    BlackGPIO DB5(GPIO_46, output, SecureMode);
    BlackGPIO DB4(GPIO_65, output, SecureMode);


    red.setValue(low);
    blue.setValue(high);  // Rojo
    green.setValue(high);

    cout << "Color colocado" << endl;

    /** Inicialización de valores **/
    RS.setValue(low);
    Enable.setValue(high);

    cout << "Valores inicializados" << endl;
    cout << "Rutina de inicializacion..." << endl;

    /** Rutina de inicialización de la LCD **/
    usleep(150000); //Wait for more than 100 ms
    cout << "Sending first vector of data..." << endl;

    //writeLCD(low, low, low, high, high, RS, DB7, DB6, DB5, DB4); // 0011
    RS.setValue(low);
    DB7.setValue(low);
    DB6.setValue(low);
    DB5.setValue(high);
    DB4.setValue(high);

    cout << "Sended... Telling the uC that the data is there" << endl;

    //    go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    cout << "First vector done... waiting" << endl;
    usleep(8000); //Wait for more than 4.1 ms
    cout << "Sending second vector of data..." << endl;

    //writeLCD(low, low, low, high, high, RS, DB7, DB6, DB5, DB4); // 0011
    RS.setValue(low);
    DB7.setValue(low);
    DB6.setValue(low);
    DB5.setValue(high);
    DB4.setValue(high);

    cout << "Sended... Telling the uC that the data is there" << endl;

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    cout << "Second vector done... waiting" << endl;
    usleep(500); //Wait for more than 100 us

    //writeLCD(low, low, low, high, high, RS, DB7, DB6, DB5, DB4); // 0011
    RS.setValue(low);
    DB7.setValue(low);
    DB6.setValue(low);
    DB5.setValue(high);
    DB4.setValue(high);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    usleep(500); //Wait for more than 100 us

    //writeLCD(low, low, low, high, low, RS, DB7, DB6, DB5, DB4); // 0010
    RS.setValue(low);
    DB7.setValue(low);
    DB6.setValue(low);
    DB5.setValue(high);
    DB4.setValue(low);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    usleep(500); //Wait for more than 100 us

    //writeLCD(low, low, low, high, low, RS, DB7, DB6, DB5, DB4); // 0010
    RS.setValue(low);
    DB7.setValue(low);
    DB6.setValue(low);
    DB5.setValue(high);
    DB4.setValue(low);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    //writeLCD(low, high, low, high, low, RS, DB7, DB6, DB5, DB4); // 0100 - 2 líneas, 5x8 puntos
    RS.setValue(low);
    DB7.setValue(high);
    DB6.setValue(low);
    DB5.setValue(high);
    DB4.setValue(low);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    usleep(100); //Wait for more than 53 us

    //writeLCD(low, low, low, low, low, RS, DB7, DB6, DB5, DB4); // 0000
    RS.setValue(low);
    DB7.setValue(low);
    DB6.setValue(low);
    DB5.setValue(low);
    DB4.setValue(low);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    //writeLCD(low, high, low, low, low, RS, DB7, DB6, DB5, DB4); // 1000
    RS.setValue(low);
    DB7.setValue(high);
    DB6.setValue(low);
    DB5.setValue(low);
    DB4.setValue(low);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    usleep(100); //Wait for more than 53 us

    //writeLCD(low, low, low, low, low, RS, DB7, DB6, DB5, DB4); // 0000
    RS.setValue(low);
    DB7.setValue(low);
    DB6.setValue(low);
    DB5.setValue(low);
    DB4.setValue(low);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    //writeLCD(low, low, low, low, high, RS, DB7, DB6, DB5, DB4); // 0001
    RS.setValue(low);
    DB7.setValue(low);
    DB6.setValue(low);
    DB5.setValue(low);
    DB4.setValue(high);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    usleep(10000); //Wait for more than 3 ms

    //writeLCD(low, low, low, low, low, RS, DB7, DB6, DB5, DB4); //
    RS.setValue(low);
    DB7.setValue(low);
    DB6.setValue(low);
    DB5.setValue(low);
    DB4.setValue(low);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    //writeLCD(low, low, high, high, high, RS, DB7, DB6, DB5, DB4); // 0111 - Incrementa, Shift cursor
    RS.setValue(low);
    DB7.setValue(low);
    DB6.setValue(high);
    DB5.setValue(high);
    DB4.setValue(high);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    usleep(100); // Wait for more than 53 us

    //writeLCD(low, low, low, low, low, RS, DB7, DB6, DB5, DB4); // 0000
    RS.setValue(low);
    DB7.setValue(low);
    DB6.setValue(low);
    DB5.setValue(low);
    DB4.setValue(low);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);

    //writeLCD(low, high, high, high, high, RS, DB7, DB6, DB5, DB4); // 1111 - Cursor se muestra, Parpadea.
    RS.setValue(low);
    DB7.setValue(high);
    DB6.setValue(high);
    DB5.setValue(high);
    DB4.setValue(high);

    //go(Enable);
    Enable.setValue(low);
    usleep(30000); //30 ms
    Enable.setValue(high);
    cout << "LCD inicializada" << endl;

    usleep(100); //Wait for more than 53 us
    sleep(15);
    return 0;
}
