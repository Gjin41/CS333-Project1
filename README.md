# CS333-Project1
## Description
* This is a Linux kernel module which creates a character device and also generates random numbers in Kernel space that allows apps in User space to open and read.
* `333/vchar_driver.c` is the module and `333/user_app/user_random.c` is the user app to get random numbers.

## Usage
  To use this program, please follow these steps:
  
```
cd 333
make
```

* **Attach the module to kernel**\
```sudo insmod vchar_driver.ko```

* To check major number and minor number\
`dmesg`\
You will see the line `Allocated device number (x, y)` which x is the major number and y is the minor number.

* To show list of current devices\
`sudo cat /proc/devices`

* To check if device file is created or not\
`ls -la /dev/vchar_dev`

* **Get the right to read**\
`sudo chmod 666 /dev/vchar_dev`

* **To use this module in User space**
```
cd user_app
make
./user_app
```
And a selection board show up
```
Select below options:
	o (to open a device node)
	c (to close the device node)
	r (to get a random number)
	q (to quit the application)
Enter your option:
```

* **To detach the module from kernel**\
`sudo rmmod vchar_driver`

* **To clean in** `/333` **and** `/333/user_app`\
`make clean`

##Authors
- Giang Nguyen, email: ntgiang@apcs.vn
- Hoang Pham, email: pnhhoang@apcs.vn
