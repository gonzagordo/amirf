// PRIMER EMISOR 


#include <SPI.h>
#include <amirf.h>
#include <nRF24L01.h>

uint8_t  dato[5];
byte dato_a_enviar[]= {"0toaaaaaaaaaaa",};
uint8_t  corto=1;
byte direccion[5];
void setup()
{
  Serial.begin(9600);
 Serial.println( "emisor en pruebas amirf " );
 
   // arrancamos y configuramos la comunicacion spi con el modulo 
   AMirf.init();
   //AMirf.debug();
   //ponemos la direccion del modulo al que vamos a emitir
    AMirf.setTADDR((byte *)"RX_01");
   //configuramos canal y tamaño de dato
    AMirf.channel = 1;
    AMirf.payload = sizeof(dato_a_enviar);
   // aplicamos valores y arrancamos modulo como receptor
   
   AMirf.config();
   AMirf.configRegister(SETUP_RETR,B0001101);
  
   AMirf.enable_DPL();
   AMirf.debug(); 
}

void loop() 
{
  
AMirf.send((byte *)dato_a_enviar);
Serial.print( "SE VA A ENVIAR =    " );
Serial.write( dato_a_enviar,7);
dato_a_enviar[0]++;

AMirf.readRegister(OBSERVE_TX,&dato[0],1);
Serial.print( "repeticiones =    " );
Serial.println(dato[0]);

                
delay(2000);
 
}

  //+++++++++++++++++++subrutinas++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++
