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

#ifndef __kernel__
#define __kernel__

#include <stdarg.h>
#include <bool.h>
#include <regs.h>
#include <types.h>
#include <video.h>
#include <floppy.h>

/* --- Drivers --- */

/* dma.c */
void _dma_transfer ( unsigned char channel, unsigned char page, unsigned int offset, unsigned int len, unsigned char mode );
void dma_transfer( unsigned char channel, unsigned long addr, unsigned int len, unsigned char read );

/* floppy.c */
void lbaRead(char *buf, unsigned char block, unsigned char drv, unsigned char count);
void fdcInit();
void fdcHandler( struct regs *r );
void fdcDetectFloppyDrives();
void fdcSendByte( int byte );
int fdcGetByte();
void fdcResetDrive();
void fdcRecalibrate();
void fdcBlock ( int block, int *cylinder, int *head, int *sector );
unsigned int waitfdc( unsigned int sensei);
void fdcMotor( int status );
int fdcSeek( int block );
void fdcCheckInt( int *st0, int *track );
void fdcInitDma( bool read );
int fdc_rw(int block, unsigned char *blockbuff, int read, unsigned long nosectors);
void fdcRead( int block, int bs, char *buf);
void fdcWrite( int block, int bs, char *buf);

/* kb.c */
void kb_wait();
unsigned char getc();
void keyboard_handler( struct regs *r );
void keyboard_install();

/* timer.c */
void timer_handler( struct regs *r );
void timer_wait( int ticks );
void wait( int ticks );
void timer_install();

/* video.c */
void clear();
void scroll();
void puts( char *message );
void kputs( char *message );
void movecsr( unsigned int x, unsigned int );
void updatecsr();
void init_video();
void kputc( unsigned char c );
void putc( unsigned char c );
int detect_videocard();

/* --- Libc --- */

/* gdt.c */
void gdt_set_gate( int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran );
void gdt_install();

/* idt.c */
void idt_set_gate( unsigned char num, unsigned long base, unsigned short sel, unsigned char flags );
void idt_install();

/* irq.c */
void irq_install_handler( int irq, void (*handler)(struct regs *r) );
void irq_uninstall_handler( int irq );

/* isrs.c */
void isrs_install();
void fault_handler(struct regs *r);

/* math.c */
unsigned int pow( int num, int power );
unsigned int abs( int num );

/* printf.c */
int printf( const char *fmt, ... );

/* printk.c */
int printk( const char *fmt, ... );

/* queue.c */
void init_queue();
void qLoadTask( void *addr, char priority );
void gRunTask( int count );

/* rand.c */
int rand();
int srand( int seed );

/* stdio.c */
void do_div( int num, int radix );
void itoa( char*, int, int, int, char);
void sitoa( char*, int, int );

/* string.c */
void *memcpy( void *destPtr, void *srcPtr, unsigned int len );
void *memset( void *destPtr, int value, unsigned int len );
int strcmp(  char *s1, char *s2 );
int strncmp( char *s1, char *s2, unsigned int len );
int strncpy( char *dest, char *src, int len );
unsigned int strlen( const char *str );
char **split( char *str, char spl );

/* --- System --- */

/* debug.c */
void debug( char *str );
void kexit();

/* malloc.c */
void init_vmm();
void *malloc( size_t size );
void free( void *addr );

/* paging.c */
void init_paging();
void *page( int index );

/* x86.c */
long getebx();
void switch_es( long new );
void cli();
void sti();
void hlt();
unsigned char inportb( unsigned short port );
void outportb( unsigned short port, unsigned char data );

#endif
