#ifndef UNITCONVERSOR_H
#define UNITCONVERSOR_H

#include "EnginePrerequisites.h"

#include "math/matrix3.h"
#include <OGRE/OgreMatrix3.h>
#include "math/quaternion.h"
#include <OGRE/OgreQuaternion.h>
#include "math/vector3.h"
#include <OGRE/OgreVector3.h>

namespace Caelum {

/** DO NOT EXPORT THIS CLASS, THIS IS JUST AN INTERNAL HELPER CLASS
 * @brief The UnitConversor class
 */
class UnitConversor {
  public:
    static inline void RadianToOgreRadian(const Radian &src, Ogre::Radian &dest) {
        dest = src.valueRadians();
    }
    static inline void OgreRadianToRadian(const Ogre::Radian &src, Radian &dest) {
        dest = src.valueRadians();
    }

    static inline void QuaternionToOgreQuaternion(const Quaternion &src, Ogre::Quaternion &dest) {
        dest.w = src.w;
        dest.x = src.x;
        dest.y = src.y;
        dest.z = src.z;
    }
    static inline void OgreQuaternionToQuaternion(const Ogre::Quaternion &src, Quaternion &dest) {
        dest.w = src.w;
        dest.x = src.x;
        dest.y = src.y;
        dest.z = src.z;
    }

    static inline void Vector3ToOgreVector3(const Vector3 &src, Ogre::Vector3 &dest) {
        dest.x = src.x;
        dest.y = src.y;
        dest.z = src.z;
    }
    static inline void OgreVector3ToVector3(const Ogre::Vector3 &src, Vector3 &dest) {
        dest.x = src.x;
        dest.y = src.y;
        dest.z = src.z;
    }

  private:
    UnitConversor() {}
};

}

#endif // UNITCONVERSOR_H
