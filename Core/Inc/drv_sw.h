/*
 * drv_sw.h
 *
 *  Created on: Mar 18, 2021
 *      Author: kimi
 */

#ifndef INC_DRV_SW_H_
#define INC_DRV_SW_H_

/********** Include **********/

/********** Define **********/

typedef enum SwState {
	SW_OFF = GPIO_PIN_SET,
	SW_ON = GPIO_PIN_RESET
} sw_t;

typedef enum SwId {
	SW_ID_L = 0,
	SW_ID_C,
	SW_ID_R,
	SW_NUM
} sw_id_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void DrvSwInit(void);
void DrvSwMain(void);
sw_t DrvSwGetState(sw_id_t sw_id);

#endif /* INC_DRV_SW_H_ */
