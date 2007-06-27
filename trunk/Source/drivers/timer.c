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

int seconds;
int minutes;
int hours;
int timer_ticks;

void inc_systime()
{
	seconds++;

	if ( seconds == 60 )
	{
		minutes++;
		seconds = 0;
	} else if ( minutes == 60 ) {
		hours++;
		minutes = 0;
	}
}

void timer_handler(struct regs *r)
{
    timer_ticks++;

    if ( timer_ticks % 18 == 0 )
    {
		inc_systime();
    }
}
void timer_wait(int ticks)
{
    unsigned long eticks;

    eticks = timer_ticks + ticks;
	while(timer_ticks < eticks);
}

// wait prototype used in a lot of code, so it's redundant

void wait( int ticks )
{
	timer_wait(ticks);
}

void timer_install()
{
	irq_install_handler(0, timer_handler);
	seconds = 0;
	timer_ticks = 0;
}
