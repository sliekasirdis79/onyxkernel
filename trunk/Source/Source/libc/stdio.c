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

void do_div( int num, int radix )
{
	unsigned long temp;
	unsigned char where[10];
	unsigned char t = 10;

	where[0] = '/0';
	do
	{
	temp = (unsigned long) num % radix;
	if ( temp < 10 )
		where[10] = (char)(temp + '0');
		//putc( temp + '0' );
	else 
		where[t--] = (char)(temp + 'A');
		//putc( temp + 'A' );
	num = (unsigned long) num / radix;
	} 
	while ( num != 0 );
		
	puts( where );
}	

void itoa(char *buf, int base, int d, signed int length, char flags)
{
   char *p = buf;
   char *p1, *p2;
   unsigned long ud = d;
   int divisor = 10;

   /* If %d is specified and D is minus, put `-' in the head. */
   if(base == 10 && d < 0)
   {
      *p++ = '-';
      buf++;
      ud = -d;
   }
   else if( base == 16 )
      divisor = 16;
   else if( base < 16 )
	   divisor = base;

   /* Divide UD by DIVISOR until UD == 0. */
	do
	{
		int remainder = ud % divisor;
		// funky if
		*p++ = (remainder < 10) ? remainder + '0' : remainder + flags - 10;
		length--;
	}
	while(ud /= divisor);

   
	while(length > 0)
	{
		*p++ = '0';
		length--;
	}

	/* Terminate BUF. */
	*p = 0;

	/* Reverse BUF. */
	p1 = buf;
	p2 = p - 1;
	while(p1 < p2)
	{
		char tmp = *p1;

		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}

void sitoa( char *buf, int base, int d )
{
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;
	
	/* If %d is specified and D is minus, put `-' in the head. */
	if(base == 10 && d < 0)
	{
		*p++ = '-';
		buf++;
		ud = -d;
		
	}
	else if(base == 16)
		divisor = 16;
	else if ( base == 2 )
	{
		divisor = base;
		
	}

	/* Divide UD by DIVISOR until UD == 0. */
	do
	{
		int remainder = ud % divisor;

		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
		//length--;
	}
	while(ud /= divisor);
	
	/*
	while( (length % 4) != 0)
	{
		*p++ = '0';
		length--;
	}
	*/

	/* Terminate BUF. */
		*p = 0;

	/* Reverse BUF. */
		p1 = buf;
		p2 = p - 1;
	while(p1 < p2)
	{
		char tmp = *p1;

		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}



