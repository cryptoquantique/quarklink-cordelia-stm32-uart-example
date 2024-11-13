/**
  ******************************************************************************
  * @file           : CQ_AT_Cmds.c
  * @brief          : AT Command functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Crypto Quantique.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
**/

#include "CQ_AT_Cmds.h"
#include "CQ_Led.h"
#include "CQ_Debug.h"
#include "stm32f4xx_hal.h"
#include "string.h"
#include "stdio.h"
#include "main.h"

CordeliaStatus status = { 0, 0, 0, 0, 0, 0 ,0}; // clear status flags

// *********************************************************************
// / AT Commands
// *********************************************************************

// Array of supported AT commands
AT_Responses commands[] = {
		{ "OK", handleATOK }, 							// OK
		{ "ERROR:", handleATError }, 					// error
		{ "ipv4_acquired", handleATipv4 }, 				// WiFi connected
		{ "eventwlan:disconnect",handleATWiFidisconnected },	// WiFi disconnected
		{ "HTTP Client error:", handleATWiFidisconnected },	// HTTP Error - reconnect
		{ "MQTT_EVENT_CONNACK", handleATmqtt },			// MQTT connected
		{ "MQTT_EVENT_SERVER_DISCONNECT", handleATmqttDisconnect },	// MQTT Disconnected
		{ "Device is revoked", handleATrevoked },		// Device Revoked
		{ "+eventmqtt:recv,", handleATsub },			// MQTT message received from a publisher
		{ "Enrolment complete", handleEnrolment },		// IoTHubEnrolment
		{ "amazonaws.com", handleAWSEnrolment },		// AWS IoTHub type
		{ "quarklink.io", handleMQTTEnrolment },		// MQTT IoTHub type
		};

// *********************************************************************
// * AT command handler functions
// *********************************************************************

/**
 * @brief  AT command handler - 'OK' response. Status flags are set to control onward commands
 * @param  Pointer to a UART RX Buffer
 * @retval None
 **/
void handleATOK(const char *params) {
	// INFO_AT("OK response received\r\n");

	// Got OK response, WiFi is not configured/connected so lets send it
	// wlan_in_progress is used because we get the OK response to the command being received
	// WiFi is considered connected once we received the 'ipv4_acquired' string
	// We don't need to config the WiFi as it's part of the QL provisioning process
	if (!status.wlan & !status.wlan_in_progress) {
		INFO_AT("Connecting to WiFi\r\n");
		char wifi[65] = {0}; // user should to check size once own WiFi credentials are added.
		snprintf(wifi, 65, "AT+wlanconnect=%s,,WPA_WPA2,%s,,,\r\n", SSID, PWD);
		HAL_UART_Transmit(&huart6, (uint8_t*) &wifi, (strlen(wifi)), 100);
		DEBUG_AT("%s",wifi);
		// Set the correct status flag
		status.wlan_in_progress = 1;
	}

	// After a call to Query IoTHub we need to connect to the correct hub
	if (status.wlan && status.enroled && !status.connected && !status.recv) {
		MQTTConnect();
	}

	// Got OK response, WiFi is configured and connected, MQTT is configured so lets publish
	if (status.wlan && status.enroled && status.connected && !status.recv) {
		// Publish something
		char string3[] = "AT+iotpublish=3,\"Hello from STM32\"\r\n";
		HAL_UART_Transmit(&huart6, (uint8_t*) &string3, (strlen(string3)),100);
		DEBUG_AT("%s",string3);
		INFO_AT("Publish an MQTT Message\r\n");

		// Just hang this here to stop us looping in an OK loop after every button press/publish.
		status.recv = 1;
	}
}

/**
 * @brief  AT command handler - 'ERROR'
 * @param  Pointer to a UART RX Buffer
 * @retval None
 **/
void handleATError(const char *params) {
	// HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	INFO_AT("Received an ERROR\r\n");
#if 0
	if ((!status.wlan && !status.enroled)) {
		// write at+test to flush the buffer and force a retry via the OK handler
		// This is for the HTTP errors that we sometimes see
		char string1[] = "AT+test\r\n";
		HAL_UART_Transmit(&huart6, (uint8_t*) &string1, (strlen (string1)), 100);
	}
#endif
	if ((!status.wlan)) {
		char string1[] = "AT+test\r\n";
		HAL_UART_Transmit(&huart6, (uint8_t*) &string1, (strlen (string1)), 100);
	}
	if ((status.wlan && !status.enroled))
	{
		DEBUG_AT("**** Not enrolled! ****\r\n");
	}
	if ((status.wlan && !status.connected))
	{
		DEBUG_AT("**** Not connected! ****\r\n");
	}
}

/**
 * @brief  AT command handler - 'handleATipv4' - received once WiFi has an IP address
 * @param  Pointer to a UART RX Buffer
 * @retval None
 */
void handleATipv4(const char *params) {
	// Reset the flags because we're going to reenrol
	status.connected = 0;
	status.enroled = 0;
	status.recv = 0;
	status.aws = 0;
	status.mqtt= 0;

	// Reset the IoTHub LED as we're not connected.
	HAL_GPIO_WritePin(IoTHub_Connected_GPIO_Port, IoTHub_Connected_Pin,
			GPIO_PIN_RESET);

	// Set the wlan_in_progress flag - if we have previously sent the WiFi details, Cordelia will auto connect so for completeness we set this flag.
	status.wlan_in_progress = 1;
	// WiFi connected so we can set the status flag
	status.wlan = 1;

	// Set the LED on to indicate at WiFi is connected.
	HAL_GPIO_WritePin(WiFi_Connected_GPIO_Port, WiFi_Connected_Pin,
			GPIO_PIN_SET);

	// Now we need to enrol onto QuarkLink
	char enrol[] = "AT+iotenrol\r\n";
	HAL_UART_Transmit(&huart6, (uint8_t*) &enrol, (strlen(enrol)), 100);
	DEBUG_AT("%s",enrol);
	INFO_AT("Connected to WiFi, attempting to enrol\r\n");
}


/**
 * @brief  AT command handler - 'HTTP Client error:' - received once WiFi has been disconnected
 * @param  Pointer to a UART RX Buffer
 * @retval None
 */
void handleATWiFidisconnected(const char *params) {
	// All we can do here is try to reconnect WiFi.
	// Lets reset the flags and send a simple AT+test to trigger the OK Handler

	// Reset the flags because we're going to reconnect
	status.wlan = 0;
	status.wlan_in_progress = 0;
	status.connected = 0;
	// Reset the WiFi COnnected LED as we're not connected.
	HAL_GPIO_WritePin(WiFi_Connected_GPIO_Port, WiFi_Connected_Pin,
			GPIO_PIN_RESET);
	// Reset the IoTHub LED as we're not connected.
	HAL_GPIO_WritePin(IoTHub_Connected_GPIO_Port, IoTHub_Connected_Pin,
			GPIO_PIN_RESET);
}


/**
 * @brief  AT command handler - 'handleATmqtt' - received after MQTT connection setup complete
 * @param  Pointer to a UART RX Buffer
 * @retval None
 */
void handleATmqtt(const char *params) {
	INFO_AT("Connected to MQTT Broker\r\n");
	// MQTT is connected so set the flag so that the publish command can be sent.
	// We get an OK (or ERROR) once the command is complete so we can handle the next step in the respective handler.
	status.connected = 1;

	// Set the LED on to indicate at IoTHub is connected.
	HAL_GPIO_WritePin(IoTHub_Connected_GPIO_Port, IoTHub_Connected_Pin,
			GPIO_PIN_SET);
}

/**
 * @brief  AT command handler - 'MQTT_EVENT_SERVER_DISCONNECT' response.
 * Status flags are checked to control which IoTHub re-connection process we should follow
 * @param  Pointer to a UART RX Buffer
 * @retval None
 **/
void handleATmqttDisconnect(const char *params) {
	INFO_AT("Disconnected from IoTHub\r\n");
	DEBUG_AT("Disconnected from IoTHub\r\n");

	// MQTT is disconnected so clear the flag so that the enrol request can be sent.
	// We get an OK (or ERROR) once the command is complete so we can handle the next step in the respective handler.
	status.connected = 0;
	status.recv = 0;

	// Clear the LED on to indicate at IoTHub is disconnected.
	HAL_GPIO_WritePin(IoTHub_Connected_GPIO_Port, IoTHub_Connected_Pin, GPIO_PIN_RESET);

	if (status.aws == 2) {
		INFO_AT("AWS JITP Process complete\r\n");
		// Lets try to reconnect - first JITP connect always fails as we need to kick off the process
		char string1[] = "AT+iotconnect\r\n";
		HAL_UART_Transmit(&huart6, (uint8_t*) &string1, (strlen(string1)), 100);
		INFO_AT("Connecting to AWS IotHub\r\n");
		DEBUG_AT("%s",string1);
	} else {
		// INFO_AT("Disconnected from IoTHub\r\n");
		// Lets try to re-enrol
		char string[] = "AT+iotenrol\r\n";
		HAL_UART_Transmit(&huart6, (uint8_t*) &string, (strlen(string)), 100);
		DEBUG_AT("%s",string);
		INFO_AT("Attempting to re-enrol\r\n");
	}
}

/**
 * @brief  AT command handler - 'Device is revoked' response.
 * @param  Pointer to a UART RX Buffer
 * @retval None
 **/
void handleATrevoked(const char *params) {
	INFO_AT("Device is revoked\r\n");

	status.enroled = 0;
	status.aws = 0;		// Reset the AWS count so we try to double mqttconnect due to JITP process.
	status.mqtt = 0;	// Reset the MWTT IoT hub flag as we're going to recheck
	// Handle re-enrolment via a button press.
}

/**
 * @brief  AT command handler - 'handleATsub' - received after MQTT Subscribed message is received
 * @param  Pointer to the remaining Rx_buffer string
 * @retval None
 */
void handleATsub(const char *params) {
	INFO_AT("MQTT Subscribed message received\r\n");
	DEBUG_AT("MQTT Subscribed message received\r\n");
	Start_SUBLED_Flashing();
}

/**
 * @brief  AT command handler - 'Enrolment complete' - received after QL enrolment process is successful
 * @param  Pointer to the remaining Rx_buffer string
 * @retval None
 */
void handleEnrolment(const char *params) {
	status.enroled = 1;
	INFO_AT("Successfully enroled\r\n");
	QueryIoTHub();
}

/**
 * @brief  AT command handler - 'amazonaws.com' - is seen when we query the IoTHub
 * We need to know if we have AWS or QL MQTT IoTHub as the connection process differs
 * @param  Pointer to the remaining Rx_buffer string
 * @retval None
 */
void handleAWSEnrolment(const char *params) {
	// we've got an IoTHub address for AWS
	status.aws++;
	status.mqtt=0;
	// We need to check the state and try to reconnect as the first connection will failed due to the JIPT process
}

/**
 * @brief  AT command handler - 'quarklink.io' - is seen when we query the IoTHub, but also when we enrol
 * We need to know if we have AWS or QL MQTT IoTHub as the connection process differs
 * @param  Pointer to the remaining Rx_buffer string
 * @retval None
 */
void handleMQTTEnrolment(const char *params) {
	// we've got an IoTHub address for AWS
	status.aws=0;
	status.mqtt=1;
	// We need to check the state and try to reconnect as the first connection will failed due to the JIPT process
}

/**
 * @brief  Function to set status flags and query the IoTHub type
 * We need to know if we have AWS or QL MQTT IoTHub as the connection process differs
 * @param  None
 * @retval None
 */
void QueryIoTHub(void) {
	// We clear the MQTT flag here as it may have been incorrectly set when enroling
	// as the Ignite instance name will contain quarklink.io
	status.mqtt=0;
	status.aws=0;
	char string[] = "at+get=MQTT,iotHubEndpoint\r\n";
	HAL_UART_Transmit(&huart6, (uint8_t*) &string, (strlen(string)), 100);
	DEBUG_AT("%s",string);
	INFO_AT("Querying IoTHub\r\n");
}

/**
 * @brief  Function to trigger the IoTHob connection process
 * we print a different INFO_AT message to inform the user what is happening
 * @param  None
 * @retval None
 */
void MQTTConnect(void) {
	if (status.mqtt) {
		// Lets connect to the MQTT broker
		char string[] = "AT+iotconnect\r\n";
		HAL_UART_Transmit(&huart6, (uint8_t*) &string, (strlen(string)), 100);
		DEBUG_AT("%s",string);
		INFO_AT("Connecting to QL MQTT\r\n");
	}

	if (status.aws==1) {
		// Lets try to reconnect - first JITP connect always fails as we need to kick off the process
		char string1[] = "AT+iotconnect\r\n";
		HAL_UART_Transmit(&huart6, (uint8_t*) &string1, (strlen(string1)), 100);
		DEBUG_AT("%s",string1);
		INFO_AT("Beginning the AWS JITP Process\r\n");
	}
}
