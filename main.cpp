#include "mbed.h"
#include "rotate_servo.h"

//This using declaration identifies the correct namespace for the sleep_for function to be used within this object
using rtos::ThisThread::sleep_for;

//Digital out objects for controlling the 5 LEDs representing the Moles.
DigitalOut mole1(D2);
DigitalOut mole2(D3);
DigitalOut mole3(D4);
DigitalOut mole4(D5);
DigitalOut mole5(D6);

//This rotate_servo object drives the servo to point to the LED representing the mole being "whacked"
rotate_servo servoD7(D7);

//Like the digitalin class except it can use interrupts and callbacks as well as read the value of the pin.
InterruptIn blue_button(BUTTON1);

//A binary semaphore; only one "token in the bucket" so there is never a queue of updates for the button.
/*Semaphore blue_button_up_semaphore(0, 1);
Semaphore blue_button_down_semaphore(0, 1);*/

Semaphore print_semaphore(1, 1);

//A thread for process related to the blue button on the nucleo.
/*Thread blue_button_up_thread;
Thread blue_button_down_thread;*/

//These 5 threads are to run the processes for each of the five LEDs, including random interval generation and any deferred execution/Interrupt service routines.
Thread mole1_thread;
Thread mole2_thread;
Thread mole3_thread;
Thread mole4_thread;
Thread mole5_thread;



//Callback function which releases a blue_button_semaphore
/*void blue_button_up_callback(){
    blue_button_up_semaphore.release();
}

void blue_button_down_callback(){
    blue_button_down_semaphore.release();
}*/

void mole1_fn(){
    printf("Sequence 1 started\n");
    while (true) {
        unsigned long long rand_interval1 = rand() % 5000;
        sleep_for(chrono::milliseconds(rand_interval1));
        print_semaphore.acquire();
        printf("Mole number 1\n");
        print_semaphore.release();
    }
}

void mole2_fn(){
    printf("Sequence 2 started\n");
    while (true) {
        unsigned long long rand_interval2 = rand() % 5000;
        sleep_for(chrono::milliseconds(rand_interval2));
        print_semaphore.acquire();
        printf("Mole number 2\n");
        print_semaphore.release();
    }
}

void mole3_fn(){
    printf("Sequence 3 started\n");
    while (true) {
        unsigned long long rand_interval3 = rand() % 5000;
        sleep_for(chrono::milliseconds(rand_interval3));
        print_semaphore.acquire();
        printf("Mole number 3\n");
        print_semaphore.release();
    }
}

void mole4_fn(){
    printf("Sequence 4 started\n");
    while (true) {
        unsigned long long rand_interval4 = rand() % 5000;
        sleep_for(chrono::milliseconds(rand_interval4));
        print_semaphore.acquire();
        printf("Mole number 4\n");
        print_semaphore.release();
    }
}

void mole5_fn(){
    printf("Sequence 5 started\n");
    while (true) {
        unsigned long long rand_interval5 = rand() % 5000;
        sleep_for(chrono::milliseconds(rand_interval5));
        print_semaphore.acquire();
        printf("Mole number 5\n");
        print_semaphore.release();
    }
}

//A function to run in the button pressing thread, which will print something but only if a blue_button_semaphore is available for aquisition.
/*void blue_button_up_fn(){
    while (true) {
        blue_button_up_semaphore.acquire();
        //printf("button up function\n");
        
        mole1_thread.start(mole1_fn);
    }
}

void blue_button_down_fn(){
    while (true) {
        blue_button_down_semaphore.acquire();
        printf("sequence 1 terminated\n");
        mole1_thread.start(mole1_fn);
        mole1_thread.terminate();
        mole1_thread.join();
    }
}*/

//The servo driver must happen in the main thread or there is lots of jitter.
int main() {
    /*blue_button_up_thread.start(blue_button_up_fn);
    blue_button_down_thread.start(blue_button_down_fn);
    blue_button.rise(blue_button_up_callback);
    blue_button.fall(blue_button_down_callback);*/
    mole1_thread.start(mole1_fn);
    mole2_thread.start(mole2_fn);
    mole3_thread.start(mole3_fn);
    mole4_thread.start(mole4_fn);
    mole5_thread.start(mole5_fn);
    while (true) {
        sleep();
    }
}

