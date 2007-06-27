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

struct queue_t
{
	unsigned char priority;		/* 1 , 2, or 3 */
	unsigned char id;			/* number in the queue */
	unsigned char status;		/* true = ran, false = waiting */
	void (*load);					/* address to run */
};

struct queue_t queueList[256];

void init_queue()
{
	int i;
	for ( i = 0; i < 256; i++ )
	{
		queueList[i].id = i;
		queueList[i].priority = 3; /* lowest priority */
		queueList[i].load = (void*) init_queue + i;
		queueList[i].status = false;
	}

}

void qLoadTask( void* addr, char priority )
{
	int i;
	for ( i = 0; i < 256; i++ )
		if ( queueList[i].status == false )
		{
			queueList[i].id = i;
			queueList[i].priority = priority;
			queueList[i].status = false;
			queueList[i].load = addr;
		}
}

void qRunTasks( int count )	/* number of tasks to run */
{
	int i, c, p;
	do
	{
	for ( i = 0; i < 256; i++ )		/* Run thru all tasks in queue */
		for ( p = 0; p < 3; p++ )	/* Run thru priorities */
			if ( queueList[i].status == false && queueList[i].priority == p)
			{
				queueList[i].load;
				queueList[i].status = true;
				queueList[i].id = -1;
				queueList[i].priority = -1;
			}

	}
	while( c <= count );

}
