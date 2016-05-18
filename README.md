# How to use:

## Libraries:
### libmicrohttpd (http server api)
#### install:
  		  #./configure
  		  #make
  		  #sudo make install
#### usage:
  		  #include "microhttpd.h"
### openssl (using for md5 encryption)
####  install:
  		  #sudo apt-get install libssl-dev
####  usage:
  		  #include <openssl/md5>
### jsmn (using for json parsing)
####  clone source here:
####  usage:
  		  #make
  		  #copy libjsmn.a to "libraries/json" folder
  		  #include "jsmn.h"

##  Folder structure:
	--> app/				// main application
	--> libraries/			// contain libraries used for app
		--> json/           // json lib
	--> README.md           // this file - project guide

##  Build and Run:
	# cd app/
	# make
	# ./quadcopter

##  Test with cURL:
	- open other terminal window
	- try to use testcase in file
    - Note that: Change 'localhost' by your remote device'sIP

##  Note:
    Port forwarding 80 to 8080:
    #sudo iptables -t nat -A OUTPUT -o lo -p tcp --dport 80 -j REDIRECT --to-port 8080
    Run following command to auto port forwarding on boot:
    #sudo sh -c "iptables-save > /etc/iptables.rules"