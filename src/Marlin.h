/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef __MARLIN_H__
#define __MARLIN_H__

#include "inc/MarlinConfig.h"

#ifdef DEBUG_GCODE_PARSER
  #include "gcode/parser.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void stop();

void idle(
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    bool no_stepper_sleep = false  // pass true to keep steppers from disabling on timeout
  #endif
);

void manage_inactivity(const bool ignore_stepper_queue=false);

#if HAS_X2_ENABLE
  #define  enable_X() do{ X_ENABLE_WRITE( X_ENABLE_ON); X2_ENABLE_WRITE( X_ENABLE_ON); }while(0)
  #define disable_X() do{ X_ENABLE_WRITE(!X_ENABLE_ON); X2_ENABLE_WRITE(!X_ENABLE_ON); CBI(axis_known_position, X_AXIS); }while(0)
#elif HAS_X_ENABLE
  #define  enable_X() X_ENABLE_WRITE( X_ENABLE_ON)
  #define disable_X() do{ X_ENABLE_WRITE(!X_ENABLE_ON); CBI(axis_known_position, X_AXIS); }while(0)
#else
  #define  enable_X() NOOP
  #define disable_X() NOOP
#endif

#if HAS_Y2_ENABLE
  #define  enable_Y() do{ Y_ENABLE_WRITE( Y_ENABLE_ON); Y2_ENABLE_WRITE(Y_ENABLE_ON); }while(0)
  #define disable_Y() do{ Y_ENABLE_WRITE(!Y_ENABLE_ON); Y2_ENABLE_WRITE(!Y_ENABLE_ON); CBI(axis_known_position, Y_AXIS); }while(0)
#elif HAS_Y_ENABLE
  #define  enable_Y() Y_ENABLE_WRITE( Y_ENABLE_ON)
  #define disable_Y() do{ Y_ENABLE_WRITE(!Y_ENABLE_ON); CBI(axis_known_position, Y_AXIS); }while(0)
#else
  #define  enable_Y() NOOP
  #define disable_Y() NOOP
#endif

#if HAS_Z3_ENABLE
  #define  enable_Z() do{ Z_ENABLE_WRITE( Z_ENABLE_ON); Z2_ENABLE_WRITE(Z_ENABLE_ON); Z3_ENABLE_WRITE(Z_ENABLE_ON); }while(0)
  #define disable_Z() do{ Z_ENABLE_WRITE(!Z_ENABLE_ON); Z2_ENABLE_WRITE(!Z_ENABLE_ON); Z3_ENABLE_WRITE(!Z_ENABLE_ON); CBI(axis_known_position, Z_AXIS); }while(0)
#elif HAS_Z2_ENABLE
  #define  enable_Z() do{ Z_ENABLE_WRITE( Z_ENABLE_ON); Z2_ENABLE_WRITE(Z_ENABLE_ON); }while(0)
  #define disable_Z() do{ Z_ENABLE_WRITE(!Z_ENABLE_ON); Z2_ENABLE_WRITE(!Z_ENABLE_ON); CBI(axis_known_position, Z_AXIS); }while(0)
#elif HAS_Z_ENABLE
  #define  enable_Z() Z_ENABLE_WRITE( Z_ENABLE_ON)
  #define disable_Z() do{ Z_ENABLE_WRITE(!Z_ENABLE_ON); CBI(axis_known_position, Z_AXIS); }while(0)
#else
  #define  enable_Z() NOOP
  #define disable_Z() NOOP
#endif

#if ENABLED(MIXING_EXTRUDER)

  /**
   * Mixing steppers synchronize their enable (and direction) together
   */
  #if MIXING_STEPPERS > 5
    #define  enable_E0() { E0_ENABLE_WRITE( E_ENABLE_ON); E1_ENABLE_WRITE( E_ENABLE_ON); E2_ENABLE_WRITE( E_ENABLE_ON); E3_ENABLE_WRITE( E_ENABLE_ON); E4_ENABLE_WRITE( E_ENABLE_ON); E5_ENABLE_WRITE( E_ENABLE_ON); }
    #define disable_E0() { E0_ENABLE_WRITE(!E_ENABLE_ON); E1_ENABLE_WRITE(!E_ENABLE_ON); E2_ENABLE_WRITE(!E_ENABLE_ON); E3_ENABLE_WRITE(!E_ENABLE_ON); E4_ENABLE_WRITE(!E_ENABLE_ON); E5_ENABLE_WRITE(!E_ENABLE_ON); }
  #elif MIXING_STEPPERS > 4
    #define  enable_E0() { E0_ENABLE_WRITE( E_ENABLE_ON); E1_ENABLE_WRITE( E_ENABLE_ON); E2_ENABLE_WRITE( E_ENABLE_ON); E3_ENABLE_WRITE( E_ENABLE_ON); E4_ENABLE_WRITE( E_ENABLE_ON); }
    #define disable_E0() { E0_ENABLE_WRITE(!E_ENABLE_ON); E1_ENABLE_WRITE(!E_ENABLE_ON); E2_ENABLE_WRITE(!E_ENABLE_ON); E3_ENABLE_WRITE(!E_ENABLE_ON); E4_ENABLE_WRITE(!E_ENABLE_ON); }
  #elif MIXING_STEPPERS > 3
    #define  enable_E0() { E0_ENABLE_WRITE( E_ENABLE_ON); E1_ENABLE_WRITE( E_ENABLE_ON); E2_ENABLE_WRITE( E_ENABLE_ON); E3_ENABLE_WRITE( E_ENABLE_ON); }
    #define disable_E0() { E0_ENABLE_WRITE(!E_ENABLE_ON); E1_ENABLE_WRITE(!E_ENABLE_ON); E2_ENABLE_WRITE(!E_ENABLE_ON); E3_ENABLE_WRITE(!E_ENABLE_ON); }
  #elif MIXING_STEPPERS > 2
    #define  enable_E0() { E0_ENABLE_WRITE( E_ENABLE_ON); E1_ENABLE_WRITE( E_ENABLE_ON); E2_ENABLE_WRITE( E_ENABLE_ON); }
    #define disable_E0() { E0_ENABLE_WRITE(!E_ENABLE_ON); E1_ENABLE_WRITE(!E_ENABLE_ON); E2_ENABLE_WRITE(!E_ENABLE_ON); }
  #else
    #define  enable_E0() { E0_ENABLE_WRITE( E_ENABLE_ON); E1_ENABLE_WRITE( E_ENABLE_ON); }
    #define disable_E0() { E0_ENABLE_WRITE(!E_ENABLE_ON); E1_ENABLE_WRITE(!E_ENABLE_ON); }
  #endif
  #define  enable_E1() NOOP
  #define disable_E1() NOOP
  #define  enable_E2() NOOP
  #define disable_E2() NOOP
  #define  enable_E3() NOOP
  #define disable_E3() NOOP
  #define  enable_E4() NOOP
  #define disable_E4() NOOP
  #define  enable_E5() NOOP
  #define disable_E5() NOOP

#else // !MIXING_EXTRUDER

  #if HAS_E0_ENABLE
    #define  enable_E0() E0_ENABLE_WRITE( E_ENABLE_ON)
    #define disable_E0() E0_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define  enable_E0() NOOP
    #define disable_E0() NOOP
  #endif

  #if E_STEPPERS > 1 && HAS_E1_ENABLE
    #define  enable_E1() E1_ENABLE_WRITE( E_ENABLE_ON)
    #define disable_E1() E1_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define  enable_E1() NOOP
    #define disable_E1() NOOP
  #endif

  #if E_STEPPERS > 2 && HAS_E2_ENABLE
    #define  enable_E2() E2_ENABLE_WRITE( E_ENABLE_ON)
    #define disable_E2() E2_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define  enable_E2() NOOP
    #define disable_E2() NOOP
  #endif

  #if E_STEPPERS > 3 && HAS_E3_ENABLE
    #define  enable_E3() E3_ENABLE_WRITE( E_ENABLE_ON)
    #define disable_E3() E3_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define  enable_E3() NOOP
    #define disable_E3() NOOP
  #endif

  #if E_STEPPERS > 4 && HAS_E4_ENABLE
    #define  enable_E4() E4_ENABLE_WRITE( E_ENABLE_ON)
    #define disable_E4() E4_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define  enable_E4() NOOP
    #define disable_E4() NOOP
  #endif

  #if E_STEPPERS > 5 && HAS_E5_ENABLE
    #define  enable_E5() E5_ENABLE_WRITE( E_ENABLE_ON)
    #define disable_E5() E5_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define  enable_E5() NOOP
    #define disable_E5() NOOP
  #endif

#endif // !MIXING_EXTRUDER

#if ENABLED(EXPERIMENTAL_I2CBUS)
  #include "feature/twibus.h"
  extern TWIBus i2c;
#endif

#if ENABLED(G38_PROBE_TARGET)
  extern bool G38_move,        // flag to tell the interrupt handler that a G38 command is being run
              G38_endstop_hit; // flag from the interrupt handler to indicate if the endstop went active
#endif

/**
 * The axis order in all axis related arrays is X, Y, Z, E
 */
void enable_all_steppers();
void disable_e_stepper(const uint8_t e);
void disable_e_steppers();
void disable_all_steppers();

void kill(const char*);

void quickstop_stepper();

extern bool Running;
inline bool IsRunning() { return  Running; }
inline bool IsStopped() { return !Running; }

extern uint8_t axis_homed, axis_known_position;

constexpr uint8_t xyz_bits = _BV(X_AXIS) | _BV(Y_AXIS) | _BV(Z_AXIS);
FORCE_INLINE bool all_axes_homed() { return (axis_homed & xyz_bits) == xyz_bits; }
FORCE_INLINE bool all_axes_known() { return (axis_known_position & xyz_bits) == xyz_bits; }

extern volatile bool wait_for_heatup;

#if HAS_RESUME_CONTINUE
  extern volatile bool wait_for_user;
#endif

#if HAS_AUTO_REPORTING || ENABLED(HOST_KEEPALIVE_FEATURE)
  extern bool suspend_auto_report;
#endif

// Inactivity shutdown timer
extern millis_t max_inactive_time, stepper_inactive_time;

#if FAN_COUNT > 0
  extern int16_t fanSpeeds[FAN_COUNT];
  #if ENABLED(EXTRA_FAN_SPEED)
    extern int16_t old_fanSpeeds[FAN_COUNT],
                   new_fanSpeeds[FAN_COUNT];
  #endif
  #if ENABLED(PROBING_FANS_OFF)
    extern bool fans_paused;
    extern int16_t paused_fanSpeeds[FAN_COUNT];
  #endif
#endif

#if ENABLED(USE_CONTROLLER_FAN)
  extern uint8_t controllerFanSpeed;
#endif

#if HAS_POWER_SWITCH
  extern bool powersupply_on;
  #define PSU_PIN_ON()  do{ OUT_WRITE(PS_ON_PIN, PS_ON_AWAKE); powersupply_on = true; }while(0)
  #define PSU_PIN_OFF() do{ OUT_WRITE(PS_ON_PIN, PS_ON_ASLEEP); powersupply_on = false; }while(0)
  #if ENABLED(AUTO_POWER_CONTROL)
    #define PSU_ON()  powerManager.power_on()
    #define PSU_OFF() powerManager.power_off()
  #else
    #define PSU_ON()  PSU_PIN_ON()
    #define PSU_OFF() PSU_PIN_OFF()
  #endif
#endif

bool pin_is_protected(const pin_t pin);
void protected_pin_err();

#if HAS_SUICIDE
  inline void suicide() { OUT_WRITE(SUICIDE_PIN, LOW); }
#endif

#endif // __MARLIN_H__
