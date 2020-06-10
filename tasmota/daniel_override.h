/*
  user_config_override.h - user configuration overrides my_user_config.h for Tasmota

  Copyright (C) 2020  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

// force the compiler to show a warning to confirm that this file is included
#warning **** user_config_override.h: Using Settings from this File ****

/*****************************************************************************************************\
 * USAGE:
 *   To modify the stock configuration without changing the my_user_config.h file:
 *   (1) copy this file to "user_config_override.h" (It will be ignored by Git)
 *   (2) define your own settings below
 *   (3) for platformio:
 *         define USE_CONFIG_OVERRIDE as a build flags.
 *         ie1 : export PLATFORMIO_BUILD_FLAGS='-DUSE_CONFIG_OVERRIDE'
 *       for Arduino IDE:
 *         enable define USE_CONFIG_OVERRIDE in my_user_config.h
 ******************************************************************************************************
 * ATTENTION:
 *   - Changes to SECTION1 PARAMETER defines will only override flash settings if you change define CFG_HOLDER.
 *   - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *   - You still need to update my_user_config.h for major define USE_MQTT_TLS.
 *   - All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
\*****************************************************************************************************/



#undef PROJECT             
#undef MODULE              
                           
#undef WIFI_CONFIG_TOOL    
                            
#undef MQTT_HOST           
#undef MQTT_PORT           
#undef MQTT_USER           
#undef MQTT_PASS           
                            
#undef MQTT_TOPIC          
#undef MQTT_GRPTOPIC       
                            
#undef MQTT_CLIENT_ID      
                            
#undef FRIENDLY_NAME       
                            
#undef GUI_SHOW_HOSTNAME   
                            
#undef MDNS_ENABLED        
                            
#undef NTP_SERVER1         
#undef NTP_SERVER2         
#undef NTP_SERVER3         
                            
#undef LATITUDE            
#undef LONGITUDE           






#define CFG_HOLDER             4618              // [Reset 1] Change this value (max 32000) to load SECTION1 configuration parameters to flash
// -- Project -------------------------------------
#define PROJECT                "tasmota"         // PROJECT is used as the default topic delimiter
// If not selected the default will be SONOFF_BASIC
#define MODULE                 WEMOS      // [Module] Select default model from tasmota_template.h

#define WIFI_CONFIG_TOOL       WIFI_RETRY        // [WifiConfig] Default tool if wifi fails to connect (default option: 4 - WIFI_RETRY)

#define MQTT_HOST              "10.0.0.1"                // [MqttHost]
#define MQTT_PORT              1883              // [MqttPort] MQTT port (10123 on CloudMQTT)
#define MQTT_USER              "esp8266"       // [MqttUser] MQTT user
#define MQTT_PASS              "DVES_PASS"       // [MqttPassword] MQTT password

#define MQTT_TOPIC             PROJECT   // [Topic] unique MQTT device topic including (part of) device MAC address
#define MQTT_GRPTOPIC          "sonoffs"        // [GroupTopic] MQTT Group topic

#define MQTT_CLIENT_ID         "DVES_%06X"       // [MqttClient] Also fall back topic using last 6 characters of MAC address or use "DVES_%12X" for complete MAC address

#define FRIENDLY_NAME          PROJECT         // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa

#define GUI_SHOW_HOSTNAME      true             // [SetOption53] Show hostname and IP address in GUI main menu

#define MDNS_ENABLED           true             // [SetOption55] Use mDNS (false = Disable, true = Enable)

#define NTP_SERVER1            "il.pool.ntp.org"       // [NtpServer1] Select first NTP server by name or IP address (129.250.35.250)
#define NTP_SERVER2            "asia.pool.ntp.org"    // [NtpServer2] Select second NTP server by name or IP address (5.39.184.5)
#define NTP_SERVER3            "pool.ntp.org"  // [NtpServer3] Select third NTP server by name or IP address (93.94.224.67)

#define LATITUDE               32.30705         // [Latitude] Your location to be used with sunrise and sunset
#define LONGITUDE              34.86843          // [Longitude] Your location to be used with sunrise and sunset


#define USE_PING                                 // Enable Ping command (+2k code)
#define USE_SPI                                  // Hardware SPI using GPIO12(MISO), GPIO13(MOSI) and GPIO14(CLK) in addition to two user selectable GPIOs(CS and DC)
#define USE_WS2812B_CLOCK



#undef USE_DOMOTICZ                             // Enable Domoticz (+6k code, +0.3k mem)
#undef USE_HOME_ASSISTANT                       // Enable Home Assistant Discovery Support (+4.1k code, +6 bytes mem)
#undef USE_SONOFF_RF                            // Add support for Sonoff Rf Bridge (+3k2 code)
#undef USE_SONOFF_RF                            // Add support for Sonoff Rf Bridge (+3k2 code)
#undef USE_RF_FLASH                           // Add support for flashing the EFM8BB1 chip on the Sonoff RF Bridge. C2CK must be connected to GPIO4, C2D to GPIO5 on the PCB (+2k7 code)
#undef USE_SONOFF_SC                            // Add support for Sonoff Sc (+1k1 code)
#undef USE_TUYA_MCU                             // Add support for Tuya Serial MCU
#undef TUYA_DIMMER_ID                        // Default dimmer Id
#undef USE_TUYA_TIME                          // Add support for Set Time in Tuya MCU
#undef USE_ARMTRONIX_DIMMERS                    // Add support for Armtronix Dimmers (+1k4 code)
#undef USE_PS_16_DZ                             // Add support for PS-16-DZ Dimmer (+2k code)
#undef USE_SONOFF_IFAN                          // Add support for Sonoff iFan02 and iFan03 (+2k code)
#undef USE_BUZZER                               // Add support for a buzzer (+0k6 code)
#undef USE_ARILUX_RF                            // Add support for Arilux RF remote controller (+0k8 code, 252 iram (non 2.3.0))
#undef USE_SHUTTER                              // Add Shutter support for up to 4 shutter with different motortypes (+11k code)
#undef USE_EXS_DIMMER                           // Add support for ES-Store WiFi Dimmer (+1k5 code)
#undef USE_DEVICE_GROUPS                        // Add support for device groups (+5k5 code)
#undef USE_PWM_DIMMER                           // Add support for MJ-SD01/acenx/NTONPOWER PWM dimmers (+2k2 code, DGR=0k4)
#undef USE_WS2812                               // WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram)
#undef USE_WS2812_HARDWARE                      // Hardware type (NEO_HW_WS2812, NEO_HW_WS2812X, NEO_HW_WS2813, NEO_HW_SK6812, NEO_HW_LC8812, NEO_HW_APA106)
#undef USE_WS2812_CTYPE                         // Color type (NEO_RGB, NEO_GRB, NEO_BRG, NEO_RBG, NEO_RGBW, NEO_GRBW)
#undef USE_MY92X1                               // Add support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#undef USE_SM16716                              // Add support for SM16716 RGB LED controller (+0k7 code)
#undef USE_SM2135                               // Add support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#undef USE_SONOFF_L1                            // Add support for Sonoff L1 led control
#undef USE_ELECTRIQ_MOODL                       // Add support for ElectriQ iQ-wifiMOODL RGBW LED controller (+0k3 code)
#undef USE_LIGHT_PALETTE                        // Add support for color palette (+0k7 code)
#undef USE_DGR_LIGHT_SEQUENCE                   // Add support for device group light sequencing (requires USE_DEVICE_GROUPS) (+0k2 code)
#undef USE_DS18x20                              // Add support for DS18x20 sensors with id sort, single scan and read retry (+2k6 code)
#undef USE_APDS9960_GESTURE                   // Enable APDS9960 Gesture feature (+2k code)
#undef USE_APDS9960_PROXIMITY                 // Enable APDS9960 Proximity feature (>50 code)
#undef USE_APDS9960_COLOR                     // Enable APDS9960 Color feature (+0.8k code)
#undef USE_ADE7953                            // [I2cDriver7] Enable ADE7953 Energy monitor as used on Shelly 2.5 (I2C address 0x38) (+1k5)

#undef USE_VEML6075                           // [I2cDriver49] Enable VEML6075 UVA/UVB/UVINDEX Sensor (I2C address 0x10) (+2k1 code)
#undef USE_VEML7700                           // [I2cDriver50] Enable VEML7700 Ambient Light sensor (I2C addresses 0x10) (+4k5 code)
#undef USE_MCP9808                            // [I2cDriver51] Enable MCP9808 temperature sensor (I2C addresses 0x18 - 0x1F) (+0k9 code)
#undef USE_DISPLAY                            // Add I2C Display Support (+2k code)
#undef USE_DISPLAY_MODES1TO5                // Enable display mode 1 to 5 in addition to mode 0
#undef USE_DISPLAY_LCD                      // [DisplayModel 1] [I2cDriver3] Enable Lcd display (I2C addresses 0x27 and 0x3F) (+6k code)
#undef USE_DISPLAY_SSD1306                  // [DisplayModel 2] [I2cDriver4] Enable SSD1306 Oled 128x64 display (I2C addresses 0x3C and 0x3D) (+16k code)
#undef USE_DISPLAY_MATRIX                   // [DisplayModel 3] [I2cDriver5] Enable 8x8 Matrix display (I2C adresseses see below) (+11k code)

#undef USE_DISPLAY_SEVENSEG                 // [DisplayModel 11] [I2cDriver47] Enable sevenseg display (I2C 0x70-0x77) (<+11k code)

#undef USE_DHT                                  // Add support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor (1k6 code)
#undef USE_MAX31855                             // Add support for MAX31855/MAX6675 K-Type thermocouple sensor using softSPI

#undef USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+4k3 code, 0k3 mem, 48 iram)







#endif  // _USER_CONFIG_OVERRIDE_H_
