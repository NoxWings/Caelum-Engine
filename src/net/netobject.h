#ifndef NETOBJECT_H
#define NETOBJECT_H

#include "EnginePrerequisites.h"

#include <deque>
#include "math/vector3.h"
#include "math/quaternion.h"
#include "util/stringutils.h"
#include <OGRE/OgreString.h>


namespace Caelum {

class NetObject {
  public:
    NetObject() {mID = 0; mBuffer = "";}
    NetObject(int id) {mID = id; mBuffer = "";}
    NetObject(const String& buffer) {loadBuffer(buffer);}
    virtual ~NetObject() {}

    virtual const String& toBuffer() = 0;
    virtual void loadBuffer(const String& buffer) = 0;
    size_t calculateSize() { return strlen(mBuffer.c_str())+1;}

    const String &getBuffer() { return mBuffer;}

    String mBuffer;
    int mID;
};

class NetCommand
        : public NetObject {
  public:
    NetCommand() : NetObject() {}
    NetCommand(const String& buffer) : NetObject(buffer) {}
    virtual ~NetCommand() {mParams.clear();}

    // Object replication
    void loadBuffer(const String& buffer, unsigned int splits) {
        mParams.clear();
        mBuffer = buffer;
        Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(mBuffer,";", splits);
        mCommand = vec[0];
        int c = 0;
        for (Ogre::vector<Ogre::String>::iterator it = vec.begin();
             it != vec.end();
             ++it) {
            mParams[c++] = (*it);
        }
    }
    void loadBuffer(const String &buffer) {
        loadBuffer(buffer, 0);
    }

    const String& toBuffer() {
        mBuffer = mCommand;
        std::deque<String>::iterator it = mParams.begin(); it++;
        while (it != mParams.end()) {
            mBuffer += (*it)+";";
        }
        return mBuffer;
    }

    void clear() {
        mBuffer = mCommand = "";
        mParams.clear();
    }

    // Command + params retrieving

    const bool isCommand(const String &command) {
        return (strcmp(mCommand.c_str(), command.c_str()) == 0);
    }

    const int getNumParams() { return mParams.size();}
    const String& getCommand() { return mCommand;}
    const String& getParam(int index) {
        if (index < int(mParams.size()))
            return mParams[index];
        else
            return "";
    }
    const int getIntParam(int index) {
        return StringUtils::to_int(getParam(index));
    }
    const Real getRealParam(int index) {
        return StringUtils::to_Real(getParam(index));
    }

    // Command + params setting
    bool isParam(int index, const String& value) {
        if (mParams.size() >= index) return false;
        return (strcmp(mParams[index].c_str(), value.c_str()) == 0);
    }
    void addParam(const String& value) {
        mParams.push_back(value);
    }
    void addParam(const int& value) {
        mParams.push_back(StringUtils::to_string(value));
    }
    void addParam(const Real& value) {
        mParams.push_back(StringUtils::to_string(value));
    }

    // Command and params
    String mCommand;
    std::deque<String> mParams;
};

class NetTransform
        : public NetObject {
  public:
    NetTransform() : NetObject() {}
    NetTransform(int id, const Vector3 &pos, const Quaternion &ori) : NetObject(id) { setProperties(id, pos, ori); }
    NetTransform(const String& buffer) : NetObject(buffer) {}
    virtual ~NetTransform() {}

    void loadBuffer(const String& buffer) {
        mBuffer = buffer;
        Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(mBuffer,";");
        if (strcmp(vec[0].c_str(),"+POSORI") != 0)
        mID = StringUtils::to_int(vec[1]);
        mPos.x = StringUtils::to_Real(vec[2]);
        mPos.y = StringUtils::to_Real(vec[3]);
        mPos.z = StringUtils::to_Real(vec[4]);
        mOri.x = StringUtils::to_Real(vec[5]);
        mOri.y = StringUtils::to_Real(vec[6]);
        mOri.z = StringUtils::to_Real(vec[7]);
        mOri.w = StringUtils::to_Real(vec[8]);
    }

    const String& toBuffer() {
        mBuffer = "+POSORI;";
        mBuffer += StringUtils::to_string(mID)+";";
        mBuffer += StringUtils::to_string(mPos.x)+";";
        mBuffer += StringUtils::to_string(mPos.y)+";";
        mBuffer += StringUtils::to_string(mPos.z)+";";
        mBuffer += StringUtils::to_string(mOri.x)+";";
        mBuffer += StringUtils::to_string(mOri.y)+";";
        mBuffer += StringUtils::to_string(mOri.z)+";";
        mBuffer += StringUtils::to_string(mOri.w);
        return mBuffer;
    }

    void setProperties(int id, const Vector3 &pos, const Quaternion &ori) {
        mID = id;
        mPos = pos; mOri = ori;
    }

    Vector3 mPos;
    Quaternion mOri;
};

}

#endif // NETOBJECT_H
