/*
 * main.c
 *
 *  Created on: Oct 15, 2015
 *      Author: uzair
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <sched.h>


#define NUMINTS  (1024*4*64)
#define FILESIZE (NUMINTS * sizeof(int))
#define PAGE_SIZE 4096


int main(int argc, char *argv[])
{
	int count = 0;
    // making a virtual mapping of /dev/mem to this process address space
    int *mem_map;
    int mem_fd = open("/dev/mem", O_RDWR);
    if (mem_fd == -1) {
    	perror("Error opening file for writing");
    	exit(EXIT_FAILURE);
    }

    if ((mem_map = malloc(FILESIZE + (PAGE_SIZE-1))) == NULL) {
    	printf("mem allocation error\r\n");
    	exit(-1);
    }

    mem_map = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0x70000000);
    if (mem_map == MAP_FAILED) {
    	close(mem_fd);
    	perror("Error mmapping the file");
    	exit(EXIT_FAILURE);
    }

    // setting this process to only run on CPU1
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(1, &mask);
	if (sched_setaffinity(0, sizeof(mask), &mask) < 0) {
		printf("ERROR in setting cpu affinity\r\n");
		exit(-1);
	}

    // to notify the OVP platform model that linux has been booted with cpu-affinity being set
    mem_map[0] = 0xffffff00;

	// burn cpu1
	while(count < 1000) {
		mem_map[0] = count;
		count++;
	}

	count = 0;


    // setting this process to only run on CPU3
	CPU_ZERO(&mask);
	CPU_SET(3, &mask);
	if (sched_setaffinity(0, sizeof(mask), &mask) < 0) {
		printf("ERROR in setting cpu affinity\r\n");
		exit(-1);
	}

    // to notify the OVP platform model that linux has been booted with cpu-affinity being set
    mem_map[0] = 0xffffff00;

	// burn cpu3
	while(count < 1000) {
		mem_map[0] = count;
		count++;
	}

	count = 0;












    // setting this process to only run on CPU0
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	if (sched_setaffinity(0, sizeof(mask), &mask) < 0) {
		printf("ERROR in setting cpu affinity\r\n");
		exit(-1);
	}

    // to notify the OVP platform model that linux has been booted with cpu-affinity being set
    mem_map[0] = 0xffffff00;

	// burn cpu0
	while(count < 1000) {
		mem_map[0] = count;
		count++;
	}


	count = 0;












    // setting this process to only run on CPU2
	CPU_ZERO(&mask);
	CPU_SET(2, &mask);
	if (sched_setaffinity(0, sizeof(mask), &mask) < 0) {
		printf("ERROR in setting cpu affinity\r\n");
		exit(-1);
	}

    // to notify the OVP platform model that linux has been booted with cpu-affinity being set
    mem_map[0] = 0xffffff00;

	// burn cpu2
	while(count < 1000) {
		mem_map[0] = count;
		count++;
	}


	count = 0;









    if (munmap(mem_map, FILESIZE) == -1) {
    	perror("Error unmapping the file");
    }
    close(mem_fd);

    return 0;

}





