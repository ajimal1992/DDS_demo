#include "trafficlight.h"
#include <string.h>

#define DELAY 5 //in seconds
#define DELAYCOUNT (DELAY*2+3)


int trigger_pin = 2;
int lamp_pins[] = {5, 6, 7};
int boot_pin = 13;
Trafficlight trafficlight1(4,5,6); //ambulance trafficlight
Trafficlight trafficlight2(9,10,11); 


char timerCount = 0; //Used to extend the duration of timer
bool inTransition = 0; //for debounce

void setup(){
    Serial.begin(115200);

    //setup lights
    pinMode(boot_pin,OUTPUT);
    digitalWrite(boot_pin,HIGH);
    
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
    TIMSK1 |= (1 << OCIE1A); //enable timer interrupt   
    sei();          // enable global interrupts
    
}

void loop(){
    //empty loop.
}

void switchISR(){
    if(timerCount==0 && !inTransition){
     	inTransition = 1; 
     	Serial.println("P!:1"); //P!:ID --P! means priority
    }
}

ISR(TIMER1_COMPA_vect){

    if(digitalRead(trafficlight1.getRedPin())==HIGH){
    	Serial.print("1"); //ID:state
    }
    else if(digitalRead(trafficlight1.getYellowPin())==HIGH){
    	Serial.print("2");
    }
    else if(digitalRead(trafficlight1.getGreenPin())==HIGH){
    	Serial.print("3");
    }
    if(digitalRead(trafficlight2.getRedPin())==HIGH){
    	Serial.println("1");
    }
    else if(digitalRead(trafficlight2.getYellowPin())==HIGH){
    	Serial.println("2");
    }
    else if(digitalRead(trafficlight2.getGreenPin())==HIGH){
    	Serial.println("3");
    }

    if(inTransition){
	    if(timerCount == 0){
	    	trafficlight2.clear();
		trafficlight2.ready();
	    }
	    else if(timerCount == 2){ //1000 ms
	    	trafficlight2.stop();
	    }
	    else if(timerCount == 3){ //1500 ms
	    	trafficlight1.go();
	    }
	    else if(timerCount == DELAYCOUNT){//6500 ms
	    	trafficlight1.clear();
		trafficlight1.ready();
	    }
	    else if(timerCount == (DELAYCOUNT+2)){//7500 ms
	    	trafficlight1.stop();
	    }
	    else if(timerCount == (DELAYCOUNT+3)){//8000 ms
	    	trafficlight2.go();
		timerCount = -1;
		inTransition = 0;
	    }
	    timerCount ++;
    }
}

