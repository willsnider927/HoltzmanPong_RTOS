//***********************************************************************************
// Include files
//***********************************************************************************
#include "em_gpio.h"

//***********************************************************************************
// defined files
//***********************************************************************************

// LED 0 pin is
#define LED0_port   gpioPortF
#define LED0_pin    4u
#define LED0_default  false   // Default false (0) = off, true (1) = on
// LED 1 pin is
#define LED1_port   gpioPortF
#define LED1_pin    5u
#define LED1_default  false // Default false (0) = off, true (1) = on
// BUTTON 0 is
#define BUTTON0_port gpioPortF
#define BUTTON0_pin  6u
#define BUTTON0_default false // Default false (0) = not pressed, true (1) = pressed
// BUTTON 1 is
#define BUTTON1_port gpioPortF
#define BUTTON1_pin  7u
#define BUTTON1_default false // Default false (0) = not pressed, true (1) = pressed

//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void gpio_open(void);
