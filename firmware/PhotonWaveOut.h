/* 
 * Copyright 2015 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __WAVE_OUT_H__
#define __WAVE_OUT_H__

#include "SparkIntervalTimer/SparkIntervalTimer.h"

#ifdef __cplusplus
extern "C" {
#endif

class WaveOut {
  public:
    int pin_p, pin_n;

    // pin_p, pin_n: pins for positive/negative wave parts
    WaveOut(int pin_p, int pin_n) : pin_p(pin_p), pin_n(pin_n), playing(false) {
      pinMode(pin_p, OUTPUT);
      pinResetFast(pin_p);
      pinMode(pin_n, OUTPUT);
      pinResetFast(pin_n);
      audio_clock = new IntervalTimer();
    }
    ~WaveOut() {
      stop();
      delete audio_clock;
    }
    
    bool play(char *wave, unsigned int wave_len, bool loop);
    void stop(void);
    
    // internal
    int advance(void);
  private:
    char *wave;
    unsigned int wave_len;
    bool loop;
    bool playing;
    volatile unsigned int wave_ix;
    IntervalTimer *audio_clock;
};

#ifdef __cplusplus
}
#endif

#endif /* __WAVE_OUT__ */

