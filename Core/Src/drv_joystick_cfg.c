/*
 * drv_joystick_cfg.c
 *
 *  Created on: 2021/04/25
 *      Author: kimi
 */

/********** Include **********/

#include "common_type.h"
#include "drv_joystick.h"
#include "drv_joystick_cfg.h"

/********** Define **********/

/********** Type **********/

/********** Constant **********/

#if 0
/* 一号機（青色） */
const joystick_config_t Joystick_config = {
	{			/* center */
		1830,	/* JOYSTICK V */
		2100	/* JOYSTICK H */
	},
	120,		/* center_alignment */
	900,		/* position_max */
};
#endif
#if 1
/* 二号機（透明） */
const joystick_config_t Joystick_config = {
	{			/* center */
		1900,	/* JOYSTICK V */
		2000	/* JOYSTICK H */
	},
	140,		/* center_alignment */
	900,		/* position_max */
};
#endif

/********** Variable **********/

/********** Function Prototype **********/

/********** Function **********/


