#include <stdbool.h>

#include "io.h"
#include "scancode.h"
#include "DebugDisplay.h"
#include "keyboard.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define INVALID_KB_CHAR 0

static char kb_char;
static bool __ctrl = false;


char asccode[58][2] =       /* Array containing ascii codes for
			       appropriate scan codes */
     {
       {   0,0   } ,
       { ESC,ESC } ,
       { '1','!' } ,
       { '2','@' } ,
       { '3','#' } ,
       { '4','$' } ,
       { '5','%' } ,
       { '6','^' } ,
       { '7','&' } ,
       { '8','*' } ,
       { '9','(' } ,
       { '0',')' } ,
       { '-','_' } ,
       { '=','+' } ,
       {'\b','\b'} ,
       {'\t','\t'} ,
       { 'q','Q' } ,
       { 'w','W' } ,
       { 'e','E' } ,
       { 'r','R' } ,
       { 't','T' } ,
       { 'y','Y' } ,
       { 'u','U' } ,
       { 'i','I' } ,
       { 'o','O' } ,
       { 'p','P' } ,
       { '[','{' } ,
       { ']','}' } ,
       {'\n','\n'} ,
       {   0,0   } ,
       { 'a','A' } ,
       { 's','S' } ,
       { 'd','D' } ,
       { 'f','F' } ,
       { 'g','G' } ,
       { 'h','H' } ,
       { 'j','J' } ,
       { 'k','K' } ,
       { 'l','L' } ,
       { ';',':' } ,
       {'\'','\"'} ,
       { '`','~' } ,
       {   0,0   } ,
       { '\\','|'} ,
       { 'z','Z' } ,
       { 'x','X' } ,
       { 'c','C' } ,
       { 'v','V' } ,
       { 'b','B' } ,
       { 'n','N' } ,
       { 'm','M' } ,
       { ',','<' } ,
       { '.','>' } ,
       { '/','?' } ,
       {   0,0   } ,
       {   0,0   } ,
       {   0,0   } ,
       { ' ',' ' } ,
   };


void change_terminal();

void keyboard_handler_main(void)
{
	unsigned char status;
	unsigned char scancode;

	/* write EOI */
	port_byte_out(0x20, 0x20);

	status = port_byte_in(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		scancode = port_byte_in(KEYBOARD_DATA_PORT);
		if (scancode < 0)
			return;
		if (scancode & 0x80) {
			if (scancode - 0x80 == KEY_CTRL)
				__ctrl = false;
		} else {
			if (scancode == KEY_CTRL)
				__ctrl = true;
			else if (__ctrl && scancode == KEY_F1)
        DebugPrintf("change_terminal 0 keyboard\n");
				//change_terminal(0);
			else if (__ctrl && scancode == KEY_F2)
        DebugPrintf("change_terminal 1 keyboard\n");
				//change_terminal(1);
			else if (__ctrl && scancode == KEY_F3)
        DebugPrintf("change_terminal 2 keyboard\n");
				//change_terminal(2);
			else {
				char ascii = asccode[(unsigned char) scancode][0];
				kb_char = ascii;
			}
		}
	}
}

char getchar()
{
	char c = INVALID_KB_CHAR;
	kb_char = INVALID_KB_CHAR;
	while (c == INVALID_KB_CHAR)
		c = kb_char;
	kb_char = INVALID_KB_CHAR;
	return c;
}
