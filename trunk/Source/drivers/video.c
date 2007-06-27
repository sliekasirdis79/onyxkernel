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

/* Clear the active video-memory */
void clear()
{
	char* vga_addr = (char*) 0xb8000;
	const long size = 80*25;
	long loop;
	
	for ( loop = 0; loop < size; loop++ )
	{
		*vga_addr++ = 0;
		*vga_addr++ = cons.color;	// Clear it with the console's color, not black-black
	}
	
	/* Reset the console location */
	cons.cur_x = 0;
	cons.cur_y = 0;
}

/* Scroll the screen up one line */
void scroll()
{
	char* vga_addr = (char*) 0xb8000;
	unsigned blank, temp;

    blank = 0x00 | (cons.color << 8);

    if( cons.cur_y >= 25)
    {
		     
        temp = cons.cur_y - 25 + 1;
        memcpy(vga_addr, vga_addr + temp * 160, (25 - temp) * 160 * 2);

        memset(vga_addr + (25 - temp) * 160, blank, 80);
        cons.cur_y = 25 - 1;
    }

}

void newline()
{
	cons.cur_y++;
	cons.cur_x = 0;
}

/* Print directly to screen, no format
 * User-mode */
void puts( char *message )
{
	while( *message != 0 )
	{
		putc(*message);
		*message++;
	}
}

void attrib( char colorattrib )
{
	cons.color = colorattrib;
}

/* Print directly to screen, no format
 * Kernel-mode */
void kputs( char *message )
{
	while ( *message != 0 )
	{
		kputc(*message);
		*message++;
	}
}

/* Move the screen's cursor to the specified pos and line 
 * x is the char position, y is the line */
 
void movecsr( unsigned int x, unsigned int y )
{
	unsigned temp;
	temp = (y * 80) + x;

	outportb(0x3D4, 14);
	outportb(0x3D5, temp >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, temp);
}

/* Update cursor to current position */
void updatecsr()
{
	movecsr( cons.cur_x / 2, cons.cur_y );	/* 160x25 chars, only 80 are ascii */
}

/* set up our console mode */
void init_video()
{
	int vc = detect_videocard();
	if ( vc == 0 )	/* Monochrome card, can't use */
	{
		puts( "Your video card is monochrome, and is un-supported... sorry =(" );
		kexit();
	}

	cons.color = 0x7;
	cons.cur_x = 0;
	cons.cur_y = 0;
	cons.mem = (char*)0;
	
	clear();
}	

/* Kernel-mode putc (doesn't update cursor) */
void kputc( unsigned char c )
{
	char* vga_addr = (char*) 0xb8000;
	unsigned short *where;
    unsigned att = (cons.color << 8);

    if ( c == '\b' )		// Back Space
    {
        if ( cons.cur_y != 0 );	// Not first line
		{
			cons.cur_x--;
			cons.cur_x--;
			where = vga_addr + (cons.cur_y * 160 + cons.cur_x);
			*where = '\0' | att;
		}
    }   
    else if ( c == '\t' )	// Tab
    {
        cons.cur_x = (cons.cur_x + 10) & ~(10 - 1);
    }
    
    else if ( c == '\r' )	// Return
    {
		cons.cur_x = 0;
    }
   
    else if ( c == '\n' )	// New line
    {
        cons.cur_x = 0;
        cons.cur_y++;
	}
 
    else if ( c >= ' ' )	// Unknown chars
    {
        where = vga_addr + (cons.cur_y * 160 + cons.cur_x);
        *where = c | att;
        cons.cur_x++;
		cons.cur_x++;
		//move_csr();
    }
    if(cons.cur_x >= 160)
    {
        cons.cur_x = 0;
        cons.cur_y++;
    }

    scroll();
    
}

/* User-mode putc & update cursor */
void putc( unsigned char c )
{
	kputc( c );
	updatecsr();
}
 
/* Detects the video type: 0 = monochrome, 1 = color */
int detect_videocard()
{
	char vc = (*(unsigned short*)0x410 & 0x30);

	if ( vc == 0x30 )
		return 0;	/* Monochrome card */
	else
		return 1;	/* Color card */
}