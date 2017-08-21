#include "trafficlight.h"

#define DELAY 5 //in seconds
#define DELAYCOUNT (DELAY*2+3)


int trigger_pin = 2;
Trafficlight trafficlight1(4,5,6); //ambulance trafficlight
Trafficlight trafficlight2(9,10,11); 

char timerCount = 0; //Used to extend the duration of timer
bool inTransition = 0; //for debounce

void setup(){
    Serial.begin(115200);
    
    //setup switch interrupt
    pinMode(trigger_pin,INPUT);
    attachInterrupt(digitalPinToInterrupt(trigger_pin),switchISR,RISING);
    
    //setup initial states
    trafficlight1.clear();
    trafficlight1.stop();
    trafficlight2.clear();
    trafficlight2.go();
    
    //set up timer interrupt
    cli();          // disable global interrupts
    TCCR1A = 0;     // clear ctrl register A
    TCCR1B = 0;     // clear ctrl register B
    OCR1A = 31249;  //set capture at every 0.5sec.
    TCCR1B |= (1 << WGM12);  //set capture mode
    TCCR1B |= (1 << CS12);   //256 prescale
    sei();          // enable global interrupts
    
}

void loop(){
    //empty loop.
}

void switchISR(){
    if(timerCount==0 && !inTransition){
     	Serial.println("b1t"); 
     	TIMSK1 |= (1 << OCIE1A); //enable timer interrupt   
     	inTransition = 1; 
    }
}

ISR(TIMER1_COMPA_vect){
    if(timerCount == 0){
    	trafficlight2.clear();
        trafficlight2.ready();
        Serial.println("1a2b");
    }
    else if(timerCount == 2){ //1000 ms
    	trafficlight2.stop();
        Serial.println("1a1b");
    }
    else if(timerCount == 3){ //1500 ms
    	trafficlight1.go();
    	Serial.println("3a1b");
    }
    else if(timerCount == DELAYCOUNT){//6500 ms
    	trafficlight1.clear();
        trafficlight1.ready();
        Serial.println("2a1b");
    }
    else if(timerCount == (DELAYCOUNT+2)){//7500 ms
    	trafficlight1.stop();
        Serial.println("1a1b");
    }
    else if(timerCount == (DELAYCOUNT+3)){//8000 ms
    	trafficlight2.go();
        Serial.println("1a3b");
        timerCount = -1;
        inTransition = 0;
        TIMSK1 &= ~(1 << OCIE1A);
    }
    timerCount ++;
}

