/*
 * sys_platform.c
 *
 *  Created on: Mar 14, 2021
 *      Author: kimi
 */

/********** Include **********/

#include "common_type.h"
#include "drv_sw.h"
#include "drv_joystick.h"
#include "drv_usb.h"
#include "drv_led.h"

/********** Define **********/

#define TIMx_SR_UIF		(0x00000001)

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

extern TIM_HandleTypeDef htim2;

uint16_t processing_time;

/********** Function Prototype **********/

/********** Function **********/

/*=== 初期化関数 ===*/
void SysPlatformInit(void)
{
	DrvLedInit();
	DrvSwInit();
	DrvJoystickInit();
	DrvUsbInit();

	/* 周期タイマ開始 */
	HAL_TIM_Base_Start(&htim2);
}

/*=== 周期関数 ===*/
void SysPlatformMain(void)
{
	processing_time = TIM2->CNT;

	DrvSwMain();
	DrvJoystickMain();
	DrvUsbMain();
	DrvLedMain();

	processing_time = TIM2->CNT - processing_time;
	/* 1msタイマー満了待ち */
	while ((TIM2->SR & TIMx_SR_UIF) != TIMx_SR_UIF) {
		/* 処理無し */
	}
	/* タイマーフラグクリア */
	TIM2->SR = 0;
}
