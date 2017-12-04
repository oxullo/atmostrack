#include <Arduino.h>

#include "st00008.h"

#define OUT1_PIN        10
#define OUT2_PIN        19

void st00008_init()
{
    pinMode(OUT1_PIN, INPUT_PULLUP);
    pinMode(OUT2_PIN, INPUT_PULLUP);
}

Orientation st00008_get_orientation()
{
    uint8_t oint = digitalRead(OUT1_PIN) << 1 | digitalRead(OUT2_PIN);

    return (Orientation)oint;
}
