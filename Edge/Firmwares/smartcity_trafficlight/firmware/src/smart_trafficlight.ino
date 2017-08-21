#include "trafficlight.h"

#define TRAFFIC_DURATION 5000

int trigger_pin = 46;
Trafficlight trafficlight1(4,5,6);
Trafficlight trafficlight2(9,10,11);

void setup(){
    Serial.begin(115200);
}

void loop(){
    static bool road = 0;
    static unsigned long previousTime = 5000;
    static int traffic_delay = 5000;
    static bool is_triggered;

    is_triggered = digitalRead(trigger_pin);

    if(is_triggered){
        if(road == 0)
        {
           traffic_delay = 20000; 
        }
        else if(road == 1)
        {
            traffic_delay = 500;
        }
	Serial.println("b1t");
    }        
    

    //TODO: create an interrupt to change traffic_delay and 
    //change priority to ambulance lane

    if((millis() - previousTime) > traffic_delay ){
        if(!is_triggered)
        {
           changePriority(road);
            //reset to normal traffic
            traffic_delay = TRAFFIC_DURATION;
            road = !road;
        }
        else if(is_triggered)
        {
            traffic_delay = 10000;
        }
        previousTime = millis();
    }
}


void changePriority(bool road)
{
    //ambulance lane
    if(road){
        trafficlight2.clear();
        trafficlight2.ready();
        Serial.println("1a,2b");
        delay(1000);
        trafficlight2.stop();
        Serial.println("1a,1b");
        delay(500);
        trafficlight1.go();
        Serial.println("3a,1b");
    }

    else{
        trafficlight1.clear();
        trafficlight1.ready();
        Serial.println("2a,1b");
        delay(1000);
        trafficlight1.stop();
        Serial.println("1a,1b");
        delay(500);
        trafficlight2.go();
        Serial.println("1a,3b");
    }
}
