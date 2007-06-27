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

#ifdef FULL_BUILD
#include <kernel.h>
#endif

#define LOBYTE( x )		( x & 0x00FF )
#define HIBYTE( x )		( ( x & 0xFF00 ) >> 8 )

unsigned char Mask[8] = { 0x0A, 0x0A, 0x0A, 0x0A, 0xD4, 0xD4, 0xD4, 0xD4 };
unsigned char Mode[8] = { 0x0B, 0x0B, 0x0B, 0x0B, 0xD6, 0xD6, 0xD6, 0xD6 };
unsigned char Clear[8]= { 0x0C, 0x0C, 0x0C, 0x0C, 0xD8, 0xD8, 0xD8, 0xD8 };
unsigned char Page[8] = { 0x87, 0x83, 0x81, 0x82, 0x8F, 0x88, 0x89, 0x8A };
unsigned char Addr[8] = { 0x00, 0x02, 0x04, 0x06, 0xC0, 0xC4, 0xC8, 0xCC };
unsigned char Count[8]= { 0x01, 0x03, 0x05, 0x07, 0xC2, 0xC6, 0xCA, 0xCE };

void _dma_transfer ( unsigned char channel, unsigned char page, unsigned int offset, unsigned int len, unsigned char mode )
{
	cli();		// Disable int's

	outportb( Mask[channel], 0x04 | channel );		// Set up the DMA channel
	outportb( Clear[channel], 0x00 );				// Clear current transfers
	outportb( Mode[channel], mode );				// Set the DMA mode

	outportb( Addr[channel], LOBYTE(offset) );		// Send the first byte to addr
	outportb( Addr[channel], HIBYTE(offset) );		// Send the second byte to addr

	outportb( Page[page], page );					// Send the physical page

	outportb( Count[channel], LOBYTE(len) );		// Send the first byte length of data
	outportb( Count[channel], HIBYTE(len) );		// Send the second byte length of data

	outportb( Mask[channel], channel );				// Close the DMA channel, we're done

	sti();		// Enable the int's
}

void dma_transfer( unsigned char channel, unsigned long addr, unsigned int len, unsigned char read )
{
	unsigned char page = 0, mode = 0;
	unsigned int offset = 0;

	if ( read )
		mode = 0x48 + channel;
	else
		mode = 0x44 + channel;

	page = addr >> 16;
	offset = addr & 0xFFFF;
	len--;

	_dma_transfer( channel, page, offset, len, mode );
}



