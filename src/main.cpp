#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AdafruitIO_WiFi.h>
#include <WiFi101.h>
#include <Button.h>

#include "bme280.h"
#include "fxo8700.h"
#include "tsl2591.h"
#include "veml6070.h"
#include "mcp9808.h"
#include "st00008.h"

#include "secrets.h"

#define BTN_A_PIN       9
#define BTN_B_PIN       6
#define BTN_C_PIN       5
#define DEBOUNCE_MS     20

#define MAX_PAGES       3

#define AIO_REPORT_PERIOD       60000

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
uint32_t ts_next_report = 0;

Adafruit_SSD1306 oled;

Button btnNext(BTN_A_PIN, true, true, DEBOUNCE_MS);

uint8_t current_page = 0;

// AdafruitIO_Group *iogroup = io.group("atmos02");
AdafruitIO_Feed *feed_tbme = io.feed("temperature-bme");
AdafruitIO_Feed *feed_tmcp = io.feed("temperature-mcp");
AdafruitIO_Feed *feed_humidity = io.feed("humidity");
AdafruitIO_Feed *feed_pressure = io.feed("atmos-pressure");
AdafruitIO_Feed *feed_visible = io.feed("visible-radiation");
AdafruitIO_Feed *feed_ir = io.feed("ir-radiation");
AdafruitIO_Feed *feed_uv = io.feed("uv-radiation");
AdafruitIO_Feed *feed_lux = io.feed("lux");

void printWiFiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void update_buttons()
{
    btnNext.read();

    if (btnNext.wasPressed()) {
        current_page = (current_page + 1) % MAX_PAGES;
    }
}

void debug_values()
{
    Serial.println("-----------------------------------");
    Serial.println("SENSORS DATA DUMP");
    Serial.println();

    bme280_printdata();
    fxo8700_printdata();
    tsl2591_printdata();
    veml6070_printdata();
    mcp9808_printdata();

    Serial.println();
}

void display_wifi()
{
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.println("** Network status");
    IPAddress ip = WiFi.localIP();
    oled.println("SSID=" + String(WiFi.SSID()) + " RSSI=" + String(WiFi.RSSI()) + "dBm");
    oled.println("IP=" + String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));
    oled.println("AIO Status=" + String(io.status()) + " REP=" + String((ts_next_report - millis()) / 1000) + "s");
    oled.display();
}

void display_atmos()
{
    double temperature = bme280_get_temperature();
    double pressure = bme280_get_pressure();
    double humidity = bme280_get_humidity();
    double temperature2 = mcp9808_get_temperature();

    // double light_intensity = tsl2591_get_full();
    // double ir_intensity = tsl2591_get_ir();
    double lux = tsl2591_get_lux();

    double uv_intensity = veml6070_get_uv();

    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.println("** Atmos data");
    oled.println(String(temperature) + "C " + String(humidity) + "% " + String(pressure / 100.0));
    oled.println(String((int)lux) + " lux UVI=" + String((int)uv_intensity));
    oled.println("T2=" + String(temperature2) + "C");
    oled.display();
}

void display_fxo()
{
    double ax, ay, az;
    fxo8700_get_acceleration(&ax, &ay, &az);

    double mx, my, mz;
    fxo8700_get_magnetic(&mx, &my, &mz);

    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.println("** Motion data");
    oled.println("A(" + String(ax) + "," + String(ay) + "," + String(az) + ")");
    oled.println("M(" + String(mx) + "," + String(my) + "," + String(mz) + ")");
    oled.println("ORI=" + String(st00008_get_orientation()));
    oled.display();
}

void check_aio_report()
{
    io.run();

    if (millis() > ts_next_report) {
        oled.clearDisplay();
        oled.setCursor(0, 0);

        oled.print("Sampling data..");
        oled.display();

        float temperature = bme280_get_temperature();
        float pressure = bme280_get_pressure();
        float humidity = bme280_get_humidity();
        float temperature2 = mcp9808_get_temperature();

        float light_intensity = tsl2591_get_full();
        float ir_intensity = tsl2591_get_ir();

        float uv_intensity = veml6070_get_uv();

        float lux = tsl2591_get_lux();

        oled.println("done");
        oled.println(String(io.statusText()));
        oled.display();

        feed_tbme->save(temperature);
        feed_tmcp->save(temperature2);
        feed_pressure->save(pressure);
        feed_humidity->save(humidity);
        feed_visible->save(light_intensity);
        feed_ir->save(ir_intensity);
        feed_uv->save(uv_intensity);
        feed_lux->save(lux);

        oled.println("Data sent");
        oled.display();
        delay(500);

        // iogroup->set("temp-bme", temperature);
        // iogroup->set("humidity", humidity);
        // iogroup->set("pressure", pressure);
        // iogroup->set("temp-mcp", temperature2);
        // iogroup->set("visible-radiation", light_intensity);
        // iogroup->set("ir-radiation", ir_intensity);
        // iogroup->set("uv-level", uv_intensity);
        // bool succeeded = iogroup->save();

        ts_next_report = millis() + AIO_REPORT_PERIOD;
    }
}

void setup()
{
    Serial.begin(9600);
    // while (!Serial);

    pinMode(BTN_A_PIN, INPUT_PULLUP);
    pinMode(BTN_B_PIN, INPUT_PULLUP);
    pinMode(BTN_C_PIN, INPUT_PULLUP);

    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    oled.setTextSize(1);
    oled.setTextColor(WHITE);

    oled.clearDisplay();
    oled.setCursor(0, 0);

    oled.println("Initializing sensors");
    oled.display();

    bme280_init();
    fxo8700_init();
    tsl2591_init();
    veml6070_init();
    mcp9808_init();
    st00008_init();

    fxo8700_report();
    tsl2591_report();

    oled.println("Initializing wifi");
    oled.display();

    WiFi.setPins(8,7,4,2);
    io.connect();
    // wait for a connection
    while(io.status() < AIO_CONNECTED) {
      oled.print(".");
      oled.display();
      delay(500);
    }

    printWiFiStatus();
}

void loop()
{
//    debug_values();
    update_buttons();

    switch (current_page) {
        case 0:
            display_wifi();
            break;

        case 1:
            display_atmos();
            break;

        case 2:
            display_fxo();
            break;
    }
    check_aio_report();
}
