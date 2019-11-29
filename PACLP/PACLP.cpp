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


char serialPort[6] = "";
int baudRate = 0;
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

int isSubstring(std::string s1, std::string s2)
{
	int M = s1.length();
	int N = s2.length();

	/* A loop to slide pat[] one by one */
	for (int i = 0; i <= N - M; i++) {
		int j;

		/* For current index i, check for pattern match */
		for (j = 0; j < M; j++)
			if (s2[i + j] != s1[j])
				break;

		if (j == M)
			return i;
	}

	return -1;
}

int main(int argc, char* argv[])
{
	if (argc == 1) {
		std::cout << "ERROR: No serialport or baud rate has been set." << std::endl;
		return -1;
	}

	//porta serial
	strcpy(serialPort, argv[1]);
	
	//baud rate
	baudRate = atoi(argv[2]);

	//LEDs to be read
	ledsToRead = argv[3];

	//LED to write and its value
	ledToWrite = argv[4];
	ledValueToWrite = atoi(argv[5]);
		

	setUpSerialConnection();

	std::cout << "******   debug   ******" << std::endl;
	std::cout << argv[1] << std::endl;
	std::cout << argv[2] << std::endl;
	std::cout << argv[3] << std::endl;
	std::cout << argv[4] << std::endl << std::endl;

	std::cout << serialPort << std::endl;
	std::cout << baudRate << std::endl;
	std::cout << ledsToRead << std::endl;
	std::cout << ledToWrite << std::endl;
	std::cout << ledValueToWrite << std::endl << std::endl << std::endl;



	std::cout <<"******   Modbus PC-PLC communication   ******" << std::endl;

	if (isSubstring("none",ledsToRead)) {
		std::cout << "Values read:" << std::endl;
		if (isSubstring("rled", ledsToRead))
			std::cout << "Red LED: " << readRegisters(0) << std::endl;//registrador 0 -> LED vermelho
		if (isSubstring("yled", ledsToRead))
			std::cout << "Yellow LED: " << readRegisters(1) << std::endl;//registrador 1 -> LED amarelo
		if (isSubstring("gled", ledsToRead))
			std::cout << "Green LED: " << readRegisters(2) << std::endl;//registrador 2 -> LED verde
	}
		
	if (isSubstring("none", ledToWrite)){
		if (!isSubstring("rled",ledToWrite))
			writeRegister(0, ledValueToWrite);
		else if (!isSubstring("yled", ledToWrite))
			writeRegister(1, ledValueToWrite);
		else if (!isSubstring("gled", ledToWrite))
			writeRegister(2, ledValueToWrite);

		std::cout << std::endl << "Updated value:" << std::endl;
		std::cout << ledToWrite << " updated successfully to " << ledValueToWrite << std::endl;
	}
	
	closeConnection();

	int aux = 0; //apenas para a janela não se fechar abruptamente
	std::cin >> aux;

}