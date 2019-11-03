#pragma once
/*
Copyright 2019 Tomoaki Itoh
This software is released under the MIT License, see LICENSE.txt.
//*/

#include "userosc.h"

struct MyPulse {

  enum {
    k_flags_none    = 0,
    k_flag_wave0    = 1<<1,
    k_flag_wave1    = 1<<2,
    k_flag_subwave  = 1<<3,
    k_flag_ringmix  = 1<<4,
    k_flag_bitcrush = 1<<5,
    k_flag_reset    = 1<<6
  };
  
  struct Params {
    uint16_t  shape;
    uint16_t  shiftshape;
    
    Params(void) :
      shape(0),
      shiftshape(0)
    { }
  };
  
  struct State {
    float    phi0;
    float    w00;
    uint32_t flags:8;

    State(void) :
      w00(440.f * k_samplerate_recipf),
      flags(k_flags_none)
    {
      reset();
    }

    inline void reset(void)
    {
      phi0 = 0;
    }
  };

  MyPulse(void) {
    init();
  }

  void init(void) {
    state = State();
    params = Params();
  }

  inline void updatePitch(float w0) {
    state.w00 = w0;
  }

  inline void updateWaves(const uint16_t flags) {
    // nop
  }

  State   state;
  Params  params;
};
