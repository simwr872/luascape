#include "Keyboard.h"


//
//   FUNCTION: PressKey(char c)
//
//   PURPOSE:  Sends a keypress to the RuneScape client.
//
//   COMMENTS:
//
//        Only sends lowercase characters.
//
bool Keyboard::PressKey(char c) {
	Key key(c);
	if (!key.Valid()) return false;

	// See header file for reasoning of wparam and lparam
	PostMessage(client, WM_KEYDOWN, key.WParam(), key.LParam(WM_KEYDOWN));

	// WM_CHAR message is generated when translating a posted
	// WM_KEYDOWN message.
	//PostMessage(client, WM_CHAR, key.WParam(), key.LParam(WM_CHAR));

	Sleep(rand() % (130 - 70 + 1) + 70);

	PostMessage(client, WM_KEYUP, key.WParam(), key.LParam(WM_KEYUP));

	return true;
}

Keyboard::Keyboard(HWND _client) {
	client = _client;
}


// Scan codes for every key in the standard 105-key ISO-layout
// http://www.quadibloc.com/comp/scan.htm
//  ---     ---------------   ---------------   ---------------   -----------
// | 01|   | 3B| 3C| 3D| 3E| | 3F| 40| 41| 42| | 43| 44| 57| 58| |+37|+46|+45|
//  ---     ---------------   ---------------   ---------------   -----------
//
//  -----------------------------------------------------------   -----------   ---------------
// | 29| 02| 03| 04| 05| 06| 07| 08| 09| 0A| 0B| 0C| 0D|     0E| |*52|*47|*49| |+45|+35|+37| 4A|
// |-----------------------------------------------------------| |-----------| |---------------|
// |   0F| 10| 11| 12| 13| 14| 15| 16| 17| 18| 19| 1A| 1B|     | |*53|*4F|*51| | 47| 48| 49|   |
// |------------------------------------------------------|  1C|  -----------  |-----------| 4E|
// |    3A| 1E| 1F| 20| 21| 22| 23| 24| 25| 26| 27| 28|!2B|    |               | 4B| 4C| 4D|   |
// |-----------------------------------------------------------|      ---      |---------------|
// |  2A| 56| 2C| 2D| 2E| 2F| 30| 31| 32| 33| 34| 35|        36|     |*4C|     | 4F| 50| 51|   |
// |-----------------------------------------------------------|  -----------  |-----------|-1C|
// |   1D|-5B|   38|                       39|-38|-5C|-5D|  -1D| |*4B|*50|*4D| |     52| 53|   |
//  -----------------------------------------------------------   -----------   ---------------

// A lookup list translating an ascii number to the
// corresponding scan code.
const int Keyboard::ascii[128] = {
	0x00,	// 00	NUL		Null
	0x00,	// 01	SOH		Start of Header
	0x00,	// 02	STX		Start of Text
	0x00,	// 03	ETX		End of Text
	0x00,	// 04	EOT		End of Trans.
	0x00,	// 05	ENQ		Enquiry
	0x00,	// 06	ACK		Acknowledgement
	0x00,	// 07	BEL		Bell
	0x0e,	// 08	BS		Backspace
	0x0f,	// 09	HT		Horizontal Tab
	0x00,	// 10	LF		Line feed
	0x00,	// 11	VT		Vertical Tab
	0x00,	// 12	FF		Form feed
	0x1c,	// 13	CR		Carriage return
	0x00,	// 14	SO		Shift Out
	0x00,	// 15	SI		Shift In
	0x00,	// 16	DLE		Data link escape
	0x00,	// 17	DC1		Device control 1
	0x00,	// 18	DC2		Device control 2
	0x00,	// 19	DC3		Device control 3
	0x00,	// 20	DC4		Device control 4
	0x00,	// 21	NAK		Negative acknowl.
	0x00,	// 22	SYN		Synchronous idle
	0x00,	// 23	ETB		End of trans. block
	0x00,	// 24	CAN		Cancel
	0x00,	// 25	EM		End of medium
	0x00,	// 26	SUB		Substitute
	0x01,	// 27	ESC		Escape
	0x00,	// 28	FS		File separator
	0x00,	// 29	GS		Group separator
	0x00,	// 30	RS		Record separator
	0x00,	// 31	US		Unit separator
	0x39,	// 32	space
	0x02,	// 33	!
	0x03,	// 34	"
	0x2b,	// 35	#
	0x05,	// 36	$
	0x06,	// 37	%
	0x08,	// 38	&
	0x28,	// 39	'
	0x0a,	// 40	(
	0x0b,	// 41	)
	0x09,	// 42	*
	0x0d,	// 43	+
	0x33,	// 44	,
	0x0c,	// 45	-
	0x34,	// 46	.
	0x35,	// 47	/
	0x0b,	// 48	0
	0x02,	// 49	1
	0x03,	// 50	2
	0x04,	// 51	3
	0x05,	// 52	4
	0x06,	// 53	5
	0x07,	// 54	6
	0x08,	// 55	7
	0x09,	// 56	8
	0x0a,	// 57	9
	0x27,	// 58	:
	0x27,	// 59	;
	0x33,	// 60	<
	0x0d,	// 61	=
	0x34,	// 62	>
	0x35,	// 63	?
	0x28,	// 64	@
	0x1e,	// 65	A
	0x30,	// 66	B
	0x2e,	// 67	C
	0x20,	// 68	D
	0x12,	// 69	E
	0x21,	// 70	F
	0x22,	// 71	G
	0x23,	// 72	H
	0x17,	// 73	I
	0x24,	// 74	J
	0x25,	// 75	K
	0x26,	// 76	L
	0x32,	// 77	M
	0x31,	// 78	N
	0x18,	// 79	O
	0x19,	// 80	P
	0x10,	// 81	Q
	0x13,	// 82	R
	0x1f,	// 83	S
	0x14,	// 84	T
	0x16,	// 85	U
	0x2f,	// 86	V
	0x11,	// 87	W
	0x2d,	// 88	X
	0x15,	// 89	Y
	0x2c,	// 90	Z
	0x1a,	// 91	[
	0x56,	// 92	backslash
	0x1b,	// 93	]
	0x07,	// 94	^
	0x0c,	// 95	_
	0x29,	// 96	`
	0x1e,	// 97	a
	0x30,	// 98	b
	0x2e,	// 99	c
	0x20,	// 100	d
	0x12,	// 101	e
	0x21,	// 102	f
	0x22,	// 103	g
	0x23,	// 104	h
	0x17,	// 105	i
	0x24,	// 106	j
	0x25,	// 107	k
	0x26,	// 108	l
	0x32,	// 109	m
	0x31,	// 110	n
	0x18,	// 111	o
	0x19,	// 112	p
	0x10,	// 113	q
	0x13,	// 114	r
	0x1f,	// 115	s
	0x14,	// 116	t
	0x16,	// 117	u
	0x2f,	// 118	v
	0x11,	// 119	w
	0x2d,	// 120	x
	0x15,	// 121	y
	0x2c,	// 122	z
	0x1a,	// 123	{
	0x56,	// 124	|
	0x1b,	// 125	}
	0x2b,	// 126	~
	0x53,	// 127	DEL		Delete
};