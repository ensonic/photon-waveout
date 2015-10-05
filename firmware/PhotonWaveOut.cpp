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

#include "PhotonWaveOut.h"

static WaveOut *current = null;

static void playback_handler(void) {
  int v = wave_data[wave_ix];
  // manual PWM, we map value to pulse-width
  if (v >= 0x80) {
    v = 0xFF-v;
    if (v > 0) {
      pinSetFast(current->pwm_n);
      delayMicroseconds(v);
      pinResetFast(current->pwm_n);
    }
  } else {
    if (v > 0) {
      pinSetFast(current->pwm_p);
      delayMicroseconds(v);
      pinResetFast(current->pwm_p);
    }
  }
  current->wave_ix++;
  if (current->wave_ix >= current->wave_len) {
    current->wave_ix = 0;
    // TODO: do loop = false
  }
}
    
bool WaveOut::play(char *wave, size_t *wave_len, bool loop) {
  if (playing)
    stop();

  this->wave = wave;
  this->wave_len = wave_len;
  this->loop = loop;
  this->wave_ix = 0;
  current = this;
  // for 8000 Hz this should be 125, but we cheat a bit to be able to map the samples straight
  audio_clock->begin(play_wave, 127, uSec);
  this->playing = true;
}

void WaveOut::stop(void) {
  if (!playing)
    return;

  audio_clock->end();
  pinResetFast(pwm_p);
  pinResetFast(pwm_n);
  this->playing = false;
  current = null;
}

