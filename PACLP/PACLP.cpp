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


char serialPort[6] = "COM7";
int baudRate = 9600;
modbus_t* ctx = NULL;

void setUpSerialConnection() {
	ctx = modbus_new_rtu(serialPort, baudRate, 'N', 8, 1);
	if (!ctx) {
		fprintf(stderr, "Failed to create the context: %s\n", modbus_strerror(errno));
		exit(1);
	}

	if (modbus_connect(ctx) == -1) {
		fprintf(stderr, "Unable to connect: %s\n", modbus_strerror(errno));
		modbus_free(ctx);
		exit(1);
	}

	modbus_set_slave(ctx, 1);
}

uint16_t readRegisters(int registerAddr) {
	uint16_t reg[1];
	int num = modbus_read_registers(ctx, registerAddr, 1, reg);
	if (num == -1) {
		fprintf(stderr, "Failed to read: %s\n", modbus_strerror(errno));
	}

	return reg[0];
}

void writeRegister(int registerAddr, uint16_t value) {

	int num = modbus_write_register(ctx, registerAddr, value);
	if (num == -1) {
		fprintf(stderr, "Failed to write: %s\n", modbus_strerror(errno));
	}
}

void closeConnection() {
	modbus_close(ctx);
	modbus_free(ctx);
}

void chooseConnectionInfo() {
	std::cout << "Digite a porta a se conectar (Recomendado COM1):\n";
	std::cin >> serialPort;
	std::cout << "\n";

	std::cout << "Digite BaudRate: (Recomendado 9600):\n";
	std::cin >> baudRate;
	std::cout << "\n";
}


int main()
{
	chooseConnectionInfo();
	setUpSerialConnection();

	uint16_t registerRead = readRegisters(0);
	std::cout << "\nValor lido: " << registerRead << "\n";

	writeRegister(0, 'L');

	closeConnection();

	//MODBUS_FC_READ_HOLDING_REGISTERS

}