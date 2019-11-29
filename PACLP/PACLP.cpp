#define _CRT_SECURE_NO_WARNINGS

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
#include <string>


char serialPort[6] = "COM1";
int baudRate = 19200;
modbus_t* ctx = NULL;

std::string ledsToRead = "";
std::string ledToWrite = "";
int ledValueToWrite = 0;

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

int main(int argc, char* argv[])
{
	if (argc == 1) {
		std::cout << "ERROR: No serialport or baud rate has been set." << std::endl;
		return -1;
	}

	//porta serial
	strcpy(argv[1], serialPort);
	
	//baud rate
	baudRate = atoi(argv[2]);

	//LEDs to be read
	ledsToRead = argv[3];

	//LED to write and its value
	ledToWrite = argv[4];
	ledValueToWrite = atoi(argv[4]);
		

	setUpSerialConnection();

	uint16_t registerReadR = -1;
	uint16_t registerReadY = -1;
	uint16_t registerReadG = -1;
	if (ledsToRead.find('rled'))
		registerReadR = readRegisters(0); //registrador 0 -> LED vermelho
	if (ledsToRead.find('yled'))
		registerReadY = readRegisters(1); //registrador 1 -> LED amarelo
	if (ledsToRead.find('gled'))
		registerReadG = readRegisters(2); //registrador 2 -> LED verde


	std::cout << "\n--------------------------------------------------------\n" << "Returned values:" <<  std::endl;
	if (registerReadR != -1)
		std::cout << "Red LED: " << registerReadR << std::endl;
	if (registerReadR != -1)
		std::cout << "Yellow LED: " << registerReadY << std::endl;
	if (registerReadR != -1)
		std::cout << "Green LED: " << registerReadG << std::endl;



	if (ledToWrite.find("rled"))
		writeRegister(0, ledValueToWrite);
	else if (ledToWrite.find("yled"))
		writeRegister(1, ledValueToWrite);
	else if (ledToWrite.find("gled"))
		writeRegister(2, ledValueToWrite);

	std::cout << std::endl << "\n--------------------------------------------------------\n" << "Returned values:" << std::endl;
	if (!ledToWrite.find("none"))
		std::cout << ledToWrite << " updated successfully to " << ledValueToWrite << std::endl;



	closeConnection();

	int aux = 0;
	std::cin >> aux;

}