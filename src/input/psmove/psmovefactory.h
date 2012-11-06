/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#ifndef SRC_INPUT_PSMOVEFACTORY_H_
#define SRC_INPUT_PSMOVEFACTORY_H_

#include "EnginePrerequisites.h"
#include "input/psmove/psmovecontroller.h"

#include <OIS/OISPrereqs.h>
#include <OIS/OISFactoryCreator.h>
#include <deque>

namespace Caelum {

#define PSMove_MAX_CONTROLLERS 4

// Fordward declare local classes
class PSMoveController;

class PSMoveFactory : public OIS::FactoryCreator {
  public:
    PSMoveFactory();
    ~PSMoveFactory();

    /// FACTORYCREATOR INTERFACE

    // Devices info
    OIS::DeviceList freeDeviceList();
    int freeDevices(OIS::Type iType);
    int totalDevices(OIS::Type iType);

    // Vendo
    bool vendorExist(OIS::Type iType, const std::string &vendor = "");

    // Object creation
    OIS::Object* createObject(OIS::InputManager *creator,
                              OIS::Type iType,
                              bool bufferMode,
                              const std::string &vendor);
    void destroyObject(OIS::Object *obj);

  private:
    int getNumPSMoves();
    void registerFreeDevice(const int ID);

  protected:
    /// String name of this vendor
    std::string mVendorName;
    /// Type of the devices created
    OIS::Type mType;
    /// Queue of open psmove controllers (int represents index in HID device)
    std::deque<int> mFreePSMoves;

    /// List of created (active) PSMoves
    std::vector<PSMoveController*> mInUsePSMoves;
};

}  // namespace NAGE

#endif  // SRC_INPUT_PSMOVEFACTORY_H_
