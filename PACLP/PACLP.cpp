
//#include "libmodbus/src/modbus.h"


#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#include <sys/time.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "libmodbus/src/modbus.h"
#include <iostream>
void test() {
	//Create a new RTU context with proper serial parameters (in this example,
//device name /dev/ttyS0, baud rate 9600, no parity bit, 8 data bits, 1 stop bit)
	//char serialPort[9] = "\\\\.\\COM2";
	char serialPort[5] = "COM7";

	modbus_t* ctx = modbus_new_rtu(serialPort, 9600, 'N', 8, 1);
	if (!ctx) {
		fprintf(stderr, "Failed to create the context: %s\n", modbus_strerror(errno));
		exit(1);
	}

	if (modbus_connect(ctx) == -1) {
		fprintf(stderr, "Unable to connect: %s\n", modbus_strerror(errno));
		modbus_free(ctx);
		exit(1);
	}

	//Set the Modbus address of the remote slave (to 3)
	modbus_set_slave(ctx, 1);

	uint16_t reg[5];// will store read registers values

	//Read 5 holding registers starting from address 10
	int num = modbus_read_registers(ctx, 1, 5, reg);
  	if (num ==-1) {// number of read registers is not the one expected		
		fprintf(stderr, "Failed to read: %s\n", modbus_strerror(errno));
	}

	modbus_write_register(ctx, 0, 6969);

	std::cout << "\n" << reg[1] << "\n";

	modbus_close(ctx);
	modbus_free(ctx);
}




int main(/*int argc, char* argv[]*/)
{
	test();

}