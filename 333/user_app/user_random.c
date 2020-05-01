#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define DEVICE_NODE "/dev/vchar_dev"

//Function to check open entry point
int open_chardev() {
    int fd = open(DEVICE_NODE, O_RDWR);
    if(fd < 0) {
        printf("Can not open the device file\n");

        exit(1);
    }
    return fd;
}

//Function to check entry point release
void close_chardev(int fd) {
    close(fd);
}

//Function to get the random number from character device
void read_data_chardev(){
	int num;
	int fd = openat(fd, "/dev/vchar_dev", O_RDONLY);
	if(fd < 0){
		printf("Failed to open character device file\n");
		return;
	}
	read(fd, &num, sizeof(num));
	close_chardev(fd);
	printf("The random number is: %d\n", abs(num));
}


int main() {
	int ret = 0;
	char option = 'q';
	int fd = -1;
	printf("Select below options:\n");
	printf("\to (to open a device node)\n");
	printf("\tc (to close the device node)\n");
	printf("\tr (to get a random number)\n");
	printf("\tq (to quit the application)\n");
	
	while (1) {
	printf("Enter your option: ");
	scanf(" %c", &option);

		switch (option) {
		    case 'o':
			if (fd < 0)
			    fd = open_chardev();
			else
			    printf("%s has already opened\n", DEVICE_NODE);
			break;
		    case 'c':
			if (fd > -1)
			    close_chardev(fd);
			else
			    printf("%s has not opened yet! Can not close\n", DEVICE_NODE);
			fd = -1;
			break;
		    case 'r':
			    read_data_chardev();
			    break;
		    case 'q':
			if (fd > -1)
			    close_chardev(fd);
			printf("Quit the application. Good bye!\n");
			return 0;
		    default:
			printf("invalid option %c\n", option);
			break;
		}
	};
}
