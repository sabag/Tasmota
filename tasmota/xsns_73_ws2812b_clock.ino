// Conditional compilation of driver
#ifdef USE_WS2812B_CLOCK

#define WS2812B_CLOCK_VERSION 6

// #define LEDCLOCK_32
// #define LEDCLOCK_36
// #define LEDCLOCK_67  // ledclock 4 - basic version - 60 led/m
#define LEDCLOCK_38     // ledclock 5 - tiny version - 60 led/m

// Define driver ID
#define XSNS_73  73

#include <NeoPixelBus.h>

#ifdef LEDCLOCK_32
  /* ledclock1 still uses 32 leds and require different led configuration */    
  #define LED_COUNT 32
  #define DOT1 15
  #define DOT_COUNT 2
  #define SEG_COUNT 1
#endif

#ifdef LEDCLOCK_36
  // ledclock2 and ledclock3 which uses 36 leds
  #define LED_COUNT 36
  #define DOT1 17
  #define DOT_COUNT 2
  #define SEG_COUNT 1
#endif

#ifdef LEDCLOCK_38
  #define LED_COUNT 38
  #define DOT1 18
  #define DOT_COUNT 2
  #define SEG_COUNT 1
#endif

#ifdef LEDCLOCK_67
  // ledclock4 which uses the 60 leds/m (3 leds per segment)
  #define LED_COUNT 67
  #define DOT1 32
  #define DOT_COUNT 3
  #define SEG_COUNT 2
#endif



#define TEST_LEDS_PERIOD_MS 1000   //1 seconds

const char HTTP_SNS_LEDCLOCK[] PROGMEM = "Color %d<br>Dots %d<br>Brightness %d<br>Saturation %d<br>12H %d";


//
// LED related variables
//

#ifdef LEDCLOCK_67
byte segGroups[14][2] = {         
  {  4,  5 },                     // top, a
  {  6,  7 },                     // top right, b
  {  9, 10 },                     // bottom right, c
  { 11, 12 },                     // bottom, d
  { 13, 14 },                     // bottom left, e
  {  2,  3 },                     // top left, f
  {  0,  1 },                     // center, g
  // left (seen from front) digit
  { 26, 27 },                     // top, a
  { 28, 29 },                     // top right, b
  { 17, 18 },                     // bottom right, c
  { 19, 20 },                     // bottom, d
  { 21, 22 },                     // bottom left, e
  { 24, 25 },                     // top left, f
  { 30, 31 }                      // center, g
};
#endif

#ifdef LEDCLOCK_38
byte segGroups[14] = {
  // right (seen from front) digit
  2,   // top, a
  3,   // top right, b
  5,   // bottom right, c
  6,   // bottom, d
  7,   // bottom left, e
  1,   // top left, f
  0,   // center, g

  15,  // top, a
  16,  // top right, b
  10,   // bottom right, c
  11,   // bottom, d
  12,  // bottom left, e
  14,  // top left, f
  17   // center, g
};
#endif

#ifdef LEDCLOCK_36
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
#endif

#ifdef LEDCLOCK_32
  byte segGroups[14] = {
  2,3,4,5,6,1,0,
  12,13,8,9,10,11,14
  };
#endif


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

RgbColor black(0);

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(LED_COUNT, 2);


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

    strip.Begin();

    for (byte i = 0; i < LED_COUNT; i++) {
        strip.SetPixelColor(i, HslColor(0, 1.0f, 0.5f));
    }

    strip.Show();

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

void showSegment(byte digitPosition, byte segment, byte color) {
    byte index = digitPosition % 2 == 0 ? 0 : 7;
    byte moduleDelta = (digitPosition < 2) ? 0 : (DOT1 + DOT_COUNT);
    float fh = color / 255.0;
    float fs = Settings.ledclock_saturation / 255.0;
    float fl = Settings.ledclock_brightness * 0.5 / 255.0;

    #ifdef LEDCLOCK_67

        for(byte i = 0; i < SEG_COUNT; i++) {     
            strip.SetPixelColor( segGroups[index+segment][i] + moduleDelta, HslColor(fh, fs, fl));
        }

    #else

        strip.SetPixelColor( segGroups[index + segment] + moduleDelta, HslColor(fh, fs, fl) );

    #endif
}

void showDigit(byte digitPosition, byte digit, byte color) {
  for (byte i = 0; i < 7; i++) {
    if (digits[digit][i] != 0) {
      showSegment(digitPosition, i, color);
    }
  }
}

void showLedTime() {

  strip.ClearTo(black);

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
    HslColor hsl( Settings.ledclock_dotsColor / 255.0, Settings.ledclock_saturation / 255.0, Settings.ledclock_brightness * 0.5 / 255.0 );
    for(byte i = 0; i < DOT_COUNT; i++) {
        strip.SetPixelColor(DOT1 + i, hsl);
    }
  }
  
  strip.Show();

}


void testLeds() {
  byte ledCount = LED_COUNT;
  for (byte i = 0; i < ledCount; i++) {
    strip.SetPixelColor(i, HslColor(0, 1.0f, 0.5f));
  }
  strip.Show();
  delay(TEST_LEDS_PERIOD_MS);
}


#endif // USE_WS2812B_CLOCK