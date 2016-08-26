
// PRIMER RECEPTOR  


#include <SPI.h>
#include <amirf.h>
#include <nRF24L01.h>

uint8_t  dato[5];
byte dato_recibido[32];
byte ack_dato[]= {"0retonno"};
char letra[7];
uint8_t  corto=1;
byte direccion[5];
void setup()
{
  Serial.begin(9600);
 Serial.println( "receptor en pruebas amirf (tamaño) " );
 
   // arrancamos y configuramos la comunicacion spi con el modulo 
   AMirf.init();
   //configuramos canal y tamaño de dato
   AMirf.payload = 32;
   AMirf.channel = 1;
   // nombre del NRF24l01 para la recepccion 
   AMirf.setRADDR((byte *)"RX_01"); 
   // aplicamos valores y arrancamos modulo como receptor
   AMirf.config();
   
   AMirf.enable_DPL();
   AMirf.debug();
   AMirf.writeAckPayload(1,(byte *) ack_dato, 8);
}

void loop() 
{
  
 if (AMirf.dataReady()){
	 int tamano_paquete; 
	 tamano_paquete = AMirf.Recived_Payload_size();
	 AMirf.payload = tamano_paquete;
     AMirf.getData(dato_recibido);
       Serial.write(dato_recibido,tamano_paquete);
       Serial.print( "  tamaño dato = " );
       Serial.println(tamano_paquete);
       
       ack_dato[0]++;
       AMirf.writeAckPayload(1,(byte *) ack_dato, 8);
       Serial.print( " se envia de vuelta=  " );
       Serial.write( ack_dato,8);
       Serial.print( "\n" ); //salto de line
       
		}

}


//+++++++++++++++++++++++++++++subrutinas++++++++++++++++++++++++++
