# Cordelia STM32 UART AT Command Demonstration

# STM32CubeIDE based project that will demonstrate the simplicity of using the STM32F4 microcontroller, in combination with a Cordelia-I WiFi Module, to connect an IoT device to a cloud service by making use of the Crypto Quantique QuarkLink security platform.

Supporting this STM32 project is the CQ Cordelia-I STM32 AT Command Demo Application note. Please follow this application note in order to
quickly get up and running with your Cordelia-I based IoT device.

Additional support is available via video demonstration available from https://www.youtube.com/@CryptoQuantique

QuarkLink AT commands have been implemented in this project to demonstrate simplicity - just basic command/response for those that are needed. Nothing more, nothing less.

The Cordelia-I WiFi module will be 'provisioned' via the QuarkLink security platform which customers can access for free from https://quarklink.io/
Once the user has created their unique QuarkLink instance they can also access the Getting Started Guide for the Wurth Cordelia-I module for more detailed information.

This project can set the WiFi SSID for the Cordelia-I module but that's part of the QuarkLink provisioning process, Cordelia-I will automatically connect to WiFi if it has details - there is NO simple 'connect' command, you need to send the WiFI credentials to the Cordelia-I module.
Once connected to a netweork through WiFi the Cordelia-I Module witl enrol onto your QuarkLink instance.
During enrolment, the IoT Hub credentials will be securely transferred (over TLS) to the Cordelia-I WiFi module and stored securely.
The |Cordelia-I will then disconnect from your QuarkLink and connect directly to the IoT Hub that has been configured as part of your QuarkLink Policy.
The Cordelia-I moduel is then free to publish and subscribe to the MQQT IoT Hub.

If WiFi is disconnected - "eventwlan:disconnect" is handled by the code to reconnect - using the hard coded WiFi credentials (CQ_AT_Cmds.h).
If the device is revoked (via QuarkLink GUI) Cordelia-I will be disconnected from the IotHub and wait for a button press to attempt to re-enrol.

The current implementation parse's the AT response - OK/ERROR and sets a state flag - connecting/connected/mqtt etc. to decide what command to send next.
Some responses from Cordelia-I are more than OK/ERROR - 'at+wlanconnect' for example replys 'OK' but it's not connected.
Responses such as this are handled by the code by waiting for an 'ipv4_acquired' which indicated a WiFi connection and then proceeds to enrol.

Messages are publishes on "blue" button press.
The project subscribes to messages of the same topic.

LEDs GPIO have been implemented to show status - i.e. WiFi connected, IOT connected, blink on message sent/recieved etc. These are optional for the user.

INFO_xxx and DEBUG_xxx marcos have been included to print useful details to the STM32 COM port as well as a startup banner.

### Revocation
QuarkLink will force a disconnect from the MQTT broker if Revoke is initiated – whether the local QuarkLink MQTT or AWS broker is used.
Cordelia-I will issue a “MQTT_EVENT_SERVER_DISCONNECT” message that triggers the handleATmqttDisconnect function call.
In the handleATmqttDisconnect function we clear the status.enroled status bit and the IoTHub indicator LED then trigger a enrol request “AT+iot=enrol\r\n”

When Cordelia-I attempts to enrol again we will receive the “Device is revoked” message. We then sit and wait on a button press before we attempt to re-enrol.
Once the user has set the re-enrol request within QuarkLink and pressed the blue button the enrolment will succeed.
Note :  Cordelia-I will experience "out of memory" issues if the user repeatedly tries to re-enrol.

