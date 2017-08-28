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
#include <string.h>
#include <math.h>
#include <time.h>

libusb_device_handle *MonoConnect(){
    
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
                    }
                    
            }
            else
            {
                    fprintf(stderr, "Unable to find the device.\n");
            }
    }
    else
    {
            fprintf(stderr, "Unable to initialize libusb.\n");
    }
    
    return devh;
}

bool MonoDisconnect(libusb_device_handle *devh){
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
                            timeout);
    
    float wavelength = hexToF(data_in);

    fprintf(stderr, "Wavelength: 0x%02X%02X%02X%02X\n", data_in[3], data_in[2], data_in[1], data_in[0]);
    fprintf(stderr, "Wavelength: %f\n", wavelength);

    return wavelength;
}

float hexToF(unsigned char *data_in){
    float number;
    memcpy(&number,data_in,sizeof(float));
    return number;
}

int hexToI(unsigned char *data_in){
    int number;
    memcpy(&number,data_in,sizeof(int));
    return number;
}

float getEntrySideSlitPosition(libusb_device_handle *devh){
    unsigned char data_in[4];
    int result = 0;

    result = libusb_control_transfer(
                            devh,
                            0xc0 ,
                            0xb3,
                            0x0000,
                            0x0020,
                            data_in,
                            0x04,
                            timeout);
    
    float slit_pos = hexToI(data_in)*toSlitmm;
    fprintf(stderr, "slit_pos: %f\n", slit_pos);
    return slit_pos;
}

float getExitSideSlitPosition(libusb_device_handle *devh){
    unsigned char data_in[4];
    int result = 0;

    result = libusb_control_transfer(
                            devh,
                            0xc0 ,
                            0xb3,
                            0x0002,
                            0x0020,
                            data_in,
                            0x04,
                            timeout);
    
    float slit_pos = hexToI(data_in)*toSlitmm;
    fprintf(stderr, "slit_pos: %f\n", slit_pos);
    return slit_pos;
}

//12 Return true if grating 400 is set
bool getGrating1(libusb_device_handle *devh){
    unsigned char data_in[4];
    int result = 0;

    result = libusb_control_transfer(
                            devh,
                            0xc0 ,
                            0xb3,
                            0x0000,
                            0x0012,
                            data_in,
                            0x04,
                            timeout);
    
    int grating = hexToI(data_in);
    if(grating>0) return true;
    return false;
}

//10 Return true if grating 750 is set
bool getGrating2(libusb_device_handle *devh){
    unsigned char data_in[4];
    int result = 0;

    result = libusb_control_transfer(
                            devh,
                            0xc0 ,
                            0xb3,
                            0x0000,
                            0x0010,
                            data_in,
                            0x04,
                            timeout);
    
    int grating = hexToI(data_in);
    if(grating>0) return true;
    return false;
}

//True if 400nm grating is set
bool getGrating400Status(libusb_device_handle *devh){
    bool status = getGrating1(devh);
    readingDelay();
    status = status|getGrating2(devh);
    return !status;
}

//True if 750nm grating is set
bool getGrating750Status(libusb_device_handle *devh){
    bool status = getGrating1(devh);
    readingDelay();
    status = status|getGrating2(devh);
    return status;
}

//Return true if it's moving and 0 if it's stop
bool getSlitsStatus(libusb_device_handle *devh){
    unsigned char data_in[4];
    int result = 0;

    result = libusb_control_transfer(
                            devh,
                            0xc0 ,
                            0xb3,
                            0x0000,
                            0x0090,
                            data_in,
                            0x04,
                            timeout);
    
    int slit = hexToI(data_in);
    if(slit>0) return true;
    return false;
}

//Return true if it's moving and 0 if it's stop
bool getGratingStatus(libusb_device_handle *devh){
    unsigned char data_in[4];
    int result = 0;
    
    result = libusb_control_transfer(
                            devh,
                            0xc0 ,
                            0xb3,
                            0x0000,
                            0x0005,
                            data_in,
                            0x04,
                            timeout);
    
    int slit = hexToI(data_in);
    if(slit>0) return true;
    return false;
}

bool setWavelength(libusb_device_handle *devh, float wavelength){
    unsigned char data_in[4];
    memcpy(data_in,&wavelength,sizeof(float));

    fprintf(stderr, "Wavelength: 0x%02X%02X%02X%02X\n", data_in[3], data_in[2], data_in[1], data_in[0]);

    int result = 0;

    result = libusb_control_transfer(
                            devh,
                            0x40 ,
                            0xb3,
                            0x0000,
                            0x0004,
                            data_in,
                            0x04,
                            timeout);
    
    if(result>=0) return false;
    return true;
}

//Check grating and slit moving status, if all has stopped return true, if it takes longer than timeout or error occurs return false
//After using, wavelength has to be checked....
bool setWavelengthWithStatusCheck(libusb_device_handle *devh, float wavelength, int timeout){
    time_t start = time(0);
    double timeSinceStart = difftime( time(0), start);
    setWavelength(devh,wavelength);
    readingDelay();
    int movingStatus = isMoving(devh);
    while(movingStatus != ALLSTOPPED){
        movingStatus = isMoving(devh);
        timeSinceStart = difftime( time(0), start);
        if(timeSinceStart>timeout) return false;
    }
    return true;
}

bool setEntrySideSlitPosition(libusb_device_handle *devh, float position){
    unsigned int position_int = floor(position/toSlitmm);
    unsigned char data_in[4];
    memcpy(data_in,&position_int,sizeof(unsigned int));
    fprintf(stderr, "SlitPosition: 0x%02X%02X%02X%02X\n", data_in[3], data_in[2], data_in[1], data_in[0]);
    int result = 0;

    result = libusb_control_transfer(
                            devh,
                            0x40 ,
                            0xb3,
                            0x0000,
                            0x00021,
                            data_in,
                            0x04,
                            timeout);
    
    if(result>=0) return false;
    return true;
}
//Check grating and slit moving status, if all has stopped return true, if it takes longer than timeout or error occurs return false
//After using, wavelength has to be checked....
bool setEntrySideSlitPositionWithStatusCheck(libusb_device_handle *devh, float position, int timeout){
    time_t start = time(0);
    double timeSinceStart = difftime( time(0), start);
    setEntrySideSlitPosition(devh,position);
    readingDelay();
    int movingStatus = isMoving(devh);
    while(movingStatus != ALLSTOPPED){
        movingStatus = isMoving(devh);
        timeSinceStart = difftime( time(0), start);
        if(timeSinceStart>timeout) return false;
    }
    return true;
}

bool setExitSideSlitPosition(libusb_device_handle *devh, float position){
    unsigned int position_int = (position/toSlitmm);
    unsigned char data_in[4];
    memcpy(data_in,&position_int,sizeof(unsigned int));
    fprintf(stderr, "ExitSlit set: %f\n", position);
    fprintf(stderr, "ExitSlit set 2: %i\n", position_int);
    fprintf(stderr, "ExitSlit size: %i\n", sizeof(unsigned int));
    fprintf(stderr, "ExitSlit: 0x%02X%02X%02X%02X\n", data_in[3], data_in[2], data_in[1], data_in[0]);
    int result = 0;

    result = libusb_control_transfer(
                            devh,
                            0x40 ,
                            0xb3,
                            0x0002,
                            0x00021,
                            data_in,
                            0x04,
                            timeout);
    
    if(result>=0) return false;
    return true;
}
//Check grating and slit moving status, if all has stopped return true, if it takes longer than timeout or error occurs return false
//After using, wavelength has to be checked....
bool setExitSideSlitPositionWithStatusCheck(libusb_device_handle *devh, float position, int timeout){
    time_t start = time(0);
    double timeSinceStart = difftime( time(0), start);
    setExitSideSlitPosition(devh,position);
    readingDelay();
    int movingStatus = isMoving(devh);
    while(movingStatus != ALLSTOPPED){
        movingStatus = isMoving(devh);
        timeSinceStart = difftime( time(0), start);
        if(timeSinceStart>timeout) return false;
    }
    return true;
}

//Set grating 1 (400) or grating 2 (750)
bool setGrating(libusb_device_handle *devh, int grating){
    
    int result = -1;
    unsigned char data_in[4];
    data_in[3]=0x00;
    data_in[2]=0x00;
    data_in[1]=0x00;
            
    if(grating == 1){
        data_in[0] = 0x00;
        result = libusb_control_transfer(
                                devh,
                                0x40 ,
                                0xb3,
                                0x0000,
                                0x0011,
                                data_in,
                                0x04,
                                timeout);
    }else if(grating == 2){
        data_in[0] = 0x01;
        result = libusb_control_transfer(
                                devh,
                                0x40 ,
                                0xb3,
                                0x0000,
                                0x0011,
                                data_in,
                                0x04,
                                timeout);
    }
    fprintf(stderr, "Grating selected: 0x%02X%02X%02X%02X\n", data_in[3], data_in[2], data_in[1], data_in[0]);
    if(result>=0) return false;
    else return true;
}

//Set grating 1 (400) or grating 2 (750), if timeout is triggered will return false.
bool setGratingWithStatusCheck(libusb_device_handle *devh, int grating, int timeout){
    time_t start = time(0);
    double timeSinceStart = difftime( time(0), start);
    bool status = setGrating(devh, grating);
    readingDelay();
    int movingStatus = isMoving(devh);
    while(movingStatus != ALLSTOPPED){
        movingStatus = isMoving(devh);
        timeSinceStart = difftime( time(0), start);
        if(timeSinceStart>timeout) return false;
    }
    return true;
}

//return 0 if nothing is moving, 1 if the grating is moving and 2 if the slit is moving, 3 if both are moving (this case is not expected)
unsigned int isMoving(libusb_device_handle *devh){
    bool slitStatus;
    bool gratingStatus;
    
    readingDelay();
    slitStatus = getSlitsStatus(devh);
    readingDelay();
    gratingStatus = getGratingStatus(devh);
    
    if(slitStatus&&gratingStatus) return ALLMOVING;
    else if(slitStatus) return SLITMOVING;
    else if(gratingStatus) return GRATINGMOVING;
    return ALLSTOPPED;
}

void readingDelay(){
    usleep(50000);
}

//Initialization, this is a replication from the commands obtained from sniffing the sw. Sniff commands in the repo.
//If status is false, it means there is no error.
bool initTotal(libusb_device_handle *devh){
    bool status = false;
    status != init1(devh);
    readingDelay();
    status != init2(devh);
    readingDelay();
    status != init3(devh);
    readingDelay();
    status != init4(devh);
    readingDelay();
    status != init5(devh);
    readingDelay();
    status != init6(devh);
    return status;
}

//return true when error
bool init1(libusb_device_handle *devh){
    
    int number_tries = 0;
    int tries2=0;
    
    while(number_tries<100){
        int num = init1Aux(devh);
        readingDelay();
        if(num==0){
            tries2++;
            if(tries2>=4) break;
        }else if(num <= 16 && num > 0){
            tries2=0;
        }else if(num>16){
            tries2=0;
            init1AuxSet00(devh);
            readingDelay();
            number_tries++;
        }
    }
    if(number_tries>=100) return true;
    return false;
}
int init1Aux(libusb_device_handle *devh){
    unsigned char data_in[4];
    int result = 0;

    result = libusb_control_transfer(
                            devh,
                            0xc0 ,
                            0xb3,
                            0x0000,
                            0x0001,
                            data_in,
                            0x04,
                            timeout);
    
    int param = hexToI(data_in);
    return param;
}
//If return is 0, all good.
int init1AuxSet00(libusb_device_handle *devh){
    unsigned char data_in[4];
    int result = 0;

    result = libusb_control_transfer(
                            devh,
                            0xc0 ,
                            0xb3,
                            0x0000,
                            0x0000,
                            data_in,
                            0x04,
                            timeout);
    
    int param = hexToI(data_in);
    return param; //Not expected
}

bool init2(libusb_device_handle *devh){
    unsigned char data_in[4];
    int result = 0;

    result = libusb_control_transfer(
                            devh,
                            0xc0 ,
                            0xb3,
                            0x0010,
                            0xffff,
                            data_in,
                            0x40,
                            timeout);
    return false;
}
bool init3(libusb_device_handle *devh){
    bool grating = getGrating2(devh); //What should do if grating is 0(false)???
    return !grating;
}
bool init4(libusb_device_handle *devh){
    unsigned char data_in[2];
    int result = 0;

    data_in[1]=0x00;
    data_in[0]=0x03;
    
    result = libusb_control_transfer(
                            devh,
                            0x40 ,
                            0xb2,
                            0x0000,
                            0x0064,
                            data_in,
                            0x02,
                            timeout);
    return false;
}
bool init5(libusb_device_handle *devh){
    unsigned char data_in[4];
    int result = 0;
    
    result = libusb_control_transfer(
                            devh,
                            0xc0 ,
                            0xb2,
                            0x0000,
                            0x006f,
                            data_in,
                            0x64,
                            timeout);
    return false;
}
bool init6(libusb_device_handle *devh){
    bool grating = getGrating2(devh); //What should do if grating is 0(false)???
    
    time_t start = time(0);
    double timeSinceStart = difftime( time(0), start);
    readingDelay();
    
    int movingStatus = isMoving(devh);
    while(movingStatus != ALLSTOPPED){
        movingStatus = isMoving(devh);
        timeSinceStart = difftime( time(0), start);
        if(timeSinceStart>timeout) return true;
    }
    return false|!grating;
}
bool init7(libusb_device_handle *devh){
    return false;
}
