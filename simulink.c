/*
 * simulink.c
 *
 *  Created on: Apr 19, 2020
 *      Author: mackenziegray
 */
#include "simulink.h"
#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123bh6pm.h"

#include "simulink.h"

#include "uartstdio.h"

uint16_t getSensorState(void)
{
    uint8_t terminator = 0xFF;

    while (true)
    {
        //receive encoder position data
        uint8_t encoderPositionLSB = UARTgetc();
        uint8_t encoderPositionMSB = UARTgetc();
        terminator = UARTgetc();

        if(terminator ==0)
        {
            return encoderPositionLSB + (encoderPositionMSB<<8);
        }
     }
}

void sendTorqueCommand(uint16_t torque)
{
    char commandArray[] = {torque & 0xFF, torque >> 8, 0};
    UARTwrite(commandArray, 3); //write command with null terminator
}
