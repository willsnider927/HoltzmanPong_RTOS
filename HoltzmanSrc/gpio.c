//***********************************************************************************

// Include files

//***********************************************************************************

#include "gpio.h"
#include "stdlib.h"
#include "os.h"
#include "game_management/game_management.h"

extern OS_Q shield_msg;
extern OS_Q btn_q;
extern OS_SEM laser_semaphore;
extern enum game_state_e gameState;


//***********************************************************************************

// defined files

//***********************************************************************************





//***********************************************************************************

// global variables

//***********************************************************************************





//***********************************************************************************

// function prototypes

//***********************************************************************************





//***********************************************************************************

// functions

//***********************************************************************************
/***************************************************************************//**

 * @brief

 *   Setup gpio pins that are being used for the application.

 ******************************************************************************/

void gpio_open(void)
{
  // Set LEDs to be standard output drive with default off (cleared)

  GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);
  GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);


  // Setup Buttons as inputs
  GPIO_DriveStrengthSet(BUTTON0_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(BUTTON0_port, BUTTON0_pin, gpioModeInput, BUTTON0_default);
  GPIO_DriveStrengthSet(BUTTON1_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(BUTTON1_port, BUTTON1_pin, gpioModeInput, BUTTON1_default);

  GPIO_ExtIntConfig(BUTTON0_port, BUTTON0_pin, BUTTON0_pin, true, true, true);
  GPIO_ExtIntConfig(BUTTON1_port, BUTTON1_pin, BUTTON1_pin, true, true, true);

  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
}


/***************************************************************************//**
 * @brief
 *   Interrupt handler to service pressing of buttons
 ******************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_ATOMIC();
  GPIO_IntClear(GPIO_IntGet());

  RTOS_ERR qErr;
  uint8_t * btn_pressed = malloc(sizeof(uint8_t));
  *btn_pressed = 0;
  if (gameState == IN_PROGRESS) {
    RTOS_ERR semErr;
    if (!GPIO_PinInGet(BUTTON0_port, BUTTON0_pin)) {
        OSSemPost(&laser_semaphore, OS_OPT_POST_1 + OS_OPT_POST_NO_SCHED, &semErr);
        if (semErr.Code) EFM_ASSERT(false);
    }
  } else if (!GPIO_PinInGet(BUTTON0_port, BUTTON0_pin)) {
        OSQPost(&btn_q, btn_pressed, 1, OS_OPT_POST_FIFO, &qErr);
        if (qErr.Code) EFM_ASSERT(false);
  }

  CORE_EXIT_ATOMIC();
}

/***************************************************************************//**
 * @brief
 *   Interrupt handler to service pressing of buttons
 ******************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_ATOMIC();
  GPIO_IntClear(GPIO_IntGet());

  RTOS_ERR qErr;
  uint8_t * btn_pressed = malloc(sizeof(uint8_t));;
  *btn_pressed = 1;
  if (gameState == IN_PROGRESS) {
      uint8_t * button1_msg = malloc(sizeof(uint8_t));
      *button1_msg = !GPIO_PinInGet(BUTTON1_port, BUTTON1_pin);
      OSQPost(&shield_msg, button1_msg, 1, OS_OPT_POST_FIFO, &qErr);
      if (qErr.Code) EFM_ASSERT(false);
  } else if (!GPIO_PinInGet(BUTTON1_port, BUTTON1_pin)) {
      OSQPost(&btn_q, btn_pressed, 1, OS_OPT_POST_FIFO, &qErr);
      if (qErr.Code) EFM_ASSERT(false);
  }

  CORE_EXIT_ATOMIC();
}

void toggle_led(void) {
  if (GPIO_PinOutGet(LED1_port, LED1_pin)) GPIO_PinOutClear(LED1_port, LED1_pin);
  else GPIO_PinOutSet(LED1_port, LED1_pin);
}

