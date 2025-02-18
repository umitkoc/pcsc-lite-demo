/*
 * MUSCLE SmartCard Development ( http://pcsclite.alioth.debian.org/pcsclite.html )
 *
 * Copyright (C) 1999
 *  David Corcoran <corcoran@musclecard.com>
 * Copyright (C) 2004-2011
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
 * $Id: prothandler.c 7004 2014-10-02 09:26:36Z rousseau $
 */

/**
 * @file
 * @brief This handles protocol defaults, PTS, etc.
 */

#include <PCSC/config.h>
#include <string.h>

#include <PCSC/misc.h>
#include <PCSC/pcscd.h>
#include <PCSC/debuglog.h>
#include <PCSC/readerfactory.h>
#include <PCSC/prothandler.h>
#include <PCSC/atrhandler.h>
#include <PCSC/ifdwrapper.h>
#include <PCSC/eventhandler.h>

/**
 * Determine which protocol to use.
 *
 * SCardConnect has a DWORD dwPreferredProtocols that is a bitmask of what
 * protocols to use.  Basically, if T=N where N is not zero will be used
 * first if it is available in ucAvailable.  Otherwise it will always
 * default to T=0.
 *
 * IFDSetPTS() is _always_ called so that the driver can initialise its data
 */
DWORD PHSetProtocol(struct ReaderContext * rContext,
	DWORD dwPreferred, UCHAR ucAvailable, UCHAR ucDefault)
{
	DWORD protocol;
	LONG rv;
	UCHAR ucChosen;

	/* App has specified no protocol */
	if (dwPreferred == 0)
		return SET_PROTOCOL_WRONG_ARGUMENT;

	/* requested protocol is not available */
	if (! (dwPreferred & ucAvailable))
	{
		/* Note:
		 * dwPreferred must be either SCARD_PROTOCOL_T0 or SCARD_PROTOCOL_T1
		 * if dwPreferred == SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1 the test
		 * (SCARD_PROTOCOL_T0 == dwPreferred) will not work as expected
		 * and the debug message will not be correct.
		 *
		 * This case may only occur if
		 * dwPreferred == SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1
		 * and ucAvailable == 0 since we have (dwPreferred & ucAvailable) == 0
		 * and the case ucAvailable == 0 should never occur (the card is at
		 * least T=0 or T=1)
		 */
		Log2(PCSC_LOG_ERROR, "Protocol T=%d requested but unsupported by the card",
			(SCARD_PROTOCOL_T0 == dwPreferred) ? 0 : 1);
		return SET_PROTOCOL_WRONG_ARGUMENT;
	}

	/* set default value */
	protocol = ucDefault;

	/* keep only the available protocols */
	dwPreferred &= ucAvailable;

	/* we try to use T=1 first */
	if (dwPreferred & SCARD_PROTOCOL_T1)
		ucChosen = SCARD_PROTOCOL_T1;
	else
		if (dwPreferred & SCARD_PROTOCOL_T0)
			ucChosen = SCARD_PROTOCOL_T0;
		else
			/* App wants unsupported protocol */
			return SET_PROTOCOL_WRONG_ARGUMENT;

	Log2(PCSC_LOG_INFO, "Attempting PTS to T=%d",
		(SCARD_PROTOCOL_T0 == ucChosen ? 0 : 1));
	rv = IFDSetPTS(rContext, ucChosen, 0x00, 0x00, 0x00, 0x00);

	if (IFD_SUCCESS == rv)
		protocol = ucChosen;
	else
		if (IFD_NOT_SUPPORTED == rv)
			Log2(PCSC_LOG_INFO, "PTS not supported by driver, using T=%d",
				(SCARD_PROTOCOL_T0 == protocol) ? 0 : 1);
		else
			if (IFD_PROTOCOL_NOT_SUPPORTED == rv)
				Log2(PCSC_LOG_INFO, "PTS protocol not supported, using T=%d",
					(SCARD_PROTOCOL_T0 == protocol) ? 0 : 1);
			else
			{
				Log3(PCSC_LOG_INFO, "PTS failed (%ld), using T=%d", rv,
					(SCARD_PROTOCOL_T0 == protocol) ? 0 : 1);

				/* ISO 7816-3:1997 ch. 7.2 PPS protocol page 14
				 * - If the PPS exchange is unsuccessful, then the interface device
				 *   shall either reset or reject the card.
				 */
				return SET_PROTOCOL_PPS_FAILED;
			}

	return protocol;
}

