/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "monochromator.h"
#include "resources.h"
#include <cstdlib>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

libusb_device_handle *connect(){
    
    struct libusb_device_handle *devh = NULL;
    int result = libusb_init(NULL);
    int device_ready = 0;
    
    
    if (result >= 0)
    {
            devh = libusb_open_device_with_vid_pid(NULL, idVendor, idProduct);

            if (devh != NULL)
            {
                    // The HID has been detected.
                    // Detach the hidusb driver from the HID to enable using libusb.
                    if(libusb_kernel_driver_active(devh, INTERFACE_NUMBER)) libusb_detach_kernel_driver(devh, INTERFACE_NUMBER);

                    result = libusb_claim_interface(devh, INTERFACE_NUMBER);
                    if (result >= 0)
                    {
                            device_ready = 1;
                    }
                    else
                    {
                            fprintf(stderr, "libusb_claim_interface error %d\n", result);
                            return NULL;
                    }
                    
            }
            else
            {
                    fprintf(stderr, "Unable to find the device.\n");
                    return NULL;
            }
    }
    else
    {
            fprintf(stderr, "Unable to initialize libusb.\n");
            return NULL;
    }
    
    return devh;
}
bool disconnect(libusb_device_handle *devh){
    try{
        libusb_close(devh);
        libusb_exit(NULL);
    }catch(int e){
        printf("An exception occurred. Exception Nr. %i '\n'",e);
        return false;
    }
    return true;
}
bool checkErrors(int result){
        if(result>0){fprintf(stderr, "No error"); return false;}
        else if(result == LIBUSB_ERROR_TIMEOUT)  fprintf(stderr, "transfer timed out");
        else if(result == LIBUSB_ERROR_PIPE)  fprintf(stderr, "the control request is not supported by the device"); 
        else if(result == LIBUSB_ERROR_NO_DEVICE)  fprintf(stderr, "the device has been disconnected"); 
        else fprintf(stderr, "other failures"); 
        return true;
}
//Gets
float getWavelength(libusb_device_handle *devh){
    unsigned char data_in[4];
    int result = 0;


    result = libusb_control_transfer(
                            devh,
                            0xc0 ,
                            0xb3,
                            0x0000,
                            0x0002,
                            data_in,
                            0x04,
                            5000);
    
    float wavelength = static_cast<float>(*data_in);
    
    return wavelength;
}

float getEntrySlitPosition(libusb_device_handle *devh){
    
    
}
float getExitSlitPosition(libusb_device_handle *devh){
    
    
}
int getGrating(libusb_device_handle *devh){
    
    
}
bool getEntrySlitStatus(libusb_device_handle *devh){
    
    
}
bool getExitSlitStatus(libusb_device_handle *devh){
    
    
}
bool getGratingStatus(libusb_device_handle *devh){
    
    
}
bool getWavelengthStatus(libusb_device_handle *devh){
    
    
}
bool setWavelength(libusb_device_handle *devh, float wavelength){
    
    
}
bool setEntrySlitPosition(libusb_device_handle *devh, float position){
    
    
}
bool setExitSlitPosition(libusb_device_handle *devh, float position){
    
    
}
bool setGrating(libusb_device_handle *devh, int grating){
    
    
}