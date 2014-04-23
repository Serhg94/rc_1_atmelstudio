//-------------------------------------------------------------------
#ifndef __rc_1_main_H__
#define __rc_1_main_H__
//-------------------------------------------------------------------
 #include "conf.h"
#include "CyberLib.h"
#include <avr/wdt.h>
#include "times.h"
//
//
#define LIFETM_LEN 60000
#define SEEK_BUTT_DEVISOR 2
 


#define DHT11LIB_VERSION "0.4.1"

#define DHTLIB_OK                0
#define DHTLIB_ERROR_CHECKSUM   -1
#define DHTLIB_ERROR_TIMEOUT    -2

class dht11
{
public:
    int read();
        int humidity;
        int temperature;
};
 int dht11::read()
{
        // BUFFER TO RECEIVE
        uint8_t bits[5];
        uint8_t cnt = 7;
        uint8_t idx = 0;

        // EMPTY BUFFER
        for (int i=0; i< 5; i++) bits[i] = 0;

        // REQUEST SAMPLE
        D14_Out;
        //pinMode(pin, OUTPUT);
        D14_Low;
		//digitalWrite(pin, LOW);
        delay(18);
        D14_High;
		//digitalWrite(pin, HIGH);
        delayMicroseconds(50);
        D14_In;
        //pinMode(pin, INPUT);
		humidity    = -300;
		temperature = -300;
        // ACKNOWLEDGE or TIMEOUT
        unsigned int loopCnt = 12000;
        while(D14_Read == LOW)
                if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

        loopCnt = 12000;
        while(D14_Read == HIGH)
                if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

        // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
        for (int i=0; i<40; i++)
        {
                loopCnt = 12000;
                while(D14_Read == LOW)
                        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

                unsigned long t = micros();

                loopCnt = 12000;
                while(D14_Read == HIGH)
                        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

                if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
                if (cnt == 0)   // next byte?
                {
                        cnt = 7;    // restart at MSB
                        idx++;      // next byte!
                }
                else cnt--;
        }

        // WRITE TO RIGHT VARS
        // as bits[1] and bits[3] are allways zero they are omitted in formulas.
        humidity    = bits[0]; 
        temperature = bits[2]; 

        uint8_t sum = bits[0] + bits[2];  

        if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
        return DHTLIB_OK;
}
//-------------------------------------------------------------------
int main();
void sendAll();
void seekDD();
void seekButtPress();
void applySets();
void readSets();
void loadSets();
void saveSets();
void changeValue(bool * val);
void initFunc();
//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
 
 
