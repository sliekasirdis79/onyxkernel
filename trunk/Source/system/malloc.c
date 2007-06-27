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

// VMM uses the page directory to get space
unsigned int pageindex;
struct memory 
{
	unsigned int size;
	unsigned int status;
	void *addr;
};

struct memory memtable[1024];

void init_vmm( long top )
{
	int i, k;
	memtable[0].addr = top;
	memtable[0].status = 1;
	memtable[0].size = 4096;

	for ( i = 1; i < 1024; i++ )
	{
		k = i;
		memtable[i].addr = memtable[k--].addr + 4096;
		memtable[i].size = 4096;
		memtable[i].status = 0;
	}
}

// Allocate space from the page directory
void *malloc( size_t size )
{
	int i, k;
	for ( i = 1; i < 1024; i++ )
	{
		if ( memtable[i].status = 0 )
		{
			if ( size != 0 )
			{
				k = i;
				memtable[i].addr = memtable[k--].addr + 4096;
				memtable[i].size = size;
				memtable[i].status = 1;
				return memtable[i].addr;
			} else
				return (void*) 0;
		}
	}
			
}

/* Free Space */
void free ( void *mem )
{
	int i;
	for ( i = 1; i < 1024; i++ )
		if ( memtable[i].addr = mem )
		{
			memtable[i].addr = (void*)0;
			memtable[i].size = 0;
			memtable[i].status = 0;
		}
	
}
