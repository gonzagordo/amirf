
// PRIMER RECEPTOR  


#include <SPI.h>
#include <amirf.h>
#include <nRF24L01.h>

uint8_t  dato[5];
byte dato_recibido[7];
char letra[7];
uint8_t  corto=1;
byte direccion[5];
void setup()
{
  Serial.begin(9600);
 Serial.println( "receptor en pruebas amirf " );
 
   // arrancamos y configuramos la comunicacion spi con el modulo 
   AMirf.init();
   //configuramos canal y tamaño de dato
   AMirf.payload = 7;
   AMirf.channel = 1;
   // nombre del NRF24l01 para la recepccion 
   AMirf.setRADDR((byte *)"RX_01"); 
   // aplicamos valores y arrancamos modulo como receptor
   AMirf.config();

 
}

void loop() 
{
  
 if (AMirf.dataReady()){
     AMirf.getData(dato_recibido);
       Serial.write(dato_recibido,7);
       Serial.print( "  tamaño dato = " );
       Serial.println(Recived_Payload_size());
   }

/*
for (int x=0; x<24; x++)
        {
                AMirf.readRegister(x,&dato[0],1);
                delay(500);
                
                Serial.print( "reg  " );
                Serial.print(x,HEX);
                Serial.print( " = " );
                Serial.println(dato[0],HEX);
                
        }
for (int x=10; x<16; x++)
        {
                AMirf.readRegister(x,&dato[0],5);
                delay(500);
                
                Serial.print( "reg  " );
                Serial.print(x,HEX);
                Serial.print( " = " );
                Serial.print(dato[0],HEX);
                Serial.print(dato[1],HEX);
                Serial.print(dato[2],HEX);
                Serial.print(dato[3],HEX);
                Serial.println(dato[4],HEX);
        }*/

 
}


//+++++++++++++++++++++++++++++subrutinas++++++++++++++++++++++++++
int Recived_Payload_size()  //funciona

// Reads an array of bytes from the given start position in the MiRF registers.

{
    int value;
    AMirf.csnLow();
    SPI.transfer(R_RX_PL_WID);//send comand
    value = SPI.transfer(value);//read value
    AMirf.csnHi();
    return (value);
}
