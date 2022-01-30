NDB Module for Nano Deubgger rev1.0

NDB is a part of Nano Debugger, which is developed by GEDU Lab. NDB provides following
services to NDB:
- Kernel geography information for kernel debugging
- Download kernel modules or other files via the kernel debug session
- Other misctaneous tasks

Command to build:
make

Command to install service and NDB module:
sudo make install

Requirements:
This module is only tested on the GDK7 platform. Please don't load it 
on other environment. It may leak kernel information. So don't load it
when you are not debugging.

Command to install:
sudo rel.sh

Command to load:
sudo modprobe ndb

Command to unload:
sudo systemctl stop ndsrv
sudo rmmod ndb

Please find more information at:
http://nanocode.cn
http://advdbg.org/gdk

Contacts: yinkui.zhang@xedge.ai
