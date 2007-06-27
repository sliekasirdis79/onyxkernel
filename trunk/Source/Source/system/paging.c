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

extern void write_cr0(unsigned long val);
extern void write_cr3(unsigned long val);
extern unsigned long read_cr0();
extern unsigned long read_cr3();

unsigned long *directory = (unsigned long*) 0x9C000;
unsigned long *pagetable = (unsigned long*) 0x9D000;
void *bmap[1024];	// stores the free pages

void init_bmap()
{
	int i;
	for ( i = 0; i < 1024; i++ )
		bmap[i] = pagetable[i];
}

void init_paging()
{
	unsigned long address = 0;
	unsigned int temp;
	/*Map the first 4 megabytes of memory */
	
	for ( temp = 0; temp < 1024; temp++ )
	{
		pagetable[temp] = address | 3;	/* Attribute set to Supervisor, Read/Write, Present (011) */
		address += 4096;				/* 4 KB pages */
		
	}

	/* Set up the first entry */
	directory[0] = pagetable;			/* 4 MB page table */
	directory[0] = directory[0] | 3;	/* Supervisor, R/W, Present ( 011 ) */

	for ( temp = 1; temp < 1024; temp++ )
		directory[temp] = 0 | 4;		/* User, R/W, Not Present (110) */

	write_cr3( directory );				/* Set up the directory */
	write_cr0( read_cr0() | 0x80000000 );	/* Set the paging bit */

	init_bmap();

}



/* pmalloc()
 * we want to get addr of first page in stack,
 * then remove that address from stack,
 * and move every other address down one */
 
long *pmalloc()
{
	char seg[2];	/* 4 bit segments * 2 = 1 byte */
	int i, r, x;	/* loop variables */
	char c;			/* store current bitmap page */
	long *addr;		/* used to store address of page */

	for ( i = 0; i < 1024; i++ )
		if ( bmap[i] != 255 )
			c = bmap[i];	/* find a free page */
	
	seg[0] = (c & 0xF);	/* first 4 bits */
	seg[1] = (c & 0x0);	/* last 4 bits */

	for ( r = 0; r < 2; r++ )	/* loop for each segment */
		for ( i = 0; i < 6; x = pow( 2, 6 ), i++ ) /* loop for each bit */
			if ( (seg[r] & x) == 0 )	/* bit x = 0 (free) */
			{
				addr = pagetable[i];	/* corresponding page */
				seg[r] |= x;	/* set the bit as used */
			}

	return addr;
}
			
	







