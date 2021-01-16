/*
void print_greeting() {
  for(int i=0; i<50;i++) Serial.print("+");
  Serial.println("starting tests");  
}

void print_array(float* arr, int arr_size) {
  int i = arr_size;
  char sbuffer[50];
  while(i --> 0) {
    sprintf(sbuffer, "arr[%d]: %f", i, arr[i]);
    Serial.println(sbuffer);
  }
}

float roundz(float x, int p) {
    if (x != 0.0) {
        return ((floor((fabs(x)*pow(float(10.0),p))+0.5))/pow(float(10.0),p))*(x/fabs(x));
    } else {
        return 0.0;
    }
}

*/
/*
 * _            _
 *| |_ ___  ___| |_ ___
 *| __/ _ \/ __| __/ __|
 *| ||  __/\__ \ |_\__ \
 * \__\___||___/\__|___/
 */

/*
// normalizer: checks if the values from sensors are correctly translated. The normalized values are rounded, such that they can be compared
bool t0() {
  bool result = true;
  int POTI_raw[] = {0,30,4000};
  float POTI_results[] = {0.0, 0.01, 0.98};
  for(int i=0;i<sizeof(POTI_raw)/sizeof(int);i++) {
    float normalized = roundz(normalize(POTI_raw[i],POTI),2);
    result = result && normalized == POTI_results[i];
  }
  return result;
}

// sensors reading: If it doesn't crash, it is okay
bool t1() {
  float POTI = read_poti();
  float rf24 = read_rf24();
  return POTI < 1 && rf24 > 0;
}

// push front
bool t2() {
  float target = drand48();
  float readings[N];
  float new_readings[N];
  create_test_readings(readings);
  memcpy(new_readings, readings, sizeof(readings));
  push_front(new_readings, N, target);
  int i = N;
  bool success = new_readings[0] == target;
  while(i --> 1) {
    success = success && new_readings[i] == readings[i-1];
  }
  return success;
}

// sensor has changed:
bool t3() {
  bool success = true;
  float readings[N];
  create_test_readings(readings);
  int i=N;
  while(i-->0)
    readings[i] = (readings[i] + 4.5) / 10; // shift values close to 0.5
  push_front(readings, N, 0.5);
  success = sensor_has_changed(readings, N, HIST_SIZE, ACTL_SIZE, 0.1);
  push_front(readings, N, 0.7);
  success = success && sensor_has_changed(readings, N, HIST_SIZE, ACTL_SIZE, 0.1);
  return success;
}

// choose sensor reading
bool t4() {
    return false;
}

bool (*test_ptr[5])() = {t0,t1,t2,t3,t4};
bool finished = false;

void run_tests() {
  srand48(time(NULL));
  while(!Serial);
  int n_tests = sizeof(test_ptr)/sizeof(test_ptr[0]);
  if(!finished) {
    print_greeting();
    for(int i=0; i < n_tests; i++) {
      bool success = (*test_ptr[i])();    
      char buffer[50];
      sprintf(buffer, "test %d: %s", i, success ? "successful" : "not successful");
      Serial.println(buffer);
    }
  }
  finished = true;
}

void create_test_readings(float* arr) {
  for(int i=0; i<N;i++) {
    arr[i] = drand48();
  }
}
*/
