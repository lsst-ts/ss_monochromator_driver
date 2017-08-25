/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   resources.h
 * Author: andres
 *
 * Created on August 24, 2017, 1:16 PM
 * This file contains the USB controls parameters for the 
 * Monochromator iHR320
 */

#ifndef RESOURCES_H
#define RESOURCES_H

#define idVendor    0x0c9b //Jobin Yvon, Inc
#define idProduct    0x0101

#define bmRequestTypeGetJ    0xc0
#define bmRequestTypeSetJ    0x40

static const double toSlitmm = 0.9955/210;
static const int INTERFACE_NUMBER = 0;
static const int timeout = 10000;

#endif /* RESOURCES_H */

