/*
 * drv_joystick.c
 *
 *  Created on: Mar 18, 2021
 *      Author: kimi
 */

/********** Include **********/

#include "math.h"
#include "common_type.h"
#include "drv_joystick_cfg.h"
#include "drv_joystick.h"

/********** Define **********/

#define ADC_TIMEOUT	(100)

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

extern ADC_HandleTypeDef hadc;

static uint16_t joystick_row_value[JOYSTICK_NUM];
static float joystick_fix_value[JOYSTICK_NUM];

static uint16_t joystick_max[JOYSTICK_NUM];
static uint16_t joystick_min[JOYSTICK_NUM];

/********** Function Prototype **********/

static uint16_t drvJoystickReadAdc(joystick_id_t joystick_id);
static void drvJoystickConvert(void);

/********** Function **********/

/*=== 初期化関数 ===*/
void DrvJoystickInit(void)
{
	for (joystick_id_t index=0; index<JOYSTICK_NUM; index++) {
		joystick_row_value[index] = 0;
		joystick_max[index] = 0;
		joystick_min[index] = 0xFFFF;
	}
}

/*=== 周期関数 ===*/
void DrvJoystickMain(void)
{
	for (joystick_id_t index=0; index<JOYSTICK_NUM; index++) {
		joystick_row_value[index] = drvJoystickReadAdc(index);
		
		if (joystick_row_value[index] > joystick_max[index]) {
			joystick_max[index] = joystick_row_value[index];
		}
		if (joystick_row_value[index] < joystick_min[index]) {
			joystick_min[index] = joystick_row_value[index];
		}
	}
	drvJoystickConvert();
}

/*=== ジョイスティック位置取得関数 ===*/
float DrvJoystickGetValue(joystick_id_t joystick_id)
{
	return joystick_fix_value[joystick_id];
}

/*=== AD変換結果読み出し関数 ===*/
static uint16_t drvJoystickReadAdc(joystick_id_t joystick_id)
{
	uint16_t read_value;

	switch (joystick_id) {
	case JOYSTICK_ID_V:
		hadc.Instance->CHSELR = 0x00000001;
		break;
	case JOYSTICK_ID_H:
		hadc.Instance->CHSELR = 0x00000002;
		break;
	default:
		hadc.Instance->CHSELR = 0x00000001;
		break;
	}

	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, ADC_TIMEOUT);
	read_value = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);

	return read_value;
}

/*=== アナログ値補正関数 ===*/
static void drvJoystickConvert(void)
{
	volatile int16_t input;
	volatile float position;
	uint8_t joystick_id;

	for (joystick_id=0; joystick_id<JOYSTICK_NUM; joystick_id++) {
		/* 中央を基準に符号付きに変換、向きも合わせる */
		input = - (joystick_row_value[joystick_id] - Joystick_config.center[joystick_id]);
		/* 中央のブレを切り捨てる */
		if (input > Joystick_config.center_alignment) {
			input -= Joystick_config.center_alignment;
		} else if (input < - Joystick_config.center_alignment) {
			input += Joystick_config.center_alignment;
		} else {
			input = 0;
		}
		/* 最大値で丸める */
		if (input > Joystick_config.position_max - Joystick_config.center_alignment) {
			input = Joystick_config.position_max - Joystick_config.center_alignment;
		} else if (input < - (Joystick_config.position_max - Joystick_config.center_alignment)) {
			input = - (Joystick_config.position_max - Joystick_config.center_alignment);
		}
		/* 位置(-1.0〜1.0)を計算 */
		joystick_fix_value[joystick_id] = ((float)input / (float)(Joystick_config.position_max - Joystick_config.center_alignment));
	}
}
