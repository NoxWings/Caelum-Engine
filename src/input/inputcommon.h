#ifndef SRC_INPUT_INPUTCOMMON_H_
#define SRC_INPUT_INPUTCOMMON_H_

#include "EnginePrerequisites.h"

namespace Caelum {

enum InputType {
    INP_Unknown = 0,
    INP_Button  = 1, //ie. Key, mouse button, joy button, etc
    INP_Axis    = 2, //ie. A joystick or mouse axis
    INP_Slider  = 3, //
    INP_POV     = 4, //ie. Arrow direction keys
    INP_Vector3 = 5  //ie. PSMove or WiiMote orientation
};

static const int MIN_AXIS = -32768;  // The minimal axis value.
static const int MAX_AXIS = 32767;   // The maximum axis value.

//! Base of all device components (button, axis, etc)
class InputComponent {
  public:
    InputComponent() : mType(INP_Unknown) {}
    InputComponent(InputType type) : mType(type) {}
    //! Indicates what type of coponent this is
    InputType mType;
};

//! Axis component
class Axis : public InputComponent {
  public:
    Axis() : InputComponent(INP_Axis), abs(0), rel(0), absOnly(false) {}
    //! Used internally
    void clear() { abs = rel = 0;}

    //! Absoulte and Relative value components
    int abs, rel;
    //! Indicates if this Axis only supports Absoulte (ie JoyStick)
    bool absOnly;
};

} // namespace Caelum

#endif // SRC_INPUT_INPUTCOMMON_H_
