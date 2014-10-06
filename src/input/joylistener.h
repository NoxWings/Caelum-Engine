#ifndef SRC_INPUT_JOYLISTENER_H_
#define SRC_INPUT_JOYLISTENER_H_

#include "EnginePrerequisites.h"

#include "input/inputcommon.h"
#include <vector>

namespace Caelum {

/** @remarks default sensitivity for vector3 component of joystick */
#define OIS_JOYSTICK_VECTOR3_DEFAULT 2.28f

//! A 3D Vector component (perhaps an orientation, as in the WiiMote)
class InputVector3 : public InputComponent {
  public:
    InputVector3() {}
    InputVector3(float _x, float _y, float _z) : InputComponent(INP_Vector3), x(_x), y(_y), z(_z) {}

    void clear() { x = y = z = 0.0f;}

    //! X component of vector
    float x;
    //! Y component of vector
    float y;
    //! Z component of vector
    float z;
};

//! POV / HAT Joystick component
class Pov : public InputComponent {
  public:
    Pov() : InputComponent(INP_POV), direction(0) {}

    static const int Centered  = 0x00000000;
    static const int North     = 0x00000001;
    static const int South     = 0x00000010;
    static const int East      = 0x00000100;
    static const int West      = 0x00001000;
    static const int NorthEast = 0x00000101;
    static const int SouthEast = 0x00000110;
    static const int NorthWest = 0x00001001;
    static const int SouthWest = 0x00001010;

    int direction;
};

//! A sliding axis - only used in Win32 Right Now
class Slider : public InputComponent {
  public:
    Slider() : InputComponent(INP_Slider), abX(0), abY(0) {}
    //! true if pushed, false otherwise
    int abX, abY;
};

/**
  * Represents the state of the joystick
  * All members are valid for both buffered and non buffered mode
  * Sticks with zero values are not present on the device
  */
class JoyStickState {
  public:
    //! Constructor
    JoyStickState() { clear(); }
    JoyStickState(int numButtons, int numAxes, int numVector3) {
        mButtons.resize(numButtons);
        mAxes.resize(numAxes);
        mVectors.resize(numVector3);
        clear();
    }

    //! internal method to reset all variables to initial values
    void clear() {
        // Clear buttons
        for( std::vector<bool>::iterator i = mButtons.begin(), e = mButtons.end(); i != e; ++i ) {
            (*i) = false;
        }
        // Clear Axis
        for( std::vector<Axis>::iterator i = mAxes.begin(), e = mAxes.end(); i != e; ++i ) {
            i->absOnly = true; //Currently, joysticks only report Absolute values
            i->clear();
        }
        // Clear Vectors
        for( std::vector<InputVector3>::iterator i = mVectors.begin(), e = mVectors.end(); i != e; ++i ) {
            i->clear();
        }
        // Clear POV and Sliders
        for( int i = 0; i < 4; ++i ) {
            mPOV[i].direction = Pov::Centered;
            mSliders[i].abX = mSliders[i].abY = 0;
        }
    }

    //! Represents all the buttons (uses a bitset)
    std::vector<bool> mButtons;
    //! Represents all the single axes on the device
    std::vector<Axis> mAxes;
    //! Represents the value of a POV. Maximum of 4
    Pov mPOV[4];
    //! Represent the max sliders
    Slider mSliders[4];
    //! Represents all Vector type controls the device exports
    std::vector<InputVector3> mVectors;
};

// /** Specialised for joystick events */
//class JoyStickEvent : public EventArg {
//  public:
//    JoyStickEvent( Object* obj, const JoyStickState &st ) : EventArg(obj), mState(st) {}
//    virtual ~JoyStickEvent() {}
//    const JoyStickState &mState;
//};

/** Joystick events */
class JoyStickEvent {
  public:
    JoyStickEvent(int joyStickID, int numButtons, int numAxes, int numVector3)
        : mJoyStickID(joyStickID), mState(numButtons, numAxes, numVector3) {}
    virtual ~JoyStickEvent() {}

    int mJoyStickID;
    JoyStickState mState;
};

/**
  * To recieve buffered joystick input, derive a class from this, and implement the
  * methods here. Then set the call back to your JoyStick instance with JoyStick::setEventCallback
  * Each JoyStick instance can use the same callback class, as a devID number will be provided
  * to differentiate between connected joysticks. Of course, each can have a seperate
  * callback instead.
  */
class JoyStickListener {
  public:
    virtual ~JoyStickListener() {}
    /** @remarks Joystick button down event */
    virtual bool buttonPressed(const JoyStickEvent &arg, int button) = 0;

    /** @remarks Joystick button up event */
    virtual bool buttonReleased(const JoyStickEvent &arg, int button) = 0;

    /** @remarks Joystick axis moved event */
    virtual bool axisMoved(const JoyStickEvent &arg, int axis) = 0;

    //-- Not so common control events, so are not required --//
    //! Joystick Event, and sliderID
    virtual bool sliderMoved(const JoyStickEvent &arg, int index) {return true;}

    //! Joystick Event, and povID
    virtual bool povMoved(const JoyStickEvent &arg, int index) {return true;}

    //! Joystick Event, and Vector3ID
    virtual bool vector3Moved(const JoyStickEvent &arg, int index) {return true;}
};

} // namespace Caelum

#endif // SRC_INPUT_JOYLISTENER_H_
