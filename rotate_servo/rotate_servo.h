#pragma once
#include "mbed.h"

//A class for an object which can rotate the servo to a specified angle, inspired by prog-lab7. Note that it must be run in the main thread in its current state, otherwise the motor seems to exhibit jitter frequently
class rotate_servo {
    // Private member variables for the rotate_servo class can only be called by methods of the class
    //A DigitalInOut object that will be used for sending pulses to the sig pin of the SG90 servo
    DigitalInOut sig_;

    //A Ticker object to set the frequency of the servo
    Ticker ticker_sig_;
    
    //This timeout object will be attached to callback functions which set the input to the servo sig pin high, or low, by setting the sig_ object to "true", or "false", at time intervals defined by the declaration of the rotate_servo class 
    Timeout timeout_sig_;
    Timeout timeout_hold_servo_;

    //These two chrono objects are used for some functions and methods that can accept time durations as parameters in their declaration
    chrono::microseconds duty_cycle_;
    chrono::milliseconds hold_time_;

    //These two variables are used to increment values that will be converted into the chrono objects in each instance of a loop; chrono objects can't be used in the parameters of a for loop
    unsigned long long start_duty_;
    unsigned long long end_duty_;

    //For descriptions of methods, see rotate_servo.cpp
    void pulse_on();

    void pulse_off();

//public methods and constructors can be called within and without the class.
public:

    rotate_servo(PinName pin);

    void start(chrono::microseconds const & duty_cycle, chrono::milliseconds const & hold_time);

    void stop();

    void sweep(unsigned long long const & start_duty, unsigned long long const & end_duty, unsigned long long const & duty_increment);
};