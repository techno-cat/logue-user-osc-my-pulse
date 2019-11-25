/*
Copyright 2019 Tomoaki Itoh
This software is released under the MIT License, see LICENSE.txt.
//*/

#include "userosc.h"
#include "my_pulse.hpp"

static MyPulse s_my_pulse;

void OSC_INIT(uint32_t platform, uint32_t api)
{
  s_my_pulse = MyPulse();
}

void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames)
{
  MyPulse::State &s = s_my_pulse.state;
  const MyPulse::Params &p = s_my_pulse.params;

  // Handle events.
  {
    const uint32_t flags = s.flags;
    s.flags = MyPulse::k_flags_none;

    s_my_pulse.updatePitch(
      osc_w0f_for_note((params->pitch)>>8, params->pitch & 0xFF));
    s_my_pulse.updateWaves(flags);
  }
  
  // Temporaries.
  float phi0 = s.phi0;
  
  q31_t * __restrict y = (q31_t *)yn;
  const q31_t * y_e = y + frames;
  
  for (; y != y_e; ) {
    float sig = 0.5f < phi0 ? 1.0f : -1.0f;
    // sig = osc_softclipf(0.125f, sig);    
    *(y++) = f32_to_q31(sig);
    
    phi0 += s.w00;
    phi0 -= (uint32_t)phi0;
  }
  
  s.phi0 = phi0;
}

void OSC_NOTEON(const user_osc_param_t * const params)
{
  s_my_pulse.state.flags |= MyPulse::k_flag_reset;
}

void OSC_NOTEOFF(const user_osc_param_t * const params)
{
  (void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value)
{ 
  MyPulse::Params &p = s_my_pulse.params;
  
  switch (index) {
  case k_user_osc_param_shape:
    p.shape = value;
    break;
  case k_user_osc_param_shiftshape:
    p.shiftshape = value;
    break;
  default:
    break;
  }
}

