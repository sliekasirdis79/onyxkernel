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


/* 
 * Main.c
 * Update Log -- Started with Version 2.3
 * 2.30 -- added Paging support
 * 2.31 -- added int80() for syscalls
 * 2.32 -- added shell
 * 2.33 -- dumpregs()
 * 2.34 -- working on shell
 * 2.35 -- Updated paging, added bitmap support
 * 2.36 -- Started using GRUB
 */


#include <kernel.h>
#include <multiboot.h>

#define MAJOR_VER	2
#define MINOR_VER	36

extern int seconds;
extern int minutes;
extern int hours;

multiboot_info_t *MultibootHeader;

/* Starts up the vital parts of the kernel. */
void init_kernel( long top )
{
	init_video();
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install();
	keyboard_install();
	//init_paging();
	init_vmm( top );
	init_shell();
	sti();
}

/* Kernel entry point */
int main( multiboot_info_t *mb, unsigned int magic_number )
{
    init_kernel(mb->mmap_addr);
	
	printk("Phoenix OS Version %i.%i, (C) Michael Lazear, 2007 {GPL}\n", MAJOR_VER, MINOR_VER);
	printk("Questions or comments? Contact: michaellazear92@gmail.com\n");
	printk("Initiating...\n");
	printk("Amount of upper ram: %iKB\n", mb->mem_upper / 0x400);
	printk("Kernel Stack: 0x%X\n", mb->mmap_addr);
	printk("Starting Onyx Shell...\n");
	fdcInit();
	printk("type onyx --help for commands\n\n%m");
	printk("root@localhost $>%m");
	
	
	while( true );
	for(;;);	// Double protection
	/* anything beyond this line will _not_ run */
	
}

/* int80() -- dump registers to the screen & exit kernel */
int int80()
{
	asm(".intel_syntax noprefix\n"
	"int 0x80\n"
	".att_syntax");
	
}

/* This is the panic function used for the int handlers */
void panic( char *pstr )
{
	cli();
	printk( "\n%acKERNEL PANIC: %s%rc\n", 0x2F, pstr );
	sti();
}



