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

/* need bool */
#include <kernel.h>


#define MAX_COMMANDS	15
int ccmd = 0;

typedef struct cmd_entry
{
	void (*entry)(char**);
	char *cmd;
	char *alias;
};

struct cmd_entry Commands[MAX_COMMANDS];

void help (char **h )
{
	printf("h%s", h[0] );
	printf("Help Pages:\n");
	printf("Commands	||	Information\n");
	printf("--help		||	Displays this message\n");
	printf("--shell		||	Shell Information\n");
}

void init_shell()
{
	reg_command( help, "--help", "help" );
	reg_command( help, "--shell", "sh" );
	reg_command( help, "onyxsh", "onyx");
}


void reg_command( void (*handle)( char **args ), char *name, char *alias )
{
	Commands[ccmd].cmd = name;
	Commands[ccmd].alias = alias;
	Commands[ccmd].entry = handle;
	ccmd++;
	//printf("Command #%d Registered: %s\n", ccmd, Commands[ccmd++].cmd );
}

bool shell( char command[80] )
{
	
	char *c = command;
	char *n;
	bool r = false;
	int i;
	int f = find( c, '\n' );
	int s = find( c, ' ' );
	
	c[f] = '\0';	///* get rid of the '\n' 
	

	for ( i = 0; i < MAX_COMMANDS; i++ )
	{
		if ( strcmp( Commands[i].alias , n) == 0 || strcmp( Commands[i].cmd, n) == 0)
		{		/* found a match */
			printk("\n", c);
			void (*cmdv)( char **args ) = Commands[i].entry;
			cmdv( &c );
			r = true;
			break;
		} else {	/* no match found */
			printf("Command: %s is an invalid command.\n", c );
			//break;
		}
	}
				
		printf("root@localhost $> ");
		return r;
}


