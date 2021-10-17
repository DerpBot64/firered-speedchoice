/*
 mgba.h
 Copyright (c) 2016 Jeffrey Pfau
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.
 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "gba/types.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "mgba.h"
#include "printf.h"
#include "global.h"
#include "gflib.h"

#define REG_DEBUG_ENABLE (vu16*) 0x4FFF780
#define REG_DEBUG_FLAGS (vu16*) 0x4FFF700
#define REG_DEBUG_STRING (char*) 0x4FFF600

EWRAM_DATA u8 gStringLogger[128] = {};

void mgba_printf(int level, const char* ptr, ...) {
#if !MODERN
	va_list args;
	level &= 0x7;
	va_start(args, ptr);
	vsnprintf(REG_DEBUG_STRING, 0x100, ptr, args);
	va_end(args);
	*REG_DEBUG_FLAGS = level | 0x100;
#endif
}

bool8 mgba_open(void) {
	*REG_DEBUG_ENABLE = 0xC0DE;
	return *REG_DEBUG_ENABLE == 0x1DEA;
}

void mgba_close(void) {
	*REG_DEBUG_ENABLE = 0;
}

char *ConvertToAscii(const u8 *str)
{
    s32 i;
    char * textBuffer = gStringLogger;
    for (i = 0; *str != EOS; i++, str++)
    {
        char modifiedCode = '?';
		if(*str >= CHAR_a && *str <= CHAR_z)
        {
            modifiedCode = *str-(CHAR_a-'a'); // lower-case characters
        }
        else if(*str >= CHAR_A && *str <= CHAR_Z)
        {
            modifiedCode = *str-(CHAR_A-'A'); // upper-case characters
        }
        else if (*str >= CHAR_0 && *str <= CHAR_9)
        {
            modifiedCode = *str-(CHAR_0-'0'); // numbers
        }
        else if (*str == CHAR_SPACE)
        {
            modifiedCode = ' '; // space
        }
        else if (*str == CHAR_EXCL_MARK)
        {
            modifiedCode = '!'; // exclamation point
        }
        else if (*str == CHAR_QUESTION_MARK)
        {
            modifiedCode = '?'; // question mark
        }
        else if (*str == CHAR_PERIOD)
        {
            modifiedCode = '.'; // period
        }
        else if (*str == CHAR_DBL_QUOT_LEFT || *str == CHAR_DBL_QUOT_RIGHT)
        {
            modifiedCode = '"'; // double quote
        }
        else if (*str == CHAR_SGL_QUOT_LEFT || *str == CHAR_SGL_QUOT_RIGHT)
        {
            modifiedCode = '"'; // single quote
        }
        else if (*str == CHAR_CURRENCY)
        {
            modifiedCode = '$'; // currency mark (pokemonies in game, dollar sign in logs)
        }
        else if (*str == CHAR_COMMA)
        {
            modifiedCode = ','; // comma
        }
        else if (*str == CHAR_MULT_SIGN)
        {
            modifiedCode = '#'; // pound, hashtag, octothorpe, whatever
        }
        else if (*str == CHAR_SLASH)
        {
            modifiedCode = '/'; // slash
        }
        textBuffer[i] = modifiedCode;
    }
    textBuffer[i] = 0;
    return textBuffer;
}
