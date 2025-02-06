#include "mbed.h"
#include "rotate_servo.h"

//This using declaration identifies the correct namespace for the sleep_for function to be used within this object
using rtos::ThisThread::sleep_for;

//Creates an object using the rotate_servo class using pin D13, which will be used to drive the servo to point towards the LED representing the mole being "whacked"
rotate_servo servoD13(D13);

//Digital out objects for controlling the five LEDs representing the Moles.
DigitalOut mole1(D8);
DigitalOut mole2(D9);
DigitalOut mole3(D10);
DigitalOut mole4(D11);
DigitalOut mole5(D12);

//These InterruptIn objects allow callback fuctions to be attached to the rising edge of each of the five buttons next to the mole LEDs. They have built in pull down resistors so that connecting each pin to 5vcc, when a button is pressed, will bring them up to a logic high.
InterruptIn mole1_button(D2, PullDown);
InterruptIn mole2_button(D3, PullDown);
InterruptIn mole3_button(D4, PullDown);
InterruptIn mole4_button(D5, PullDown);
InterruptIn mole5_button(D6, PullDown);

//Like the digitalin class except it can use interrupts and callbacks as well as read the value of the pin.
InterruptIn blue_button(BUTTON1);

//This binary semaphore will be released by a callback so all the threads can start
Semaphore start_semaphore(0, 1);

//This binary semaphore will be released when the score, and number of lives remainings, is to be calculated and printed.
Semaphore score_semaphore(0, 1);

//For each of the five moles LEDs there is one mole semaphore, which will only be available when that LED is on, and one mole button semaphore, which will be released via a callback on each rising edge of that mole button.
Semaphore mole1_semaphore(0, 1);
Semaphore mole1_button_semaphore(0, 1);
Semaphore mole2_semaphore(0, 1);
Semaphore mole2_button_semaphore(0, 1);
Semaphore mole3_semaphore(0, 1);
Semaphore mole3_button_semaphore(0, 1);
Semaphore mole4_semaphore(0, 1);
Semaphore mole4_button_semaphore(0, 1);
Semaphore mole5_semaphore(0, 1);
Semaphore mole5_button_semaphore(0, 1);

//These 10 threads are to run the processes for each of the five LEDs, and each of the five buttons, including random interval generation and any deferred execution/Interrupt service routines.
Thread mole1_thread(osPriorityNormal, 2048, nullptr, nullptr);
Thread mole1_button_thread(osPriorityNormal, 2048, nullptr, nullptr);
Thread mole2_thread(osPriorityNormal, 2048, nullptr, nullptr);
Thread mole2_button_thread(osPriorityNormal, 2048, nullptr, nullptr);
Thread mole3_thread(osPriorityNormal, 2048, nullptr, nullptr);
Thread mole3_button_thread(osPriorityNormal, 2048, nullptr, nullptr);
Thread mole4_thread(osPriorityNormal, 2048, nullptr, nullptr);
Thread mole4_button_thread(osPriorityNormal, 2048, nullptr, nullptr);
Thread mole5_thread(osPriorityNormal, 2048, nullptr, nullptr);
Thread mole5_button_thread(osPriorityNormal, 2048, nullptr, nullptr);

//This thread is to calculate the score, and lives, each time a button is pressed, then give game success or failure messages depending on the results. 
Thread score_thread(osPriorityNormal, 2048, nullptr, nullptr);

//These bool types are so that the state of each mole semaphore can be evaluated each time its button is pressed: if the mole semaphore is available a point will be given, or deducted if not.
bool score_check1;
bool score_check2;
bool score_check3;
bool score_check4;
bool score_check5;

//These two integers are to keep track of the score and number of lives respectivley.
int score = 0;
int lives = 5;

void start_callback(){
    start_semaphore.release();
}

void score_fn(){
        while (lives > 0 && score < 20){
        score_semaphore.acquire();
        if (score < 20) {
            printf("Moles whacked: %d\nLives left: %d\n\n", score, lives);
        } else {
            printf("Well done! You Whacked all 20 moles with %d lives left! Press the nucleo reset button to try for a better score.", lives);
        }
    if (lives <= 0){
        printf("Oh no! You lost all your lives with %d moles left to whack :(, press the black nucleo reset button to try again.", 20-score);
    } else {}
    }
}

void mole1_fn(){
    while (true) {
        unsigned long long rand_interval1 = rand() % 8000;
        sleep_for(chrono::milliseconds(rand_interval1));
        mole1 = true;
        mole1_semaphore.release();
        sleep_for(500ms);
        mole1 = false;
        mole1_semaphore.try_acquire();
    }
}

void mole1_callback(){
    mole1_button_semaphore.release();
}

void mole1_button_fn(){
    while(true){
        mole1_button_semaphore.acquire();
        score_check1 = mole1_semaphore.try_acquire();
        if (score_check1 == true){
            score += 1;
        } else {
            lives -= 1;
        }
        score_semaphore.release();
        servoD13.start(1800us, 20ms);
        thread_sleep_for(800);
        bool b = mole1_button_semaphore.try_acquire();
        while(b == true){
            b = mole1_button_semaphore.try_acquire();
            thread_sleep_for(1);
        }
    }
}

void mole2_fn(){
    while (true) {
        unsigned long long rand_interval2 = rand() % 8000;
        sleep_for(chrono::milliseconds(rand_interval2));
        mole2 = true;
        mole2_semaphore.release();
        sleep_for(500ms);
        mole2 = false;
        mole2_semaphore.try_acquire();
    }
}

void mole2_callback(){
    mole2_button_semaphore.release();
}

void mole2_button_fn(){
    while(true){
        mole2_button_semaphore.acquire();
        score_check2 = mole2_semaphore.try_acquire();
        if (score_check2 == true){
            score += 1;
        } else {
            lives-= 1;
        }
        score_semaphore.release();
        servoD13.start(1700us, 20ms);
        thread_sleep_for(800);
        bool b2 = mole2_button_semaphore.try_acquire();
        while(b2 == true){
            b2 = mole2_button_semaphore.try_acquire();
            thread_sleep_for(1);
        }
    }
}

void mole3_fn(){
    while (true) {
        unsigned long long rand_interval3 = rand() % 8000;
        sleep_for(chrono::milliseconds(rand_interval3));
        mole3 = true;
        mole3_semaphore.release();
        sleep_for(500ms);
        mole3 = false;
        mole3_semaphore.try_acquire();
    }
}

void mole3_callback(){
    mole3_button_semaphore.release();
}

void mole3_button_fn(){
    while(true){
        mole3_button_semaphore.acquire();
        score_check3 = mole3_semaphore.try_acquire();
        if (score_check3 == true){
            score += 1;
        } else {
            lives-= 1;
        }
        score_semaphore.release();
        servoD13.start(1500us, 20ms);
        thread_sleep_for(800);
        bool b3 = mole3_button_semaphore.try_acquire();
        while(b3 == true){
            b3 = mole3_button_semaphore.try_acquire();
            thread_sleep_for(1);
        }
    }
}

void mole4_fn(){
    while (true) {
        unsigned long long rand_interval4 = rand() % 8000;
        sleep_for(chrono::milliseconds(rand_interval4));
        mole4 = true;
        mole4_semaphore.release();
        sleep_for(500ms);
        mole4 = false;
        mole4_semaphore.try_acquire();
    }
}

void mole4_callback(){
    mole4_button_semaphore.release();
}

void mole4_button_fn(){
    while(true){
        mole4_button_semaphore.acquire();
        score_check4 = mole4_semaphore.try_acquire();
        if (score_check4 == true){
            score += 1;
        } else {
            lives-= 1;
        }
        score_semaphore.release();
        servoD13.start(1300us, 20ms);
        thread_sleep_for(800);
        bool b4 = mole4_button_semaphore.try_acquire();
        while(b4 == true){
            b4 = mole4_button_semaphore.try_acquire();
            thread_sleep_for(1);
        }
    }
}

void mole5_fn(){
    while (true) {
        unsigned long long rand_interval5 = rand() % 8000;
        sleep_for(chrono::milliseconds(rand_interval5));
        mole5 = true;
        mole5_semaphore.release();
        sleep_for(500ms);
        mole5 = false;
        mole5_semaphore.try_acquire();
    }
}

void mole5_callback(){
    mole5_button_semaphore.release();
}

void mole5_button_fn(){
    while(true){
        mole5_button_semaphore.acquire();
        score_check5 = mole5_semaphore.try_acquire();
        if (score_check5 == true){
            score += 1;
        } else {
            lives-= 1;
        }
        score_semaphore.release();
        servoD13.start(1200us, 20ms);
        thread_sleep_for(800);
        bool b5 = mole4_button_semaphore.try_acquire();
        while(b5 == true){
            b5 = mole5_button_semaphore.try_acquire();
            thread_sleep_for(1);
        }
    }
}

//The servo driver must happen in the main thread or there is lots of jitter.
int main() {

    blue_button.fall(start_callback);
    printf("Your goal is to whack all 20 moles whilst loosing as few of your five lives as possible; if you loose all five lives, you will have to try again!\nYou whack a mole by pressing the button next to one of the five white LEDs when lit up.\nPress the blue button to wake up the moles...\nGood luck!\n");

    start_semaphore.acquire();
    mole1_thread.start(mole1_fn);
    mole1_button_thread.start(mole1_button_fn);
    mole2_thread.start(mole2_fn);
    mole2_button_thread.start(mole2_button_fn);
    mole3_thread.start(mole3_fn);
    mole3_button_thread.start(mole3_button_fn);
    mole4_thread.start(mole4_fn);
    mole4_button_thread.start(mole4_button_fn);
    mole5_thread.start(mole5_fn);
    mole5_button_thread.start(mole5_button_fn);
    score_thread.start(score_fn);

    mole1_button.rise(mole1_callback);
    mole2_button.rise(mole2_callback);
    mole3_button.rise(mole3_callback);
    mole4_button.rise(mole4_callback);
    mole5_button.rise(mole5_callback);

    while(true);
}

