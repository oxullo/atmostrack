/* TSL2591 Digital Light Sensor */
/* Dynamic Range: 600M:1 */
/* Maximum Lux: 88K */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"


Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)

void tsl2591_init(void)
{
    if (!tsl.begin()) {
        Serial.println("TSL2591 init error");
        while (1);
    }

    // You can change the gain on the fly, to adapt to brighter/dimmer light situations
    // tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
    tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
    // tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain

    // Changing the integration time gives you a longer time over which to sense light
    // longer timelines are slower, but are good in very low light situtations!
    tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)
}

void tsl2591_report(void)
{
    sensor_t sensor;

    tsl.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("*** TSL2591 HDR LIGHT SENSOR");
    Serial.println();
    Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
    Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
    Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
    Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
    Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
    Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" lux"));
    Serial.print  (F("Gain:         "));
    tsl2591Gain_t gain = tsl.getGain();
    switch(gain)
    {
      case TSL2591_GAIN_LOW:
        Serial.println(F("1x (Low)"));
        break;
      case TSL2591_GAIN_MED:
        Serial.println(F("25x (Medium)"));
        break;
      case TSL2591_GAIN_HIGH:
        Serial.println(F("428x (High)"));
        break;
      case TSL2591_GAIN_MAX:
        Serial.println(F("9876x (Max)"));
        break;
    }
    Serial.print  (F("Timing:       "));
    Serial.print((tsl.getTiming() + 1) * 100, DEC);
    Serial.println(F(" ms"));
}

void tsl2591_printdata(void)
{
    // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
    // That way you can do whatever math and comparisons you want!
    uint32_t lum = tsl.getFullLuminosity();
    uint16_t ir, full;

    ir = lum >> 16;
    full = lum & 0xFFFF;

    Serial.print(F("TSL2591 IR="));
    Serial.print(ir);
    Serial.print(F(" VIS="));
    Serial.print(full - ir);
    Serial.print(F(" FULL="));
    Serial.print(full);
    Serial.print(F(" LUX="));
    Serial.println(tsl.calculateLux(full, ir));
}

double tsl2591_get_full(void)
{
    uint32_t lum = tsl.getFullLuminosity();

    return lum & 0xFFFF;
}

double tsl2591_get_ir(void)
{
    uint32_t lum = tsl.getFullLuminosity();

    return lum >> 16;
}

double tsl2591_get_lux(void)
{
    uint32_t lum = tsl.getFullLuminosity();
    uint16_t ir, full;

    ir = lum >> 16;
    full = lum & 0xFFFF;

    return tsl.calculateLux(full, ir);
}
