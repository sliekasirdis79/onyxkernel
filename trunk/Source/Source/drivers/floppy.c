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

int fdcMotorState;
int dstatus;	
int dchange;
unsigned long tbaddr = 0x80000L;
static unsigned char status[7] = { 0 };
static unsigned char statsz = 0;
static unsigned char sr0 = 0;
static unsigned char fdcTrack = 0xFF;
unsigned int tmout = 0;


const char *fdcDriveType[] = {
	"No Floppy Drive",
	"360kb, 5.25 in. Drive",
	"1.2mb, 5.25 in. Drive",
	"720kb, 3.50 in. Drive",
	"1.44mb, 3.5 in. Drive",
	"2.88mb, 3.5 in. Drive"
};

void lbaRead(char *buf, unsigned char block, unsigned char drv, unsigned char count)
{
	outportb( 0x1F1, 0x00 );
	outportb( 0x1F2, count );
	outportb( 0x1F3, block );
	outportb( 0x1F4, (block >> 8) );
	outportb( 0x1F5, (block >> 16) );
	outportb( 0x1F6, 0xE0 | (drv << 4) | (block >> 24) & 0x0F );
	outportb( 0x1F7, 0x20 );

	while ( !inportb( 0x1F7 ) & 0x08 );

	int i;

	for ( i = 0; i < 256; i++ )
	{
		unsigned char tmp = inportb( 0x1F0 );
		buf[(i * 2)]	= tmp;
		buf[(i * 2)+1]	= (tmp >> 8);
	}
	printk("\nRead complete\n");
}


void fdcInit()
{
	irq_install_handler( 6, fdcHandler );
	fdcDetectFloppyDrives();
	
}

void fdcHandler( struct regs *r )
{
	dstatus = COMPLETE;
	outportb( 0x20, 0x20 );
}

void fdcDetectFloppyDrives()
{
	unsigned char drv;
	outportb( 0x70, 0x10 );
	drv = inportb( 0x71 );

	unsigned char fd0, fd1;
	fd0 = drv >> 4;		// First floppy drive
	fd1 = drv & 0xF;	// The second floppy drive
	
	printk("\nDetecting floppy drives...\n","floppy");
	printk("Floppy Drive A (fd0) is: %s\n", fdcDriveType[fd0]);
	printk("Floppy Drive B (fd1) is: %s\n", fdcDriveType[fd1]);

						
}

void fdcSendByte( int byte )
{
	volatile int msr;
	int tmp;

	for ( tmp = 0; tmp < 128; tmp++ )
	{
		msr = inportb( MSR );
		if ( (msr & 0xC0) == 0x80 )
		{
			outportb( DATA, byte );
			return;
		}
		inportb( 0x80 );
	}
}

int fdcGetByte()
{
	volatile int msr;
	int tmp;

	for ( tmp = 0; tmp < 128; tmp++ )
	{
		msr = inportb( MSR );
		if ( (msr & 0xD0) == 0xD0 )
		{
			return inportb( DATA );
		}
		inportb( 0x80 );
	}
	return -1;
}

void fdcResetDrive()
{
	outportb( DOR, 0x00 );
	outportb( DRS, 0x00 );
	outportb( DOR, 0x0C );
	fdcSendByte( FLOPPY_SPEC );
	fdcSendByte( 0xDF );
	fdcSendByte( 0x02 );
	fdcRecalibrate();

}

void fdcRecalibrate()
{
	fdcSendByte( FLOPPY_RECALIBRATE );
	fdcSendByte( 0x00 );
	wait(50);
}

void fdcBlock ( int block, int *cylinder, int *head, int *sector ) // Head, Track, Sector
{
	*head = (block % (18 * 80)) / 80;	// 18 Sectors * 80 Tracks
	*cylinder= block / (18 * 80);
	*sector = (block % 18) + 1;
}

unsigned int waitfdc( unsigned int sensei)
{
	tmout = 1000;   /* set timeout to 1 second */
     
	/* wait for IRQ6 handler to signal command finished */
	while ( dstatus == 0 && tmout )
		;
	/* read in command result bytes */
	statsz = 0;
	while ((statsz < 7) && (inportb( MSR ) & ( 1<<4 ))) 
	{
		status[statsz++] = fdcGetByte();
	}

	if (sensei) {
		/* send a "sense interrupt status" command */
		fdcSendByte(FLOPPY_SIS);
		sr0 = fdcGetByte();
		fdcTrack = fdcGetByte();
	}
   
	dstatus = 0;
   
	if (!tmout) {
		/* timed out! */
		if (inportb( DIR ) & 0x80)
			dchange = 1;
		return 0;
	} else
		return 1;
}


void fdcMotor( int status )
{
	if ( status )
	{
		if ( !fdcMotorState )
		{
			outportb( DOR, 0x1C );	// Turn on motor
			wait(500);
		}
		fdcMotorState = fdcON;
	} else {
		outportb( DOR, 0x0C );
		fdcMotorState = fdcOFF;
	}
}

int fdcSeek( int block )
{		// Cylinder
	int track = 0, sector = 0, head = 0, result = 0, loop = 0;

	fdcMotor( fdcON );
	fdcBlock( block , &track, &head, &sector );

	for ( loop = 0; loop < 10; loop++ )
	{
		fdcSendByte( FLOPPY_SEEK );
		fdcSendByte( head << 4 );
		fdcSendByte( track );
	}
	return track;
}


void fdcCheckInt( int *st0, int *track )
{
	fdcSendByte( FLOPPY_SIS );
	*st0 = fdcGetByte();
	*track = fdcGetByte();
}

void fdcInitDma( bool read )
{
	outportb( 0x0A, 0x06 );		/* Mask DMA Channel #2 */

	switch( read )
	{
		case true:
			outportb( 0x0B, 0x5A );		/* Single Transfer, Addr inc, Automatic, Read, Channel #2 */
			break;
		case false:
			outportb( 0x0B, 0x5A );		/* Single, Addr inc, Auto, Write, Channel #2 */
			break;
	}
	outportb( 0x0A, 0x06 );		/* Unmask DMA Channel #2 */
}


int fdc_rw(int block, unsigned char *blockbuff, int read, unsigned long nosectors)
{
	int head, track, sector, tries, copycount = 0;
	unsigned char *p_tbaddr = (char *)0x80000;
	unsigned char *p_blockbuff = blockbuff;
   
	fdcBlock( block, &track, &head, &sector );
   
	fdcMotor( fdcON );

	if (!read && blockbuff) {
		/* copy data from data buffer into track buffer */
		for ( copycount = 0; copycount < (nosectors*512); copycount++ )
		{
      		*p_tbaddr = *p_blockbuff;
      		p_blockbuff++;
      		p_tbaddr++;
		}
	}
   
	for ( tries = 0; tries < 3; tries++)
	{
		/* check for diskchange */
		if (inportb( DIR ) & 0x80) 
		{
			fdcSeek(1);
			fdcRecalibrate();
			fdcMotor( fdcOFF );
			printk("FDC: Disk change detected. Trying again.\n");
	 
			return fdc_rw(block, blockbuff, read, nosectors);
		}
		
		if (!fdcSeek(track)) 
		{
			//fdcMotor( fdcOFF );
			printk("FDC: Error seeking to track\n");
			//return 0;
		}
      
		
		outportb( CCR, 0 );
      
		/* send command */
		if (read) 
		{
			fdcInitDma( true );
			dma_transfer( 2, tbaddr, nosectors*512, 0 );
			fdcSendByte( FLOPPY_READ );
		} else {
			fdcInitDma( false );
			dma_transfer( 2, tbaddr, nosectors*512, 1 );
			fdcSendByte( FLOPPY_WRITE );
		}
      
		fdcSendByte( head << 2 );
		fdcSendByte( track );
		fdcSendByte( head );
		fdcSendByte( sector );
		fdcSendByte( 2 );               /* 512 bytes/sector */
		fdcSendByte( 18 );
		fdcSendByte( 0x1b );			/* gap 3 size for 1.44M read/write */
		fdcSendByte( 0xff );            /* DTL = unused */
      
		/* wait for command completion */
		/* read/write don't need "sense interrupt status" */
		if (!waitfdc( 1 ))
		{
			printk("Timed out, trying operation again after reset()\n");
			fdcResetDrive();
			return fdc_rw(block, blockbuff, read, nosectors);
		}
      
		if ((status[0] & 0xc0) == 0)
		{
			printk( "FDC: complete");
			break;   /* worked! outta here! */
		}

		fdcRecalibrate();  /* oops, try again... */
	}
   
	/* stop the motor */
	fdcMotor( fdcOFF );

	if (read && blockbuff) 
	{
		/* copy data from track buffer into data buffer */
		p_blockbuff = blockbuff;
		p_tbaddr = (char *) 0x80000;
		for( copycount = 0; copycount < (nosectors*512); copycount++ )
		{
      		*p_blockbuff = *p_tbaddr;
      		p_blockbuff++;
      		p_tbaddr++;
		}
	}

   return (tries != 3);
}

void fdcRead( int block, int bs, char *buf)
{
	fdc_rw( block, buf, 0, bs );
}

void fdcWrite( int block, int bs, char *buf )
{
	fdc_rw( block, buf, 1, bs );
}

