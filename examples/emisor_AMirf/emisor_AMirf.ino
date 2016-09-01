// PRIMER EMISOR 


#include <SPI.h>
#include <amirf.h>
#include <nRF24L01.h>

uint8_t  dato[5];
byte dato_a_enviar[]= {"0vengaa"};
byte ack_dato[]= {"0R"};
byte dato_recibido[32];
int envio_size ;
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
    envio_size = sizeof(dato_a_enviar);
    AMirf.payload = envio_size;
   // aplicamos valores y arrancamos modulo como receptor
   
   AMirf.config();
   AMirf.configRegister(SETUP_RETR,B0001101);
  
   AMirf.enable_DPL();
   AMirf.enable_ack_payload();
   AMirf.debug(); 
   
}

void loop() 
{
  
AMirf.send((byte *)dato_a_enviar);
Serial.print( "->SE VA A ENVIAR =    " );
Serial.write( dato_a_enviar,AMirf.payload);
dato_a_enviar[0]++;

AMirf.readRegister(OBSERVE_TX,&dato[0],1);
Serial.print( " repeticiones =    " );
Serial.println(dato[0]);

if (dato[0]<29){
	 int recepcion_size; 
	 recepcion_size = AMirf.Recived_Payload_size();
	 AMirf.payload = recepcion_size;
     AMirf.getData(dato_recibido);
	   Serial.print( "<-dato devuelto = " );
       Serial.write(dato_recibido,recepcion_size);
       Serial.print( "  bytes de dato = " );
       Serial.println(recepcion_size);
	   AMirf.payload = envio_size;
       
		}
else {
		Serial.println( "no ack_payload " );
		}
                
delay(2000);
 
}

  //+++++++++++++++++++subrutinas++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++
