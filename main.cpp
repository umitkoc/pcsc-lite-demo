#include <iostream>
#include <PCSC/winscard.h>

auto main()->int {
SCARDCONTEXT hContext;
auto dwReaders = SCARD_AUTOALLOCATE;
LPTSTR mszReaders;
LONG rv;

// Establish context with the PC/SC resource manager
rv = SCardEstablishContext(SCARD_SCOPE_SYSTEM, nullptr, nullptr, &hContext);
if (rv != SCARD_S_SUCCESS) {
std::cout<<"SCardEstablishContext failed: "<<std::hex<<rv<< std::endl;
return 1;
}

// List available readers
rv = SCardListReaders(hContext, nullptr, (LPSTR)&mszReaders, &dwReaders);
if (rv != SCARD_S_SUCCESS) {
std::cout<<"SCardListReaders failed: "<< std::hex<<rv <<std::endl;
SCardReleaseContext(hContext);
return 1;
}

// Check if any readers are found
if (dwReaders == 0) {
std::cout<<"No reader found\n";
SCardFreeMemory(hContext, mszReaders);
SCardReleaseContext(hContext);
return 1;
}

// Select the first reader (modify for specific reader selection)
LPTSTR readerName = mszReaders;

// Attempt to connect to the reader
SCARDHANDLE hCard;
DWORD dwActiveProtocol;
rv = SCardConnect(hContext, readerName, SCARD_SHARE_SHARED,
                  SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCard, &dwActiveProtocol);

// Check connection status
if (rv == SCARD_S_SUCCESS) {
std::cout<<"Reader '%s' is connected"<<readerName;
SCardDisconnect(hContext, hCard);
} else {
std::cout << "Failed to connect to reader '" << readerName << "': 0x"
<< std::hex << rv << std::endl;
}

// Free memory and release context
SCardFreeMemory(hContext, mszReaders);
SCardReleaseContext(hContext);

return 0;
}