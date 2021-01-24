#include "stdlib.h"
#include <math.h>
#include <time.h>
#include <SPI.h>
#include <RCSwitch.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "config.h"
#include "SensorReader.h"


// --- setup ---

enum motor_direction { STOPPED, DOWN, UP, STOP, CYCLE_BREAK}; 
enum sensor          { POTI, RF_24 };

int l_directn[DIRECTN_SIZE];
float target = 0.0;
float motor_position;
float positn;
unsigned long t_now = 0;
unsigned long t_diff;
long overshoot_millis = 0;

RF24 rf24(PIN_RF24_CE,PIN_RF24_CSN);
RCSwitch mySwitch = RCSwitch();

const byte address[6] = "00001";

SensorReader reader(&rf24, RF24_ENABLE, PIN_POTI, POTI_MAX, THRESHOLD_RF24, 
                    THRESHOLD_POTI, HIST_SIZE, ACTL_SIZE);
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
  reader.read_sensors();
  target = reader.get_target();
  overshoot_millis = get_overshoot_millis(overshoot_millis);
  update_directn(positn, target, overshoot_millis);
  if(is_cycle(l_directn)) {
    positn = break_cycle_with_new_position(l_directn);
  }
  exec_motor_or_delay(l_directn[1], l_directn[0]);
  log_state(reader.l_poti[0], reader.l_rf24[0]);
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

void log_state(float poti_val, float rf24_val) {
  print_percentage("poti", poti_val);
  print_percentage(",rf24", rf24_val);
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
