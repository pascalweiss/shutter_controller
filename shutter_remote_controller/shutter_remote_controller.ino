/*
  Example for different sending methods
  https://github.com/sui77/rc-switch/
*/


// How to use VSCode: https://www.youtube.com/watch?v=dany7ae_0ks
// Plot with VSCode: https://github.com/devinaconley/arduino-plotter

#include "stdlib.h"
#include <math.h>
#include <time.h>
#include <SPI.h>
#include <RCSwitch.h>
#include <nRF24L01.h>
#include <RF24.h>


// --- config ---

#define N                        4     // the number of sensor readings, that are stored in the history
#define DELAY                    200
#define HIST_SIZE                2     // the number of old    readings, that are used to determine, if the sensor state has changed. (requirement N > HIST_SIZE + ACTL_SIZE)
#define ACTL_SIZE                2     // the number of actual readings, that are used to determine, if the sensor state has changed. (requirement N > HIST_SIZE + ACTL_SIZE)
#define DIRECTN_SIZE             12    // the number of directn commands, that are used to determine, if the system has a command cycle (shutter goes up and down repeatedly)
#define STOP_CYCLE_THRESHOLD     3     // the number of repeated STOP-directn-commands, to break the stop-cycle
#define RF24_ENABLE              1
#define RF24_MAX                 1500
#define POTI_MAX                 1023  // max value of the POTI // schlafzimmer: 3280, prototyp: 4095
#define POTI_MIN                 0
#define RF433_PULSE_LENGTH       200
#define RF433_PROTOCOL           2
#define RF433_UP                 "10000010001010110101011000000000"
#define RF433_DOWN               "10001010001010110101011000000000"
#define THRESHOLD_POTI           0.015  // the tolerance for the poti. A change of the poti value is only observed, when the difference to the new value exceeds the tolerance
#define THRESHOLD_RF24           0.05   // the percentual change of the RF24 value, at which a shutter (Rollladen) activity is triggered
#define THRESHOLD_MOVE_MOTOR     0.01  // the percentage of the difference of the target at which the motor should be activated
#define SECS_ROLL_UP             21    // the amount of seconds the motor takes to roll up the shutter
#define SECS_ROLL_DOWN           22    // the amount of seconds the motor takes to roll down the shutter
#define THRESHOLD_OVERSHOOT_UP   0.98  // the threshold at which the shutter is pulled up additionally for some seconds
#define SECS_OVERSHOOT_UP        3     // the seconds that the shutter is pulled up additionally, when the position reached THRESHOLD_OVERSHOOT_UP
#define THRESHOLD_OVERSHOOT_DOWN 0.02  // the threshold at which the shutter is pulled down additionally for some seconds
#define SECS_OVERSHOOT_DOWN      3     // the seconds that the shutter is pulled up additionally, when the position reached THRESHOLD_OVERSHOOT_UP

byte PIN_RF24_CE     = 7;
byte PIN_RF24_CSN    = 8;
byte PIN_RELAIS_UP   = 4;
byte PIN_RELAIS_DOWN = 5;
byte PIN_POTI        = A0;
byte PIN_RF433       = 10;


// --- setup ---

enum motor_direction { STOPPED, DOWN, UP, STOP, CYCLE_BREAK}; 
enum sensor          { POTI, RF_24 };

float l_poti[N];
float l_rf24[N];
int l_directn[DIRECTN_SIZE];
float target;
float motor_position;
float positn;
unsigned long t_now = 0;
unsigned long t_diff;
long overshoot_millis = 0;

RF24 rf24(PIN_RF24_CE,PIN_RF24_CSN);
RCSwitch mySwitch = RCSwitch();

const byte address[6] = "00001";


// --- main ---

void setup() {
  Serial.begin(9600);
  mySwitch.enableTransmit(PIN_RF433);
  mySwitch.setPulseLength(RF433_PULSE_LENGTH);
  mySwitch.setProtocol(RF433_PROTOCOL);
  rf24.begin();
  rf24.openReadingPipe(0, address);
  rf24.setPALevel(RF24_PA_MIN);
  rf24.startListening();
  pinMode(PIN_POTI, INPUT);
  pinMode(PIN_RELAIS_UP, OUTPUT);
  pinMode(PIN_RELAIS_DOWN, OUTPUT);
}

void loop() {
  update_time_diff();
  positn = get_position(t_diff);
  read_sensors();
  target = get_target();
  overshoot_millis = get_overshoot_millis(overshoot_millis);
  update_directn(positn, target, overshoot_millis);
  if(is_cycle(l_directn)) {
    positn = break_cycle_with_new_position(l_directn);
  }
  exec_motor_or_delay(l_directn[1], l_directn[0]);
  log_state();
}


// --- time diff ---  

// the time difference since the last function call
void update_time_diff() {
  unsigned long t = millis();
  t_diff = t - t_now;
  t_now = t;
}


// --- position ---  

float get_position(unsigned long t_diff) {
  int directn = l_directn[0];
  if     (directn == UP)   positn += ((float)t_diff * 1/(1000 *SECS_ROLL_UP));
  else if(directn == DOWN) positn -= ((float)t_diff * 1/(1000 *SECS_ROLL_DOWN));
  // directn == STOPPED
  if     (positn < 0) return 0;
  else if(positn > 1) return 1;
  else                return positn;
}


// --- sensor ---  

void read_sensors() {
  push_front(l_poti,N, read_poti());
  push_front(l_rf24,N,read_rf24());
}

float read_rf24() {
  float val = l_rf24[0];
  if(RF24_ENABLE) {
    if (rf24.available()) {
      rf24.read(&val, sizeof(val));
    } 
  }
  if(val > 1.0001) return 1.00;
  else if(val < -0.0001) return 0.00;
  else return val;
}

float read_poti() {
  int pot = analogRead(PIN_POTI);
  // normalize
  float normalized = (float)pot/POTI_MAX;
  if(normalized > 1.0) return 1.0;
  else if(normalized < 0.0) return 0.0;
  return normalized;
}


// --- target ---  

float get_target() {
  if(sensor_changed(l_rf24, THRESHOLD_RF24)) {
    return l_rf24[0];
  }
  else if(sensor_changed(l_poti, THRESHOLD_POTI)) {
    return l_poti[0];
  }
  return target;
}


bool sensor_changed(float* arr, float threshold) {
  float sum_old   = 0;
  float sum_new   = 0;
  // the mean of previous readings
  int i = N;
  while(i --> N - HIST_SIZE) {
    sum_old += arr[i];
  }
  i = ACTL_SIZE; 
  while(i --> 0){
    sum_new += arr[i];
  }
  float mean_old = sum_old/(HIST_SIZE);
  float mean_new = sum_new/(ACTL_SIZE);
  float diff = abs(mean_old - mean_new);
  bool result = diff > threshold; 
  return result; 
}


// --- overshoot ---  

long get_overshoot_millis(long overshoot_millis) {
  int directn = l_directn[0];
  if(overshoot_millis <= 0) {
    if(directn == UP   && positn >= THRESHOLD_OVERSHOOT_UP) {
      return (long)SECS_OVERSHOOT_UP * 1000;
    } else if(directn == DOWN && positn <= THRESHOLD_OVERSHOOT_DOWN) {
      return (long)SECS_OVERSHOOT_DOWN * 1000;
     }
     else return overshoot_millis;
  } 
  long m = overshoot_millis - t_diff;
  return m >= 0 ? m : 0;
}


// --- direction ---  

int update_directn(float positn, float target, long overshoot_millis) {
  int directn = calc_directn(positn, target, overshoot_millis);
    push_front(l_directn, DIRECTN_SIZE, directn);

  if(is_cycle(l_directn)) {
    break_cycle_with_new_position(l_directn);
  } else {
  }
}

int calc_directn(float positn, float target, long overshoot_millis) {
  if(overshoot_millis <= 0) {
    if(target_reached(l_directn[0], positn, target)) {
      return STOP;
    }
    float diff = target - positn;
    // set direction only if threshold is exceeded
    if(abs(diff) > THRESHOLD_MOVE_MOTOR) {
      if     (target > positn) return UP;
      else if(target < positn) return DOWN;
    }
  }
  if(l_directn[0] == STOP) return STOPPED;
  return l_directn[0];
}

bool target_reached(int directn, float positn, float target) {
  if(directn == UP && positn >= target) return true;
  else if(directn == DOWN && positn <= target) return true;
  else return false;
}


// --- break cycle ---


bool is_cycle(int *l_directn) {
  int stop_count = 0;
  for(int i = 0; i < DIRECTN_SIZE; i++) {
    if(l_directn[i] == STOP) {
      stop_count++;
      if(stop_count > STOP_CYCLE_THRESHOLD) return true;
    }
  }
  return false;
}

float break_cycle_with_new_position(int *l_directn) {
  push_front(l_directn, DIRECTN_SIZE, CYCLE_BREAK);
  push_front(l_directn, DIRECTN_SIZE, STOPPED); 
  float new_position = target;
  return new_position;
}


// --- motor ---  

void exec_motor_or_delay(int old_directn, int directn) {
  if(old_directn == directn || directn == STOPPED) delay(DELAY);
  else {
    if(directn == STOP) disable_motor(old_directn);
    else enable_motor(directn); 
  }
}

void enable_motor(int directn) {
  if(directn == UP) {
    mySwitch.send(RF433_UP);delay(200);
    mySwitch.send(RF433_UP);
  } else {
    mySwitch.send(RF433_DOWN);delay(200);
    mySwitch.send(RF433_DOWN);
  }
}

void disable_motor(int old_directn) {
  if(old_directn == DOWN) {
    mySwitch.send(RF433_DOWN);delay(10);
    mySwitch.send(RF433_DOWN);delay(10);
    mySwitch.send(RF433_UP);delay(10);
  }
  else {
    mySwitch.send(RF433_UP);delay(10);
    mySwitch.send(RF433_UP);delay(10);
    mySwitch.send(RF433_DOWN);delay(10);
  }
}

// --- helper ---

void push_front(float* arr, int arr_size, float val) {
  int i = arr_size;
  while(i --> 1) {
    arr[i] = arr[i - 1];
  }
  arr[0] = val;
}

void push_front(int* arr, int arr_size, int val) {
  int i = arr_size;
  while(i --> 1) {
    arr[i] = arr[i - 1];
  }
  arr[0] = val;
}



// --- logging ---  

void log_state() {
  print_percentage("poti",l_poti[0]);
  print_percentage(",rf24", l_rf24[0]);
  print_percentage(",target",target);
  print_percentage(",pos", positn);
  print_int(",direction_1",l_directn[1]);
  print_int(",direction_0",l_directn[0]);
  print_int(",overshoot", overshoot_millis/1000);
  
  Serial.println();
}


void print_percentage(char* n, float val) {
  Serial.print(n);Serial.print(":");Serial.print(val*100);
}

void print_int(char* n, int val) {
  Serial.print(n);Serial.print(":");Serial.print(val);
}

void print_float(char* n, float val) {
  Serial.print(n); Serial.print(":");Serial.print(val);
}
