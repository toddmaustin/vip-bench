#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
// include build configuration defines
#include "../config.h"
#include "kalman.h"

// supported sizes: 256 (default), 512, 1024, 2048
#define DATASET_SIZE 256

KalmanFilter kf;

int
main(void)
{
  // initialize the privacy enhanced execution target
  VIP_INIT;

  // initialize the pseudo-RNG
  mysrand(42);
  // mysrand(time(NULL));

  // set up Kalman filter
  kf.init(1);
  kf.setProcessNoise(0.1, 0.01);
  kf.setMeasurementNoise(0.1);
 
  // set initial state
  kf.set(22.4);

  for (unsigned step = 1; step < DATASET_SIZE; step++)
  {
    // Kalman filter step
    kf.predict(20.0/1000.0);
    float x = kf.get();
    fprintf(stdout, "INFO: x = %f\n", x);
    kf.correct(22.8);
  }

  fprintf(stdout, "INFO: data is properly sorted.\n");
  return 0;
}


#ifdef notdef

#include <kalman.h>

unsigned long timer = 0;

void setup() {

}

void loop() {

  // Delta time : time since last prediction
  float dt = (millis()-timer)/1000.f;
  timer = millis();
}

KalmanFilter kf;
float mouse[2];

void setup() {


void update() {
  float dt = getLastFrameTime();
  kf.predict(dt);
  kf.get(mouse, 2);
  kf.correct(getMousePos(), 2);
}
#endif /* notdef */

