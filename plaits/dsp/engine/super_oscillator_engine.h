// Copyright 2016 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// 7 variable shape oscillators with spread, shape and pw.

#ifndef PLAITS_DSP_ENGINE_SUPER_OSCILLATOR_ENGINE_H_
#define PLAITS_DSP_ENGINE_SUPER_OSCILLATOR_ENGINE_H_

#include "plaits/dsp/engine/engine.h"
#include "plaits/dsp/oscillator/variable_shape_oscillator.h"

namespace plaits {
  
class SuperVoice {
 public:
  SuperVoice() { }
  ~SuperVoice() { }

  void Init(float rank) {
    rank_ = rank;
    var_osc_.Init();
  }

  void Render(
        float f0,
        float shape,
        float pw,
        float spread,
        size_t size,
        float* out
      ) {

    const float linear_amount = rank_ * (rank_ + 0.01f) * spread * 0.25f;
    f0 *= 1.0f + linear_amount;

    var_osc_.Render<false>(
      f0, f0, pw, shape, out, size);
  };

 private:
  float rank_;

  VariableShapeOscillator var_osc_;
};

class SuperOscillatorEngine : public Engine {
 public:
  SuperOscillatorEngine() { }
  ~SuperOscillatorEngine() { }
  
  virtual void Init(stmlib::BufferAllocator* allocator);
  virtual void Reset();
  virtual void Render(const EngineParameters& parameters,
      float* out,
      float* aux,
      size_t size,
      bool* already_enveloped);
  
 private:
  SuperVoice super_voice_[7];
  float* temp_buffer_;
  float* phase_buffer_[7];
  int rotatingIndex;
  
  DISALLOW_COPY_AND_ASSIGN(SuperOscillatorEngine);
};

}  // namespace plaits

#endif  // PLAITS_DSP_ENGINE_SUPER_OSCILLATOR_ENGINE_H_