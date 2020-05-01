/*
 * simulink.h
 *
 *  Created on: Apr 19, 2020
 *      Author: mackenziegray
 */
#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123bh6pm.h"

#ifndef SIMULINK_H_
#define SIMULINK_H_

uint16_t getSensorState(void); //reads from simulink

void sendTorqueCommand(uint16_t torque); //sends torque command to motor controller

#endif /* SIMULINK_H_ */
