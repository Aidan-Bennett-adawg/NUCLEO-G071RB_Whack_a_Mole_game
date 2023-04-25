#include "mbed.h"
#include "rotate_servo.h"

//This using declaration identifies the correct namespace for the sleep_for function to be used within this object
using rtos::ThisThread::sleep_for;

//This private method of rotate_servo makes sig_ true, which sends a signal down whichever pin has been specified in the instantiation of the rotate_servo class, and then attaches the pulse_off function, as a callback function, to the timeout_sig_ Timeout object; the pulse_off function will be called after time "duty_cycle_" which is specified when calling the start function of the rotate_servo class.
void rotate_servo::pulse_on() {
  sig_ = true;
  timeout_sig_.attach(callback(this, &rotate_servo::pulse_off), duty_cycle_);
}

//This private method of rotate_servo makes sig_ false, which stops the signal down whichever pin has been specified in the instantiation of the rotate_servo class
void rotate_servo::pulse_off() { 
    sig_ = false; 
}

//This is the constructor for the rotate_servo class which will take a PinName type variable "pin" in its argument. The sig_ object is then defined using "pin" as one argument, and setting this to an open drain output
rotate_servo::rotate_servo(PinName pin) :
    sig_(pin, PIN_OUTPUT, OpenDrain, 0) { }

//This start method of the rotate_servo class...
void rotate_servo::start(chrono::microseconds const & duty_cycle, chrono::milliseconds const & hold_time) {
    hold_time_ = hold_time;
    duty_cycle_ = duty_cycle;
    ticker_sig_.attach(callback(this, &rotate_servo::pulse_on), 20ms);
    timeout_hold_servo_.attach(callback(this, &rotate_servo::stop), hold_time_);
    sleep_for(hold_time_);
}

void rotate_servo::stop() {
    ticker_sig_.detach();
}

//These need to be unsigned long long so they are compatible with the chrono objects
void rotate_servo::sweep(unsigned long long const & start_duty, unsigned long long const & end_duty, unsigned long long const & duty_increment) {
    start_duty_ = start_duty;
    end_duty_ = end_duty;
    if (start_duty_ <= end_duty_) {
        for (unsigned long long i = start_duty_ ; i <+ end_duty_ ; i += duty_increment){
            start(chrono::microseconds(i), 40ms);
        }
    } else {
        for (unsigned long long i = start_duty_ ; i >= end_duty_ ; i -= duty_increment){
            start(chrono::microseconds(i), 40ms);
        }
    }
}
