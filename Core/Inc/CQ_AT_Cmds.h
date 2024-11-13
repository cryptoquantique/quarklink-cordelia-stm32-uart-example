/**
  ******************************************************************************
  * @file           : CQ_AT_Cmds.h
  * @brief          : AT Command function header file
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

#ifndef INC_CQ_AT_CMDS_H_
#define INC_CQ_AT_CMDS_H_

void handleEventStartup(const char *params);
void handleATOK(const char *params);
void handleATError(const char *params);
void handleATipv4(const char *params);
void handleATWiFidisconnected(const char *params);
void handleATmqtt(const char *params);
void handleATmqttDisconnect(const char *params);
void handleATrevoked(const char *params);
void handleATsub(const char *params);
void handleEnrolment(const char *params);
void handleAWSEnrolment(const char *params);
void handleMQTTEnrolment(const char *params);
void MQTTConnect(void);
void QueryIoTHub(void);

// Define a GPIO to toggle the Cordelia RESET pin
#define CordeliaReset_Pin GPIO_PIN_5
#define CordeliaReset_GPIO_Port GPIOC

// Define a structure for AT commands
typedef struct {
	const char command[30]; // max length of any string to parse
	void (*handler)(const char *params);
} AT_Responses;

// Number of possible AT command we are going to handle
#define AT_RESPONSES_SIZE 12


// *********************************************************************
// * Cordelia  control flags
// *********************************************************************
// state flags
typedef struct {
	int wlan;				// WiFi Connected
	int enroled;			// Device enrolled
	int connected;			// IoTHub Connected
	int wlan_in_progress;	// WiFi details sent, awaiting IP address
	int recv;				// flag to catch the birth MQTT message
	int aws;				// AWS IoTHub type
	int mqtt;				// QL MQTT IoTHub type
} CordeliaStatus;
extern CordeliaStatus status;

extern AT_Responses commands[];

// These are part of the QuarkLink Provisioning process so not used here
// These WiFi parameters are defined during QuarkLink provisioning of Cordelia
#define SSID "My_WiFi_SSID"  // Your WiFI SSID here
#define PWD  "My_WiFi_Password" // Your WiFI password here

#endif /* INC_CQ_AT_CMDS_H_ */
