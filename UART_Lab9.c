#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/uart.h"

#define 	RED_MASK 		0x02
#define 	BLUE_MASK 		0x04
#define 	GREEN_MASK 		0x08
#define 	OFF 				0x00



void
PortFunctionInit(void) {

    volatile uint32_t ui32Loop;

    // Enable the clock of the GPIO port that is used for the on-board LED and switch.
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

    //
    // Do a dummy read to insert a few cycles after enabling the peripheral.
    //
    ui32Loop = SYSCTL_RCGC2_R;

    // Unlock GPIO Port F
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R |= 0x01; // allow changes to PF0

    //
    // Enable pin PF1 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
		
	  //
    // Enable pin PF2 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);


		//
    // Enable pin PF3 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);


}

void UART_int_init()
{
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);


    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
		UARTFlowControlSet(UART0_BASE,UART_FLOWCONTROL_RX);
		IntPrioritySet(INT_UART0, 0x00);
    IntMasterEnable(); //enable processor interrupts
    IntEnable(INT_UART0); //enable the UART interrupt
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts

}


void UARTIntHandler(void)
{
    uint32_t ui32Status;
    UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts
    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
		
		char newChar;
		newChar = UARTCharGet(UART0_BASE);
	
		if (newChar != NULL) {
			switch (newChar) {
			// Red LED is ON, prints R to terminal
			case 'R':
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, RED_MASK); 
				UARTCharPut(UART0_BASE, newChar); 
				break;
			
			// Red LED is OFF, prints r to terminal
			case 'r':
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, OFF); 
				UARTCharPut(UART0_BASE, newChar);
				break;
			
			// Blue LED is ON, prints B to terminal
			case 'B':
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, BLUE_MASK); 
				UARTCharPut(UART0_BASE, newChar); 
				break;
			
			// Blue LED is OFF, prints b to terminal
			case 'b':
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, OFF); 
				UARTCharPut(UART0_BASE, newChar); 
				break;
			
			// Green LED is ON, prints G to terminal
			case 'G':
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GREEN_MASK); 
				UARTCharPut(UART0_BASE, newChar); 
				break;
			
			// Green LED is OFF, prints g to terminal
			case 'g':
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, OFF); 
				UARTCharPut(UART0_BASE, newChar); 
				break;
			
			// prints "Invalid Input" to terminal
			default: 
				UARTCharPut(UART0_BASE, '\r');
				UARTCharPut(UART0_BASE, '\n');
				UARTCharPut(UART0_BASE, 'I');
				UARTCharPut(UART0_BASE, 'n');
				UARTCharPut(UART0_BASE, 'v');
				UARTCharPut(UART0_BASE, 'a');
				UARTCharPut(UART0_BASE, 'l');
				UARTCharPut(UART0_BASE, 'i');
				UARTCharPut(UART0_BASE, 'd');
				UARTCharPut(UART0_BASE, ' ');
				UARTCharPut(UART0_BASE, 'I');
				UARTCharPut(UART0_BASE, 'n');
				UARTCharPut(UART0_BASE, 'p');
				UARTCharPut(UART0_BASE, 'u');
				UARTCharPut(UART0_BASE, 't');
				UARTCharPut(UART0_BASE, ' ');
				UARTCharPut(UART0_BASE, '\n');
				break;
			}
			// set the char to NULL to get new inputs
			newChar = NULL; 
		}

}

int main(void) {
	
		PortFunctionInit();

		UART_int_init();

		UARTCharPut(UART0_BASE, '\r');
		UARTCharPut(UART0_BASE, '\n');
    UARTCharPut(UART0_BASE, 'E');
    UARTCharPut(UART0_BASE, 'n');
    UARTCharPut(UART0_BASE, 't');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'r');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'K');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'y');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'I');
    UARTCharPut(UART0_BASE, 'n');
		UARTCharPut(UART0_BASE, 'p');
		UARTCharPut(UART0_BASE, 'u');
		UARTCharPut(UART0_BASE, 't');	
		UARTCharPut(UART0_BASE, ':');	
		UARTCharPut(UART0_BASE, ' ');	
		
		while (1) //let interrupt handler do the UART echo function
		{

		}

}
