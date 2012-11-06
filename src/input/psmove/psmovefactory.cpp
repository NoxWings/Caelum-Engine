/*  Copyright (c) 20012-2013 David G. Miguel <noxwings@gmail.com>
 *  All rights reserved
 *
 *  Project-Caelum
 *  (name of the project could be changed in future revisions)
 *
 *  This file is part of Project-Caelum.
 */

#include "input/psmove/psmovefactory.h"
#include "input/psmove/psmovecontroller.h"

#include <OIS/OISException.h>
#include <OIS/OISObject.h>
#include <psmoveapi/psmove.h>

#include <algorithm>


using namespace Caelum;

PSMoveFactory::PSMoveFactory()
    : mVendorName("PSMove"),
      mType(OIS::OISJoyStick) {

    // Discover how many PSMove there are
    int count = getNumPSMoves();

    // Store free PSMoves HID handlers
    for (int i = 0; i < count; i++)
        mFreePSMoves.push_back(i);
}

PSMoveFactory::~PSMoveFactory() {
    // Clear free psmoves list
    mFreePSMoves.clear();
    // Disconnect active PSMoves
    std::vector<PSMoveController*>::iterator it;
    for (it = mInUsePSMoves.begin();
         it != mInUsePSMoves.end();
         it++) {
        delete (*it);
    }
}

OIS::DeviceList PSMoveFactory::freeDeviceList() {
    OIS::DeviceList list;
    std::deque<int>::iterator it;
    for (it = mFreePSMoves.begin(); it != mFreePSMoves.end(); it++) {
        list.insert(std::make_pair(mType, mVendorName));
    }
    return list;
}

int PSMoveFactory::freeDevices(OIS::Type iType) {
    if (iType == mType)
        return (int)mFreePSMoves.size();
    else
        return 0;
}

int PSMoveFactory::totalDevices(OIS::Type iType) {
    if (iType == mType)
        return getNumPSMoves();
    else
        return 0;
}

bool PSMoveFactory::vendorExist(OIS::Type iType,
                               const std::string &vendor) {
    return (mType == iType && mVendorName == vendor);
}

OIS::Object* PSMoveFactory::createObject(OIS::InputManager *creator,
                                         OIS::Type iType,
                                         bool bufferMode,
                                         const std::string &vendor) {
    if (mFreePSMoves.size() > 0
            && (vendor == "" || vendor == mVendorName)) {
        // Remove a psmove from the free list
        int id = mFreePSMoves.front();
        mFreePSMoves.pop_front();

        // Create the psmove controller
        PSMoveController* move = new PSMoveController(); // TODO complete call

        // Add the new PSMove for polling
        mInUsePSMoves.push_back(move);

        return move;
    } else
        OIS_EXCEPT(OIS::E_InputDeviceNonExistant, "No device found which, matches description!");


}

void PSMoveFactory::destroyObject(OIS::Object *obj) {
    if (obj == NULL)
        return;

    // Locate PSMove in the list
    std::vector<PSMoveController*>::iterator it;
    it = std::find(mInUsePSMoves.begin(), mInUsePSMoves.end(), obj);

    // ERROR HANDLE: (If device is not found)
    if (it == mInUsePSMoves.end())
        OIS_EXCEPT(OIS::E_General, "Device not found in PSMove collection!");

    // Recover ID from unused move
    int moveID = obj->getID();

    // Erase psmove from the list
    mInUsePSMoves.erase(it);
    delete obj;

    // Return unused controller to pool
    registerFreeDevice(moveID);
}

int PSMoveFactory::getNumPSMoves() {
    int count = psmove_count_connected();
    return (count > 4)? 4:count;
}

void PSMoveFactory::registerFreeDevice(const int ID) {
    // Check is this id is not already registered
    std::deque<int>::iterator it;
    it = std::find(mFreePSMoves.begin(), mFreePSMoves.end(), ID);

    // ERROR HANDLE: (If HID is already registered as a Free Device)
    if (it != mFreePSMoves.end())
        OIS_EXCEPT(OIS::E_General, "Unable to register free device: duplicated HID");

    // Register HID device as free
    mFreePSMoves.push_front(ID);

    // Optional: sort the list in order to connect psmoves
    // allways in the same order.
    std::sort(mFreePSMoves.begin(), mFreePSMoves.end());
}
