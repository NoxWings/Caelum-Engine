#ifndef OCEANLISTENER_H
#define OCEANLISTENER_H

#include "EnginePrerequisites.h"
#include "render/oceansimulator.h"
#include "render/realisticsky.h"
#include <Hydrax/Hydrax.h>
#include <SKYX/SkyX.h>

namespace Caelum {

class OceanListener :
        public Hydrax::RttManager::RttListener {
  public:
    OceanListener(OceanSimulator *os) {mHydrax = os->getHydrax(); mSkyX = 0;}
    OceanListener(OceanSimulator *os, RealisticSky *sky) {mHydrax = os->getHydrax(); mSkyX = sky->getSkyX();}
    virtual ~OceanListener() {}

    void preRenderTargetUpdate(const Hydrax::RttManager::RttType &Rtt);
    void postRenderTargetUpdate(const Hydrax::RttManager::RttType &Rtt);

  private:
    Hydrax::Hydrax *mHydrax;
    SkyX::SkyX *mSkyX;

};

} // end of namespace Caelum

#endif // OCEANLISTENER_H
