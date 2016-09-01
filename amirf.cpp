/**
 EDITADA POR GONZAGORDO
 **/

#include "amirf.h"

//Defines for setting the MiRF registers for transmitting or receiving mode
Nrf24l AMirf = Nrf24l();

//CONSTRUCTOR
Nrf24l::Nrf24l(){
	cePin = 8;
	csnPin = 7;
	channel = 1;
	payload = 16;

}
//recibir por el SPI
void Nrf24l::transferSync(uint8_t *dataout,uint8_t *datain,uint8_t len){
	uint8_t i;
	for(i = 0;i < len;i++){
		datain[i] = SPI.transfer(dataout[i]);
	}
}
//transmitir por el SPI
void Nrf24l::transmitSync(uint8_t *dataout,uint8_t len){
	uint8_t i;
	for(i = 0;i < len;i++){
		SPI.transfer(dataout[i]);
	}
}
//inicializa pines he inicializa SPI
void Nrf24l::init() 
// Initializes pins to communicate with the MiRF module
// Should be called in the early initializing phase at startup.
{   
    pinMode(cePin,OUTPUT);
    pinMode(csnPin,OUTPUT);

    ceLow();
    csnHi();

    // Initialize SPI module
	// Setup SPI++++++++++++++++++++++++++++++
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV16);
}

void Nrf24l::config() 
// Sets the important registers in the MiRF module and powers the module
// in receiving mode
// NB: channel and payload must be set now.
{
    // Set RF channel
	configRegister(RF_CH,channel);

    // Set length of incoming payload 
	configRegister(RX_PW_P0, payload);
	configRegister(RX_PW_P1, payload);

    // Start receiver 
    powerUpRx();
    flushRx();
}

void Nrf24l::setRADDR(uint8_t * adr) 
// Sets the receiving address
{
	ceLow();
	writeRegister(RX_ADDR_P1,adr,mirf_ADDR_LEN);
	ceHi();
}

void Nrf24l::setTADDR(uint8_t * adr)
// Sets the transmitting address
{
	/*
	 * RX_ADDR_P0 must be set to the sending addr for auto ack to work.
	 */

	writeRegister(RX_ADDR_P0,adr,mirf_ADDR_LEN);
	writeRegister(TX_ADDR,adr,mirf_ADDR_LEN);
}

extern bool Nrf24l::dataReady() 
// Checks if data is available for reading
{
    // See note in getData() function - just checking RX_DR isn't good enough
	uint8_t status = getStatus();

    // We can short circuit on RX_DR, but if it's not set, we still need
    // to check the FIFO for any pending packets
    if ( status & (1 << RX_DR) ) return 1;
    return !rxFifoEmpty();
}

extern bool Nrf24l::rxFifoEmpty(){
	uint8_t fifoStatus;

	readRegister(FIFO_STATUS,&fifoStatus,sizeof(fifoStatus));
	return (fifoStatus & (1 << RX_EMPTY));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// devuelve como valor el tamaño del payload recibido funciona
extern int Nrf24l::Recived_Payload_size()  {
    int value;
    csnLow();
    SPI.transfer(R_RX_PL_WID);//send comand
    value = SPI.transfer(value);//read value
    csnHi();
    return (value);
}

//activa la funcion de dinamic payload en todos los pipes y el envio
extern void Nrf24l::enable_DPL(){
	uint8_t FEATURE_VIEJO;
		
		readRegister(FEATURE,&FEATURE_VIEJO,1);
		configRegister(FEATURE, 1<< EN_DPL); //ACTIVA FUNCION DYNAMIC PAYLOAD 
		
		configRegister( DYNPD, 1<< DPL_P0 |  1<< DPL_P1 |  1<< DPL_P2 |  1<< DPL_P3 |  1<< DPL_P4 |  1<< DPL_P5 );
		//ACTIVA DINAMIC PAYLOAD EN TODOS LOS CANALES 
		payload_mode=1;// dinamic payload
		Serial.println( "dpl activado" );
		
		//+++++++++++debug++++++++++++++++++++++++++++++
		readRegister(FEATURE,&FEATURE_VIEJO,1);
		Serial.println( "feature resultante" );
		Serial.println(FEATURE_VIEJO,BIN);
		//++++++++++++fin debug++++++++++++++++++++++++		
		
	}
//activa la funcion de payload en el ack packet	(REQUIERE DYNAMIC PAYLOAD ACTIVO )
	extern void Nrf24l::enable_ack_payload(){
	uint8_t FEATURE_VIEJO;
		
		readRegister(FEATURE,&FEATURE_VIEJO,1);
		
		configRegister(FEATURE, 1<< EN_DPL|1<< EN_ACK_PAY);
		//+++++++++++debug++++++++++++++++++++++++++++++
		readRegister(FEATURE,&FEATURE_VIEJO,1);
		Serial.println( "feature resultante" );
		Serial.println(FEATURE_VIEJO,BIN);
		//++++++++++++fin debug++++++++++++++++++++++++		
				
		Serial.println( "ACK PAYLOAD ACTIVADO" );
	}
	
//desactiva la funcion de payload en el ack packet	
	extern void Nrf24l::disable_ack_payload(){
	uint8_t FEATURE_VIEJO;
		
		readRegister(FEATURE,&FEATURE_VIEJO,1);
		
		configRegister(FEATURE, 0<< EN_ACK_PAY);
		
		Serial.println( "ACK PAYLOAD DESACTIVADO" );
		
	}

//desactiva la funcion de dinamic payload en todos los pipes y el envio
extern void Nrf24l::disable_DPL(){
	uint8_t FEATURE_VIEJO;
		
		readRegister(FEATURE,&FEATURE_VIEJO,1);
		configRegister(FEATURE, 0<< EN_DPL);
		
		configRegister( DYNPD, 0<< DPL_P0 |  0<< DPL_P1 |  0<< DPL_P2 |  0<< DPL_P3 |  0<< DPL_P4 |  0<< DPL_P5 );
		
		payload_mode=0;// static payload
		Serial.println( "dpl desactivado" );
	}

//carga datos de retorno (aknolage) para un pipe concreto 
extern void Nrf24l::writeAckPayload(uint8_t pipe, uint8_t * value, uint8_t len)
 {
    
    csnLow();                    // Pull down chip select
    SPI.transfer(W_ACK_PAYLOAD | ( pipe & 0b111 ) ); // Write cmd to write aknowlage_payload
    transmitSync(value,len);   // Write payload
    csnHi();                    // Pull up chip select

  
  }
 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


extern void Nrf24l::getData(uint8_t * data) 
// Reads payload bytes into data array
{
    csnLow();                               // Pull down chip select
    SPI.transfer( R_RX_PAYLOAD );            // Send cmd to read rx payload
    transferSync(data,data,payload); // Read payload
    csnHi();                               // Pull up chip select
    // NVI: per product spec, p 67, note c:
    //  "The RX_DR IRQ is asserted by a new packet arrival event. The procedure
    //  for handling this interrupt should be: 1) read payload through SPI,
    //  2) clear RX_DR IRQ, 3) read FIFO_STATUS to check if there are more 
    //  payloads available in RX FIFO, 4) if there are more data in RX FIFO,
    //  repeat from step 1)."
    // So if we're going to clear RX_DR here, we need to check the RX FIFO
    // in the dataReady() function
    configRegister(STATUS,(1<<RX_DR));   // Reset status register
}

void Nrf24l::configRegister(uint8_t reg, uint8_t value)
// Clocks only one byte into the given MiRF register
{
    csnLow();
    SPI.transfer(W_REGISTER | (REGISTER_MASK & reg));
    SPI.transfer(value);
    csnHi();
}

void Nrf24l::readRegister(uint8_t reg, uint8_t * value, uint8_t len)
// Reads an array of bytes from the given start position in the MiRF registers.
{
    csnLow();
    SPI.transfer(R_REGISTER | (REGISTER_MASK & reg));
    transferSync(value,value,len);
    csnHi();
}

void Nrf24l::writeRegister(uint8_t reg, uint8_t * value, uint8_t len) 
// Writes an array of bytes into inte the MiRF registers.
{
    csnLow();
    SPI.transfer(W_REGISTER | (REGISTER_MASK & reg));
    transmitSync(value,len);
    csnHi();
}


void Nrf24l::send(uint8_t * value) 
// Sends a data package to the default address. Be sure to send the correct
// amount of bytes as configured as payload on the receiver.
{
    uint8_t status;
    status = getStatus();

    while (PTX) {
	    status = getStatus();

	    if((status & ((1 << TX_DS)  | (1 << MAX_RT)))){
		    PTX = 0;
		    break;
	    }
    }                  // Wait until last paket is send

    ceLow();
    
    powerUpTx();       // Set to transmitter mode , Power up
    
    csnLow();                    // Pull down chip select
    SPI.transfer( FLUSH_TX );     // Write cmd to flush tx fifo
    csnHi();                    // Pull up chip select
    
    csnLow();                    // Pull down chip select
    SPI.transfer( W_TX_PAYLOAD ); // Write cmd to write payload
    transmitSync(value,payload);   // Write payload
    csnHi();                    // Pull up chip select

    ceHi();                     // Start transmission
}

/**
 * isSending.
 *
 * Test if chip is still sending.
 * When sending has finished return chip to listening.
 *
 */

bool Nrf24l::isSending(){
	uint8_t status;
	if(PTX){
		status = getStatus();
	    	
		/*
		 *  if sending successful (TX_DS) or max retries exceded (MAX_RT).
		 */

		if((status & ((1 << TX_DS)  | (1 << MAX_RT)))){
			powerUpRx();
			return false; 
		}

		return true;
	}
	return false;
}

uint8_t Nrf24l::getStatus(){
	uint8_t rv;
	readRegister(STATUS,&rv,1);
	return rv;
}

void Nrf24l::powerUpRx(){
	PTX = 0;
	ceLow();
	configRegister(CONFIG, mirf_CONFIG | ( (1<<PWR_UP) | (1<<PRIM_RX) ) );
	ceHi();
	configRegister(STATUS,(1 << TX_DS) | (1 << MAX_RT)); 
}

void Nrf24l::flushRx(){
    csnLow();
    SPI.transfer( FLUSH_RX );
    csnHi();
}

void Nrf24l::flushTx(){
    csnLow();
    SPI.transfer( FLUSH_TX );
    csnHi();
}


void Nrf24l::powerUpTx(){
	PTX = 1;
	configRegister(CONFIG, mirf_CONFIG | ( (1<<PWR_UP) | (0<<PRIM_RX) ) );
}

void Nrf24l::ceHi(){
	digitalWrite(cePin,HIGH);
}

void Nrf24l::ceLow(){
	digitalWrite(cePin,LOW);
}

void Nrf24l::csnHi(){
	digitalWrite(csnPin,HIGH);
}

void Nrf24l::csnLow(){
	digitalWrite(csnPin,LOW);
}

void Nrf24l::debug (){
		uint8_t  dato[5];
		Serial.println( "++++++++++++++++++++++++++++++ " );
		Serial.println( "++++++configuracion+++++++++++" );
	  for (int x=0; x<30; x++)
        {
                readRegister(x,&dato[0],1);
                
                
                Serial.print( "reg  " );
                Serial.print(x,HEX);
                Serial.print( " = " );
                Serial.print(dato[0],HEX);
                Serial.print( " reg  " );
                Serial.print(x,HEX);
                Serial.print( " = " );
                Serial.println(dato[0],BIN);
                
        }
        Serial.println( "++++++++++++++++++++++++++++++ " );
		Serial.println( "++++++direcciones+++++++++++" );
for (int x=10; x<16; x++)
        {
                readRegister(x,&dato[0],5);
                
                
                Serial.print( "reg  " );
                Serial.print(x,HEX);
                Serial.print( " = " );               
                Serial.write(dato,5);
                Serial.print( "\r \n" );
        }
        Serial.println( "++++++++++++++++++++++++++++++ " );
        Serial.println( "++++++++++++++++++++++++++++++ " );
  
	  }
  
