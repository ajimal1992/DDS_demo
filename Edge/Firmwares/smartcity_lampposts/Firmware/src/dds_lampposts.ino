#include <string.h>

int lamp_pins[] = {5, 6, 7};
int boot_pin = 13;

//handles for serial comms
char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

void translate(char* data){
    char cmd[10];
    char *val;
    strcpy(cmd,data);
    strtok_r(cmd,":",&val);
    
    if(strcmp(cmd,"c1")==0){
        if(strcmp(val,"0")==0)
            digitalWrite(lamp_pins[0],LOW);
        else
            digitalWrite(lamp_pins[0],HIGH);
    }
    else if(strcmp(cmd,"c2")==0){
        if(strcmp(val,"0")==0)
            digitalWrite(lamp_pins[1],LOW);
        else
            digitalWrite(lamp_pins[1],HIGH);
    }
    else if(strcmp(cmd,"c3")==0){
        if(strcmp(val,"0")==0)
            digitalWrite(lamp_pins[2],LOW);
        else
            digitalWrite(lamp_pins[2],HIGH);
    }

}

void setup(){
    Serial.begin(115200);
    
    //setup lights
    char i;
    for(i=0; i<3; i++){
        pinMode(lamp_pins[i],OUTPUT);
        digitalWrite(lamp_pins[i],LOW);
    }

    //set up timer interrupt
    cli();          // disable global interrupts
    TCCR1A = 0;     // clear ctrl register A
    TCCR1B = 0;     // clear ctrl register B
    OCR1A = 31249;  //set capture at every 0.5sec.
    TCCR1B |= (1 << WGM12);  //set capture mode
    TCCR1B |= (1 << CS12);   //256 prescale
    sei();          // enable global interrupts
    TIMSK1 |= (1 << OCIE1A); //start timer
}

void loop(){
    while (Serial.available() > 0){
    	inChar = Serial.read();
    	if(inChar == '$' || index >= 19){
    	    inData[index] = '\0';
    	    index = -1;
    	    //Serial.write(inData);
            translate(inData);
    	}
    	else{
    	    inData[index] = inChar;
    	}
    	index++;   	
    }
}

ISR(TIMER1_COMPA_vect){
    //print the lamppost state
    char i;
    for(i=0; i<3; i++){
        Serial.print(digitalRead(lamp_pins[i]));
    }
    Serial.println();
}


