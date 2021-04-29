/*
 * drv_joystick.h
 *
 *  Created on: Mar 18, 2021
 *      Author: kimi
 */

#ifndef INC_DRV_JOYSTICK_H_
#define INC_DRV_JOYSTICK_H_

/********** Include **********/

/********** Define **********/

typedef enum JoystickId {
	JOYSTICK_ID_V = 0,
	JOYSTICK_ID_H,
	JOYSTICK_NUM
} joystick_id_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void DrvJoystickInit(void);
void DrvJoystickMain(void);
float DrvJoystickGetValue(joystick_id_t joystick_id);

#endif /* INC_DRV_JOYSTICK_H_ */
