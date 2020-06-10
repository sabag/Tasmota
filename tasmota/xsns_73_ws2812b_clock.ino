// Conditional compilation of driver
#ifdef USE_WS2812B_CLOCK

#define WS2812B_CLOCK_VERSION 2

// Define driver ID
#define XSNS_73  73

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>


#define LED_COUNT 36
#define LED_PIN 4 //D2
#define DOT1 17
#define DOT2 18

#define TEST_LEDS_PERIOD_MS 3000   //3 seconds

const char HTTP_SNS_LEDCLOCK[] PROGMEM = "Color %d<br>Dots %d<br>Brightness %d<br>Saturation %d<br>12H %d";


//
// LED related variables
//

CRGB leds[LED_COUNT];


byte segGroups[14] = {
  // right (seen from front) digit
  2,   // top, a
  3,   // top right, b
  5,   // bottom right, c
  6,   // bottom, d
  7,   // bottom left, e
  1,   // top left, f
  0,   // center, g

  14,  // top, a
  15,  // top right, b
  9,   // bottom right, c
  10,   // bottom, d
  11,  // bottom left, e
  13,  // top left, f
  16   // center, g

};

/* ledclock1 still uses 32 leds
    and require different led configuration
  #define LED_COUNT 32
  #define DOT1 15
  #define DOT2 16
  byte segGroups[14] = {
  2,3,4,5,6,1,0,
  12,13,8,9,10,11,14
  };
*/

byte digits[10][7] = {
  { 1, 1, 1, 1, 1, 1, 0 },  // 0
  { 0, 1, 1, 0, 0, 0, 0 },  // 1
  { 1, 1, 0, 1, 1, 0, 1 },  // 2
  { 1, 1, 1, 1, 0, 0, 1 },  // 3
  { 0, 1, 1, 0, 0, 1, 1 },  // 4
  { 1, 0, 1, 1, 0, 1, 1 },  // 5
  { 1, 0, 1, 1, 1, 1, 1 },  // 6
  { 1, 1, 1, 0, 0, 0, 0 },  // 7
  { 1, 1, 1, 1, 1, 1, 1 },  // 8
  { 1, 1, 1, 1, 0, 1, 1 },  // 9
};


#define LEDCLOCK_BRIGHTNESS 250
#define LEDCLOCK_SATURATION 255
#define LEDCLOCK_DIGITCOLOR 0
#define LEDCLOCK_DOTSCOLOR 0
#define LEDCLOCK_USE12H 0

/*
struct {
  uint8_t brightness = 250;
  uint8_t saturation = 255;
  uint8_t digitColor = 0;
  uint8_t dotsColor = 0;
  boolean use12H = true;
} data;
*/


/**
 * The callback function Xsns<driver_ID>() interfaces Tasmota with the sensor driver.
 *
 * It provides the Tasmota callback IDs.
 *
 * @param   byte    callback_id  Tasmota function ID.
 * @return  boolean              Return value.
 * @pre     None.
 * @post    None.
 *
 */
bool Xsns73(byte callback_id) {

  // Set return value to `false`
  boolean result = false;

  // Check if I2C interface mode
// if(i2c_flg) {

  // Check which callback ID is called by Tasmota
  switch (callback_id) {
    case FUNC_INIT:
      AddLog_P2(LOG_LEVEL_INFO, PSTR("WS2812b LED Clock - Driver Init version %d"), WS2812B_CLOCK_VERSION);
      ledClockInit();
      result = true;
      break;

    case FUNC_EVERY_50_MSECOND:
      break;

    case FUNC_EVERY_SECOND:
        // AddLog_P2(LOG_LEVEL_INFO, PSTR("WS2812B Time %02d:%02d:%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
        showLedTime();
        result = true;
      break;

    case FUNC_JSON_APPEND:
      result = appendData();
      break;
    case FUNC_SAVE_BEFORE_RESTART:
      break;

#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        result = webuiShow();
        break;
#endif  // USE_WEBSERVER

    case FUNC_COMMAND_SENSOR:
        if (XSNS_73 == XdrvMailbox.index) {
          result = WS2812B_Command();
        }
        break;

    case FUNC_COMMAND:
      break;
  }
// } // if(i2c_flg)

  // Return boolean result
  return result;
}

void ledClockInit(void){

    if (!Settings.ledclock_brightness) {
        Settings.ledclock_brightness = LEDCLOCK_BRIGHTNESS;
    }
    if (!Settings.ledclock_saturation) {
        Settings.ledclock_saturation = LEDCLOCK_SATURATION;
    }
    if (!Settings.ledclock_digitColor) {
        Settings.ledclock_digitColor = LEDCLOCK_DIGITCOLOR;
    }
    if (!Settings.ledclock_dotsColor) {
        Settings.ledclock_dotsColor = LEDCLOCK_DOTSCOLOR;
    }
    if (!Settings.ledclock_use12h) {
        Settings.ledclock_use12h = LEDCLOCK_USE12H;
    }

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
    FastLED.setDither(0);
    FastLED.setBrightness(Settings.ledclock_brightness);

}

bool webuiShow(void){
    WSContentSend_PD(HTTP_SNS_LEDCLOCK, 
        Settings.ledclock_digitColor,
        Settings.ledclock_dotsColor,
        Settings.ledclock_brightness,
        Settings.ledclock_saturation,
        Settings.ledclock_use12h
    );
    return true;
}

bool appendData(){
    snprintf_P(mqtt_data, sizeof(mqtt_data), 
        PSTR("{\"WS2812B\":{\"version\":%d,\"digitColor\":%d,\"dotsColor\":%d,\"saturation\":%d,\"brightness\":%d}"),
        WS2812B_CLOCK_VERSION, 
        Settings.ledclock_digitColor, 
        Settings.ledclock_dotsColor, 
        Settings.ledclock_saturation, 
        Settings.ledclock_brightness);
    return true;
}
bool WS2812B_Command(){
    AddLog_P2(LOG_LEVEL_INFO, PSTR("WS2812b command: %s"), XdrvMailbox.data);

    uint8_t args_count = 0;

    if (XdrvMailbox.data_len > 0) {
        args_count = 1;
    } else {
        return false;
    }

    for (uint32_t idx = 0; idx < XdrvMailbox.data_len; idx++) {
        if (' ' == XdrvMailbox.data[idx]) {
        XdrvMailbox.data[idx] = ',';
        }
        if (',' == XdrvMailbox.data[idx]) {
        args_count++;
        }
    }

    if(args_count < 2){
        return false;
    }

    char sub_string[XdrvMailbox.data_len +1];

    char* command = subStr(sub_string, XdrvMailbox.data, ",", 1); 
    char* valueString = subStr(sub_string, XdrvMailbox.data, ",", 2);

    uint8_t value = atoi(valueString);

    if (!strcmp(command,"color")) {
        Settings.ledclock_digitColor = value; 
        Response_P(PSTR("{\"digitColor\":\"%d\"}"),Settings.ledclock_digitColor);   

    } else if (!strcmp(command,"dots")) {
        Settings.ledclock_dotsColor = value;
        Response_P(PSTR("{\"dotsColor\":\"%d\"}"),Settings.ledclock_dotsColor);

    } else if (!strcmp(command,"saturation")) {
        Settings.ledclock_saturation = value;
        Response_P(PSTR("{\"saturation\":\"%d\"}"),Settings.ledclock_saturation);

    } else if (!strcmp(command,"brightness")) {
        Settings.ledclock_brightness = value;
        Response_P(PSTR("{\"brightness\":\"%d\"}"),Settings.ledclock_brightness);

    } else if (!strcmp(command,"use12h")) {
        Settings.ledclock_use12h = value;
        Response_P(PSTR("{\"use12h\":\"%d\"}"),Settings.ledclock_use12h);

    }
    return true;
}

void showLedTime() {

  FastLED.clear();
  FastLED.setBrightness(Settings.ledclock_brightness);

  // minutes 
  int minute = RtcTime.minute;
  showDigit(0, minute % 10, Settings.ledclock_digitColor);
  showDigit(1, minute / 10, Settings.ledclock_digitColor);

  // hours
  int hours = RtcTime.hour;
  if (Settings.ledclock_use12h == 1) {
    hours = hours > 12 ? hours - 12 : hours;
  }
  showDigit(2, hours % 10, Settings.ledclock_digitColor);
  if (hours > 9) {
    showDigit(3, hours / 10, Settings.ledclock_digitColor);
  }

  // dots as seconds
  int seconds = RtcTime.second;
  if (seconds % 2 == 0) {
    leds[DOT1].setHSV(Settings.ledclock_dotsColor, Settings.ledclock_saturation, Settings.ledclock_brightness);
    leds[DOT2].setHSV(Settings.ledclock_dotsColor, Settings.ledclock_saturation, Settings.ledclock_brightness);
  }
  FastLED.show();
}

void showSegment(byte digitPosition, byte segment, byte color) {
  byte index = digitPosition % 2 == 0 ? 0 : 7;
  byte stripIndex = segGroups[ index + segment ];
  if (digitPosition >= 2) {
    stripIndex += (DOT2 + 1); // this is the first led of the second module
  }
  leds[stripIndex].setHSV(color, Settings.ledclock_saturation, Settings.ledclock_brightness);
}

void showDigit(byte digitPosition, byte digit, byte color) {
  for (byte i = 0; i < 7; i++) {
    if (digits[digit][i] != 0) {
      showSegment(digitPosition, i, color);
    }
  }
}

void testLeds() {
  byte ledCount = DOT1 * 2 + 2;
  for (byte i = 0; i < ledCount; i++) {
    leds[i].setHSV(0, 250, 255);
  }
  FastLED.show();
  delay(TEST_LEDS_PERIOD_MS);
}


#endif // USE_WS2812B_CLOCK