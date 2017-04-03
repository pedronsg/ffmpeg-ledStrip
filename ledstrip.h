/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ledstrip.h
 * Author: Pedro
 *
 * Created on 10 de Dezembro de 2016, 18:23
 */

#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#ifdef __cplusplus
extern "C" {
#endif
#include "ws2811/ws2811.h"
#ifdef __cplusplus
}
#endif


#include <stdint.h>
#include <stdlib.h>

class Ledstrip {
private:
	bool isPlaying;
	bool videoLoop;
	int brightness;
	uint64_t nano();
	
public:
	Ledstrip(int gpioPin);
	void setBrightness(int value);
	void play(const char *filename);
	int stop();
	void loop(bool l);
};


#endif /* LEDSTRIP_H */

