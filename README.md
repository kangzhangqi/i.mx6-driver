## This is a linux bottom driver source code for i.mx6. And the chip we use is i.mx6QuadPlus from NXP. 

---

- _i.mx6 doc_ : This is some datasheet,sch,layout file of board.

- _led\_driver_: This is the led driver source code and test application source code.
The _output_ floder is the binary file.Copy these two files to board,_insmod xxx.ko_,and use _cat /proc/devices_ and _ls -l /dev_ to check the device node and it's host code.Then use _./test_ to verrify this led driver.
