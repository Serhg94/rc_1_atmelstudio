#include "CyberLib.h"

//**********Small UART****************************
/*
#define UART_DOUBLESPEED
#define UART_TXREADY UDRE0
#define UART_RXREADY RXC0
#define UART_DOUBLE	U2X0
#define UDR  UDR0
#define UCRA UCSR0A
#define UCRB UCSR0B
#define UCRC UCSR0C
#define UCRC_VALUE ((1<<UCSZ01) | (1<<UCSZ00))
#define RXCIE RXCIE0
#define TXCIE TXCIE0
#define RXEN RXEN0
#define TXEN TXEN0
#define UBRRL UBRR0L
#define UBRRH UBRR0H
#define SIG_UART_TRANS SIG_USART_TRANS
#define SIG_UART_RECV  SIG_USART_RECV
#define SIG_UART_DATA  SIG_USART_DATA
*/
#define UART_CALC_BAUDRATE(baudRate) ((uint32_t)((F_CPU) + ((uint32_t)baudRate * 4UL)) / ((uint32_t)(baudRate) * 8UL) - 1)

#ifdef _AVR_IOM328P_H_
void UART_Init(uint32_t UART_BAUD_RATE)
{
	UBRR0H=BAUDRATE_HIGH;
	UBRR0L=BAUDRATE_LOW;
	//UBRRH = (UART_CALC_BAUDRATE(UART_BAUD_RATE)>>8) & 0xFF;
	//UBRRL = (UART_CALC_BAUDRATE(UART_BAUD_RATE) & 0xFF);
	UCSR0A = ( 1<<U2X0 );
	UCSR0B = ((1<<TXEN0) | (1<<RXEN0));
	UCSR0B |= (1<<RXCIE0);
	//UCSRC = ((1<<UCSZ1) | (1<<UCSZ0));
}

void UART_SendByte(uint8_t data)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

bool UART_ReadByte(uint8_t& data)
{
	if (UCSR0A & (1<<RXC0))
	{
		data = UDR0;
		return true;
	} else return false;
}

void UART_SendArray(uint8_t *buffer, uint16_t bufferSize)
{
	for(uint16_t i=0; i<bufferSize; i++)
	UART_SendByte(buffer[i]);
	while (!(UCSR0A & (1<<UDRE0)));
}

void UART_Fluch()
{
	while (!(UCSR0A & (1<<TXC0)));
}
#endif

#ifdef _AVR_IOM8_H_
void UART_Init(uint32_t UART_BAUD_RATE)
{
	UBRRH=BAUDRATE_HIGH;
	UBRRL=BAUDRATE_LOW;
	//UBRRH = (UART_CALC_BAUDRATE(UART_BAUD_RATE)>>8) & 0xFF;
	//UBRRL = (UART_CALC_BAUDRATE(UART_BAUD_RATE) & 0xFF);
	UCSRA = ( 1<<U2X );
	UCSRB = ((1<<TXEN) | (1<<RXEN));
	UCSRB |= (1<<RXCIE);
	//UCSRC = ((1<<UCSZ1) | (1<<UCSZ0));
}

void UART_SendByte(uint8_t data)
{
	while (!(UCSRA & (1<<UDRE)));
	UDR = data;
}

bool UART_ReadByte(uint8_t& data)
{
	if (UCSRA & (1<<RXC))
	{
		data = UDR;
		return true;
	} else return false;
}

void UART_SendArray(uint8_t *buffer, uint16_t bufferSize)
{
	for(uint16_t i=0; i<bufferSize; i++)
	UART_SendByte(buffer[i]);
	while (!(UCSRA & (1<<UDRE)));
}

void UART_Fluch()
{
	while (!(UCSRA & (1<<TXC)));
}
#endif

//**********AnalogRead***************************
uint16_t AnRead(uint8_t An_pin)
{
  ADMUX=An_pin;   
  _delay_us(10);	  
  ADCSRA=B11000110;	//B11000111-125kHz B11000110-250kHz 
  while (ADCSRA & (1 << ADSC));
  An_pin = ADCL;
  uint16_t An = ADCH; 
  return (An<<8) + An_pin;
}
//******************EEPROM*******************************
void WriteEEPROM_Byte(uint8_t addr, uint8_t data)  //сохранить в EEPROM
{
		eeprom_write_byte((uint8_t*)addr, data);
}

void WriteEEPROM_Word(uint16_t addr, uint16_t data)
{
		eeprom_write_word((uint16_t*)addr, data);
}

void WriteEEPROM_Long(uint8_t addr, uint32_t data)  //сохранить в EEPROM
{           
  addr *= 4;
        eeprom_write_byte((uint8_t*)addr, data & 0xFF);
        eeprom_write_byte((uint8_t*)addr+1, (data & 0xFF00) >> 8);
        eeprom_write_byte((uint8_t*)addr+2, (data & 0xFF0000) >> 16);
        eeprom_write_byte((uint8_t*)addr+3, (data & 0xFF000000) >> 24);
		
	  // addr *= 2;
        // eeprom_write_word((uint16_t*)addr, data & 0xFFFF);
        // eeprom_write_word((uint16_t*)addr+1, (data & 0xFFFF0000) >> 16);
}

uint8_t ReadEEPROM_Byte(uint8_t addr)
{
		return eeprom_read_byte((uint8_t*)addr);
}

uint16_t ReadEEPROM_Word(uint16_t addr)
{
		return eeprom_read_word((uint16_t*)addr);
}

uint32_t ReadEEPROM_Long(uint8_t addr)  // считываем значение из EEPROM
{
  addr *= 4; 
        uint32_t ir_code = eeprom_read_byte((uint8_t*)addr+3); 
        ir_code = (ir_code << 8) | eeprom_read_byte((uint8_t*)addr+2);
        ir_code = (ir_code << 8) | eeprom_read_byte((uint8_t*)addr+1);
        ir_code = (ir_code << 8) | eeprom_read_byte((uint8_t*)addr);
		//eeprom_read_word((uint16_t*) addr)
  return ir_code;
}
