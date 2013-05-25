#ifndef UNITCONVERSOR_H
#define UNITCONVERSOR_H

#include "EnginePrerequisites.h"

#include "math/matrix3.h"
#include <OGRE/OgreMatrix3.h>
#include "math/quaternion.h"
#include <OGRE/OgreQuaternion.h>
#include "math/vector3.h"
#include <OGRE/OgreVector3.h>
#include <bullet/btBulletCollisionCommon.h>

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

    static inline Ogre::Vector3 toOgre(const Vector3 &v) {
        return Ogre::Vector3(v.x, v.y, v.z);
    }

    static inline Ogre::Quaternion toOgre(const Quaternion &q) {
        return Ogre::Quaternion(q.w, q.x, q.y, q.z);
    }

    static inline btVector3 toBullet(const Vector3 &v) {
        return btVector3(v.x, v.y, v.z);
    }

    static inline btQuaternion toBullet(const Quaternion &q) {
        return btQuaternion(q.x, q.y, q.z, q.w);
    }

  private:
    UnitConversor() {}
};

}

#endif // UNITCONVERSOR_H
