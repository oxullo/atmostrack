#include <Wire.h>
#include <Adafruit_MCP9808.h>

Adafruit_MCP9808 mcp9808 = Adafruit_MCP9808();

void mcp9808_init()
{
    if (!mcp9808.begin()) {
        Serial.println("MCP9808 init error");
        while (1);
    }
}

void mcp9808_printdata()
{
    Serial.print("TEMP=");
    Serial.println(mcp9808.readTempC());
}

double mcp9808_get_temperature()
{
    return mcp9808.readTempC();
}
