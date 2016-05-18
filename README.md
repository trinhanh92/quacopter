# How to use:

## Libraries:
### [+] libmicrohttpd (http server api)
#### install:
  		  ./configure
  		  make
  		  sudo make install
#### usage:
  		  include "microhttpd.h"
### [+] openssl (using for md5 encryption)
####  install:
  		  sudo apt-get install libssl-dev
####  usage:
  		  include <openssl/md5>
### [+] jsmn (using for json parsing)
####  clone source here:
####  usage:
  		  make
  		  copy libjsmn.a to "libraries/json" folder
  		  include "jsmn.h"

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