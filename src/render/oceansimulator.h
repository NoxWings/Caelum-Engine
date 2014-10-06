#ifndef OCEANSIMULATOR_H
#define OCEANSIMULATOR_H

#include "EnginePrerequisites.h"

#include "game/fixedcomponent.h"
#include "math/vector3.h"

namespace Hydrax {
class Hydrax;
}

namespace SkyX {
class SkyX;
class ColorGradient;
}

namespace Caelum {

class RenderLayer;
class RealisticSky;
class Terrain;

class OceanSimulator :
        public FixedComponent {
  public:
    OceanSimulator(const String &name, RenderLayer *layer, RealisticSky *sky = NULL);
    virtual ~OceanSimulator();

    void prepareGradients();

    void create(const String &configFileName, RealisticSky *sky);
    void setPosition(const Vector3 &pos);
    void addTerrainAbsortionPass(Terrain *terrain, long x, long y);

    void update(Real deltaTime);

    Hydrax::Hydrax* getHydrax() {return mOcean;}
  private:
    Hydrax::Hydrax *mOcean;
    SkyX::SkyX *mSky;
    SkyX::ColorGradient *mWaterGradient, *mSunGradient, *mAmbientGradient;
    RenderLayer *mLayer;
};

}
#endif // OCEANSIMULATOR_H
