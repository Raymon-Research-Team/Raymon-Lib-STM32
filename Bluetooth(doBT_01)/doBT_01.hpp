#pragma once
#include "stm32f1xx_hal.h"
#include <iostream>
#include <string.h>

class doBT_01
{
  public:
	string inputBuffer;
	//===============================================================================
	// Constructor with USART handle
	doBT_01(UART_HandleTypeDef *huart)
		: huart(huart), baudRate(115200), bleEnabled(true), sppEnabled(true),
		  ledAlternate(false), powerOnLOGEnabled(true), transmissionPower(9)
	{
		strcpy(serviceUUID, "FFF0");
		strcpy(characteristicUUIDs[0], "FFF1");
		strcpy(characteristicUUIDs[1], "FFF2");
		strcpy(characteristicUUIDs[2], "FFF3");
	}
	//===============================================================================
	// Query baud rate
	bool queryBaudRate(char *response, uint16_t responseSize)
	{
		return sendATCommand("AT+QT\r\n", response, responseSize);
	}
	//===============================================================================
	// Set baud rate
	bool setBaudRate(int baudRate)
	{
		char command[50];
		uint8_t code = baudRateToCode(baudRate);
		if (code == 0)
			return false; // Invalid baud rate
		snprintf(command, sizeof(command), "AT+CT%02d\r\n", code);
		char response[50];
		return sendATCommand(command, response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	// Reset module
	bool resetModule()
	{
		char response[50];
		return sendATCommand("AT+CZ\r\n", response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	// Query BLE name
	bool queryBLEName(char *response, uint16_t responseSize)
	{
		return sendATCommand("AT+TM\r\n", response, responseSize);
	}
	//===============================================================================
	// Set BLE name
	bool setBLEName(const char *name)
	{
		char command[50];
		snprintf(command, sizeof(command), "AT+BM%s\r\n", name);
		char response[50];
		return sendATCommand(command, response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	// Query SPP name
	bool querySPPName(char *response, uint16_t responseSize)
	{
		return sendATCommand("AT+TD\r\n", response, responseSize);
	}
	//===============================================================================
	// Set SPP name
	bool setSPPName(const char *name)
	{
		char command[50];
		snprintf(command, sizeof(command), "AT+BD%s\r\n", name);
		char response[50];
		return sendATCommand(command, response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	// Query BLE MAC address
	bool queryBLEMACAddress(char *response, uint16_t responseSize)
	{
		return sendATCommand("AT+TN\r\n", response, responseSize);
	}
	//===============================================================================
	// Set BLE MAC address
	bool setBLEMACAddress(const char *macAddress)
	{
		char command[50];
		snprintf(command, sizeof(command), "AT+BN%s\r\n", macAddress);
		char response[50];
		return sendATCommand(command, response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	// Enable/disable BLE
	bool setBLEEnable(bool enable)
	{
		char command[50];
		snprintf(command, sizeof(command), "AT+B4%02d\r\n", enable ? 1 : 0);
		char response[50];
		return sendATCommand(command, response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	// Enable/disable SPP
	bool setSPPEnable(bool enable)
	{
		char command[50];
		snprintf(command, sizeof(command), "AT+B5%02d\r\n", enable ? 1 : 0);
		char response[50];
		return sendATCommand(command, response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	// Restore factory settings
	bool restoreFactorySettings()
	{
		char response[50];
		return sendATCommand("AT+CW\r\n", response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	// Set LED connection status
	bool setLEDConnectionStatus(bool alternate)
	{
		char command[50];
		snprintf(command, sizeof(command), "AT+CF%02d\r\n", alternate ? 1 : 0);
		char response[50];
		return sendATCommand(command, response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	// Enable/disable chip power-on LOG information
	bool setPowerOnLOG(bool enable)
	{
		char command[50];
		snprintf(command, sizeof(command), "AT+CR%02d\r\n", enable ? 1 : 0);
		char response[50];
		return sendATCommand(command, response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	// Set transmission power
	bool setTransmissionPower(int powerLevel)
	{
		if (powerLevel < 0 || powerLevel > 9)
			return false;
		char command[50];
		snprintf(command, sizeof(command), "AT+BR%02d\r\n", powerLevel);
		char response[50];
		return sendATCommand(command, response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	// Modify BLE UUID
	bool setServiceUUID(const char *uuid)
	{
		char command[50];
		snprintf(command, sizeof(command), "AT+U0F%s\r\n", uuid);
		char response[50];
		return sendATCommand(command, response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
	bool setCharacteristicUUID(int characteristicNumber, const char *uuid)
	{
		if (characteristicNumber < 1 || characteristicNumber > 3)
			return false;
		char command[50];
		snprintf(command, sizeof(command), "AT+U0F%d%s\r\n", characteristicNumber, uuid);
		char response[50];
		return sendATCommand(command, response, sizeof(response)) && strstr(response, "OK");
	}
	//===============================================================================
  private:
	void Usart_Flush(UART_HandleTypeDef *huart)
	{
		while (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE))
		{
			uint8_t temp = huart->Instance->DR; // خواندن و دور ریختن داده‌ها
		}
	}
	//===============================================================================
	// Helper function to send AT command and get response
	bool sendATCommand(const char *command, char *response, uint16_t responseSize)
	{
		// Send command via USART
		Usart_Flush(huart);
		HAL_UART_Transmit(huart, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);

		// Receive response (assuming response ends with \r\n)
		uint8_t buffer[50];
		HAL_UART_Receive(huart, buffer, 50, 100);

		// Copy response to output buffer
		strncpy(response, (char *)buffer, responseSize - 1);
		response[responseSize - 1] = '\0'; // Ensure null-termination

		return true;
	}
	//===============================================================================
	// Convert baud rate to code
	uint8_t baudRateToCode(int baudRate)
	{
		switch (baudRate)
		{
		case 9600:
			return 1;
		case 19200:
			return 2;
		case 38400:
			return 3;
		case 57600:
			return 4;
		case 115200:
			return 5;
		case 256000:
			return 6;
		case 512000:
			return 7;
		case 230400:
			return 8;
		case 460800:
			return 9;
		case 1000000:
			return 10;
		case 31250:
			return 11;
		case 2400:
			return 12;
		case 4800:
			return 13;
		default:
			return 0; // Invalid baud rate
		}
	}
	//===============================================================================
	// USART handle
	UART_HandleTypeDef *huart;

	//===============================================================================
	// Current settings
	int baudRate;
	bool bleEnabled;
	bool sppEnabled;
	bool ledAlternate;
	bool powerOnLOGEnabled;
	int transmissionPower;
	char serviceUUID[10];
	char characteristicUUIDs[3][10];
	//===============================================================================
};
