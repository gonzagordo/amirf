// PRIMER EMISOR 


#include <SPI.h>
#include <amirf.h>
#include <nRF24L01.h>

uint8_t  dato[5];
char dato_a_enviar[7]= {"tomaa0",};
uint8_t  corto=1;
byte direccion[5];
void setup()
{
  Serial.begin(9600);
 Serial.println( "emisor en pruebas amirf " );
 
   // arrancamos y configuramos la comunicacion spi con el modulo 
   AMirf.init();
   //ponemos la direccion del modulo al que vamos a emitir
    AMirf.setTADDR((byte *)"RX_01");
   //configuramos canal y tamaño de dato
    AMirf.channel = 1;
    AMirf.payload = 7;
   // aplicamos valores y arrancamos modulo como receptor
   AMirf.config();
   AMirf.configRegister(SETUP_RETR,B0001101);
   
   
  //+++++++++++++++++++++++++++++++++++++++++++++
  for (int x=0; x<24; x++)
        {
                AMirf.readRegister(x,&dato[0],1);
                
                
                Serial.print( "reg  " );
                Serial.print(x,HEX);
                Serial.print( " = " );
                Serial.println(dato[0],HEX);
                
        }
for (int x=10; x<16; x++)
        {
                AMirf.readRegister(x,&dato[0],5);
                
                
                Serial.print( "reg  " );
                Serial.print(x,HEX);
                Serial.print( " = " );
                Serial.print(dato[0],HEX);
                Serial.print(dato[1],HEX);
                Serial.print(dato[2],HEX);
                Serial.print(dato[3],HEX);
                Serial.println(dato[4],HEX);
        }
  //+++++++++++++++++++++++++++++++++++++++++++++++++
 
}

void loop() 
{
  
AMirf.send((byte *)dato_a_enviar);
Serial.print( "SE VA A ENVIAR =    " );
Serial.println( dato_a_enviar[0] );
dato_a_enviar[5]++;

AMirf.readRegister(OBSERVE_TX,&dato[0],1);
Serial.print( "repeticiones =    " );
Serial.println(dato[0]);

                
delay(500);



 
}
