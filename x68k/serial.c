
#include <stdio.h>
#include <string.h>
#include <iocslib.h>
#include "serial.h"

void SER_puts(char *s){
	short count = 0;
	
	for(count = 0; count<strlen(s); count++){
		/* this takes one byte at a time*/
		_iocs_out232c(s[count]);
	}
	_iocs_out232c('\r');
}

void SER_cls(){
	SER_puts("\033[2J\033[1;37m\033[;H");
}

void SER_error(char *s){
	char errtxt[1024];
	sprintf(errtxt, "\033[1;31mError:\033[1;37m %s", s);
	SER_puts(errtxt);
}

void SER_warn(char *s){
	char errtxt[1024];
	sprintf(errtxt, "\033[1;33mWarning:\033[1;37m %s", s);
	SER_puts(errtxt);
}

void SER_info(char *s){
	char errtxt[1024];
	sprintf(errtxt, "\033[1;32mInfo:\033[1;37m %s", s);
	SER_puts(errtxt);
}

void SER_debug(char *s, char mode){
	#ifdef DEBUG
	switch(mode){
	case(SER_DEBUG_ERR):
		SER_error(s);
	break;
		case(SER_DEBUG_WARN):
		SER_warn(s);
	break;
		case(SER_DEBUG_INFO):
		SER_info(s);
	break;
	}
	#endif
}
