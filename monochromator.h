/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   monochromator.h
 * Author: andres
 *
 * Created on August 24, 2017, 1:16 PM
 */

#ifndef MONOCHROMATOR_H
#define MONOCHROMATOR_H

#include "libusb.h"

//Gets
float getWavelength(libusb_device_handle *devh);
float getEntrySideSlitPosition(libusb_device_handle *devh);
float getExitSideSlitPosition(libusb_device_handle *devh);
bool getGrating1(libusb_device_handle *devh);
bool getGrating2(libusb_device_handle *devh);
bool getGrating400Status(libusb_device_handle *devh);
bool getGrating750Status(libusb_device_handle *devh);

bool getSlitsStatus(libusb_device_handle *devh); //true if it's moving
bool getGratingStatus(libusb_device_handle *devh); //true if it's moving

//Sets
bool setWavelength(libusb_device_handle *devh,float wavelength);
//Check grating and slit moving status, if all has stopped return, if not continue checking
bool setWavelengthWithStatusCheck(libusb_device_handle *devh,float wavelength, int timeout=600); 

bool setEntrySideSlitPosition(libusb_device_handle *devh,float position);
//Check grating and slit moving status, if all has stopped return true, if it takes longer than timeout or error occurs return false
//After using, wavelength has to be checked....
bool setEntrySideSlitPositionWithStatusCheck(libusb_device_handle *devh, float position, int timeout=600);

bool setExitSideSlitPosition(libusb_device_handle *devh,float position);
//Check grating and slit moving status, if all has stopped return true, if it takes longer than timeout or error occurs return false
//After using, wavelength has to be checked....
bool setExitSideSlitPositionWithStatusCheck(libusb_device_handle *devh, float position, int timeout=600);

bool setGrating(libusb_device_handle *devh,int grating); //1 for 400, 2 for 750
//Set grating 1 (400) or grating 2 (750), if timeout is triggered will return false.
bool setGratingWithStatusCheck(libusb_device_handle *devh, int grating, int timeout=600);

libusb_device_handle *MonoConnect();
bool MonoDisconnect(libusb_device_handle *devh);

bool checkErrors(int result);

//tools 
float hexToF(unsigned char *data_in);
int hexToI(unsigned char *data_in);
unsigned int isMoving(libusb_device_handle *devh);
void readingDelay();

//Initialization, this is a replication from the commands obtained from sniffing the sw. Sniff commands in the repo.
bool initTotal(libusb_device_handle *devh);

bool init1(libusb_device_handle *devh);
int init1Aux(libusb_device_handle *devh);
int init1AuxSet00(libusb_device_handle *devh);

bool init2(libusb_device_handle *devh);
bool init3(libusb_device_handle *devh);
bool init4(libusb_device_handle *devh);
bool init5(libusb_device_handle *devh);
bool init6(libusb_device_handle *devh);
bool init7(libusb_device_handle *devh);

#endif /* MONOCHROMATOR_H */

