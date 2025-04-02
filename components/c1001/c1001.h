#pragma once

#include "sys/time.h"

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#ifdef USE_NUMBER
#include "esphome/components/number/number.h"
#endif
#ifdef USE_BUTTON
#include "esphome/components/button/button.h"
#endif
#ifdef USE_SELECT
#include "esphome/components/select/select.h"
#endif
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/uart/uart.h"

#define LOOP_WAIT 3
#define SERIAL_BUFFER_LEN 150

#define HUMAN_PRESENCE 0x80    //Human presence data
#define FALL_DETECTION 0x83     //Fall data markers

namespace esphome
{
  namespace c1001
  {

    typedef enum
    {
      eFALLLed = 0x03,
      eHPLed = 0x04,
    } eLed;

    typedef enum
    {
      eSleepMode = 0x02,
      eFallingMode = 0x01,
    } eWorkMode;

    typedef enum
    {
      eHumanPresence,    ///< Human presence query
      eHumanMovement,    ///< Movement information query
      eHumanMovingRange, ///< Movement distance, range 0~100
      eHumanDistance,

    } esmHuman;

    typedef enum
    {
      // eFallSwitch,///<Fall mode switch query
      eFallState,             ///< Fall state
      eFallBreakHeight,       ///< Fall break height
      eHeightRatioSwitch,     ///< Height ratio switch query
      estaticResidencyState,  ///< Static residency state
      estaticResidencySwitch, ///< Static residency switch
      eFallSensitivity,       ///< Fall sensitivity
    } eDmFall;

    typedef enum
    {
      eFallBreakHeightC,   ///< Fall break height
      eHeightRatioSwitchC, ///< Height ratio switch
      eReportFreqC,        ///< Trajectory point information reporting frequency
      eReportSwitchC,      ///< Trajectory point reporting switch
      eAltTimeC,           ///< Height cumulative time
      eFallSensitivityC,   ///< Fall sensitivity setting
      eResidenceSwitchC,   ///< Residency switch
      eResidenceTime,      ///< Residency time

    } eDmFallConfig;

    static const char *TAG = "c1001.sensor";

    class C1001Component : public sensor::Sensor, public PollingComponent, public uart::UARTDevice
    {
    public:
      void set_uart(uart::UARTComponent *uart) { this->uart_ = uart; }

      void setup() override;
      void update() override;
      void loop() override;
      void dump_config() override;

      void process_message(uint8_t *);

      uint16_t getFallData(eDmFall);
      uint32_t getFirmwareVersion(void);
      uint32_t getFallTime(void);
      uint32_t getStaticResidencyTime(void);
      uint8_t getWorkMode(void);
      uint16_t dmGetInstallHeight(void);
      bool getLEDLightState(eLed);
      uint16_t smHumanData(esmHuman);
      uint32_t getUnmannedTime(void);

      uint8_t dmFallConfig(eDmFallConfig, uint32_t);
      void dmUnmannedTime(uint32_t);
      void dmFallTime(uint32_t);
      void dmInstallHeight(uint16_t);
      uint8_t configWorkMode(eWorkMode);
      uint8_t configLEDLight(eLed, uint8_t);

      uint8_t sensorRestart(void);
      uint8_t getData(uint8_t, uint8_t, uint16_t, uint8_t *, uint8_t *);
      uint8_t sumData(uint8_t, uint8_t *);

      void setFallLEDCallback(std::function<void(bool)> x) { this->FallLEDCallback_ = x; }
      void setFallLEDStateCallback(std::function<void(bool)> x) { this->FallLEDStateCallback_ = x; }
      void setWorkModeCallback(std::function<void(bool)> x) { this->WorkModeCallback_ = x; }
      void setBodyMovementCallback(std::function<void(uint8_t)> x) { this->BodyMovementCallback_ = x; }

      void set_fall_presence_sensor(binary_sensor::BinarySensor *s) { fallpresencesensor = s; }
      void set_fall_led_state_sensor(binary_sensor::BinarySensor *s) { fallledstatesensor = s; }
      void set_fall_dwell_sensor(binary_sensor::BinarySensor *s) { falldwellsensor = s; }
      void set_fall_dwell_time_sensor(sensor::Sensor *s) { falldwelltimesensor = s; }
      void set_fall_unmanned_time_sensor(sensor::Sensor *s) { fallunmannedtimesensor = s; }
      void set_fall_fall_time_sensor(sensor::Sensor *s) { fallfalltimesensor = s; }
      void set_fall_install_height_sensor(sensor::Sensor *s) { fallinstallheightsensor = s; }
      void set_fall_sensitivity_sensor(sensor::Sensor *s) { fallsensitivitysensor = s; }
      void set_fall_event_sensor(binary_sensor::BinarySensor *s) { falleventsensor = s; }
      void set_fall_movement_speed_sensor(text_sensor::TextSensor *s) { fallmovementspeedsensor = s; }
      void set_fall_movement_range_sensor(sensor::Sensor *s) { fallmovementrangesensor = s; }
      void set_firmware_version_sensor(text_sensor::TextSensor *s) { firmwareversionsensor = s; }

      void set_movement_sensor(sensor::Sensor *s) { movementsensor = s; }
      void set_distance_sensor(sensor::Sensor *s) { distancesensor = s; }
      void set_mode_text_sensor(text_sensor::TextSensor *s) { modetextsensor = s; }

      #ifdef USE_NUMBER
        SUB_NUMBER(dwell_time)
        SUB_NUMBER(unmanned_time)
        SUB_NUMBER(fall_time)
        SUB_NUMBER(install_height)
      #endif
      #ifdef USE_BUTTON
        SUB_BUTTON(restart)
      #endif
      #ifdef USE_SELECT
        SUB_SELECT(fall_sensitivity)
      #endif
      #ifdef USE_SWITCH
        SUB_SWITCH(human_presence_led)
      #endif

    protected:
      std::function<void(bool)> FallLEDCallback_ = NULL;
      std::function<void(bool)> FallLEDStateCallback_ = NULL;
      std::function<void(bool)> WorkModeCallback_ = NULL;
      std::function<void(uint8_t)> BodyMovementCallback_ = NULL;
      uart::UARTComponent *uart_;
      void restart_()  { sensorRestart(); }

      // fall stuff
      binary_sensor::BinarySensor *falleventsensor;
      binary_sensor::BinarySensor *falldwellsensor;
      sensor::Sensor *fallsensitivitysensor;

      binary_sensor::BinarySensor *fallpresencesensor;
      binary_sensor::BinarySensor *fallledstatesensor;
      sensor::Sensor *falldwelltimesensor;
      sensor::Sensor *fallunmannedtimesensor;
      sensor::Sensor *fallfalltimesensor;
      sensor::Sensor *fallinstallheightsensor;
      text_sensor::TextSensor *fallmovementspeedsensor;
      sensor::Sensor *fallmovementrangesensor;
      text_sensor::TextSensor *firmwareversionsensor;

      sensor::Sensor *movementsensor;
      sensor::Sensor *distancesensor;
      text_sensor::TextSensor *modetextsensor;

      struct timespec last_loop_time;
      uint8_t last_loop_value = 0;
    };

  } // namespace c1001
} // namespace esphome
