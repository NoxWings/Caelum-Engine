/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_INPUT_PSMOVECONTROLLER_H_
#define SRC_INPUT_PSMOVECONTROLLER_H_

#include "EnginePrerequisites.h"

#include <OIS/OISJoyStick.h>
#include <psmoveapi/psmove.h>

namespace Caelum {

// Fordward declaration
class PSMoveFactory;

//Number of ring buffer events. should be nice sized (the structure is not very big)
//Will be rounded up to power of two automatically
#define OIS_PSMOVE_EVENT_BUFFER 32

class PSMoveController : public OIS::JoyStick {
  public:
    PSMoveController(OIS::InputManager* creator,  int id, bool buffered,
                     PSMoveFactory* local_creator);
    ~PSMoveController();

    /// Joystick Interface
    void setBuffered(bool buffered);
    void capture();

    OIS::Interface* queryInterface(OIS::Interface::IType type);
    void _initialize();


    bool connect();
    void disconnect();

    bool isConnected();

    const char* getSerial();

  private:
    PSMove *mMove;
    PSMove_Connection_Type mCtype;
};
}  // namespace Caelum

#endif  // SRC_INPUT_PSMOVECONTROLLER_H_



            void _threadUpdate();

      protected:
            void _doButtonCheck(bool new_state, int ois_button, unsigned int &pushed, unsigned int &released);
            bool _doPOVCheck(const cWiiMote::tButtonStatus &bState, unsigned int &newPosition);

            //! The creator who created us
            WiiMoteFactoryCreator *mWiiCreator;

            //! Actual WiiMote HID device
            cWiiMote mWiiMote;

            //! Used to signal thread that remote is ready
            volatile bool mtInitialized;

            //! Ringbuffer is used to store events from thread and be read from capture
            WiiMoteRingBuffer mRingBuffer;

            //Following variables are used entirely within threaded context
            int mtLastButtonStates;
            unsigned int mtLastPOVState;
            float mtLastX, mtLastY, mtLastZ;
            float mtLastNunChuckX, mtLastNunChuckY, mtLastNunChuckZ;
            int mLastNunChuckXAxis, mLastNunChuckYAxis;

            //Small workaround for slow calibration of wiimote data
            int _mWiiMoteMotionDelay;

            //Simple rumble force
            WiiMoteForceFeedback *mRumble;
      };
}
#endif //OIS_WiiMote_H
#endif
