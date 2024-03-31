#ifndef defines_h
#define defines_h

/*
 * $Id$
 *
 * NAME:
 *	defines.h -- Copyright (C) 1998 David Corcoran
 *                corcordt@cs.purdue.edu
 *
 * DESCRIPTION:
 *       Some global definitions and typedefs
 *
 * AUTHOR:
 *	David Corcoran, 3/17/98
 *
 *	Modified for Macintosh by Mark Hartman
 */


/* #define CPU_PC_UNIX - 
 *     Any Unix running machine
 *
 * #define CPU_MAC_OS  - 
 *     Any Macintosh machine with Device Toolkit
 */

#define CPU_PC_UNIX

/* #define SUN_SPARC -
 *     Any Sun Sparc Architecture
 *
 */

/*
 * Pervasive constant: The card readed buffer size (also the maximal
 * length of a command), Status byte size.
 */

#define MAX_BUFFER_SIZE		264
#define STATUS_SIZE		2

/*
 * Boolean constants
 */

#ifndef TRUE

  #define TRUE	1
  #define FALSE	0

#endif

/*
 * Type definitions
 */

#ifndef BYTE

  typedef unsigned char BYTE;

#endif

#ifndef HANDLE

 #ifdef CPU_MAC_OS
   typedef Handle HANDLE;
 #endif

 #ifdef CPU_PC_UNIX
   typedef int HANDLE;
 #endif

#endif

#ifndef bool

 typedef int bool;

#endif


struct BinData {
  BYTE Header[MAX_BUFFER_SIZE];	        /* Cla, Ins, P1, P2, P3 	*/
  BYTE TxBuffer[MAX_BUFFER_SIZE];	/* Data with size P3		*/
  BYTE CommandStatus;		        /* Status of last command 	*/
};


#define ISO_INVALID_COMMAND_SIZE   (unsigned char)0xA0 /* Command > MAX_BUFFER_SIZE       */
#define ISO_INVALID_SEEK_MODE	   (unsigned char)0xA1	/* Invalid File Seek Mode	   */
#define ISO_COMMAND_SUCCESS	   (unsigned char)0xA2	/* Command Executed Successfully   */
#define ISO_INVALID_PARAMETER	   (unsigned char)0xA3  /* Invalid parameter given to function */

/*
 * Compiler dependencies
 */

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

#endif
