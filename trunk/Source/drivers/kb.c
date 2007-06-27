/************************************************************************************
* Copyright (c) 2007, Michael Lazear												*
* All rights reserved.																*
*																					*
* Redistribution and use in source and binary forms, with or without				*
* modification, are permitted provided that the following conditions are met:		*
*     * Redistributions of source code must retain the above copyright				*
*       notice, this list of conditions and the following disclaimer.				*
*     * Redistributions in binary form must reproduce the above copyright			*
*       notice, this list of conditions and the following disclaimer in the			*
*       documentation and/or other materials provided with the distribution.		*
*     * Neither the name of the <Eclypse Systems> nor the							*
*       names of its contributors may be used to endorse or promote products		*
*       derived from this software without specific prior written permission.		*
*																					*
* THIS SOFTWARE IS PROVIDED BY <Michael Lazear> ``AS IS'' AND ANY					*
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED			*
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE			*
* DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY				*
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES		*
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;		*
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND		*
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT		*
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS		*
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.						*
*************************************************************************************/


#include <kernel.h>

unsigned char kbdUS[] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't','y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`', 0, '\\', 'z', 
  'x', 'c', 'v', 'b', 'n','m', ',', '.', '/', 0,'*', 0,' ', 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 30, 0, '-', 17, 0, 16, '+', 0, 31, 0, 0, 0, 0, 0, 0, 0,
  0, 0,
};

// Shift not yet implemented
unsigned char kbdUS_shift[] =
{
	0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
	'\t', 'Q', 'W', 'E', 'R', 'T','Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':','|', ' ', 0, '\\', 'Z', 
  'X', 'C', 'V', 'B', 'N','M', '<', '>', '?', 0,'*', 0,' ', 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 30, 0, '-', 17, 0, 16, '+', 0, 31, 0, 0, 0, 0, 0, 0, 0,
  0, 0 
};

// Key buffer
unsigned char key_buf = 0xFF;

void kb_wait( )
{
	while( key_buf == 0xFF );
}

// Returns current key & decrements count
unsigned char getc()
{
	unsigned char ret;
	if ( key_buf != 0xFF )
	{
		ret = key_buf;
		key_buf = 0xFF;
		return ret;
	} else {
		kb_wait();
		ret = key_buf;
		key_buf = 0xFF;
		return ret;
	}

}

char line[80];
char idex = 0;
/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r)
{
	unsigned char scancode;
    scancode = inportb(0x60);
    if (scancode & 0x80)
    {
		
    }
    else
    {
		key_buf = kbdUS[scancode];
		
		//putc(key_buf);
		line[idex++] = key_buf;
		putc( getc() );
		check( line );
		updatecsr();	
		
	
    }
	
	
}

int find( char *str, char c )
{
	int i;
	for ( i = 0; i < strlen(str); i++ )
		if ( str[i] == c )
			return i;
}

void check( char buffer[80] )
{
	char *b = buffer;
	int i;
	for ( i = 0; i < strlen(b); i++ )
		if ( b[i] == '\n' )
		{
			shell( b );		/* init OS shell -- not provided in kernel*/
			b[i] = '/0';	/* remove the '\n' */
			idex = 0;		/* reset index */
			int d;
			for ( d = 0; d < strlen(b); d++ )
				if ( b[d] != '/0' )	/* make sure it's not already null */
					b[d] = '/0';	/* clear the string */
		} else
			continue;
}


/* Installs the keyboard handler into IRQ1 */
void keyboard_install()
{
    irq_install_handler(1, keyboard_handler);
}
