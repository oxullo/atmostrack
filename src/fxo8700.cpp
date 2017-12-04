#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXOS8700.h>

Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

void fxo8700_report(void)
{
    sensor_t accel, mag;

    accelmag.getSensor(&accel, &mag);

    Serial.println("------------------------------------");
    Serial.println("*** FXO8700 ACCELEROMETER");
    Serial.println();
    Serial.print  ("Sensor:       "); Serial.println(accel.name);
    Serial.print  ("Driver Ver:   "); Serial.println(accel.version);
    Serial.print  ("Unique ID:    0x"); Serial.println(accel.sensor_id, HEX);
    Serial.print  ("Min Delay:    "); Serial.print(accel.min_delay); Serial.println(" s");
    Serial.print  ("Max Value:    "); Serial.print(accel.max_value, 4); Serial.println(" m/s^2");
    Serial.print  ("Min Value:    "); Serial.print(accel.min_value, 4); Serial.println(" m/s^2");
    Serial.print  ("Resolution:   "); Serial.print(accel.resolution, 8); Serial.println(" m/s^2");
    Serial.println("");
    Serial.println("------------------------------------");
    Serial.println("*** FXO8700 MAGNETOMETER");
    Serial.println();
    Serial.print  ("Sensor:       "); Serial.println(mag.name);
    Serial.print  ("Driver Ver:   "); Serial.println(mag.version);
    Serial.print  ("Unique ID:    0x"); Serial.println(mag.sensor_id, HEX);
    Serial.print  ("Min Delay:    "); Serial.print(accel.min_delay); Serial.println(" s");
    Serial.print  ("Max Value:    "); Serial.print(mag.max_value); Serial.println(" uT");
    Serial.print  ("Min Value:    "); Serial.print(mag.min_value); Serial.println(" uT");
    Serial.print  ("Resolution:   "); Serial.print(mag.resolution); Serial.println(" uT");
}

void fxo8700_init(void)
{
    if(!accelmag.begin(ACCEL_RANGE_4G))
    {
        Serial.println("FXOS8700 init error");
        while(1);
    }
}

void fxo8700_printdata(void)
{
    sensors_event_t aevent, mevent;

    /* Get a new sensor event */
    accelmag.getEvent(&aevent, &mevent);

    /* Display the accel results (acceleration is measured in m/s^2) */
    Serial.print("FXO8700 ACCEL ");
    Serial.print("X="); Serial.print(aevent.acceleration.x, 4); Serial.print("m/s^2 ");
    Serial.print("Y="); Serial.print(aevent.acceleration.y, 4); Serial.print("m/s^2 ");
    Serial.print("Z="); Serial.print(aevent.acceleration.z, 4); Serial.print("m/s^2 ");
    Serial.println();

    /* Display the mag results (mag data is in uTesla) */
    Serial.print("FXO8700 MAGN ");
    Serial.print("X="); Serial.print(mevent.magnetic.x, 1); Serial.print("uT ");
    Serial.print("Y="); Serial.print(mevent.magnetic.y, 1); Serial.print("uT ");
    Serial.print("Z="); Serial.print(mevent.magnetic.z, 1); Serial.print("uT ");
    Serial.println();
}

void fxo8700_get_acceleration(double *x, double *y, double *z)
{
    sensors_event_t aevent, mevent;
    accelmag.getEvent(&aevent, &mevent);

    *x = aevent.acceleration.x;
    *y = aevent.acceleration.y;
    *z = aevent.acceleration.z;
}

void fxo8700_get_magnetic(double *x, double *y, double *z)
{
    sensors_event_t aevent, mevent;
    accelmag.getEvent(&aevent, &mevent);

    *x = mevent.magnetic.x;
    *y = mevent.magnetic.y;
    *z = mevent.magnetic.z;
}
