#include "render/ogre/oceanlistener.h"

using namespace Caelum;


void OceanListener::preRenderTargetUpdate(const Hydrax::RttManager::RttType &Rtt) {
    bool underwater = mHydrax->_isCurrentFrameUnderwater();
    switch (Rtt) {
    case Hydrax::RttManager::RTT_REFLECTION:
    {
        mSkyX->setStarfieldEnabled(false);
    }
        break;
    case Hydrax::RttManager::RTT_REFRACTION: {}
        break;
    case Hydrax::RttManager::RTT_DEPTH:
    case Hydrax::RttManager::RTT_DEPTH_REFLECTION:
    {
        mSkyX->getMeshManager()->getEntity()->setVisible(false);
        mSkyX->getMoonManager()->getMoonBillboard()->setVisible(false);
    }
        break;
    }
}

void OceanListener::postRenderTargetUpdate(const Hydrax::RttManager::RttType &Rtt) {
    bool underwater = mHydrax->_isCurrentFrameUnderwater();
    switch (Rtt) {
    case Hydrax::RttManager::RTT_REFLECTION:
    {
        mSkyX->setStarfieldEnabled(true);
    }
        break;
    case Hydrax::RttManager::RTT_REFRACTION: {}
        break;
    case Hydrax::RttManager::RTT_DEPTH:
    case Hydrax::RttManager::RTT_DEPTH_REFLECTION:
    {
        mSkyX->getMeshManager()->getEntity()->setVisible(true);
        mSkyX->getMoonManager()->getMoonBillboard()->setVisible(true);
    }
        break;
    }
}

