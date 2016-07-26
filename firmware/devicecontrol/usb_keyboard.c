
#include "usb_keyboard.h"
#include "usb_device.h"
#include "usbd_hid.h"


void USB_Send_Key_Press(char c, uint8_t modifier)
{
	uint8_t report[9];
	uint8_t index = 0;
	report[index++] = 0x02;
	report[index++] = 0x00;
	report[index++] = (uint8_t)c;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;

	USBD_HID_SendReport(&hUsbDeviceFS, report, index);
}

void USB_Send_Key_Release(char c)
{
	uint8_t report[9];
	uint8_t index = 0;
	report[index++] = 0x0;
	report[index++] = 0x00;
	report[index++] = (uint8_t)c;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;

	USBD_HID_SendReport(&hUsbDeviceFS, report, index);
}

void USB_Send_All_Keys_Released(void)
{
	uint8_t report[9];
	uint8_t index = 0;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;
	report[index++] = 0x00;

	USBD_HID_SendReport(&hUsbDeviceFS, report, index);
}
