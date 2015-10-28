/*
 * main.c
 *
 *  Created on: Oct 19, 2015
 *      Author: uzair
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096

int main(int argc, char *argv[]) {
	printf("Hello World!!\r\n");

	// writing to 0x70000000 (addressing a specific word in 512MB memory within the Arm Versatile Platform) to notify our SystemC models that linux has been booted and linux apps are now ready to run
    int mem_fd = open("/dev/mem", O_RDWR);
    if (mem_fd == -1) {
    	perror("Error opening file for writing");
    	exit(EXIT_FAILURE);
    }

    int *mem_map = malloc(PAGE_SIZE);				// dynamic array in C
    if (mem_map == NULL) {
    	printf("mem allocation error\r\n");
    	exit(-1);
    }

    mem_map = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0x70000000);
    if (mem_map == MAP_FAILED) {
    	close(mem_fd);
    	perror("Error mmapping the file");
    	exit(EXIT_FAILURE);
    }

    mem_map[0] = 0x12345678;

    if (munmap(mem_map, PAGE_SIZE) == -1) {
    	perror("Error unmapping the file");
    }
    close(mem_fd);

    return 0;
}