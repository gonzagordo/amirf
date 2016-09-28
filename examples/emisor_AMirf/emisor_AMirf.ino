//  EMISOR  con rf24 
// envia correctamente datos estaticos y dinamicos y recibe ok los ack payload


#include <SPI.h>
#include <amirf.h>
#include <nRF24L01.h>

uint8_t  dato[5];      				//usado en el emisor conteo de fallos
byte dato_a_enviar[]= {11,3,255};
byte ack_dato[]= {"0patatero"};  //usado en receptor
byte dato_recibido[32];
int envio_size ;
int recepcion_size;
int canal_retorno = 1;			//usado en receptor

void setup()
{
  Serial.begin(9600);
  Serial.println( "emisor en pruebas amirf  " );
  Serial.println( "dpl ok, ack_payload ok   " );
   //arrancamos y configuramos la comunicacion spi con el modulo 
   AMirf.init();
   //ponemos la direccion del modulo al que vamos a emitir
   AMirf.setTADDR((byte *)"RX_01");
   //configuramos canal y tamaño de dato
   AMirf.payload = 32;
   AMirf.channel = 1;
   envio_size = sizeof(dato_a_enviar);
   AMirf.payload = envio_size;
   // aplicamos valores y arrancamos modulo como receptor
   
   AMirf.config();
//numero de repes y tiempo entre ellas util solo en emisor
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

Serial.print( "-> " );
Serial.print( dato_a_enviar[0]);
Serial.print( "-> " );
Serial.print( dato_a_enviar[1]);
Serial.print( "-> " );
Serial.print( dato_a_enviar[2]);

dato_a_enviar[1]++;

if (dato_a_enviar[2]==0) dato_a_enviar[2]=255;
else dato_a_enviar[2]=0;


AMirf.readRegister(OBSERVE_TX,&dato[0],1);
Serial.print( " repeticiones =    " );
Serial.println(dato[0]);

if (AMirf.dataReady()){
	
     recepcion_size=AMirf.getData(dato_recibido);
	   Serial.print( "<-dato devuelto = " );
       Serial.write(dato_recibido,recepcion_size);
       Serial.print( "  bytes de dato = " );
       Serial.println(recepcion_size);
       
	   for (int i=0;i<recepcion_size;i++){
		   Serial.print( ">> ");
		   Serial.println(dato_recibido[i]);
		   }
	  
       
		}
else {
		Serial.println( "no ack_payload " );
		}
                
delay(2000);
 
}

  //+++++++++++++++++++subrutinas++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++
