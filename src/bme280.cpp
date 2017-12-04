#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)


Adafruit_BME280 bme; // I2C

void bme280_init(void)
{
    if (!bme.begin()) {
        Serial.println("BME280 init error");
        while (1);
    }
}

void bme280_printdata(void)
{
    Serial.print("BME280 ");

    Serial.print("TEMP=");
    Serial.print(bme.readTemperature());
    Serial.print("C PRESSURE=");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.print("hPa ALT=");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.print("m HUM=");
    Serial.print(bme.readHumidity());
    Serial.println("%");
}

double bme280_get_temperature(void)
{
    return bme.readTemperature();
}

double bme280_get_pressure(void)
{
    return bme.readPressure();
}

double bme280_get_humidity(void)
{
    return bme.readHumidity();
}
