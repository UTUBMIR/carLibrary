# Car libray

A set of simple functions that allows to easily controll my rc car over internet...

* this library depends on curl, to install it on linux run: 
```console
sudo apt install libcurl4-openssl-dev
```


## in code

```cpp
// sets base url for car
void setUrl(char* url);

// stops program by specified amount of milliseconds
int sleepm(long msec);

// sets speed for left and right motor, from -90 up to 90
int drive(int left, int right);

// set speed for left and right motor, from -90 up to 90. timer specifies milliseconds untill stop
int driveWithTimer(int left, int right, int timer);

// turns servor for the radar to look at the specified angle
int look(int angle);

// turns lights on or off
int light(bool state);

// returns distance from radar to obstacle
int watch();
```

### example

```cpp
#include "car.h"

int main() {
    setUrl("https://carUrl"); // replace with actual url

    look(0); // look straight forward
    light(false); // turning off the lights
    driveWithTimer(90, 90, 1000); // drive straight one second on full speed
    sleepm(1000); // sleep while car is driving
    
    if (watch() < 50) { // if distance is smaller than 50 centimeters
        light(true); // turn on the lights
    }
}
```