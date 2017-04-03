/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Pedro
 *
 * Created on 10 de Dezembro de 2016, 20:47
 */

#include <cstdlib>
#include "ledstrip.h"
#include <unistd.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
	Ledstrip leds(18);
//	leds.loop(true);
	leds.setBrightness(90);
//	leds.play(argv[1]);
	leds.play( "/root/.netbeans/remote/192.168.1.104/pedro-vaio-Windows-x86_64/C/Users/Pedro/Documents/NetBeansProjects/H264Decoder/dist/Debug/GNU-Linux/t.h264");
	sleep(3);
	leds.play( "/root/.netbeans/remote/192.168.1.104/pedro-vaio-Windows-x86_64/C/Users/Pedro/Documents/NetBeansProjects/H264Decoder/dist/Debug/GNU-Linux/in.h264");
	
	
	return 0;
}

