/*
 * MUSCLE SmartCard Development ( http://pcsclite.alioth.debian.org/pcsclite.html )
 *
 * Copyright (C) 1999-2002
 *  David Corcoran <corcoran@musclecard.com>
 * Copyright (C) 2002-2011
 *  Ludovic Rousseau <ludovic.rousseau@free.fr>
 *
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: debug.c 7004 2014-10-02 09:26:36Z rousseau $
 */

/**
 * @file
 * @brief This handles debugging for libpcsclite.
 */

#include <PCSC/config.h>
#include <PCSC/misc.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

/* We shall not export the log_msg() sumbol */
#undef PCSC_API
#include <PCSC/debuglog.h>
#include <PCSC/strlcpycat.h>

#define DEBUG_BUF_SIZE 2048

#ifdef NO_LOG

void log_msg(const int priority, const char *fmt, ...)
{
	(void)priority;
	(void)fmt;
}

#else

/** default level is quiet to avoid polluting fd 2 (possibly NOT stderr) */
static char LogLevel = PCSC_LOG_CRITICAL+1;

static signed char LogDoColor = 0;	/**< no color by default */

static void log_init(void)
{
	char *e;

#ifdef LIBPCSCLITE
	e = getenv("PCSCLITE_DEBUG");
#else
	e = getenv("MUSCLECARD_DEBUG");
#endif
	if (e)
		LogLevel = atoi(e);

	/* log to stderr and stderr is a tty? */
	if (isatty(fileno(stderr)))
	{
		char *term;

		term = getenv("TERM");
		if (term)
		{
			const char *terms[] = { "linux", "xterm", "xterm-color", "Eterm", "rxvt", "rxvt-unicode" };
			unsigned int i;

			/* for each known color terminal */
			for (i = 0; i < COUNT_OF(terms); i++)
			{
				/* we found a supported term? */
				if (0 == strcmp(terms[i], term))
				{
					LogDoColor = 1;
					break;
				}
			}
		}
	}
} /* log_init */

void log_msg(const int priority, const char *fmt, ...)
{
	char DebugBuffer[DEBUG_BUF_SIZE];
	va_list argptr;
	static int is_initialized = 0;

	if (!is_initialized)
	{
		log_init();
		is_initialized = 1;
	}

	if (priority < LogLevel) /* log priority lower than threshold? */
		return;

	va_start(argptr, fmt);
	(void)vsnprintf(DebugBuffer, DEBUG_BUF_SIZE, fmt, argptr);
	va_end(argptr);

	{
		if (LogDoColor)
		{
			const char *color_pfx = "", *color_sfx = "\33[0m";

			switch (priority)
			{
				case PCSC_LOG_CRITICAL:
					color_pfx = "\33[01;31m"; /* bright + Red */
					break;

				case PCSC_LOG_ERROR:
					color_pfx = "\33[35m"; /* Magenta */
					break;

				case PCSC_LOG_INFO:
					color_pfx = "\33[34m"; /* Blue */
					break;

				case PCSC_LOG_DEBUG:
					color_pfx = ""; /* normal (black) */
					color_sfx = "";
					break;
			}
			fprintf(stderr, "%s%s%s\n", color_pfx, DebugBuffer, color_sfx);
		}
		else
			fprintf(stderr, "%s\n", DebugBuffer);
	}
} /* log_msg */

#endif

