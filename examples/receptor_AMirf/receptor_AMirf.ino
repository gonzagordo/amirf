
// PRIMER RECEPTOR  


#include <SPI.h>
#include <amirf.h>
#include <nRF24L01.h>

uint8_t  dato[5];
byte dato_recibido[32];
byte ack_dato[]= {"0AAAAAAA"};
int retorno_size =sizeof(ack_dato);
int canal_retorno= 1;
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
   AMirf.enable_ack_payload();
   AMirf.debug();
   AMirf.writeAckPayload(canal_retorno,(byte *) ack_dato, retorno_size);
}

void loop() 
{
  
 if (AMirf.dataReady()){
	 int tamano_paquete; 
	 tamano_paquete = AMirf.Recived_Payload_size();
	 AMirf.payload = tamano_paquete;
     AMirf.getData(dato_recibido);
	   Serial.print( "<-recibido = " );
       Serial.write(dato_recibido,tamano_paquete);
       Serial.print( "  bytes de dato = " );
       Serial.println(tamano_paquete);
       
       ack_dato[0]++;
       AMirf.writeAckPayload(canal_retorno,(byte *) ack_dato, retorno_size);
       Serial.print( "->se envia de vuelta=  " );
       Serial.write( ack_dato,8);
       Serial.println( "" ); //salto de linea
       
		}

}


//+++++++++++++++++++++++++++++subrutinas++++++++++++++++++++++++++
