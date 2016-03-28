
// PRIMERA PRUEBA SE ESCRIBE Y SE LEE CORRECTAMENTE LAS DOS PRIMERAS DIRECCIONES 


#include <SPI.h>
#include <amirf.h>
#include <nRF24L01.h>

uint8_t  dato[5];
uint8_t  corto=1;
byte direccion[5];
void setup()
{
  Serial.begin(9600);
 Serial.println( "bateria pruebas amirf " );
 
  // arrancamos y configuramos la comunicacion spi con el modulo 
 AMirf.init();
 
}

void loop() 
{
  
 // nombre del NRF24l01 al que vamos a emitir. Del NRF24L01 DESTINO. 
 AMirf.setTADDR((byte *)"RX_01");
AMirf.setRADDR((byte *)"TX_01"); 


for (int x=10; x<16; x++)
        {
                AMirf.readRegister(x,&dato[0],5);
                delay(500);
                
                Serial.print( "reg  " );
                Serial.print(x);
                Serial.print( " = " );
                Serial.print(dato[0],HEX);
                Serial.print(dato[1],HEX);
                Serial.print(dato[2],HEX);
                Serial.print(dato[3],HEX);
                Serial.println(dato[4],HEX);
        }

 
}
