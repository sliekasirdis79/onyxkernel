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

/* Version 2.33 -- Added dumpreg */

#include <kernel.h>

void debug( char *str )
{
	printk("[Onyx Kernel: Debug Mode][Debug:]$ %s\n", str );
}

void kexit()
{
	puts("Exiting Onyx Kernel");
	cli();
	hlt();
}

unsigned char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void dumpreg( struct regs *r )
{
	/* Dump CPU Registers & bsod for those windows users =) */

	attrib( 0x2F );
	clear();
	printk("Starting WDEMOD... WinDozEmulator On Drugs -- ");
	printk("Green Screen of Death Mode!\n\n");
	printk("Dumping CPU State...\n");
	printk("Int: %i -- Exception: %s\n", r->int_no, exception_messages[r->int_no] );
	printk("Error:  0x%x\n", r->err_code );
	printk("Eflags:	0x%x\n", r->eflags);  
	printk("	Segments	\n");
	printk("cs: 0x%2 || ds: 0x%2 || es: 0x%2\n", r->cs, r->ds, r->es );
	printk("fs: 0x%2 || gs: 0x%2 || ss: 0x%2\n", r->fs, r->gs, r->ss );
	printk("esp:	0x%x	uesp:	0x%u\n", r->esp, r->useresp );
	printk("esi:	0x%x\n", r->esi );
	printk("ebp:	0x%x\n", r->ebp );
	printk("edi:	0x%x\n", r->edi );
	printk("eip:	0x%x\n", r->eip );
	printk("	Registers\n");
	printk("eax:	0x%x\n", r->eax );
	printk("ebx:	0x%x\n", r->ebx );
	printk("ecx:	0x%x\n", r->ecx );
	printk("edx:	0x%x\n", r->edx );
	kexit();
	//attrib( 0x7 );

	
	/* We want to re-enter the kernel */
}
	

