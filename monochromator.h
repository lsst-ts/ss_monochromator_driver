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
float getWavelength();
float getEntrySlitPosition();
float getExitSlitPosition();
int getGrating();

bool getEntrySlitStatus(); //true if it's moving
bool getExitSlitStatus(); //true if it's moving
bool getGratingStatus(); //true if it's moving
bool getWavelengthStatus(); //true if it's moving

//Sets
bool setWavelength(float wavelength);
bool setEntrySlitPosition(float position);
bool setExitSlitPosition(float position);
bool setGrating(int grating); //1 for 400, 2 for 750

libusb_device_handle *connect();
bool disconnect(libusb_device_handle *devh);

bool checkErrors(int result);

#endif /* MONOCHROMATOR_H */

