#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H


class Trafficlight{
    public:
        Trafficlight(int red, int yellow, int green);
        void stop();
        void go();
        void ready();
        void clear();
        int getRedPin();
        int getYellowPin();
        int getGreenPin();

    private:
        int red_;
        int yellow_;
        int green_;
};

#endif
