#ifndef CAR
#define CAR

typedef struct {
    int code;
    char* text;
} responseData;

struct memory {
    char* buffer;
    unsigned int size;
};

unsigned int writeCallback(void* ptr, unsigned int size, unsigned int nmemb, void* userdata);

// sets base url for car
void setUrl(char* url);

responseData get(const char* url);

// request command with format
responseData request(const char* format, ...);

// stops program by specified amount of milliseconds
int sleepm(long msec);

// sets speed for left and right motor, from -90 up to 90
int drive(int left, int right);

// set speed for left and right motor, from -90 up to 90. timer specifies milliseconds untill stop
int driveWithTimer(int left, int right, int timer);

// turns servor for the radar to look at the specified angle
int look(int angle);

// returns distance from radar to obstacle
int watch();

#endif