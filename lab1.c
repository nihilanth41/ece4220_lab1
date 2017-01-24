/* ECE3220 Lab 10 template
 * Author: Luis Rivera
 * 
 * This program shows how to map the ports of the TS7250 board so they can be used
 * in an application (User Space). */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
	int fd;		// for the file descriptor of the special file we need to open.
	unsigned long *BasePtr;		// base pointer, for the beginning of the memory page (mmap)
	unsigned long *PBDR, *PBDDR;	// pointers for port B DR/DDR
	unsigned long *PFDR, *PFDDR;	// pointers for port F DR/DDR
	int input, i;
	
	printf("Enter a number (0-4): ");
	scanf(" %d", &input);
	if(input < 0 || input > 4) 
	{
		printf("ERROR: Enter a number between 0 and 4\n");
		return EXIT_FAILURE;
	}

	fd = open("/dev/mem", O_RDWR|O_SYNC);	// open the special file /dem/mem
	if(fd == -1){
		printf("Error opening /dev/mem. Are you root?\n");
		return(-1);  // failed open
	}

	// We need to map Address 0x80840000 (beginning of the page)
	BasePtr = (unsigned long*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x80840000);
	if(BasePtr == MAP_FAILED){
		printf("\n Unable to map memory space \n");
		return(-2);
	}  // failed mmap

	// To access other registers in the page, we need to offset the base pointer to reach the
	// corresponding addresses. Those can be found in the board's manual.
	PBDR = BasePtr + 1;	// Address of port B DR is 0x80840004
	PBDDR = BasePtr + 5;	// Address of port B DDR is 0x80840014
	
	PFDR = BasePtr + 12; 
	PFDDR = BasePtr + 13;

	// Set user-specified pushButton as input (b == 0)
	*PBDDR &= ~(1 << input);
	printf("Waiting for button %d to be pressed...\n", input);
	do {
		// Read pushbutton
		int button_status = (*PBDR & (1 << input));
		if(button_status == 0) 
		{
			printf("Button %d pressed!\n", input);
			fflush(stdout);
			// Play sound
			// Set F1 as output (b == 1)
			*PFDDR |= 0x02;
			// Set F1 as HIGH
			for(i=0; i<20; i++)
			{
				// 2kHZ 
				// 1/2000 = 500uS
				usleep(100);
				*PFDR  |= 0x02;
				usleep(100);
				*PFDR &= ~(0x02);
			}
			// Set as input
			*PFDDR &= ~(0x02);
			usleep(500);
			break;
		}

	} while(1);
	
	close(fd);	// close the special file

	return 0;
}
