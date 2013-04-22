#include "math/math.h"

//#include "OgreVector2.h"
#include "math/vector3.h"
#include <cstdlib>


__inline float asm_rand() {
#if  OGRE_COMPILER == OGRE_COMPILER_MSVC &&  OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_32
  #if 0
    #if OGRE_COMP_VER >= 1300
   static unsigned __int64 q = time( NULL );
   _asm {
      movq mm0, q
      // do the magic MMX thing
      pshufw mm1, mm0, 0x1E
      paddd mm0, mm1
      // move to integer memory location and free MMX
      movq q, mm0
      emms
   }
   return float( q );
    #endif
  #else
    // VC6 does not support pshufw
    return float( rand() );
  #endif
#else
    // GCC etc
   return float( rand() );
#endif
}

__inline float asm_rand_max() {
#if  OGRE_COMPILER == OGRE_COMPILER_MSVC &&  OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_32
  #if 0
    #if OGRE_COMP_VER >= 1300
        return (std::numeric_limits< unsigned __int64 >::max)();
        return 9223372036854775807.0f;
    #endif
  #else
    // VC6 does not support unsigned __int64
    return float( RAND_MAX );
  #endif
#else
    // GCC etc
        return float( RAND_MAX );
#endif
}

// returns 1 / a for a * a = r
// -- Use this for Vector normalisation!!!
float asm_rsq( float r ) {
    return 1. / sqrt( r );
}

namespace Caelum
{

    const Real Math::POS_INFINITY = std::numeric_limits<Real>::infinity();
    const Real Math::NEG_INFINITY = -std::numeric_limits<Real>::infinity();
    const Real Math::PI = Real( 4.0 * atan( 1.0 ) );
    const Real Math::TWO_PI = Real( 2.0 * PI );
    const Real Math::HALF_PI = Real( 0.5 * PI );
        const Real Math::fDeg2Rad = PI / Real(180.0);
        const Real Math::fRad2Deg = Real(180.0) / PI;
        const Real Math::LOG2 = log(Real(2.0));

    int Math::mTrigTableSize;
   Math::AngleUnit Math::msAngleUnit;

    Real  Math::mTrigTableFactor;
    Real *Math::mSinTable = NULL;
    Real *Math::mTanTable = NULL;

    Math::RandomValueProvider* Math::mRandProvider = NULL;

    //-----------------------------------------------------------------------
    Math::Math( unsigned int trigTableSize )
    {
        msAngleUnit = AU_DEGREE;
        mTrigTableSize = trigTableSize;
        mTrigTableFactor = mTrigTableSize / Math::TWO_PI;

        mSinTable = new Real[mTrigTableSize];
        mTanTable = new Real[mTrigTableSize];
        //mSinTable = OGRE_ALLOC_T(Real, mTrigTableSize, MEMCATEGORY_GENERAL);
        //mTanTable = OGRE_ALLOC_T(Real, mTrigTableSize, MEMCATEGORY_GENERAL);

        buildTrigTables();
    }

    //-----------------------------------------------------------------------
    Math::~Math()
    {
        delete [] mSinTable;
        delete [] mTanTable;
        //OGRE_FREE(mSinTable, MEMCATEGORY_GENERAL);
        //OGRE_FREE(mTanTable, MEMCATEGORY_GENERAL);
    }

    //-----------------------------------------------------------------------
    void Math::buildTrigTables(void)
    {
        // Build trig lookup tables
        // Could get away with building only PI sized Sin table but simpler this
        // way. Who cares, it'll ony use an extra 8k of memory anyway and I like
        // simplicity.
        Real angle;
        for (int i = 0; i < mTrigTableSize; ++i)
        {
            angle = Math::TWO_PI * i / mTrigTableSize;
            mSinTable[i] = sin(angle);
            mTanTable[i] = tan(angle);
        }
    }
        //-----------------------------------------------------------------------
        Real Math::SinTable (Real fValue)
    {
        // Convert range to index values, wrap if required
        int idx;
        if (fValue >= 0)
        {
            idx = int(fValue * mTrigTableFactor) % mTrigTableSize;
        }
        else
        {
            idx = mTrigTableSize - (int(-fValue * mTrigTableFactor) % mTrigTableSize) - 1;
        }

        return mSinTable[idx];
    }
        //-----------------------------------------------------------------------
        Real Math::TanTable (Real fValue)
    {
        // Convert range to index values, wrap if required
                int idx = int(fValue *= mTrigTableFactor) % mTrigTableSize;
                return mTanTable[idx];
    }
    //-----------------------------------------------------------------------
    int Math::ISign (int iValue)
    {
        return ( iValue > 0 ? +1 : ( iValue < 0 ? -1 : 0 ) );
    }
    //-----------------------------------------------------------------------
    Radian Math::ACos (Real fValue)
    {
        if ( -1.0 < fValue )
        {
            if ( fValue < 1.0 )
                return Radian(acos(fValue));
            else
                return Radian(0.0);
        }
        else
        {
            return Radian(PI);
        }
    }
    //-----------------------------------------------------------------------
    Radian Math::ASin (Real fValue)
    {
        if ( -1.0 < fValue )
        {
            if ( fValue < 1.0 )
                return Radian(asin(fValue));
            else
                return Radian(HALF_PI);
        }
        else
        {
            return Radian(-HALF_PI);
        }
    }
    //-----------------------------------------------------------------------
    Real Math::Sign (Real fValue)
    {
        if ( fValue > 0.0 )
            return 1.0;

        if ( fValue < 0.0 )
            return -1.0;

        return 0.0;
    }
    //-----------------------------------------------------------------------
    Real Math::InvSqrt(Real fValue)
    {
        return Real(asm_rsq(fValue));
    }
    //-----------------------------------------------------------------------
    Real Math::UnitRandom ()
    {
        if (mRandProvider)
            return mRandProvider->getRandomUnit();
        else return asm_rand() / asm_rand_max();
    }

    //-----------------------------------------------------------------------
    Real Math::RangeRandom (Real fLow, Real fHigh)
    {
        return (fHigh-fLow)*UnitRandom() + fLow;
    }

    //-----------------------------------------------------------------------
    Real Math::SymmetricRandom ()
    {
        return 2.0f * UnitRandom() - 1.0f;
    }

    //-----------------------------------------------------------------------
    void Math::SetRandomValueProvider(RandomValueProvider* provider)
    {
        mRandProvider = provider;
    }


   //-----------------------------------------------------------------------
    void Math::setAngleUnit(Math::AngleUnit unit)
   {
       msAngleUnit = unit;
   }
   //-----------------------------------------------------------------------
   Math::AngleUnit Math::getAngleUnit(void)
   {
       return msAngleUnit;
   }
    //-----------------------------------------------------------------------
    Real Math::AngleUnitsToRadians(Real angleunits)
    {
       if (msAngleUnit == AU_DEGREE)
           return angleunits * fDeg2Rad;
       else
           return angleunits;
    }

    //-----------------------------------------------------------------------
    Real Math::RadiansToAngleUnits(Real radians)
    {
       if (msAngleUnit == AU_DEGREE)
           return radians * fRad2Deg;
       else
           return radians;
    }

    //-----------------------------------------------------------------------
    Real Math::AngleUnitsToDegrees(Real angleunits)
    {
       if (msAngleUnit == AU_RADIAN)
           return angleunits * fRad2Deg;
       else
           return angleunits;
    }

    //-----------------------------------------------------------------------
    Real Math::DegreesToAngleUnits(Real degrees)
    {
       if (msAngleUnit == AU_RADIAN)
           return degrees * fDeg2Rad;
       else
           return degrees;
    }

    //-----------------------------------------------------------------------
    /*    bool Math::pointInTri2D(const Vector2& p, const Vector2& a,
                const Vector2& b, const Vector2& c)
    {
                // Winding must be consistent from all edges for point to be inside
                Vector2 v1, v2;
                Real dot[3];
                bool zeroDot[3];

                v1 = b - a;
                v2 = p - a;

                // Note we don't care about normalisation here since sign is all we need
                // It means we don't have to worry about magnitude of cross products either
                dot[0] = v1.crossProduct(v2);
                zeroDot[0] = Math::RealEqual(dot[0], 0.0f, 1e-3);


                v1 = c - b;
                v2 = p - b;

                dot[1] = v1.crossProduct(v2);
                zeroDot[1] = Math::RealEqual(dot[1], 0.0f, 1e-3);

                // Compare signs (ignore colinear / coincident points)
                if(!zeroDot[0] && !zeroDot[1]
                && Math::Sign(dot[0]) != Math::Sign(dot[1]))
                {
                        return false;
                }

                v1 = a - c;
                v2 = p - c;

                dot[2] = v1.crossProduct(v2);
                zeroDot[2] = Math::RealEqual(dot[2], 0.0f, 1e-3);
                // Compare signs (ignore colinear / coincident points)
                if((!zeroDot[0] && !zeroDot[2]
                        && Math::Sign(dot[0]) != Math::Sign(dot[2])) ||
                        (!zeroDot[1] && !zeroDot[2]
                        && Math::Sign(dot[1]) != Math::Sign(dot[2])))
                {
                        return false;
                }


                return true;
    }*/
        //-----------------------------------------------------------------------
        bool Math::pointInTri3D(const Vector3& p, const Vector3& a,
                const Vector3& b, const Vector3& c, const Vector3& normal) {
        // Winding must be consistent from all edges for point to be inside
                Vector3 v1, v2;
                Real dot[3];
                bool zeroDot[3];

        v1 = b - a;
        v2 = p - a;

                // Note we don't care about normalisation here since sign is all we need
                // It means we don't have to worry about magnitude of cross products either
        dot[0] = v1.crossProduct(v2).dotProduct(normal);
                zeroDot[0] = Math::RealEqual(dot[0], 0.0f, 1e-3);


        v1 = c - b;
        v2 = p - b;

                dot[1] = v1.crossProduct(v2).dotProduct(normal);
                zeroDot[1] = Math::RealEqual(dot[1], 0.0f, 1e-3);

                // Compare signs (ignore colinear / coincident points)
                if(!zeroDot[0] && !zeroDot[1]
                        && Math::Sign(dot[0]) != Math::Sign(dot[1]))
                {
            return false;
                }

        v1 = a - c;
        v2 = p - c;

                dot[2] = v1.crossProduct(v2).dotProduct(normal);
                zeroDot[2] = Math::RealEqual(dot[2], 0.0f, 1e-3);
                // Compare signs (ignore colinear / coincident points)
                if((!zeroDot[0] && !zeroDot[2]
                        && Math::Sign(dot[0]) != Math::Sign(dot[2])) ||
                        (!zeroDot[1] && !zeroDot[2]
                        && Math::Sign(dot[1]) != Math::Sign(dot[2])))
                {
                        return false;
                }


        return true;
        }
    //-----------------------------------------------------------------------
    bool Math::RealEqual( Real a, Real b, Real tolerance )
    {
        if (fabs(b-a) <= tolerance)
            return true;
        else
            return false;
    }
    //-----------------------------------------------------------------------
    Vector3 Math::calculateTangentSpaceVector(
        const Vector3& position1, const Vector3& position2, const Vector3& position3,
        Real u1, Real v1, Real u2, Real v2, Real u3, Real v3)
    {
            //side0 is the vector along one side of the triangle of vertices passed in,
            //and side1 is the vector along another side. Taking the cross product of these returns the normal.
            Vector3 side0 = position1 - position2;
            Vector3 side1 = position3 - position1;
            //Calculate face normal
            Vector3 normal = side1.crossProduct(side0);
            normal.normalise();
            //Now we use a formula to calculate the tangent.
            Real deltaV0 = v1 - v2;
            Real deltaV1 = v3 - v1;
            Vector3 tangent = deltaV1 * side0 - deltaV0 * side1;
            tangent.normalise();
            //Calculate binormal
            Real deltaU0 = u1 - u2;
            Real deltaU1 = u3 - u1;
            Vector3 binormal = deltaU1 * side0 - deltaU0 * side1;
            binormal.normalise();
            //Now, we take the cross product of the tangents to get a vector which
            //should point in the same direction as our normal calculated above.
            //If it points in the opposite direction (the dot product between the normals is less than zero),
            //then we need to reverse the s and t tangents.
            //This is because the triangle has been mirrored when going from tangent space to object space.
            //reverse tangents if necessary
            Vector3 tangentCross = tangent.crossProduct(binormal);
            if (tangentCross.dotProduct(normal) < 0.0f)
            {
                    tangent = -tangent;
                    binormal = -binormal;
            }

        return tangent;

    }
    //-----------------------------------------------------------------------
    Vector3 Math::calculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3)
    {
        Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
        normal.normalise();
        return normal;
    }
    //-----------------------------------------------------------------------
    Vector3 Math::calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
    {
        Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
        return normal;
    }
        //-----------------------------------------------------------------------
        Real Math::gaussianDistribution(Real x, Real offset, Real scale)
        {
                Real nom = Math::Exp(
                        -Math::Sqr(x - offset) / (2 * Math::Sqr(scale)));
                Real denom = scale * Math::Sqrt(2 * Math::PI);

                return nom / denom;

        }
        //---------------------------------------------------------------------
        /*Matrix4 Math::makeViewMatrix(const Vector3& position, const Quaternion& orientation,
                const Matrix4* reflectMatrix)
        {
                Matrix4 viewMatrix;

                // View matrix is:
                //
                //  [ Lx  Uy  Dz  Tx  ]
                //  [ Lx  Uy  Dz  Ty  ]
                //  [ Lx  Uy  Dz  Tz  ]
                //  [ 0   0   0   1   ]
                //
                // Where T = -(Transposed(Rot) * Pos)

                // This is most efficiently done using 3x3 Matrices
                Matrix3 rot;
                orientation.ToRotationMatrix(rot);

                // Make the translation relative to new axes
                Matrix3 rotT = rot.Transpose();
                Vector3 trans = -rotT * position;

                // Make final matrix
                viewMatrix = Matrix4::IDENTITY;
                viewMatrix = rotT; // fills upper 3x3
                viewMatrix[0][3] = trans.x;
                viewMatrix[1][3] = trans.y;
                viewMatrix[2][3] = trans.z;

                // Deal with reflections
                if (reflectMatrix)
                {
                        viewMatrix = viewMatrix * (*reflectMatrix);
                }

                return viewMatrix;

        }*/

} // namespace Caelum

