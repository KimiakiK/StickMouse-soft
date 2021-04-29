/*
 * drv_joystick_cfg.h
 *
 *  Created on: 2021/04/25
 *      Author: kimi
 */

#ifndef INC_DRV_JOYSTICK_CFG_H_
#define INC_DRV_JOYSTICK_CFG_H_

/********** Include **********/

#include "common_type.h"
#include "drv_joystick.h"

/********** Define **********/

/********** Type **********/

typedef struct joystick_config {
	int16_t center[JOYSTICK_NUM];
	int16_t center_alignment;
	int16_t position_max;
} joystick_config_t;

/********** Constant **********/

extern const joystick_config_t Joystick_config;

/********** Variable **********/

/********** Function Prototype **********/

#endif /* INC_DRV_JOYSTICK_CFG_H_ */
