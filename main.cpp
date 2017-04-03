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

#include "decoder.h"
#include "ledstrip.h"



void frame_callback(uint8_t *data[4], AVPacket* pkt) ;
void endVideo_callback();

//GPIO18, 144 leds ledstrip
Ledstrip leds(18, 144);

//constructor with callbak when decode frame and another callbak when Media ends
Decoder decoder(frame_callback, endVideo_callback);


//callback from frame, here we extract the rgb color to put in the leds
void frame_callback(uint8_t *data[4], AVPacket* pkt) {
	//get the RGB color of each pixel on the first row of frame and set the led with respective colors
	for (int i = 0; i < leds.getCount(); i++) {
		int p = i * 3;
		leds.setLed(i, (int) data[0][p], (int) data[0][p + 1], (int) data[0][p + 2]);
	}
	//turn leds on
	leds.on();
}

//callback when Media ends
void endVideo_callback() {
	//start playing another video
	decoder.play("video2.mp4");
	//you can play also images
	//decoder.play("image.png");

}

int main(int argc, char** argv) {
	//start playing video
	decoder.play("video1.mp4");
	return 0;
}


