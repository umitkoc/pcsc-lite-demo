#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <PCSC/winscard.h>


int main()
{
    LONG rv;
    SCARDCONTEXT hContext;
    rv = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &hContext);
    if (rv != SCARD_S_SUCCESS)
    {
        printf("SCardEstablishContext: Cannot Connect to Resource Manager %lX\n", rv);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
