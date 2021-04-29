/*
 * drv_led.c
 *
 *  Created on: Mar 21, 2021
 *      Author: kimi
 */

/********** Include **********/

#include "common_type.h"
#include "drv_led.h"

/********** Define **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

extern TIM_HandleTypeDef htim17;

/********** Function Prototype **********/

/********** Function **********/

/*=== 初期化関数 ===*/
void DrvLedInit(void)
{
	DrvLedSetLedOutput(LED_OUTPUT_ON);
}

/*=== 周期関数 ===*/
void DrvLedMain(void)
{
	/* 処理無し */
}

/*=== LED出力状態設定関数 ===*/
void DrvLedSetLedOutput(led_output_state_t led_output_state)
{
	if (led_output_state == LED_OUTPUT_ON) {
		htim17.Instance->CCR1 = 4900;
		HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
		htim17.Instance->CCER |= 0x0004;
	} else {
		HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);
	}
}
