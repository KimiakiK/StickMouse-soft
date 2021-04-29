/*
 * drv_led.h
 *
 *  Created on: Mar 21, 2021
 *      Author: kimi
 */

#ifndef INC_DRV_LED_H_
#define INC_DRV_LED_H_

/********** Include **********/

/********** Define **********/

/********** Type **********/

typedef enum LedOutputState {
	LED_OUTPUT_OFF = 0,
	LED_OUTPUT_ON,
} led_output_state_t;

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void DrvLedInit(void);
void DrvLedMain(void);
void DrvLedSetLedOutput(led_output_state_t led_output_state);

#endif /* INC_DRV_LED_H_ */
