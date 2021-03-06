/*
  EDITADA POR GONZAGORDO   
*/

#ifndef _AMIRF_H_
#define _AMIRF_H_

#include <Arduino.h>
#include <SPI.h>

#include "nRF24L01.h"
// Nrf24l settings

#define mirf_ADDR_LEN	5
#define mirf_CONFIG     ( (1 << MASK_TX_DS) | (1<<EN_CRC) | (0<<CRCO) )

class Nrf24l {
	public:
		Nrf24l();

		void init();
		void config();
		void send(uint8_t *value);
		void dynamic_send(uint8_t *value, uint8_t len);
		void setRADDR(uint8_t * adr);
		void setTADDR(uint8_t * adr);
		bool dataReady();
		bool isSending();
		bool rxFifoEmpty();
		bool txFifoEmpty();
		int getData(uint8_t * data);
		uint8_t getStatus();
		
		void transmitSync(uint8_t *dataout,uint8_t len);
		void transferSync(uint8_t *dataout,uint8_t *datain,uint8_t len);
		void configRegister(uint8_t reg, uint8_t value);
		void readRegister(uint8_t reg, uint8_t * value, uint8_t len);
		void writeRegister(uint8_t reg, uint8_t * value, uint8_t len);
		
		void enable_ack_payload();// en pruebas
		void disable_ack_payload();// en pruebas
		void writeAckPayload(uint8_t pipe, uint8_t *value, int len);// en pruebas
		
		  //funcionan  añadidas por mi		
		int Recived_Payload_size();
		void enable_DPL();
		void disable_DPL();
		//+++++++++++++++++++++++++++
		void powerUpRx();
		void powerUpTx();
		
		void csnHi();
		void csnLow();

		void ceHi();
		void ceLow();
		void flushRx();
		void flushTx();

		void debug ();
		
		/*
		 * modo payload dyn/static
		 */

		bool payload_mode = 0; //0 static 1 dinamic
		/*
		 * In sending mode.
		 */

		uint8_t PTX;

		/*
		 * CE Pin controls RX / TX, default 8.
		 */

		uint8_t cePin;

		/*
		 * CSN Pin Chip Select Not, default 7.
		 */

		uint8_t csnPin;

		/*
		 * Channel 0 - 127 or 0 - 84 in the US.
		 */
		uint8_t channel;

		/*
		 * Payload width in bytes default 16 max 32.
		 */

		uint8_t payload;
};

extern Nrf24l AMirf;

#endif /* _AMIRF_H_ */
