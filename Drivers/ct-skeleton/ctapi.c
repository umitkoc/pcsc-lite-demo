/*****************************************************************
/
/ File   :   ctapi.c
/ Author :   David Corcoran
/ Date   :   September 19, 1998
/ Purpose:   Defines CT-API functions
/ License:   See file LICENSE
/
******************************************************************/

#include "defines.h"
#include "ctapi.h"
#include "serial.h"

/* Initializes the port on which the reader resides */

int  CT_init ( unsigned int Ctn,  unsigned int pn ) {
  
  bool BretVal;        /* Return value from IO_InitializePort() */
  int IretVal;         /* Return for this function */  


/* CHANGE :: You may have to change some of the IO_InitializePort() 
   parameters such as baudrate, parity, and bits.                   */

  switch( pn ) {

  case PORT_COM1:

    #ifdef SUN_SPARC
     BretVal = IO_InitializePort(9600,8,'E',"/dev/cua/a");
    #endif

    #ifndef SUN_SPARC
     BretVal = IO_InitializePort(9600,8,'E',"/dev/cua0");
    #endif

    break;

  case PORT_COM2:

    #ifdef SUN_SPARC
     BretVal = IO_InitializePort(9600,8,'E',"/dev/cua/b");
    #endif

    #ifndef SUN_SPARC
     BretVal = IO_InitializePort(9600,8,'E',"/dev/cua1");
    #endif

    break;

  case PORT_COM3:

    #ifdef SUN_SPARC
     BretVal = IO_InitializePort(9600,8,'E',"/dev/cua/c");
    #endif

    #ifndef SUN_SPARC
     BretVal = IO_InitializePort(9600,8,'E',"/dev/cua2");
    #endif

    break;

  case PORT_COM4:

    #ifdef SUN_SPARC
     BretVal = IO_InitializePort(9600,8,'E',"/dev/cua/d");
    #endif

    #ifndef SUN_SPARC
     BretVal = IO_InitializePort(9600,8,'E',"/dev/cua3");
    #endif

    break;

  case PORT_Printer:

   BretVal = IO_InitializePort(9600,8,'E',"Printer Port");
   break;

  case PORT_Modem:

   BretVal = IO_InitializePort(9600,8,'E',"Modem Port");
   break;

  default:
    BretVal = IO_InitializePort(9600,8,'E',"/dev/smartcard");
    break;
  }
  
  if (BretVal != TRUE) {
    IretVal = ERR_MEMORY;        /* Could not allocate port */
  } else {
    IretVal = OK;
  }
  
  return IretVal;  
}

/* Closes the port in which the reader resides */

int  CT_close( unsigned int Ctn ) {
  
  if (IO_Close() == TRUE) {
    return OK;
    
  } else {
    return ERR_CT;
    
  }
}            

/* Sends/Receives Data to/from the Reader */

int  CT_data( unsigned int ctn, unsigned char *dad, unsigned char *sad,
              unsigned int  lc, unsigned char *cmd, unsigned int  *lr,
              unsigned char *rsp ) {
  
  /* Reader specific CT-BCS commands */
  
  int IretVal;                                /* Return Value    */


 if ( *dad == 1 ) {             /* This command goes to the reader */

    // Don't get confused here this is for the return saying
    // the source was the reader and the destination the host

    *sad = 1;                   /* Source Reader    */
    *dad = 2;                   /* Destination Host */

  /*******************/
  /* CT-BCS Commands */
  /*******************/

  /* Request ICC  - Turns on the reader/card */

  if ( (cmd[0] = 0x20) && (cmd[1] == 0x12) ) {


     // CHANGE:: Request ICC code goes here.


    
  /* Resets the Card/Terminal and returns Atr */

  } else if ( (cmd[0] = 0x20) && (cmd[1] == 0x11) ) {



     // CHANGE:: Reset code goes here.



  /* Get Status - Gets reader status */

  } else if ( (cmd[0] = 0x20) && (cmd[1] == 0x13) ) {



     // CHANGE:: Get Status code goes here.



  /* Eject ICC - Deactivates Reader  */

  } else if ( (cmd[0] = 0x20) && (cmd[1] == 0x15) ) {



     // CHANGE:: Eject ICC code goes here.



  } else {


    // CHANGE:: Write directly to the reader.


  }

 } else if ( *dad == 0 ) {      /* This command goes to the card */
    
    // Don't get confused here this is for the return saying
    // the source was the card and the destination the host

    *sad = 0;  /* Source Smartcard */
    *dad = 2;  /* Destination Host */
    

    // CHANGE:: Lots of code here.  This sends commands to the
    // smartcard directly and places responses into rsp with size
    // of lr.




 } else {
   IretVal = ERR_INVALID;              /* Invalid SAD/DAD Address */
 }

  if (IretVal != OK) {
    *lr = 0;
  }
  
  return IretVal;
}

