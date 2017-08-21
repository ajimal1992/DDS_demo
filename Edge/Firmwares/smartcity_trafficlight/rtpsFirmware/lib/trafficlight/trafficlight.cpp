#include "Arduino.h"
#include "trafficlight.h"

Trafficlight::Trafficlight(int red, int yellow, int green){
    red_ = red;
    yellow_ = yellow;
    green_ = green;
    pinMode(red, OUTPUT);
    pinMode(yellow, OUTPUT);
    pinMode(green, OUTPUT);

}

void Trafficlight::stop(){
    clear();
    digitalWrite(red_, HIGH);
}

void Trafficlight::go(){
    clear();
    digitalWrite(green_, HIGH);
}

void Trafficlight::ready(){
     clear();
    digitalWrite(yellow_, HIGH);
}

void Trafficlight::clear(){
    digitalWrite(red_, LOW);
    digitalWrite(yellow_, LOW);
    digitalWrite(green_, LOW);
}

int Trafficlight::getRedPin(){
    return red_;
}

int Trafficlight::getGreenPin(){
    return green_;
}

int Trafficlight::getYellowPin(){
    return yellow_;
}
