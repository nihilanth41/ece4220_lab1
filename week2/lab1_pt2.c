#ifndef MODULE 
#define MODULE
#endif

#ifndef __KERNEL__
#define __KERNEL__
#endif

#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

int init_module(void) {
	unsigned long *BasePtr;		// base pointer, for the beginning of the memory page (mmap)
	unsigned long *PBDR, *PBDDR;	// pointers for port B DR/DDR
	
	// Attempt to map file descriptor
	BasePtr = (unsigned long *) __ioremap(0x80840000, 4096, 0);
	if(NULL == BasePtr) 
	{
		printk(KERN_INFO "Unable to map memory space\n");
		return -1;
	}
	
	// Configure PORTB registers
	PBDR = BasePtr + 1;
	PBDDR = BasePtr + 5;

	// Red/B5 output	
	*PBDDR |= 0x20;
	// Yellow/B6 output
	*PBDDR |= 0x40;
	// Green/B7 output
	*PBDDR |= 0x80;
	
	// Red on
	*PBDR |= 0x20;
	// Yellow on
	*PBDR |= 0x40;
	// Green off
	*PBDR &= ~(0x80);

	printk(KERN_INFO "MODULE INSTALLED\n");
	return 0;
}

void cleanup_module(void)
{
	unsigned long *BasePtr;		// base pointer, for the beginning of the memory page (mmap)
	unsigned long *PBDR, *PBDDR;	// pointers for port B DR/DDR
	
	// Attempt to map file descriptor
	BasePtr = (unsigned long *) __ioremap(0x80840000, 4096, 0);
	if(NULL == BasePtr) 
	{
		printk(KERN_INFO "Unable to map memory space\n");
		return;
	}
	// Configure PORTB registers
	PBDR = BasePtr + 1;
	PBDDR = BasePtr + 5;

	// Red/B5 output	
	*PBDDR |= 0x20;
	// Yellow/B6 output
	*PBDDR |= 0x40;
	// Green/B7 output
	*PBDDR |= 0x80;
	
	// Red off
	*PBDR &= ~(0x20);
	// Yellow on
	*PBDR &= ~(0x40);
	// Green off
	*PBDR &= ~(0x80);
	
	printk(KERN_INFO "MODULE REMOVED\n");
	return;
}
