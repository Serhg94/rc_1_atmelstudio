/*
 * conf.h
 *
 * Created: 14.04.2014 3:17:26
 *  Author: Сережа
 */ 


#ifndef CONF_H_
#define CONF_H_

#define SN 4
#define VERSION "0.7.1a"
#define BAUDRATE_HIGH 0x00
#define BAUDRATE_LOW 0x10
#define F_CPU 16000000UL

#define LIFETM_LEN 100000
#define SEEK_INPUT_STATES_DEVISOR 100
#define SEEK_BUTT_DEVISOR 8
#define INPUT_STATES_TIMEOUT LIFETM_LEN/SEEK_INPUT_STATES_DEVISOR
#define BUTT_TIMEOUT LIFETM_LEN/SEEK_BUTT_DEVISOR



#endif /* CONF_H_ */