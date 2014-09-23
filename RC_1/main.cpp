#include "main.h"
//
//
const uint8_t end1 = B11111110;
const uint8_t end2 = B11111101;
long int INPUT_STATES_TIMEOUT = LIFETM_LEN/SEEK_INPUT_STATES_DEVISOR;
long int BUTT_TIMEOUT = LIFETM_LEN/SEEK_BUTT_DEVISOR;
uint8_t input[20];               // СЃСЋРґР° РїСЂРёС…РѕРґСЏС‚ Р±Р°Р№С‚С‹ РёР· РїРѕСЂС‚Р°
int inputsize=0;
long int lifetm;
long int but[4];
bool set[15];
bool dd[4];
bool tread;
//
long int inputs_timeout[8];
bool _inputs[8];
//
unsigned long dd1time;
unsigned long dd2time;
unsigned long light_time=45000;
//
//dht11 sensor;
//
//
void initFunc() 
{    
  wdt_enable(WDTO_500MS);
  D2_Out;//pinMode(2, OUTPUT);  
  D2_Low;//digitalWrite(2, LOW);
  sei(); // разрешаем прерывания
  init(); // инициализация таймеров ардуины
  UART_Init(115200);
  D13_Out;//pinMode(13, OUTPUT);
  D13_High;//digitalWrite(13, HIGH);
  D11_Out;//pinMode(11, OUTPUT);  
  D10_Out;//pinMode(10, OUTPUT);  
  D9_Out;//pinMode(9, OUTPUT);  
  D8_Out;//pinMode(8, OUTPUT);  
  
  D7_In;//pinMode(7, INPUT);  
  D6_In;//pinMode(6, INPUT);  
  D5_In;//pinMode(5, INPUT);  
  D4_In;//pinMode(4, INPUT);
  D7_High;//digitalWrite(7, HIGH);
  D6_High;//digitalWrite(6, HIGH);
  D5_High;//digitalWrite(5, HIGH);
  D4_High;//digitalWrite(4, HIGH);  

  D12_In;//pinMode(12, INPUT);  
  D3_In;//pinMode(3, INPUT);
  D12_High;//digitalWrite(12, HIGH);
  D3_High;//digitalWrite(3, HIGH);
  D16_Out;//pinMode(A4, INPUT);  
  D17_Out;
  D18_In;//pinMode(A4, INPUT);  
  D19_In;//pinMode(A5, INPUT);
  D18_High;//digitalWrite(A4, HIGH);
  D19_High;//digitalWrite(A5, HIGH);

  set[0]=false;
  set[1]=false;
  set[2]=false;
  set[3]=false;
  set[10]=false;
  set[11]=false;
  set[12]=false;
  set[13]=false;
  set[14]=false;
  
  but[0]=-1;
  but[1]=-1;
  but[2]=-1;
  but[3]=-1;

  tread = true;
  loadSets();

  dd[0]=false;
  dd[1]=false;
  dd[2]=false;
  dd[3]=false;

  D0_In;

  _inputs[0] = D4_Read;
  _inputs[1] = D5_Read;
  _inputs[2] = D6_Read;
  _inputs[3] = D7_Read;
  _inputs[4] = D3_Read;
  _inputs[5] = D12_Read;
  _inputs[6] = D18_Read;
  _inputs[7] = D19_Read;

}
//
// изменение значения булевской переменной
void changeValue(bool * val)
{
  if (*val==false) *val=true;
    else *val=false;
}
//
// чтение массива настройки из энергонезависимой памяти
void saveSets()
{
  WriteEEPROM_Byte(0, set[4]);
  WriteEEPROM_Byte(1, set[5]);
  WriteEEPROM_Byte(2, set[6]);
  WriteEEPROM_Byte(3, set[7]);
  WriteEEPROM_Byte(4, set[8]);
  WriteEEPROM_Byte(5, set[9]);
}
//
// загрузка массива настройки в энергонезависимую память
void loadSets()
{
  set[4]=ReadEEPROM_Byte(0);
  set[5]=ReadEEPROM_Byte(1);
  set[6]=ReadEEPROM_Byte(2);
  set[7]=ReadEEPROM_Byte(3);
  set[8]=ReadEEPROM_Byte(4);
  set[9]=ReadEEPROM_Byte(5);
}
//
// чтение массива настройки из управляющей команды
void readSets()
{
  if (input[1] & B10000000) set[0]=true; 
  else set[0]=false;
  if (input[1] & B01000000) set[1]=true; 
  else set[1]=false;
  if (input[1] & B00100000) set[2]=true;
  else set[2]=false;
  if (input[1] & B00010000) set[3]=true; 
  else  set[3]=false;
  if (input[1] & B00001000) set[4]=true; 
  else set[4]=false;
  if (input[1] & B00000100) set[5]=true; 
  else set[5]=false;
  if (input[1] & B00000010) set[6]=true; 
  else set[6]=false;
  if (input[1] & B00000001) set[7]=true; 
  else set[7]=false;
  if (input[2] & B01000000) set[8]=true; 
  else set[8]=false;
  if (input[2] & B00100000) set[9]=true; 
  else set[9]=false;
    if (input[2] & B00010000) set[10]=true; 
  else set[10]=false;
    if (input[2] & B00001000) set[11]=true; 
  else set[11]=false;
    if (input[2] & B00000100) set[12]=true; 
  else set[12]=false;
    if (input[2] & B00000010) set[13]=true; 
  else set[13]=false;
    if (input[2] & B00000001) set[14]=true; 
  else set[14]=false;
  saveSets();
}
//
// установка состояний выходов в соответствии с массивом настройки
void applySets()
{
  if (set[0]==true) D8_High; else D8_Low;  
  if (set[1]==true) D9_High; else D9_Low;    
  if (set[2]==true) D10_High; else 
  {
    if ((set[8]==1)&&(dd[0]==1)) D10_High;
    else D10_Low;   
  }
  if (set[3]==true) D11_High; else 
  {
    if ((set[9]==1)&&(dd[1]==1)) D11_High;
    else D11_Low;  
  }
  if (set[10]==true) D16_High; else D16_Low;  
  if (set[11]==true) D17_High; else D17_Low;    
}
//
// определение состояния входов
void seekInputState(int pin, bool state)
{
	if (inputs_timeout[pin]!=-1) {
		if (lifetm<inputs_timeout[pin]) {
			if (LIFETM_LEN+lifetm-inputs_timeout[pin] > INPUT_STATES_TIMEOUT){
				_inputs[pin] = state;
				inputs_timeout[pin]=-1;
			}
		}
		else
		if (lifetm-inputs_timeout[pin] > INPUT_STATES_TIMEOUT){
			_inputs[pin] = state;
			inputs_timeout[pin]=-1;
		}
	}
	if ((_inputs[pin]!=state)&&(inputs_timeout[pin]==-1))
	inputs_timeout[pin]=lifetm;
}
void seekInputStates()
{
	seekInputState(0, D4_Read);
	seekInputState(1, D5_Read);
	seekInputState(2, D6_Read);
	seekInputState(3, D7_Read);
	seekInputState(4, D3_Read);
	seekInputState(5, D12_Read);
	seekInputState(6, D18_Read);
	seekInputState(7, D19_Read);
}
//
// ищет нажатия кнопок, с обработкой дребезга контактов
void seekButtPress()
{
  if (set[4]==1)
  {
    if (but[0]!=-1)
    {
      if (lifetm<but[0]) 
        {if (LIFETM_LEN+lifetm-but[0] > BUTT_TIMEOUT) but[0]=-1;}
      else if (lifetm-but[0] > BUTT_TIMEOUT) but[0]=-1;
    }
    if ((_inputs[0]==false)&&(but[0]==-1))
    {
      changeValue(&set[0]);
      but[0]=lifetm;
    }
  }
  if (set[5]==1)
  {
    if (but[1]!=-1)
    {
      if (lifetm<but[1]) 
        {if (LIFETM_LEN+lifetm-but[1] > BUTT_TIMEOUT) but[1]=-1;}
      else if (lifetm-but[1] > BUTT_TIMEOUT) but[1]=-1;
    }
    if ((_inputs[1]==false)&&(but[1]==-1))
    {
      changeValue(&set[1]);
      but[1]=lifetm;
    }
  }
  if ((set[6]==1)&&(set[8]==0))
  {
    if (but[2]!=-1)
    {
      if (lifetm<but[2]) 
        {if (LIFETM_LEN+lifetm-but[2] > BUTT_TIMEOUT) but[2]=-1;}
      else if (lifetm-but[2] > BUTT_TIMEOUT) but[2]=-1;
    }
    if ((_inputs[2]==false)&&(but[2]==-1))
    {
      changeValue(&set[2]);
      but[2]=lifetm;
    }
  }
  if ((set[7]==1)&&(set[9]==0))
  {
    if (but[3]!=-1)
    {
      if (lifetm<but[3]) 
        {if (LIFETM_LEN+lifetm-but[3] > BUTT_TIMEOUT) but[3]=-1;}
      else if (lifetm-but[3] > BUTT_TIMEOUT) but[3]=-1;
    }
    if ((_inputs[3]==false)&&(but[3]==-1))
    {
      changeValue(&set[3]);
      but[3]=lifetm;
    }
  }
}
// смотрит, сработали ли датчики движения - если да, продлевает таймеры
//
void seekDD()
{
  if (set[8]==1)
  {
    if (dd[2]==true)
    {
      unsigned long a = (millis()-dd1time);
      if (a > 10)
      {
        dd[2]=false;
        dd[0]=false;
      }
    }
    if ((_inputs[2]==false))
    {
      dd[0]=true;
      dd[2]=true;
      dd1time=millis();
    }
  }
  if (set[9]==1)
  {
    if (dd[3]==true)
    {
      unsigned long a = (millis()-dd2time);
      if (a > light_time)
      {
        dd[3]=false;
        dd[1]=false;
      }
    }
    if ((_inputs[3]==true))
    {
      dd[1]=true;
      dd[3]=true;
      dd2time=millis();
    }
  }
}
// отправка массива состояния контроллера
// 
void sendAll()
{
  uint8_t buf[12];
  buf[0] = B00000000; 
  buf[1] = B00000000; buf[2] = B00000000; buf[3] = B00000000; buf[4] = B00000000; buf[5] = B00000000;
  buf[6] = B00000000; buf[7] = B00000000; buf[8] = B00000000; buf[9] = B00000000; buf[10] = B00000000;
  buf[11] = B00000000; 
  D0_High;//digitalWrite(0, HIGH);
  D2_High;//digitalWrite(2, HIGH);
  buf[0] = (uint8_t)SN;
  if (set[0]==true) buf[1] |= B10000000;  
  if (set[1]==true) buf[1] |= B01000000;  
  if (set[2]==true) buf[1] |= B00100000;  
  if (set[3]==true) buf[1] |= B00010000;  
  if (set[4]==true) buf[1] |= B00001000;  
  if (set[5]==true) buf[1] |= B00000100;  
  if (set[6]==true) buf[1] |= B00000010;  
  if (set[7]==true) buf[1] |= B00000001;  
  if (set[8]==true) buf[2] |= B01000000;  
  if (set[9]==true) buf[2] |= B00100000;  
  if (set[10]==true) buf[2] |= B00010000;  
  if (set[11]==true) buf[2] |= B00001000;  
  if (set[12]==true) buf[2] |= B00000100;  
  if (set[13]==true) buf[2] |= B00000010;  
  if (set[14]==true) buf[2] |= B00000001;  
  int ibuf = A1_Read;
  buf[3] = (uint8_t)(ibuf >> 8);
  buf[4] = (uint8_t)(ibuf & B11111111);
  if (_inputs[0]==true) buf[5] |= B10000000; 
  if (_inputs[1]==true) buf[5] |= B01000000; 
  if (_inputs[2]==true) buf[5] |= B00100000; 
  if (_inputs[3]==true) buf[5] |= B00010000; 
  //buf[6] = (uint8_t)sensor.temperature;
  //buf[7] = (uint8_t)sensor.humidity;
  ibuf = A0_Read;
  buf[6] = (uint8_t)(ibuf >> 8);
  buf[7] = (uint8_t)(ibuf & B11111111);
  if (_inputs[4]==true) buf[8] |= B10000000; 
  if (_inputs[5]==true) buf[8] |= B01000000; 
  if (_inputs[6]==true) buf[8] |= B00100000; 
  if (_inputs[7]==true) buf[8] |= B00010000; 
  buf[9] = 0 - (buf[0]+buf[1]+buf[2]+buf[3]+buf[4]+buf[5]+buf[6]+buf[7]+buf[8]);
  buf[10] = end1;
  buf[11] = end2;
  UART_SendArray(buf, 12);
  delay(1);
  D2_Low;//digitalWrite(2, LOW);
  D0_Low;//digitalWrite(0, LOW);
}
//
//
int main(void)
{
  initFunc();
  while(1)
  {
	  wdt_reset();
	  lifetm++;
	  if (lifetm==LIFETM_LEN)
	  {
		D13_Inv;
		//if (tread == true)
		//{	
		//	tread = false;
		//	sensor.read();
		//}
		//else tread = true;
		lifetm = 0;
	  }
	  seekInputStates();
	  seekButtPress();
	  seekDD();
	  applySets();
	}
}
//
//
#if defined(USART_RX_vect)
  ISR(USART_RX_vect)
#elif defined(USART0_RX_vect)
  ISR(USART0_RX_vect)
#elif defined(USART_RXC_vect)
  ISR(USART_RXC_vect) 
#endif
{
  while(UART_ReadByte(input[inputsize]))
  {
    inputsize++;
    if ((inputsize > 1)&&(input[inputsize-2]==end1)&&(input[inputsize-1]==end2))
    {
      if (inputsize == 6)
      {
        if ((input[0] == (uint8_t)SN) && (input[0] == input[1])&& (input[2] == input[1])&& (input[2] == input[3])) 
        {
          sendAll();
          inputsize = 0;
        }
        if ((input[0] == (uint8_t)SN) && ( (uint8_t)(0-(input[0]+input[1]+input[2]))==(uint8_t)input[3] ))
        {
          readSets();
          inputsize = 0;
        }
      }
      inputsize = 0;
    }
    if (inputsize > 18) {inputsize = 0;}
  }
}