#include <Wire.h>
#include "Adafruit_VEML6070.h"

Adafruit_VEML6070 uv = Adafruit_VEML6070();

void veml6070_init(void)
{
    uv.begin(VEML6070_1_T);  // pass in the integration time constant
}


void veml6070_printdata(void)
{
    Serial.print("VEML6070 UVL=");
    Serial.println(uv.readUV());
}

double veml6070_get_uv(void)
{
    return uv.readUV();
}
