
#include "chaos-uhid.h"

using namespace Chaos;

static __u8 rdesc[] = {
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x05,        // Usage (Game Pad)
	0xA1, 0x01,        // Collection (Application)
	0x85, 0x01,        //   Report ID (1)
	0x09, 0x30,        //   Usage (X)
	0x09, 0x31,        //   Usage (Y)
	0x09, 0x32,        //   Usage (Z)
	0x09, 0x35,        //   Usage (Rz)
	0x15, 0x00,        //   Logical Minimum (0)
	0x26, 0xFF, 0x00,  //   Logical Maximum (255)
	0x75, 0x08,        //   Report Size (8)
	0x95, 0x04,        //   Report Count (4)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x09, 0x39,        //   Usage (Hat switch)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x07,        //   Logical Maximum (7)
	0x35, 0x00,        //   Physical Minimum (0)
	0x46, 0x3B, 0x01,  //   Physical Maximum (315)
	0x65, 0x14,        //   Unit (System: English Rotation, Length: Centimeter)
	0x75, 0x04,        //   Report Size (4)
	0x95, 0x01,        //   Report Count (1)
	0x81, 0x42,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
	0x65, 0x00,        //   Unit (None)
	0x05, 0x09,        //   Usage Page (Button)
	0x19, 0x01,        //   Usage Minimum (0x01)
	0x29, 0x0E,        //   Usage Maximum (0x0E)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x01,        //   Logical Maximum (1)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x0E,        //   Report Count (14)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
	0x09, 0x20,        //   Usage (0x20)
	0x75, 0x06,        //   Report Size (6)
	0x95, 0x01,        //   Report Count (1)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x7F,        //   Logical Maximum (127)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
	0x09, 0x33,        //   Usage (Rx)
	0x09, 0x34,        //   Usage (Ry)
	0x15, 0x00,        //   Logical Minimum (0)
	0x26, 0xFF, 0x00,  //   Logical Maximum (255)
	0x75, 0x08,        //   Report Size (8)
	0x95, 0x02,        //   Report Count (2)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
	0x09, 0x21,        //   Usage (0x21)
	0x95, 0x36,        //   Report Count (54)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x85, 0x05,        //   Report ID (5)
	0x09, 0x22,        //   Usage (0x22)
	0x95, 0x1F,        //   Report Count (31)
	0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x04,        //   Report ID (4)
	0x09, 0x23,        //   Usage (0x23)
	0x95, 0x24,        //   Report Count (36)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x02,        //   Report ID (2)
	0x09, 0x24,        //   Usage (0x24)
	0x95, 0x24,        //   Report Count (36)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x08,        //   Report ID (8)
	0x09, 0x25,        //   Usage (0x25)
	0x95, 0x03,        //   Report Count (3)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x10,        //   Report ID (16)
	0x09, 0x26,        //   Usage (0x26)
	0x95, 0x04,        //   Report Count (4)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x11,        //   Report ID (17)
	0x09, 0x27,        //   Usage (0x27)
	0x95, 0x02,        //   Report Count (2)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x12,        //   Report ID (18)
	0x06, 0x02, 0xFF,  //   Usage Page (Vendor Defined 0xFF02)
	0x09, 0x21,        //   Usage (0x21)
	0x95, 0x0F,        //   Report Count (15)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x13,        //   Report ID (19)
	0x09, 0x22,        //   Usage (0x22)
	0x95, 0x16,        //   Report Count (22)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x14,        //   Report ID (20)
	0x06, 0x05, 0xFF,  //   Usage Page (Vendor Defined 0xFF05)
	0x09, 0x20,        //   Usage (0x20)
	0x95, 0x10,        //   Report Count (16)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x15,        //   Report ID (21)
	0x09, 0x21,        //   Usage (0x21)
	0x95, 0x2C,        //   Report Count (44)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x06, 0x80, 0xFF,  //   Usage Page (Vendor Defined 0xFF80)
	0x85, 0x80,        //   Report ID (-128)
	0x09, 0x20,        //   Usage (0x20)
	0x95, 0x06,        //   Report Count (6)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x81,        //   Report ID (-127)
	0x09, 0x21,        //   Usage (0x21)
	0x95, 0x06,        //   Report Count (6)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x82,        //   Report ID (-126)
	0x09, 0x22,        //   Usage (0x22)
	0x95, 0x05,        //   Report Count (5)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x83,        //   Report ID (-125)
	0x09, 0x23,        //   Usage (0x23)
	0x95, 0x01,        //   Report Count (1)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x84,        //   Report ID (-124)
	0x09, 0x24,        //   Usage (0x24)
	0x95, 0x04,        //   Report Count (4)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x85,        //   Report ID (-123)
	0x09, 0x25,        //   Usage (0x25)
	0x95, 0x06,        //   Report Count (6)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x86,        //   Report ID (-122)
	0x09, 0x26,        //   Usage (0x26)
	0x95, 0x06,        //   Report Count (6)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x87,        //   Report ID (-121)
	0x09, 0x27,        //   Usage (0x27)
	0x95, 0x23,        //   Report Count (35)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x88,        //   Report ID (-120)
	0x09, 0x28,        //   Usage (0x28)
	0x95, 0x3F,        //   Report Count (63)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x89,        //   Report ID (-119)
	0x09, 0x29,        //   Usage (0x29)
	0x95, 0x02,        //   Report Count (2)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x90,        //   Report ID (-112)
	0x09, 0x30,        //   Usage (0x30)
	0x95, 0x05,        //   Report Count (5)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x91,        //   Report ID (-111)
	0x09, 0x31,        //   Usage (0x31)
	0x95, 0x03,        //   Report Count (3)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x92,        //   Report ID (-110)
	0x09, 0x32,        //   Usage (0x32)
	0x95, 0x03,        //   Report Count (3)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x93,        //   Report ID (-109)
	0x09, 0x33,        //   Usage (0x33)
	0x95, 0x0C,        //   Report Count (12)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x94,        //   Report ID (-108)
	0x09, 0x34,        //   Usage (0x34)
	0x95, 0x3F,        //   Report Count (63)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xA0,        //   Report ID (-96)
	0x09, 0x40,        //   Usage (0x40)
	0x95, 0x06,        //   Report Count (6)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xA1,        //   Report ID (-95)
	0x09, 0x41,        //   Usage (0x41)
	0x95, 0x01,        //   Report Count (1)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xA2,        //   Report ID (-94)
	0x09, 0x42,        //   Usage (0x42)
	0x95, 0x01,        //   Report Count (1)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xA3,        //   Report ID (-93)
	0x09, 0x43,        //   Usage (0x43)
	0x95, 0x30,        //   Report Count (48)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xA4,        //   Report ID (-92)
	0x09, 0x44,        //   Usage (0x44)
	0x95, 0x0D,        //   Report Count (13)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xF0,        //   Report ID (-16)
	0x09, 0x47,        //   Usage (0x47)
	0x95, 0x3F,        //   Report Count (63)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xF1,        //   Report ID (-15)
	0x09, 0x48,        //   Usage (0x48)
	0x95, 0x3F,        //   Report Count (63)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xF2,        //   Report ID (-14)
	0x09, 0x49,        //   Usage (0x49)
	0x95, 0x0F,        //   Report Count (15)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xA7,        //   Report ID (-89)
	0x09, 0x4A,        //   Usage (0x4A)
	0x95, 0x01,        //   Report Count (1)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xA8,        //   Report ID (-88)
	0x09, 0x4B,        //   Usage (0x4B)
	0x95, 0x01,        //   Report Count (1)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xA9,        //   Report ID (-87)
	0x09, 0x4C,        //   Usage (0x4C)
	0x95, 0x08,        //   Report Count (8)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xAA,        //   Report ID (-86)
	0x09, 0x4E,        //   Usage (0x4E)
	0x95, 0x01,        //   Report Count (1)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xAB,        //   Report ID (-85)
	0x09, 0x4F,        //   Usage (0x4F)
	0x95, 0x39,        //   Report Count (57)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xAC,        //   Report ID (-84)
	0x09, 0x50,        //   Usage (0x50)
	0x95, 0x39,        //   Report Count (57)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xAD,        //   Report ID (-83)
	0x09, 0x51,        //   Usage (0x51)
	0x95, 0x0B,        //   Report Count (11)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xAE,        //   Report ID (-82)
	0x09, 0x52,        //   Usage (0x52)
	0x95, 0x01,        //   Report Count (1)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xAF,        //   Report ID (-81)
	0x09, 0x53,        //   Usage (0x53)
	0x95, 0x02,        //   Report Count (2)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xB0,        //   Report ID (-80)
	0x09, 0x54,        //   Usage (0x54)
	0x95, 0x3F,        //   Report Count (63)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xE0,        //   Report ID (-32)
	0x09, 0x57,        //   Usage (0x57)
	0x95, 0x02,        //   Report Count (2)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xB3,        //   Report ID (-77)
	0x09, 0x55,        //   Usage (0x55)
	0x95, 0x3F,        //   Report Count (63)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xB4,        //   Report ID (-76)
	0x09, 0x55,        //   Usage (0x55)
	0x95, 0x3F,        //   Report Count (63)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xB5,        //   Report ID (-75)
	0x09, 0x56,        //   Usage (0x56)
	0x95, 0x3F,        //   Report Count (63)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xD0,        //   Report ID (-48)
	0x09, 0x58,        //   Usage (0x58)
	0x95, 0x3F,        //   Report Count (63)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0xD4,        //   Report ID (-44)
	0x09, 0x59,        //   Usage (0x59)
	0x95, 0x3F,        //   Report Count (63)
	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0xC0,              // End Collection

	// 507 bytes

};

static int uhid_write(int fd, const struct uhid_event *ev)
{
	ssize_t ret;
	
	ret = write(fd, ev, sizeof(*ev));
	if (ret < 0) {
		fprintf(stderr, "Cannot write to uhid: %m\n");
		return -errno;
	} else if (ret != sizeof(*ev)) {
		fprintf(stderr, "Wrong size written to uhid: %zd != %zu\n",
				ret, sizeof(ev));
		return -EFAULT;
	} else {
		return 0;
	}
}

static int create(int fd)
{
	struct uhid_event ev;
	
	memset(&ev, 0, sizeof(ev));
	ev.type = UHID_CREATE2;
	strcpy((char*)ev.u.create2.name, "test-uhid-device");
	//ev.u.create2.rd_data = rdesc;
	memcpy(ev.u.create2.rd_data, rdesc, 507);
	ev.u.create2.rd_size = sizeof(rdesc);
	ev.u.create2.bus = BUS_USB;
	ev.u.create2.vendor = 0x2f24;//  && product==0x00f8;
	ev.u.create2.product = 0x00f8;
	ev.u.create2.version = 100;
	ev.u.create2.country = 0;
	
	return uhid_write(fd, &ev);
}

static void destroy(int fd)
{
	struct uhid_event ev;
	
	memset(&ev, 0, sizeof(ev));
	ev.type = UHID_DESTROY;
	
	uhid_write(fd, &ev);
}

/* This parses raw output reports sent by the kernel to the device. A normal
 * uhid program shouldn't do this but instead just forward the raw report.
 * However, for ducomentational purposes, we try to detect LED events here and
 * print debug messages for it. */
static void handle_output(struct uhid_event *ev)
{
	/* LED messages are adverised via OUTPUT reports; ignore the rest */
	if (ev->u.output.rtype != UHID_OUTPUT_REPORT)
		return;
	/* LED reports have length 2 bytes */
	if (ev->u.output.size != 2)
		return;
	/* first byte is report-id which is 0x02 for LEDs in our rdesc */
	if (ev->u.output.data[0] != 0x2)
		return;
	
	/* print flags payload */
	fprintf(stderr, "LED output report received with flags %x\n",
			ev->u.output.data[1]);
}

static int event(int fd)
{
	struct uhid_event ev;
	ssize_t ret;
	
	memset(&ev, 0, sizeof(ev));
	ret = read(fd, &ev, sizeof(ev));
	if (ret == 0) {
		fprintf(stderr, "Read HUP on uhid-cdev\n");
		return -EFAULT;
	} else if (ret < 0) {
		fprintf(stderr, "Cannot read uhid-cdev: %m\n");
		return -errno;
	} else if (ret != sizeof(ev)) {
		fprintf(stderr, "Invalid size read from uhid-dev: %zd != %zu\n",
				ret, sizeof(ev));
		return -EFAULT;
	}
	
	switch (ev.type) {
		case UHID_START:
			fprintf(stderr, "UHID_START from uhid-dev\n");
			break;
		case UHID_STOP:
			fprintf(stderr, "UHID_STOP from uhid-dev\n");
			break;
		case UHID_OPEN:
			fprintf(stderr, "UHID_OPEN from uhid-dev\n");
			break;
		case UHID_CLOSE:
			fprintf(stderr, "UHID_CLOSE from uhid-dev\n");
			break;
		case UHID_OUTPUT:
			fprintf(stderr, "UHID_OUTPUT from uhid-dev\n");
			handle_output(&ev);
			break;
		case UHID_OUTPUT_EV:
			fprintf(stderr, "UHID_OUTPUT_EV from uhid-dev\n");
			break;
		default:
			fprintf(stderr, "Invalid event from uhid-dev: %u\n", ev.type);
	}
	
	return 0;
}

//static bool btn1_down;
//static bool btn2_down;
//static bool btn3_down;
//static signed char abs_hor;
//static signed char abs_ver;
//static signed char wheel;

static int send_event(int fd, void* data)
{
	struct uhid_event ev;
	
	memset(&ev, 0, sizeof(ev));
	ev.type = UHID_INPUT2;
	ev.u.input2.size = 64;
	
	//	ev.u.input.data[0] = 0x1;
	//	if (btn1_down)
	//		ev.u.input.data[1] |= 0x1;
	//	if (btn2_down)
	//		ev.u.input.data[1] |= 0x2;
	//	if (btn3_down)
	//		ev.u.input.data[1] |= 0x4;
	//
	//	ev.u.input.data[2] = abs_hor;
	//	ev.u.input.data[3] = abs_ver;
	//	ev.u.input.data[4] = wheel;
	
	memcpy(ev.u.input2.data, data, 64);	// hardcoded size
	//ev.u.input.data[0] = 0x1;
//	for (int i = 0; i < 64; i++) {
//		printf("%02x ", ((unsigned char*)data)[i]);
//	}
//	printf("\n");
	return uhid_write(fd, &ev);
}

//static int keyboard(int fd)
//{
//	char buf[128];
//	ssize_t ret, i;
//
//	ret = read(STDIN_FILENO, buf, sizeof(buf));
//	if (ret == 0) {
//		fprintf(stderr, "Read HUP on stdin\n");
//		return -EFAULT;
//	} else if (ret < 0) {
//		fprintf(stderr, "Cannot read stdin: %m\n");
//		return -errno;
//	}
//
//	for (i = 0; i < ret; ++i) {
//		switch (buf[i]) {
//		case '1':
//			btn1_down = !btn1_down;
//			ret = send_event(fd);
//			if (ret)
//				return ret;
//			break;
//		case '2':
//			btn2_down = !btn2_down;
//			ret = send_event(fd);
//			if (ret)
//				return ret;
//			break;
//		case '3':
//			btn3_down = !btn3_down;
//			ret = send_event(fd);
//			if (ret)
//				return ret;
//			break;
//		case 'a':
//			abs_hor = -20;
//			ret = send_event(fd);
//			abs_hor = 0;
//			if (ret)
//				return ret;
//			break;
//		case 'd':
//			abs_hor = 20;
//			ret = send_event(fd);
//			abs_hor = 0;
//			if (ret)
//				return ret;
//			break;
//		case 'w':
//			abs_ver = -20;
//			ret = send_event(fd);
//			abs_ver = 0;
//			if (ret)
//				return ret;
//			break;
//		case 's':
//			abs_ver = 20;
//			ret = send_event(fd);
//			abs_ver = 0;
//			if (ret)
//				return ret;
//			break;
//		case 'r':
//			wheel = 1;
//			ret = send_event(fd);
//			wheel = 0;
//			if (ret)
//				return ret;
//			break;
//		case 'f':
//			wheel = -1;
//			ret = send_event(fd);
//			wheel = 0;
//			if (ret)
//				return ret;
//			break;
//		case 'q':
//			return -ECANCELED;
//		default:
//			fprintf(stderr, "Invalid input: %c\n", buf[i]);
//		}
//	}
//
//	return 0;
//}



ChaosUhid::ChaosUhid(ControllerState* state) {
	this->stateToPoll = state;
//	ret = tcgetattr(STDIN_FILENO, &state);
//	if (ret) {
//		fprintf(stderr, "Cannot get tty state\n");
//	} else {
//		state.c_lflag &= ~ICANON;
//		state.c_cc[VMIN] = 1;
//		ret = tcsetattr(STDIN_FILENO, TCSANOW, &state);
//		if (ret)
//			fprintf(stderr, "Cannot set tty state\n");
//	}
	
//	if (argc >= 2) {
//		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
//			fprintf(stderr, "Usage: %s [%s]\n", argv[0], path);
//			return EXIT_SUCCESS;
//		} else {
//			path = argv[1];
//		}
//	}
	path = "/dev/uhid";
	
	fprintf(stderr, "Open uhid-cdev %s\n", path);
	fd = open(path, O_RDWR | O_CLOEXEC | O_NONBLOCK);
	if (fd < 0) {
		fprintf(stderr, "Cannot open uhid-cdev %s: %m\n", path);
		exit(EXIT_FAILURE);
	}
	
	fprintf(stderr, "Create uhid device\n");
	ret = create(fd);
	if (ret) {
		close(fd);
		exit(EXIT_FAILURE);
	}
	
	pfds[0].fd = fd;//STDIN_FILENO;
	pfds[0].events = POLLIN;
//	pfds[1].fd = fd;
//	pfds[1].events = POLLIN;
	
	fprintf(stderr, "Press 'q' to quit...\n");
}

ChaosUhid::~ChaosUhid() {
	destroy(fd);
}

void ChaosUhid::doAction() {
	//printf("Polling?\n");
	ret = poll(pfds, 1, 10);
	if (ret < 0) {
		fprintf(stderr, "Cannot poll for fds: %m\n");
		return;
	}
	if (pfds[0].revents & POLLHUP) {
		fprintf(stderr, "Received HUP on uhid-cdev\n");
		return;
	}
	
	
	//printf("Sending events\n");
	ret = send_event(fd, this->stateToPoll->getHackedState());
	if (ret)
		return;
	
	//printf("Event Handle\n");
	if (pfds[0].revents & POLLIN) {
		ret = event(fd);
		if (ret) {
			fprintf(stderr, "ret = event(fd);\n");
			return;
		}
	}
//	printf("Reall Nah\n");
}

void ChaosUhid::sendUpdate(void* data) {
	
}

