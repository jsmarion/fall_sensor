#include "sstream"
#include "queue"

#include "esphome/core/log.h"
#include "c1001.h"

namespace esphome
{
  namespace c1001
  {

    static inline void timespec_diff(struct timespec *a, struct timespec *b, struct timespec *result)
    {
      result->tv_sec = a->tv_sec - b->tv_sec;
      result->tv_nsec = a->tv_nsec - b->tv_nsec;
      if (result->tv_nsec < 0)
      {
        --result->tv_sec;
        result->tv_nsec += 1000000000L;
      }
    }

    // set up queue to process responses
    std::queue<std::vector<uint8_t>> _q;
    
    void routineSubroutine(const std::vector<uint8_t>& a)
    {
      // ESP_LOGV(TAG, "Entering RoutineSubroutine, size = %d", a.size()));
      _q.push(a);
    }

    void C1001Component::setup()
    {


      ESP_LOGD(TAG, "Calling states in setup()");
      this->getWorkMode(); // initialize mode state
      this->getLEDLightState(eFALLLed); // this is not working
      this->getFirmwareVersion();
    }

    void C1001Component::update()
    {
      if (!_q.empty())
      {
        std::vector<uint8_t> a = _q.front();
        _q.pop();
        // ESP_LOGV(TAG, "POP: Size of a %d", a.size());

        static uint8_t buffer[SERIAL_BUFFER_LEN] = {0};

        for (int ii = 0; ii < a.size(); ii++)
          {
            // ESP_LOGV(TAG, "Popped char 0x%02X", a[ii]);
            buffer[ii] = a[ii];
          }
          process_message(buffer);
          ESP_LOGV(TAG, "Queue size %d", _q.size());
      }
      // ESP_LOGD(TAG, "Calling update()");
      // uint8_t state = getWorkMode();
      // publish_state(state);
      // if (this->modetextsensor->get_state() == "fall") {
      //   uint16_t state = this->smHumanData(eHumanPresence);
      //   publish_state(state);
      // }
    }

    void C1001Component::loop()
    {

      static uint8_t buffer[SERIAL_BUFFER_LEN] = {0};
      static uint8_t index = 0;
      static uint8_t loopwait = 0;

      // ESP_LOGV(TAG, "Entering C1001::loop()");

      while (this->available())
      {

        // if(this->read() == 0x53){           //Receive header frame 1
        //   if(this->read() == 0x59){         //Receive header frame 2
        //     dataLen = stream->readBytesUntil(MESSAGE_END2, Msg, 20);
        //     if (dataLen > 0 && dataLen < 20){
        //       Msg[dataLen] = 0x43;
        //       this->newData = true;
        //     }
        //   }
        // }

        buffer[index] = this->read();
        index++;
        index %= SERIAL_BUFFER_LEN;
        loopwait = 0;
        if (buffer[index - 1] == 0x43)
          break;
      }
      if (index > 0)
      {
        loopwait++;
      }

      if (this->modetextsensor->get_state() == "fall")
      {
        struct timespec now, diff;
        clock_gettime(CLOCK_REALTIME, &now);
        timespec_diff(&now, &last_loop_time, &diff);
        // ESP_LOGD(TAG, "Fall LED time %d", diff.tv_nsec);
        if ((diff.tv_nsec) > 500000000)
        {
          last_loop_time = now;
          // bool state = this->parent_->getLEDLightState(eFALLLed);
          // this->publish_state(state);
          // ESP_LOGD(TAG, "Calling human presence with %d", last_loop_value);
          switch (last_loop_value)
          {
          case 0:
            this->smHumanData(eHumanPresence);
            break;
          case 1:
            this->smHumanData(eHumanMovement);
            break;
          case 2:
            this->smHumanData(eHumanMovingRange);
            break;
          case 3:
            this->getFallData(eFallState);
            break;
          case 4:
            this->getFallData(eFallSensitivity);
            break;
          case 5:
            this->getFallData(estaticResidencyState);
            break;
          case 6:
            this->getStaticResidencyTime();
            break;
          case 7:
            this->getUnmannedTime();
            break;
          case 8:
            this->dmGetInstallHeight();
            break;
          case 9:
            this->getFallTime();
            break;
          }
          last_loop_value = (last_loop_value + 1) % 10;
          // publish_state(state);
        }
      }

      if (loopwait > LOOP_WAIT)
      { // some time has passed without receiving another character. this should be the end of a message.
        // ESP_LOGV(TAG, "message recieved len=%d", index);
        if ((buffer[0] == 0x53) && (buffer[1] == 0x59) && (buffer[index - 2] == 0x54) && (buffer[index - 1] == 0x43))
        { // message starts with the right preamble
          uint16_t msglen = (buffer[4] << 8) | buffer[5];
          if (index != msglen + 9) // messasge has correct lengtheFallingMode
          {
            ESP_LOGV(TAG, "message incorrect length (expected: %d, received: %d); discarding", msglen + 5, index);

            // ESP_LOGV(TAG, "Begin");
            static uint8_t temp[SERIAL_BUFFER_LEN] = {0};
            for (uint8_t ii = 0; ii < index; ii++)
            {
              // ESP_LOGV(TAG, "char = 0x%02X", buffer[ii]);
              uint8_t jj = 0;

              if (buffer[ii] == 0x53) {           //Receive header frame 1
                temp[jj++] = buffer[ii];
                // ESP_LOGV(TAG, "got one = 0x%02X", temp[jj-1]);
                ii++;
                if (buffer[ii] == 0x59 ) {         //Receive header frame 2
                  temp[jj++] = buffer[ii];
                  // ESP_LOGV(TAG, "got one = 0x%02X", temp[jj-1]);
                  ii++;
                  while ((buffer[ii-2] != 0x54) || (buffer[ii-1] != 0x43))
                  {
                    temp[jj++] = buffer[ii++];
                    // ESP_LOGV(TAG, "got one = 0x%02X", temp[jj-1]);
                    if (ii > index)
                      break;
                  }
                  temp[jj] = '\0';
                  // ESP_LOGV(TAG, "Close temp, jj = %d", jj);
                  routineSubroutine({ &temp[0], &temp[jj] } ); // creates a temp uint8_t vector
                }
              }
              else
              {
                ESP_LOGV(TAG, "Bad start char = 0x%02X", buffer[ii]);
              }
              ii--;
            }
            ESP_LOGV(TAG, "Done");
          }
          else
          {
            uint8_t csum = sumData(6 + msglen, buffer);
            if (csum != buffer[msglen + 6]) // checksum ok
            {
              ESP_LOGV(TAG, "message checksum fail; discarding. csum = 0x%02X, check = 0x%02X", buffer[msglen + 4], csum);
            }
            else
            {
              process_message(buffer);
            }
         }
        }
        else
        {
          ESP_LOGV(TAG, "message received, invalid start character");
        }
        // reset message, ready for next one
        loopwait = 0;
        index = 0;
      }
    }

    void C1001Component::dump_config()
    {
      ESP_LOGCONFIG(TAG, "C1001 fall sensor");
    }

    void C1001Component::process_message(uint8_t *buffer)
    {
      uint16_t msglen = (buffer[4] << 8) | buffer[5];
      uint8_t con = buffer[2];
      uint8_t cmd = buffer[3];
      // ESP_LOGD(TAG, "con 0x%02X, cmd 0x%02X, msglen %d received", con, cmd, msglen);

      switch (con)
      {
      case 0x01:
        switch (cmd)
        {
        case 0x01:
          if (msglen == 1)
          {
            ESP_LOGD(TAG, "Heartbeat");
          }
          break;
        case 0x03:
          if (msglen == 1)
          {
            // memcpy(this->messagedata,buffer,SOLIS_S5_SERIAL_BUFFER_LEN); // copy message for processing on next update cycle
            // this->messagelength = index; // length > 0 indicates the message data has been updated / ready for parsing
            // ESP_LOGD(TAG, "Fall LED mode change %d", buffer[6]);
            bool state = ((buffer[6] != 0) ? true : false);
            if (FallLEDCallback_ != NULL)
              FallLEDCallback_(state);
            if (FallLEDStateCallback_ != NULL)
              FallLEDStateCallback_(state);
          }
          break;
        case 0x04:
          if (msglen == 1)
          {
            ESP_LOGD(TAG, "Human presence LED state %d", buffer[6]);
          }
          break;
        case 0x84:
          if (msglen == 1)
          {
            ESP_LOGD(TAG, "Fall LED mode change %d", buffer[6]);
            bool state = ((buffer[6] != 0) ? true : false);
            if (FallLEDCallback_ != NULL)
              FallLEDCallback_(state);
            if (FallLEDStateCallback_ != NULL)
              FallLEDStateCallback_(state);
          }
          break;
        default:
          ESP_LOGD(TAG, "Unknown message con=0x%02X cmd=0x%02X", con, cmd);
          break;
        }
        break;
      case 0x02:
        switch (cmd)
        {
        case 0xA4:
          {
            std::ostringstream convert;
            for (int ii = 6; ii < msglen + 6; ii++) {
                convert << (char)(buffer[ii]);
            }
            std::string key_string = convert.str();
            ESP_LOGD(TAG, "Firmware version %s", key_string.c_str());
            if (this->firmwareversionsensor != nullptr)
              this->firmwareversionsensor->publish_state(key_string);
          }
          break;
        case 0xA8:
          if (msglen == 1)
          {
            // memcpy(this->messagedata,buffer,SOLIS_S5_SERIAL_BUFFER_LEN); // copy message for processing on next update cycle
            // this->messagelength = index; // length > 0 indicates the message data has been updated / ready for parsing
            // ESP_LOGD(TAG, "Publishing work mode %d", buffer[6]);
            bool state = ((buffer[6] == 1) ? false : true);
            if (WorkModeCallback_ != NULL)
              WorkModeCallback_(state);

            if (this->modetextsensor != nullptr)
            {
              std::string text_state = ((buffer[6] == 1) ? "fall" : ((buffer[6] == 2) ? "sleep" : "unknown"));
              this->modetextsensor->publish_state(text_state);
            }
          }
          break;
        default:
          ESP_LOGD(TAG, "Unknown message con=0x%02X cmd=0x%02X", con, cmd);
          break;
        }
        break;
      case 0x05:
        switch (cmd)
        {
        case 0x01:      // initlization complete message
          if (msglen == 2)
          {
          ESP_LOGD(TAG, "Initialization complete.");
          this->setup();
          }
        }
        break;
      case 0x06:
        switch (cmd)
        {
        case 0x02:      // Install height
        case 0x82:
          if (msglen == 2)
          {
            uint32_t install_height = buffer[6] << 8 | buffer[7];
            // ESP_LOGD(TAG, "Install height %d", install_height);
            if (this->fallinstallheightsensor != nullptr)
              this->fallinstallheightsensor->publish_state(install_height);
            if (this->install_height_number_ != nullptr)
              this->install_height_number_->publish_state(install_height);
          }
          break;
        default:
          ESP_LOGD(TAG, "Unknown message con=0x%02X cmd=0x%02X", con, cmd);
          break;
        }
        break;
      case 0x07:
        switch (cmd)
        {
        case 0x07:
          ESP_LOGD(TAG, "Position out of bounds %d", buffer[6]);
          break;
        default:
          ESP_LOGD(TAG, "Unknown message con=0x%02X cmd=0x%02X", con, cmd);
          break;
        }
        break;
      case HUMAN_PRESENCE:
        switch (cmd)
        {
        case 0x01:
          if (msglen == 1)
          {
            ESP_LOGD(TAG, "Proactive presence reporting %d", buffer[6]);
          }
          break;
        case 0x02:
          if (msglen == 1)
          {
            ESP_LOGD(TAG, "Body stillness %d", buffer[6]);
          }
          break;
        case 0x03:
          if (msglen == 1)
          {
            uint8_t state = buffer[6];
            ESP_LOGD(TAG, "Body movement %d", state);
            if (BodyMovementCallback_ != NULL)
              BodyMovementCallback_(state);

            if (this->movementsensor != nullptr)
            {
              this->movementsensor->publish_state((float)state);
            }
          }
          break;
        case 0x04:
          if (msglen == 2)
          {
            uint16_t state = (buffer[6] << 8) | buffer[7];
            // ESP_LOGD(TAG, "Human distance %d", state);
            if (this->distancesensor != nullptr)
              this->distancesensor->publish_state((float)state);
          }
          break;
        case 0x05:
          if (msglen == 6)
          {
            ESP_LOGD(TAG, "Body orientation x=%hd, y=%hd, z=%hd", (short)((buffer[6] << 8) | buffer[7]),
                      (short)((buffer[8] << 8) | buffer[9]), (short)((buffer[10] << 8) | buffer[11]));
          }
          break;
        case 0x81:      // eHumanPresence
          // ESP_LOGD(TAG, "Human presence %d", buffer[6]);
          if (this->fallpresencesensor != nullptr)
            this->fallpresencesensor->publish_state((bool)buffer[6]);
          break;
        case 0x82:      // eHumanMovement
          // ESP_LOGD(TAG, "Movement speed %d", buffer[6]);
          if (this->fallmovementspeedsensor != nullptr)
          {
            std::string mstate = "Undefined";
            if (buffer[6] == 0)
              mstate = "None";
            else if (buffer[6] == 1)
              mstate = "Still";
            else if (buffer[6] == 2)
              mstate = "Active";
            this->fallmovementspeedsensor->publish_state(mstate);
          }
          break;
        case 0x83:    // eHumanMovingRange
          // ESP_LOGD(TAG, "Body movement range %d", buffer[6]);
          if (this->fallmovementrangesensor != nullptr)
            this->fallmovementrangesensor->publish_state(buffer[6]);
          break;
        case 0x84:    // eHumanDistance
          // TODO: need to implement
          ESP_LOGD(TAG, "Unknown message con=0x%02X cmd=0x%02X", con, cmd);
          break;
        case 0x12:    // Unmanned time
        case 0x92:
          if (msglen == 4)
          {
            uint32_t unmanned_time = buffer[6] << 24 | buffer[7] << 16 | buffer[8] << 8 | buffer[9];
            // ESP_LOGD(TAG, "Fall unmanned time %d", unmanned_time);
            if (this->fallunmannedtimesensor != nullptr)
              this->fallunmannedtimesensor->publish_state(unmanned_time);
            if (this->unmanned_time_number_ != nullptr)
              this->unmanned_time_number_->publish_state(unmanned_time);
          }
          break;
        default:
          ESP_LOGD(TAG, "Unknown message con=0x%02X cmd=0x%02X", con, cmd);
          break;
        }
        break;

      case FALL_DETECTION:
        switch (cmd)
        {
        case 0x0C:      // Fall time
        case 0x8C:
          if (msglen == 4)
          {
            uint32_t fall_time = buffer[6] << 24 | buffer[7] << 16 | buffer[8] << 8 | buffer[9];
            // ESP_LOGD(TAG, "Fall fall time %d, msglen %d", fall_time, msglen);
            if (this->fallfalltimesensor != nullptr)
              this->fallfalltimesensor->publish_state(fall_time);
            if (this->fall_time_number_ != nullptr)
              this->fall_time_number_->publish_state(fall_time);
          }
          break;
        case 0x81:        // eFallState
          if (msglen == 1)
          {
            // ESP_LOGD(TAG, "Fall state %d", buffer[6]);
            if (this->falleventsensor != nullptr)
              this->falleventsensor->publish_state((bool)buffer[6]);
          }
          break;
        case 0x05:        // estaticResidencyState
        case 0x85:
          if (msglen == 1)
          {
            // ESP_LOGD(TAG, "Stationary residency %d", buffer[6]);
            if (this->falldwellsensor != nullptr)
              this->falldwellsensor->publish_state((bool)buffer[6]);
          }
          break;
        case 0x0A:        // Dwell time
        case 0x8A:
          if (msglen == 4)
          {
            uint32_t dwell_time = buffer[6] << 24 | buffer[7] << 16 | buffer[8] << 8 | buffer[9];
            // ESP_LOGD(TAG, "Fall dwell time %d", dwell_time);
            if (this->falldwelltimesensor != nullptr)
              this->falldwelltimesensor->publish_state(dwell_time);
            if (this->dwell_time_number_ != nullptr)
              this->dwell_time_number_->publish_state(dwell_time);
          }
          break;
        case 0x8B:        // estaticResidencySwitch
          // TODO: need to implement
          ESP_LOGD(TAG, "Unknown message con=0x%02X cmd=0x%02X", con, cmd);
          break;
        case 0x0D:        // eFallSensitivity
        case 0x8D:
          if (msglen == 1)
          {
            // ESP_LOGD(TAG, "Fall Sensitivity %d", buffer[6]);
            uint8_t sensitivity = (uint8_t)buffer[6];
            if (this->fallsensitivitysensor != nullptr)
              this->fallsensitivitysensor->publish_state(sensitivity);
            if (this->sensitivity_select_ != nullptr)
              this->sensitivity_select_->publish_state(std::to_string(sensitivity));
          }
          break;
        case 0x91:        // eFallBreakHeight
          // TODO: need to implement
          ESP_LOGD(TAG, "Unknown message con=0x%02X cmd=0x%02X", con, cmd);
          break;
        case 0x95:        // eHeightRatioSwitch
          // TODO: need to implement
          ESP_LOGD(TAG, "Unknown message con=0x%02X cmd=0x%02X", con, cmd);
          break;
        default:
          ESP_LOGD(TAG, "Unknown message con=0x%02X cmd=0x%02X", con, cmd);
          break;
        }
        break;

      default:
        ESP_LOGD(TAG, "Unknown message con=0x%02X cmd=0x%02X", con, cmd);
        break;
      }
    }

    /*--------------*/

    uint16_t C1001Component::getFallData(eDmFall dm)
    {
      uint8_t readBuf[15];
      uint8_t data = 0x0f;
      uint16_t ret = 0;
      switch (dm)
      {
      case eFallState:
        if (getData(FALL_DETECTION, 0x81, 1, &data, readBuf) == 0)
        {
          ret = readBuf[6];
        }
        break;
      case estaticResidencyState:
        if (getData(FALL_DETECTION, 0x85, 1, &data, readBuf) == 0)
        {
          ret = readBuf[6];
        }
        break;
      case estaticResidencySwitch:
        if (getData(FALL_DETECTION, 0x8B, 1, &data, readBuf) == 0)
        {
          ret = readBuf[6];
        }
        break;
      case eFallSensitivity:
        if (getData(FALL_DETECTION, 0x8D, 1, &data, readBuf) == 0)
        {
          ret = readBuf[6];
        }
        break;
      case eFallBreakHeight:
        if (getData(FALL_DETECTION, 0x91, 1, &data, readBuf) == 0)
        {
          ret = readBuf[6] << 8 | readBuf[7];
        }
        break;
      case eHeightRatioSwitch:
        if (getData(FALL_DETECTION, 0x95, 1, &data, readBuf) == 0)
        {
          ret = readBuf[6];
        }
        break;
      default:
        break;
      }
      return ret;
    }

    uint32_t C1001Component::getFirmwareVersion(void)
    {
        uint8_t readBuf[15];
        uint8_t data = 0x0f;
        uint32_t ret = 0;
        if (getData(0x02, 0xA4, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6] << 24 | readBuf[7] << 16 | readBuf[8] << 8 | readBuf[9];
        }
        return ret;
    }

    uint32_t C1001Component::getFallTime(void)
    {
        uint8_t readBuf[15];
        uint8_t data = 0x0f;
        uint32_t ret = 0;
        if (getData(FALL_DETECTION, 0x8c, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6] << 24 | readBuf[7] << 16 | readBuf[8] << 8 | readBuf[9];
        }
        return ret;
    }

    uint32_t C1001Component::getStaticResidencyTime(void)
    {
      uint8_t readBuf[15];
      uint8_t data = 0x0f;
      uint32_t ret = 0;
      if (getData(FALL_DETECTION, 0x8a, 1, &data, readBuf) == 0)
      {
        ret = readBuf[6] << 24 | readBuf[7] << 16 | readBuf[8] << 8 | readBuf[9];
      }
      return ret;
    }

    uint8_t C1001Component::getWorkMode(void)
    {
      uint8_t buf[10];
      uint8_t data = 0x0f, ret = 2;
      if (getData(0x02, 0xA8, 1, &data, buf) == 0)
      {
        ret = buf[6];
      }
      return ret;
    }

    uint16_t C1001Component::dmGetInstallHeight(void)
    {
        uint8_t readBuf[15];
        uint8_t data = 0x0f;
        uint16_t ret = 0;
        if (getData(0x06, 0x82, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6] << 8 | readBuf[7];
        }
        return ret;
    }

    bool C1001Component::getLEDLightState(eLed led)
    {
      uint8_t data = 0x0f;
      uint8_t buf[10];
      uint8_t ret = 2;
      if (led == eFALLLed)
      {
        if (getData(0x01, 0x84, 1, &data, buf) == 0)
        {
          ret = buf[6];
        }
      }
      else
      {
        if (getData(0x01, 0x83, 1, &data, buf) == 0)
        {
          ret = buf[6];
        }
      }
      return ret;
    }

    uint16_t C1001Component::smHumanData(esmHuman hm)
    {
      uint8_t data = 0x0f;
      uint8_t buf[15];
      uint16_t ret = 0;
      switch (hm)
      {
      case eHumanPresence:
        if (getData(HUMAN_PRESENCE, 0x81, 1, &data, buf) == 0)
        {
          ret = buf[6];
        }
        break;
      case eHumanMovement:
        if (getData(HUMAN_PRESENCE, 0x82, 1, &data, buf) == 0)
        {
          ret = buf[6];
        }
        break;
      case eHumanMovingRange:
        if (getData(HUMAN_PRESENCE, 0x83, 1, &data, buf) == 0)
        {
          ret = buf[6];
        }
        break;
      case eHumanDistance:
        if (getData(HUMAN_PRESENCE, 0x84, 1, &data, buf) == 0)
        {
          ret = buf[6] << 8 | buf[7];
        }
        break;
      default:
        break;
      }
      return ret;
    }

    uint32_t C1001Component::getUnmannedTime(void)
    {
        uint8_t readBuf[15];
        uint8_t data = 0x0f;
        uint32_t ret = 0;
        if (getData(HUMAN_PRESENCE, 0x92, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6] << 24 | readBuf[7] << 16 | readBuf[8] << 8 | readBuf[9];
        }
        return ret;
    }

    uint8_t C1001Component::dmFallConfig(eDmFallConfig con, uint32_t data)
    {
      uint8_t readBuf[15];
      uint8_t sendBuf[4];
      uint8_t ret = 1;
      switch (con)
      {
      case eFallBreakHeightC:
        sendBuf[0] = (data & 0xffff) >> 8;
        sendBuf[1] = data & 0xff;
        if (getData(FALL_DETECTION, 0x11, 2, sendBuf, readBuf) == 0)
        {
          ret = 0;
        }
        break;
      case eHeightRatioSwitchC:
        sendBuf[0] = data & 0xff;
        if (getData(FALL_DETECTION, 0x15, 1, sendBuf, readBuf) == 0)
        {
          ret = 0;
        }
        break;
      case eReportFreqC:
        sendBuf[0] = data >> 24 & 0xff;
        sendBuf[1] = data >> 16 & 0xff;
        sendBuf[2] = data >> 8 & 0xff;
        sendBuf[3] = data & 0xff;
        if (getData(FALL_DETECTION, 0x13, 4, sendBuf, readBuf) == 0)
        {
          ret = 0;
        }
        break;
      case eReportSwitchC:
        sendBuf[0] = data & 0xff;
        if (getData(FALL_DETECTION, 0x14, 1, sendBuf, readBuf) == 0)
        {
          ret = 0;
        }
        break;
      case eAltTimeC:
        sendBuf[0] = data >> 24 & 0xff;
        sendBuf[1] = data >> 16 & 0xff;
        sendBuf[2] = data >> 8 & 0xff;
        sendBuf[3] = data & 0xff;
        if (getData(FALL_DETECTION, 0x0f, 4, sendBuf, readBuf) == 0)
        {
          ret = 0;
        }
        break;
      case eFallSensitivityC:
        sendBuf[0] = data & 0xff;
        if (getData(FALL_DETECTION, 0x0D, 1, sendBuf, readBuf) == 0)
        {
          ret = 0;
        }
        break;
      case eResidenceSwitchC:
        sendBuf[0] = data & 0xff;
        if (getData(FALL_DETECTION, 0x0b, 1, sendBuf, readBuf) == 0)
        {
          ret = 0;
        }
      case eResidenceTime:
        sendBuf[0] = data >> 24 & 0xff;
        sendBuf[1] = data >> 16 & 0xff;
        sendBuf[2] = data >> 8 & 0xff;
        sendBuf[3] = data & 0xff;
        if (getData(FALL_DETECTION, 0x0A, 4, sendBuf, readBuf) == 0)
        {
          ret = 0;
        }
      default:
        break;
      }

      // sensorReset();

      return ret;
    }

    void C1001Component::dmUnmannedTime(uint32_t Time)
    {
        uint8_t buf[4];
        uint8_t readBuf[15];
        buf[0] = Time >> 24 & 0xff;
        buf[1] = Time >> 16 & 0xff;
        buf[2] = Time >> 8 & 0xff;
        buf[3] = Time & 0xff;
        getData(HUMAN_PRESENCE, 0x12, 4, buf, readBuf);
    }

    void C1001Component::dmFallTime(uint32_t Time)
    {
        uint8_t buf[4];
        uint8_t readBuf[15];
        buf[0] = Time >> 24 & 0xff;
        buf[1] = Time >> 16 & 0xff;
        buf[2] = Time >> 8 & 0xff;
        buf[3] = Time & 0xff;
        getData(FALL_DETECTION, 0x0C, 4, buf, readBuf);
    }

    void C1001Component::dmInstallHeight(uint16_t he)
    {
        uint8_t buf[2];
        uint8_t readBuf[15];
        buf[0] = he >> 8 & 0xff;
        buf[1] = he & 0xff;
        getData(0x06, 0x02, 2, buf, readBuf);
    }

    uint8_t C1001Component::configWorkMode(eWorkMode mode)
    {
      uint8_t data = 0x0f;
      uint8_t buf[10];
      uint8_t cmdBuf[10] = {0x53, 0x59, 0x02, 0x08, 0x00, 0x01, 0x00, 0x00, 0x54, 0x43};

      if (getData(0x02, 0xA8, 1, &data, buf) == 0) // Query the current working mode
      {
        ESP_LOGD(TAG, "Work mode received %d", buf[6]);
        if (buf[6] == mode) // Determine whether mode switching is necessary
        {
          return 0;
        }
        else // Mode switch required
        {
          ESP_LOGD(TAG, "Writing mode %d", mode);
          cmdBuf[6] = mode;
          cmdBuf[7] = sumData(7, cmdBuf);
          this->write_array(cmdBuf, 10);

          // delay(10000); // Waiting for mode switch to start
          delay(5000); // Waiting for mode switch to start

          if (getData(0x02, 0xA8, 1, &data, buf) == 0) // Query current working mode
          {
            if (buf[6] == mode) // Verify if the mode switch was successful
            {
              return 0;
            }
            else
            {
              return 1;
            }
          }
          return 1;
        }
      }
      return 1;
    }

    uint8_t C1001Component::configLEDLight(eLed led, uint8_t sta)
    {
      uint8_t data = sta;
      uint8_t buf[10];
      if (led == eFALLLed)
      {
        if (getData(0x01, 0x03, 1, &data, buf) == 0)
        {
          return 0;
        }
      }
      else
      {
        if (getData(0x01, 0x04, 1, &data, buf) == 0)
        {
          return 0;
        }
      }
      return 1;
    }

    uint8_t C1001Component::sensorRestart(void)
    {
      uint8_t data = 0x0f;
      uint8_t buf[10];
      if (getData(0x01, 0x02, 1, &data, buf) == 0)
      {
        // delay(10000);
        return 0;
      }
      return 1;
    }

    uint8_t C1001Component::getData(uint8_t con, uint8_t cmd, uint16_t len, uint8_t *senData, uint8_t *retData)
    {
      uint32_t timeStart = millis();
      uint32_t timeStart1 = 0;
      uint8_t data = 0;
      // unit8_t state = CMD_WHITE;
      uint16_t _len = 0;
      uint8_t count = 0;

      uint8_t cmdBuf[20];
      cmdBuf[0] = 0x53;
      cmdBuf[1] = 0x59;
      cmdBuf[2] = con;
      cmdBuf[3] = cmd;
      cmdBuf[4] = (len >> 8) & 0xff;
      cmdBuf[5] = len & 0xff;
      memcpy(&cmdBuf[6], senData, len);
      cmdBuf[6 + len] = sumData(6 + len, cmdBuf);
      cmdBuf[7 + len] = 0x54;
      cmdBuf[8 + len] = 0x43;

      while (true)
      {
        if ((millis() - timeStart1) > 500)
        {
          while (this->available() > 0)
          {
            int b = this->read();
          }
          this->write_array(cmdBuf, 9 + len);
          timeStart1 = millis();
          break;
        }

        // if (this->available() > 0)
        // {
        //   this->read_byte(&data);
        //   // DBG(data);
        //   // timeStart1 = millis();
        // }

        // // Update timeout check
        // if ((millis() - timeStart) > TIME_OUT)
        // {
        //   DBG("Time out");
        //   return 2;
        // }

        // switch (state)
        // {
        // case CMD_WHITE:
        //   if (data == 0x53)
        //   {
        //     retData[0] = data;
        //     state = CMD_HEAD;
        //     count = 0; // Reset count
        //   }
        //   break;
        // case CMD_HEAD:
        //   if (data == 0x59)
        //   {
        //     state = CMD_CONFIG;
        //     retData[1] = data;
        //   }
        //   else
        //   {
        //     state = CMD_WHITE;
        //   }
        //   break;
        // case CMD_CONFIG:
        //   if (data == con)
        //   {
        //     state = CMD_CMD;
        //     retData[2] = data;
        //   }
        //   else
        //   {
        //     state = CMD_WHITE;
        //   }
        //   break;
        // case CMD_CMD:
        //   if (data == cmd)
        //   {
        //     state = CMD_LEN_H;
        //     retData[3] = data;
        //   }
        //   else
        //   {
        //     state = CMD_WHITE;
        //   }
        //   break;
        // case CMD_LEN_H:
        //   retData[4] = data;
        //   _len = data << 8;
        //   state = CMD_LEN_L;
        //   break;
        // case CMD_LEN_L:
        //   retData[5] = data;
        //   _len |= data;
        //   state = CMD_DATA;
        //   // DBG(_len);
        //   break;
        // case CMD_DATA:
        //   if (count < _len)
        //   {
        //     retData[6 + count] = data;
        //     count++;
        //   }
        //   else
        //   {
        //     if (data == sumData(6 + count, retData))
        //     {
        //       retData[6 + _len] = data;
        //       state = CMD_END_H;
        //     }
        //     else
        //     {
        //       state = CMD_WHITE;
        //     }
        //   }
        //   break;
        // case CMD_END_H:
        //   retData[7 + _len] = data;
        //   state = CMD_END_L;
        //   break;
        // case CMD_END_L:
        //   retData[8 + _len] = data;
        //   delay(50);
        //   return 0;
        // default:
        //   break;
        // }
        // delay(50);
      }
      // delay(50);
      return 0;
    }

    uint8_t C1001Component::sumData(uint8_t len, uint8_t *buf)
    {
      uint16_t data = 0;
      uint8_t *_buf = buf;
      for (uint8_t i = 0; i < len; i++)
      {
        data += _buf[i];
      }
      return data & 0xff;
    }

  } // namespace c1001
} // namespace esphome
