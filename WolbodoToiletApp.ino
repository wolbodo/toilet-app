// STOP MODEM MANAGER!: sudo systemctl stop ModemManager.service
// STOP 4G TTY: $ echo '1-4'|sudo tee /sys/bus/usb/drivers/usb/unbind

#define ON LOW
#define OPEN LOW
#define OFF HIGH
#define CLOSE HIGH

//overview board:
//https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/hardware-overview-pro-micro
#define TOILET_LIGHT 21
#define ENTRANCE_LIGHT 20
#define WATER_VALVE 19
#define NOT_USED_RELAY 18

//We use HC-SR501, a sub dollar module: http://www.ebay.com/sch/i.html?LH_BIN=1&_nkw=5pcs+HC-SR501&_sop=15
//for pins & sensitivity & delay (note that jumper is for repeated use instead of alarm single use)
#define TOILET_MOTION 7
#define ENTRANCE_MOTION 8

//settings: (time is in tenth of seconds, so 1200 = 120s = 2min)
#define TOILET_LIGHT_TIMEOUT 2400
#define ENTRANCE_LIGHT_TIMEOUT 1200
#define WATER_VALVE_CYCLE_TIMEOUT 900
#define WATER_VALVE_OPEN_PERIOD 50
#define WATER_VALVE_CLOSED_PERIOD 100
#define WATER_VALVE_CYCLE_COUNT 2

//do not modify this logic:
#define WATER_VALVE_OPEN_CLOSED_PERIOD (WATER_VALVE_OPEN_PERIOD + WATER_VALVE_CLOSED_PERIOD)
#define WATER_VALVE_TIMEOUT (WATER_VALVE_OPEN_CLOSED_PERIOD * WATER_VALVE_CYCLE_COUNT)
#define WATER_VALVE_CYCLE_DELAY (WATER_VALVE_CYCLE_TIMEOUT - WATER_VALVE_TIMEOUT - WATER_VALVE_CLOSED_PERIOD)

int toiletLightTimer = 0, entranceLightTimer = 0, waterValveTimer = 0;
boolean toiletMotion, entranceMotion;

void setup()
{
    pinMode(TOILET_LIGHT, OUTPUT);
    pinMode(ENTRANCE_LIGHT, OUTPUT);
    pinMode(WATER_VALVE, OUTPUT);
    pinMode(NOT_USED_RELAY, OUTPUT);
  
    pinMode(TOILET_MOTION, INPUT);
    pinMode(ENTRANCE_MOTION, INPUT);

    digitalWrite(TOILET_LIGHT, OFF);
    digitalWrite(ENTRANCE_LIGHT, OFF);
    digitalWrite(WATER_VALVE, OFF);
    digitalWrite(NOT_USED_RELAY, OFF);
}

void loop()
{

    toiletMotion = digitalRead(TOILET_MOTION);
    entranceMotion = digitalRead(ENTRANCE_MOTION);

    if (toiletMotion)
    {
        toiletLightTimer = TOILET_LIGHT_TIMEOUT;
        digitalWrite(TOILET_LIGHT, ON);
    }
    if (toiletLightTimer > 0 && --toiletLightTimer == 0)
    {
        digitalWrite(TOILET_LIGHT, OFF);
    }
 
    if (entranceMotion)
    {
        entranceLightTimer = ENTRANCE_LIGHT_TIMEOUT;
        digitalWrite(ENTRANCE_LIGHT, ON);
    }
    if (entranceLightTimer > 0 && --entranceLightTimer == 0)
    {
        digitalWrite(ENTRANCE_LIGHT, OFF);
    }
  
    if (waterValveTimer > 0 && --waterValveTimer <= WATER_VALVE_CYCLE_DELAY)
    {
        switch(waterValveTimer % WATER_VALVE_OPEN_CLOSED_PERIOD)
        {
           case WATER_VALVE_OPEN_PERIOD:
               digitalWrite(WATER_VALVE, OPEN);
               break;
           case 0:
               digitalWrite(WATER_VALVE, CLOSE);
               break;
           default: //no change
               break;
        }
    }
    if (entranceMotion && waterValveTimer == 0)
    {
        waterValveTimer = WATER_VALVE_CYCLE_TIMEOUT;
    }
  
    delay(100);
}
