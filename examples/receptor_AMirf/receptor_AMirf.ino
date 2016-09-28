//  RECEPTOR  con rf24 
// recibe correctamente datos estaticos y dinamicos y envia ok los ack payload
  


#include <SPI.h>
#include <amirf.h>
#include <nRF24L01.h>

uint8_t  dato[5];      				//usado en el emisor conteo de fallos
byte dato_a_enviar[]= {"0tu"};
byte ack_dato[]= {"0patatero"};  //usado en receptor
byte dato_recibido[32];
int envio_size ;
int recepcion_size;
int canal_retorno = 1;			//usado en receptor

void setup()
{
  Serial.begin(9600);
  Serial.println( "receptor en pruebas amirf (tamaño) " );
  Serial.println( "dpl ok, ack_payload ok   " );
   //arrancamos y configuramos la comunicacion spi con el modulo 
   AMirf.init();
 // nombre del NRF24l01 para la recepccion 
   AMirf.setRADDR((byte *)"RX_01"); 
   //configuramos canal y tamaño de dato
   AMirf.payload = 32;
   AMirf.channel = 1;
  
   // aplicamos valores y arrancamos modulo como receptor
   
   AMirf.config();
//numero de repes y tiempo entre ellas util solo en emisor
   AMirf.configRegister(SETUP_RETR,B0001101);
  
   AMirf.enable_DPL();
   AMirf.enable_ack_payload();
   AMirf.debug();


   AMirf.writeAckPayload(canal_retorno,(byte *) ack_dato,sizeof(ack_dato));
}

void loop() 
{
  
 if (AMirf.dataReady()){
	 //int tamano_paquete; 
	 //tamano_paquete = AMirf.Recived_Payload_size();
	 //AMirf.payload = tamano_paquete;
     recepcion_size=AMirf.getData(dato_recibido);
	   Serial.print( "<-recibido = " );
       Serial.write(dato_recibido,recepcion_size);
       Serial.print( "  bytes de dato = " );
       Serial.println(recepcion_size);
       
       Serial.println(dato_recibido[0],DEC);
       Serial.println(dato_recibido[1],DEC);
       Serial.println(dato_recibido[2],DEC);
       
       
       ack_dato[0]++;
       AMirf.writeAckPayload(canal_retorno,(byte *) ack_dato,sizeof(ack_dato));
       Serial.print( "->se envia de vuelta=  " );
       Serial.write( ack_dato , sizeof(ack_dato) );
       Serial.println( "" ); //salto de linea
       
		}

}


//+++++++++++++++++++++++++++++subrutinas++++++++++++++++++++++++++
