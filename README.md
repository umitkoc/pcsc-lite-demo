# pcsc-lite-demo


## requirement

For Ubuntu:
```
copy include folder from project
paste /usr/local/include/PCSC

run command below:
mkdir build
cd build
cmake ..
make && ./program

Note:open Drivers folder for Drivers install
before  open ACS-Unified-Driver-Lnx-Mac-118-P
command: sudo ./bootstrap
         sudo ./configure
         sudo make install 
         
used command after for device control
open terminal : pscs_scan

```




<a href="https://muscle.apdu.fr/musclecard.com/software.html">Driver Install :</a>



<a href="https://man.freebsd.org/cgi/man.cgi?query=pcscd&sektion=8&manpath=freebsd-release-ports">FreeBSD  :</a>


<a href="https://bugs.gentoo.org/775968">bugs.gentoo  :</a>




<a href="https://ludovicrousseau.blogspot.com/2013/11/linux-nfc-driver-conflicts-with-ccid.html">ludovicrousseau.blogspot.com  :</a>








