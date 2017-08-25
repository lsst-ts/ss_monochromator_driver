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

bool getSlitsStatus(libusb_device_handle *devh); //true if it's moving
bool getGratingStatus(libusb_device_handle *devh); //true if it's moving

//Sets
bool setWavelength(libusb_device_handle *devh,float wavelength);
bool setEntrySideSlitPosition(libusb_device_handle *devh,float position);
bool setExitSideSlitPosition(libusb_device_handle *devh,float position);
bool setGrating(libusb_device_handle *devh,int grating); //1 for 400, 2 for 750

libusb_device_handle *MonoConnect();
bool MonoDisconnect(libusb_device_handle *devh);

bool checkErrors(int result);

//tools 
float hexToF(unsigned char *data_in);
int hexToI(unsigned char *data_in);

#endif /* MONOCHROMATOR_H */

