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

int printk( const char *fmt, ... )
{
	va_list args;
	va_start(args, fmt);
	int count;
	char buf[20];
	char *p;
	int len = 8;
	
	while ( *fmt != 0 )
	{
		switch ( *fmt ){
			case '%':
				fmt++;
				switch ( *fmt )
				{
				case 'l':	// Update length of ints, etc
					len = va_arg( args, int );
					break;
				case 'b':
					sitoa( buf, 2, va_arg( args, int ) );
					p = buf;
					puts(p);
					break;
				case '2':
					itoa( buf, 16, va_arg( args, char ), 2, 'A' );
				case 'd':	// decimal
					//itoa(va_arg(args, int), 10);
					//do_div( va_arg( args, long ) , 10 ); 
					itoa( buf, 10, va_arg( args, long ), len, 'a' );
					p = buf;
					puts(p);
					break;
					
				case 'i':
					sitoa( buf, 10, va_arg( args, long ));
					p = buf;
					puts(p);
					break;
				case 'x':	// hex
					//itoa(va_arg(args, int), 16);
					//do_div( va_arg( args, long ) , 16 ); 
					itoa( buf, 16, va_arg( args, long ), len, 'a' );
					p = buf;
					puts(p);
					break;
				case 'X':	// hex
					//itoa(va_arg(args, int), 16);
					//do_div( va_arg( args, long ) , 16 ); 
					itoa( buf, 16, va_arg( args, long ), len , 'A' );
					p = buf;
					puts(p);
					break;
				case 'o':	// Octal
					//do_div( va_arg( args, long ), 8 );
					break;
				case 's':	// string (char*)
					kputs(va_arg(args, char*));
					break;
				case 'c':	// char
					kputc(va_arg(args, char));
					break;
				case 'm':	// update cursor
					updatecsr();
					break;
				case 'u':	// Uniform
						sitoa( buf, 16, va_arg( args, char ) );
						p = buf;
						puts(p);
						break;
				case 'r':	// restore
					fmt++;
					switch( *fmt )
					{
					case 'c':	// color
						cons.color = 0x7;
						break;
					case 'y':	// line
						cons.cur_y = 0;
						break;
					case 'x':	// position
						cons.cur_x = 0;
						break;
					}
					break;
				case 'a':	// adjust
					fmt++;
					switch( *fmt )
					{
					case 'c':	// color
						cons.color = va_arg(args, char);
						break;
					case 'y':	// line
						cons.cur_y = va_arg(args, int);
						break;
					case 'x':	// position
						cons.cur_x = va_arg(args, int);
						break;
					}
					break;
				}
				fmt++;
				break;
			default:
				kputc(*fmt);
				fmt++;
		}
		count++;
	}
	
	va_end(args);
	return count;
}
