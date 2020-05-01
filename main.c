/**
 * main.c
 *
 * must send initial torque command (zero) to kick off
 *
 * create header file "simulink.h" for abstraction
 *
 */

#include "simulink.h"
#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"

//#include "driverlib/interrupt.h"
//#include "inc/hw_ints.h"
//#include "driverlib/debug.h"
//#include "driverlib/fpu.h"
//#include "driverlib/rom.h"

const float kP = 22;
const float kD = 8;
const float kI = 1;

int16_t setpoint = 1023;

uint16_t prevError = 0;

int16_t integral = 0;

int16_t derivative = 0;

int main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    UARTStdioConfig(0, 115200, 16000000);

    //The first parameter selects the UART module, in this case it's UART0.
    //The second parameter selects the baud rate, 115200 in here.
    //And last is the system clock or 16Mhz if you select using the PIOSC as source clock for the UART.

    sendTorqueCommand(0);

    while(true)
    {
        int16_t encoderPosition = getSensorState(); // Get sensor state, blocking

        int16_t error = setpoint - encoderPosition; // compute error
        integral = integral + error;

        if (integral > 30000)
        {
            integral = 30000;
        }
        else if (integral < -30000)
        {
            integral = -30000;
        }

        derivative = error - prevError;

        //Control effort in mNm
        int16_t controlEffort = (error)*kP + (derivative)*kD + (integral)*kI;

        prevError = error;

        sendTorqueCommand(controlEffort); // send torque command to motor controller
    }
    return 0;
}
