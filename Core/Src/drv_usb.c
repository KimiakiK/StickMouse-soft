/*
 * drv_usb.c
 *
 *  Created on: Mar 21, 2021
 *      Author: kimi
 */

/********** Include **********/

#include "common_type.h"
#include "usbd_custom_hid_if.h"
#include "drv_sw.h"
#include "drv_joystick.h"
#include "drv_usb.h"

/********** Define **********/

#define POINTER_TABLE_SIZE	20

typedef enum stick_mode {
	STICK_MODE_POINTER = 0,
	STICK_MODE_WHEEL
} stick_mode_t;

/********** Type **********/

typedef struct HID_Mouse {
	uint8_t button;
	int8_t x;
	int8_t y;
	int8_t v_wheel;
	int8_t h_wheel;
} hid_mouse_t;

/********** Constant **********/

const uint8_t pointer_table [POINTER_TABLE_SIZE][POINTER_TABLE_SIZE] = {
/*    1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 */
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 0 */
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 1 */
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 2 */
	{ 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, /* 3 */
	{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0}, /* 4 */
	{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, /* 5 */
	{ 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, /* 6 */
	{ 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, /* 7 */
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, /* 8 */
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0}, /* 9 */
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, /*10 */
	{ 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, /*11 */
	{ 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0}, /*12 */
	{ 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0}, /*13 */
	{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0}, /*14 */
	{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0}, /*15 */
	{ 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0}, /*16 */
	{ 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0}, /*17 */
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0}, /*18 */
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, /*19 */
};

/********** Variable **********/

extern USBD_HandleTypeDef hUsbDeviceFS;

static hid_mouse_t hid_mouse;

static uint8_t table_index;
static uint16_t wheel_count[JOYSTICK_NUM];

static stick_mode_t stick_mode;
static sw_t old_center_sw;

/********** Function Prototype **********/

static void drvUsbInitHid(void);
static void drvUsbMakePointerData(void);
static void drvUsbMakeButtonData(void);

static int8_t drvUsbSmoothPointer(float input);
static uint16_t drvUsbWaitScroll(uint8_t index);

/********** Function **********/

/*=== 初期化関数 ===*/
void DrvUsbInit(void)
{
	table_index = 0;
	wheel_count[JOYSTICK_ID_H] = 0;
	wheel_count[JOYSTICK_ID_V] = 0;

	stick_mode = STICK_MODE_POINTER;
	old_center_sw = SW_OFF;

	drvUsbInitHid();
}

/*=== 周期関数 ===*/
void DrvUsbMain(void)
{
	drvUsbInitHid();

	drvUsbMakePointerData();
	drvUsbMakeButtonData();

	USBD_CUSTOM_HID_SendReport( &hUsbDeviceFS, (uint8_t*) &hid_mouse, sizeof( hid_mouse_t ) );

	table_index ++;
	if (table_index > POINTER_TABLE_SIZE) {
		table_index = 0;
	}
}

static void drvUsbInitHid(void)
{
	hid_mouse.button = 0;
	hid_mouse.x = 0;
	hid_mouse.y = 0;
	hid_mouse.v_wheel = 0;
	hid_mouse.h_wheel = 0;
}

static void drvUsbMakePointerData(void)
{
	sw_t sw_state;
	uint8_t index;

	sw_state = DrvSwGetState(SW_ID_C);

	if (sw_state == SW_ON) {
		if (old_center_sw == SW_OFF) {
			if (stick_mode == STICK_MODE_POINTER) {
				stick_mode = STICK_MODE_WHEEL;
			} else {
				stick_mode = STICK_MODE_POINTER;
			}
		}
	} else {
		if (stick_mode == STICK_MODE_POINTER) {
			/* マウスポインタ移動 */
			hid_mouse.x = drvUsbSmoothPointer(DrvJoystickGetValue(JOYSTICK_ID_H));
			hid_mouse.y = drvUsbSmoothPointer(DrvJoystickGetValue(JOYSTICK_ID_V));
			wheel_count[JOYSTICK_ID_H] = 0;
			wheel_count[JOYSTICK_ID_V] = 0;
		} else {
			/* スクロール */
			for (index=0; index<JOYSTICK_NUM; index++) {
				if (wheel_count[index] > 0) {
					wheel_count[index] --;
				}
				if (wheel_count[index] == 0) {
					wheel_count[index] = drvUsbWaitScroll(index);
				}
			}
		}
	}

	old_center_sw = sw_state;
}

static void drvUsbMakeButtonData(void)
{
	sw_t sw_state;

	sw_state = DrvSwGetState(SW_ID_L);
	if (sw_state == SW_ON) {
		hid_mouse.button |= 0x01;
	} else {
		hid_mouse.button &= 0xFE;
	}

	sw_state = DrvSwGetState(SW_ID_R);
	if (sw_state == SW_ON) {
		hid_mouse.button |= 0x02;
	} else {
		hid_mouse.button &= 0xFD;
	}
}

static int8_t drvUsbSmoothPointer(float input)
{
	volatile int8_t point_input;
	volatile int8_t point;
	volatile uint8_t abs;
	volatile uint8_t index;
	volatile int8_t sign;

	point_input = input * 41.0f;
	
	if (point_input == 0) {
		point = 0;
	} else {

		if (point_input < 0) {
			abs = -point_input;
			sign = -1;
		} else {
			abs = point_input;
			sign = 1;
		}
		index = abs % POINTER_TABLE_SIZE;
		point = (pointer_table[index][table_index] + abs/POINTER_TABLE_SIZE) * sign;
	}

	return point;
}

static uint16_t drvUsbWaitScroll(uint8_t index)
{
	uint16_t wait;
	int8_t sign;
	float input;

	const float wait_max = 250.0f;
	const float wait_min = 50.0f;
	const float alignment = 0.2f;

	input = DrvJoystickGetValue(index);


	if (input > 0.0f) {
		sign = 1;
	} else {
		sign = -1;
		input = -input;
	}

	if (index == JOYSTICK_ID_H) {
		input = input / 4.0f;
	}

	if (input < alignment) {
		wait = 0;
	} else {
		wait = (wait_max + wait_min) - (input * wait_max);

		if (index == JOYSTICK_ID_H) {
			hid_mouse.h_wheel = sign;
		} else {
			hid_mouse.v_wheel = -sign;
		}
	}

	return wait;
}
