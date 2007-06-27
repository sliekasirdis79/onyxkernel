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


#ifndef __floppy_
#define __floppy__

#define FLOPPY_READBLOCK	1
#define FLOPPY_WRITEBLOCK	2

// Floppy base addresses
#define FLOPPY_PRIMARY		0x3f0
#define FLOPPY_SECONDARY	0x370

// Floppy Registers
#define DOR					0x3F2	// Digital Output Register
#define MSR					0x3F4	// Main Status Register (input reg)
#define DRS					0x3F4	// Data Rate Selector (output reg)
#define DATA				0x3F5	// Data Register
#define DIR					0x3F7	// Digital Input Register
#define CCR					0x3F7	// Configuration Control Register
			
// Floppy Commands
#define FLOPPY_RECALIBRATE	0x07	
#define FLOPPY_SIS			0x08	// Sense Interrupt Status
#define FLOPPY_SEEK			0x0f	
#define FLOPPY_WRITE		0xC5
#define FLOPPY_READ			0xE6
#define FLOPPY_SPEC			0x03

#ifndef SEEK_START
#define SEEK_START			0
#define SEEK_CURRENT		1
#endif

#define COMPLETE			0xFF
#define WORKING				0x00
#define fdcON				1
#define fdcOFF				0


#endif
