/*
 * drv_sw.c
 *
 *  Created on: Mar 18, 2021
 *      Author: kimi
 */

/********** Include **********/

#include "common_type.h"
#include "drv_sw.h"

/********** Define **********/

#define SW_FIX_COUNT	(4)

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

static sw_t sw_raw_state[SW_NUM];
static sw_t sw_fix_state[SW_NUM];
static uint8_t sw_input_count[SW_NUM];

/********** Function Prototype **********/

static sw_t drvSwReadPin(sw_id_t sw_id);

/********** Function **********/

/*=== 初期化関数 ===*/
void DrvSwInit(void)
{
	for (sw_id_t index=0; index<SW_NUM; index++) {
		sw_raw_state[index] = SW_OFF;
		sw_fix_state[index] = SW_OFF;
		sw_input_count[index] = 0;
	}
}

/*=== 周期関数 ===*/
void DrvSwMain(void)
{
	sw_t read_state;

	for (sw_id_t index=0; index<SW_NUM; index++) {
		read_state = drvSwReadPin(index);
		if (sw_raw_state[index] == read_state) {
			sw_input_count[index] ++;
		} else {
			sw_input_count[index] = 0;
		}
		if (sw_input_count[index] >= SW_FIX_COUNT) {
			sw_fix_state[index] = sw_raw_state[index];
		}
		sw_raw_state[index] = read_state;
	}
}

/*=== SW状態取得関数 ===*/
sw_t DrvSwGetState(sw_id_t sw_id)
{
	return sw_fix_state[sw_id];
}

/*=== 端子状態読み出し関数 ===*/
static sw_t drvSwReadPin(sw_id_t sw_id)
{
	sw_t read_state;

	switch (sw_id) {
	case SW_ID_L:
		read_state = (sw_t)HAL_GPIO_ReadPin(BTN_LEFT_GPIO_Port, BTN_LEFT_Pin);
		break;
	case SW_ID_C:
		read_state = (sw_t)HAL_GPIO_ReadPin(JOYSTICK_BTN_GPIO_Port, JOYSTICK_BTN_Pin);
		break;
	case SW_ID_R:
		read_state = (sw_t)HAL_GPIO_ReadPin(BTN_RIGHT_GPIO_Port, BTN_RIGHT_Pin);
		break;
	default:
		read_state = SW_OFF;
		break;
	}

	return read_state;
}
