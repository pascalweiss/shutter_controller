#include "SensorReader.h"
#include "config.h"


SensorReader::SensorReader(byte pin_rf24_ce, byte pin_rf24_csn, bool enable_rf24, int pin_poti, 
                           int poti_max, float threshold_rf24, float threshold_poti,
                           int size_hist, int size_actl) {
                                 
    this->SIZE_HIST = size_hist;
    this->SIZE_ACTL = size_actl;
    this->PIN_POTI = pin_poti;
    this->MAX_POTI = poti_max;
    this->RF24_THRESHOLD = threshold_rf24;
    this->POTI_THRESHOLD = threshold_poti;
    this->ENABLE_RF24 = enable_rf24;
    RF24 rf24(pin_rf24_ce,pin_rf24_csn);
    this->rf24 = &rf24;
}

void SensorReader::init() {
  pinMode(PIN_POTI, INPUT);                        
  if(this->ENABLE_RF24) set_up_rf24();
}

void SensorReader::set_up_rf24() {
  const byte address[6] = "00001";
  this->rf24->begin();
  this->rf24->openReadingPipe(0, address);
  this->rf24->setPALevel(RF24_PA_MIN);
  this->rf24->startListening();
}

void SensorReader::read_sensors() {
    this->push_front(this->l_poti, this->read_poti());
    this->push_front(l_rf24, read_rf24());
}

void SensorReader::push_front(float* arr, float val) {
  int i = N_READINGS;
  while(i --> 1) {
    arr[i] = arr[i - 1];
  }
  arr[0] = val;
}


float SensorReader::read_rf24() {
  float val = l_rf24[0];
  if(this->ENABLE_RF24) {
    if (this->rf24->available()) {
      this->rf24->read(&val, sizeof(val));
    } 
  }
  if(val > 1.0001) return 1.00;
  else if(val < -0.0001) return 0.00;
  else return val;
}

float SensorReader::read_poti() {
  int pot = analogRead(this->PIN_POTI);
  // normalize
  float normalized = (float)pot/this->MAX_POTI;
  if(normalized > 1.0) return 1.0;
  else if(normalized < 0.0) return 0.0;
  return normalized;
}



// --- target ---  

float SensorReader::get_target() {
  if(sensor_changed(l_rf24, RF24_THRESHOLD)) {
    this->target = this->l_rf24[0];
  }
  else if(sensor_changed(l_poti, POTI_THRESHOLD)) {
    this->target = this->l_poti[0];
  }
  return this->target;
}


bool SensorReader::sensor_changed(float* arr, float threshold) {
  float sum_old   = 0;
  float sum_new   = 0;
  // the mean of previous readings
  int i = N_READINGS;
  while(i --> N_READINGS - SIZE_HIST) {
    sum_old += arr[i];
  }
  i = this->SIZE_ACTL; 
  while(i --> 0){
    sum_new += arr[i];
  }
  float mean_old = sum_old/(this->SIZE_ACTL);
  float mean_new = sum_new/(this->SIZE_ACTL);
  float diff = abs(mean_old - mean_new);
  bool result = diff > threshold; 
  return result; 
}
