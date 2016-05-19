# Quadcopter - Guides
### Libraries

 **libmicrohttpd** (http server api)  
 
 - Download [lastest libmicrohttpd](http://ftpmirror.gnu.org/libmicrohttpd/libmicrohttpd-0.9.49.tar.gz)  
 - Install:
   
```sh
./configure
 make
 sudo make install
```

 - Usage:
```c
#include "microhttpd.h"
```
   

----------


**openssl** (used for md5 generator)

 - Download:
   
```sh 
  sudo apt-get install libssl-dev
```
  
 - Usage:
  
```c
#include <openssl/md5.h>
```


----------

**json** (used for json string parsing)

 - Download: clone soure [here](https://github.com/zserge/jsmn)
    
 - Usage:  

```sh
 make
 copy libjsmn.a to "libraries/json" folder
 #include "jsmn.h"
```
    
   
### Folder structure:
```c
--> app/            // main application  
--> libraries/      // contain libraries used for app  
    --> json/       // json lib  
--> README.md       // this file - project guide  
--> testcase_curl.txt           // testcase  
```

### Build and Run:

```sh
cd app/
make
./quadcopter
```
###Test with cURL:
- open other terminal window
- try to use testcase in file `testcase_curl.txt`
- Note that: Change 'localhost' by your remote device'sIP

###Note:
- Port forwarding 80 to 8080:
```c
sudo iptables -t nat -A OUTPUT -o lo -p tcp --dport 80 -j REDIRECT --to-port 8080
```
- Run following command to auto port forwarding on boot:
```c
sudo sh -c "iptables-save > /etc/iptables.rules"
```
- Export linux library path:
  Open `.bashrc` file: `vi ~/.bashrc` and put:
```sh
LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH
```

