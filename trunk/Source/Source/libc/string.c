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

void *memcpy( void* destPtr, void* srcPtr, unsigned int len )
{
	void *ret = destPtr;
	const char* src = (const char *) srcPtr;
	char* dst = (char *)destPtr;

	while ( len != 0 )
	{
		*dst = *src;
		dst++;
		src++;
		len--;
	}
	return(ret);
}

void *memset( void* dest, int value, unsigned len )
{
	void *ptr = dest;
	while ( len )
	{
		*(unsigned char*) dest = value;
		dest = (unsigned char *) dest + 1;
		--len;
	}
	return(ptr);
}

int strcmp( char *s1, char *s2 )
{
	while ( *s1 && *s2 && *s1 == *s2 )
	{
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

int strncmp( char *s1, char* s2, unsigned int len )
{
	char val = 0;
	while ( len > 0 )
	{
		if ( (val = *s1 - *s2++) != 0 || !*s1++ )
			break;
		len--;
	}
	return val;
}

int strncpy( char *dest, char *src, int len )
{
	char *d, *s;
	int val = 0;

	d = dest;
	s = src;

	while ( *s != '\0' && len != 0 )
	{
		*d++ = *s++;
		len--;
		val++;
	}
	return val;
}

unsigned int strlen( const char *str )
{
	int count = 0;
	while ( *str++ != '\0' )
	{
		count++;
	}
	return count;
}

char *split( char *str, char spl )
{
	char **buf;
	int f = find( str, spl );
	void *x, *y, *z;	// storage buffers
	memcpy( x, str, f );	// First split;
	buf = x;
	return buf;
}
	
	

