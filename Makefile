lab10:
	gcc -g lab1.c -o lab1.bin 

arm: 
	/usr/local/opt/crosstool/arm-linux/gcc-3.3.4-glibc-2.3.2/bin/arm-linux-gcc -g lab1.c -o lab1-arm.bin
