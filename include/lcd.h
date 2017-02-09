#ifndef LCD_H_
#define LCD_H_

#include "BlackGPIO.h"
#include <unistd.h>
#include <string>
#define timeSleep 100  //us

using namespace std;
using namespace BlackLib;

class lcd
{
    private:
        BlackGPIO red;
        BlackGPIO green;
        BlackGPIO blue;
        BlackGPIO RS;
        BlackGPIO Enable;
        BlackGPIO DB7;
        BlackGPIO DB6;
        BlackGPIO DB5;
        BlackGPIO DB4;

    public:
        lcd() :
            red(GPIO_66, output, SecureMode),
            green(GPIO_69, output, SecureMode),
            blue(GPIO_45, output, SecureMode),
            RS(GPIO_67, output, SecureMode),
            Enable(GPIO_68, output, SecureMode),
            DB7(GPIO_44, output, SecureMode),
            DB6(GPIO_26, output, SecureMode),
            DB5(GPIO_46, output, SecureMode),
            DB4(GPIO_65, output, SecureMode)
        {
            RS.setValue(low);
            Enable.setValue(high);
            red.setValue(high);
            green.setValue(high);
            blue.setValue(high);
            reset();
        }

        ~lcd()
        {

        }

        bool setColor(unsigned int c)
        {
            switch(c)
            {
                case 0:
                    red.setValue(high);
                    green.setValue(high);  // Apagado
                    blue.setValue(high);
                    return true;
                case 1:
                    red.setValue(low);
                    green.setValue(high);  // Rojo
                    blue.setValue(high);
                    return true;
                case 2:
                    red.setValue(high);
                    green.setValue(low);  // Verde
                    blue.setValue(high);
                    return true;
                case 3:
                    red.setValue(high);
                    green.setValue(high);  // Azul
                    blue.setValue(low);
                    return true;
                case 4:
                    red.setValue(low);
                    green.setValue(low);   // Amarillo
                    blue.setValue(high);
                    return true;
                case 5:
                    red.setValue(high);
                    green.setValue(low);   // Cian
                    blue.setValue(low);
                    return true;
                case 6:
                    red.setValue(low);
                    green.setValue(high);   // Magenta
                    blue.setValue(low);
                    return true;
                case 7:
                    red.setValue(low);
                    green.setValue(low);   // Blanco
                    blue.setValue(low);
                    return true;
                default:
                    return false;
            }
        }

        void writeData(digitalValue rs, digitalValue db7, digitalValue db6, digitalValue db5, digitalValue db4)
        {
            RS.setValue(rs);
            DB7.setValue(db7);
            DB6.setValue(db6);
            DB5.setValue(db5);
            DB4.setValue(db4);
        }

        void signalEnable()
        {
            Enable.setValue(low);
            usleep(100); // 100 us
            Enable.setValue(high);  // Rising edge
        }

        void sendLCD(digitalValue rs, digitalValue db7, digitalValue db6, digitalValue db5, digitalValue db4, int sleepTime)
        {
            writeData(rs, db7, db6, db5, db4);
            signalEnable();
            usleep(sleepTime);
        }

        void reset()
        {
            /** Rutina que indicará al micro HD44780 que se desea un reinicio **/
            usleep(150000); //Wait for more than 100 ms
            sendLCD(low, low, low, high, high, 8000);
            sendLCD(low, low, low, high, high, 500);
            sendLCD(low, low, low, high, high, 500);
            /*En este punto, el micro HD44780 sabe que está en un proceso de reinicio
             *Se envían los comandos de configuración*/
            sendLCD(low, low, low, high, low, 500);
            sendLCD(low, low, low, high, low, 1); // 2 líneas, arreglo de 5x8 puntos
            sendLCD(low, high, low, high, low, 100); // 2 líneas, arreglo de 5x8 puntos
            sendLCD(low, low, low, low, low, 1); // Display off
            sendLCD(low, high, low, low, low, 100); // Display off
            sendLCD(low, low, low, low, low, 1); // Clear display
            sendLCD(low, low, low, low, high, 10000); // Clear display
            sendLCD(low, low, low, low, low, 1);
            sendLCD(low, low, high, high, low, 100); // Incrementa, no shift cursor
            sendLCD(low, low, low, low, low, 1);
            sendLCD(low, high, high, low, low, 100); // Cursor no se muestra, no parpadea.
        }

        void writeMessage(string s)
        {
            int c = 0;
            clearDisplay();
            while(!s.empty())
            {
                digitalValue db7, db6, db5, db4, db3, db2, db1, db0;
                db7 = db6 = db5 = db4 = db3 = db2 = db1 = db0 = low;
                if(s[0] & 128)
                    db7 = high;
                if(s[0] & 64)
                    db6 = high;
                if(s[0] & 32)
                    db5 = high;
                if(s[0] & 16)
                    db4 = high;
                if(s[0] & 8)
                    db3 = high;
                if(s[0] & 4)
                    db2 = high;
                if(s[0] & 2)
                    db1 = high;
                if(s[0] & 1)
                    db0 = high;
                s.erase(0,1);
                sendLCD(high, db7, db6, db5, db4, timeSleep);
                sendLCD(high, db3, db2, db1, db0, timeSleep);
                c++;
                if(c==16)
                    lineJump();
            }
        }

        void displayOn(bool cursor, bool blink)
        {
            sendLCD(low, low, low, low, low, timeSleep); //Display ON
            if(cursor && blink)
                sendLCD(low, high, high, high, high, timeSleep);
            else if(cursor && !blink)
                sendLCD(low, high, high, high, low, timeSleep);
            else
                sendLCD(low, high, high, low, low, timeSleep);
            usleep(100); //Tiempo de ejecución: 40 us
        }

        void displayOff()
        {
            sendLCD(low, low, low, low, low, timeSleep);
            sendLCD(low, high, low, low, low, timeSleep);
            usleep(100); // Tiempo de ejecución: 40 us
        }

        void cursorHome()
        {
            sendLCD(low, low, low, low, low, timeSleep);
            sendLCD(low, low, low, high, low, timeSleep);
            usleep(2000); // Tiempo de ejecución: 1.64 ms
        }

        void clearDisplay()
        {
            sendLCD(low, low, low, low, low, timeSleep);
            sendLCD(low, low, low, low, high, timeSleep);
            usleep(2000); //Tiempo de ejecución: 1.64 ms
            cursorHome();
        }

        void lineJump()
        {
            sendLCD(low, high, high, low, low, 100); //Coloca dirección de
            sendLCD(low, low, low, low, low, 100); //cursor en segunda línea.
        }

        void shiftLeft()
        {
            sendLCD(low, low, low, low, high, timeSleep); //Shift display selector
            sendLCD(low, high, low, low, low, timeSleep); //Entire display, left, DC, DC
        }

        void shiftRight()
        {
            sendLCD(low, low, low, low, high, timeSleep); //Shift display selector
            sendLCD(low, high, high, low, low, timeSleep); //Entire display, right, DC, DC
        }
};

#endif /* LCD_H_ */
