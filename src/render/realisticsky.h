#ifndef RENDER_REALISTICSKY_H
#define RENDER_REALISTICSKY_H

#include "EnginePrerequisites.h"

#include "game/fixedcomponent.h"
#include "math/vector3.h"

namespace SkyX {
class SkyX;
class BasicController;
}

namespace Caelum {

class RenderLayer;
class Camera;

// ********************************
// Vector2 and Vector 4 Workaround
// ********************************

class Vector2 {
  public:
    Vector2() : x(0), y(0) {}
    Vector2(Real fx, Real fy) : x(fx), y(fy) {}
    Vector2(const Vector2& v) : x(v.x), y(v.y) {}
    Real x, y;
};

class Vector4 {
  public:
    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(Real fx, Real fy, Real fz, Real fw) : x(fx), y(fy), z(fz), w(fw) {}
    Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
    Real x, y, z, w;
};

struct AtmosphereOptions {
    /** Default constructor */
    AtmosphereOptions()
        : InnerRadius(9.77501f), OuterRadius(10.2963f), HeightPosition(0.01f)
        , RayleighMultiplier(0.0022f), MieMultiplier(0.000675f), SunIntensity(30)
        , WaveLength(Vector3(0.57f, 0.54f, 0.44f)), G(-0.991f), Exposure(2.0f)
        , NumberOfSamples(4) {}

    /** Constructor
        @param _InnerRadius Inner atmosphere radius
        @param _OuterRadius Outer atmosphere radius
        @param _HeightPosition Height position, in [0, 1] range, 0=InnerRadius, 1=OuterRadius
        @param _RayleighMultiplier Rayleigh multiplier
        @param _MieMultiplier Mie multiplier
        @param _SunIntensity Sun intensity
        @param _WaveLength Wave length for RGB channels
        @param _G Phase function
        @param _Exposure Exposure
        @param _NumerOfSamples Number of samples */
    AtmosphereOptions(const Real& _InnerRadius, const Real& _OuterRadius, const Real& _HeightPosition,
            const Real& _RayleighMultiplier, const Real& _MieMultiplier,
            const Real& _SunIntensity, const Vector3& _WaveLength,
            const Real& _G, const Real& _Exposure, const int&  _NumerOfSamples)
        : InnerRadius(_InnerRadius), OuterRadius(_OuterRadius), HeightPosition(_HeightPosition)
        , RayleighMultiplier(_RayleighMultiplier), MieMultiplier(_MieMultiplier)
        , SunIntensity(_SunIntensity), WaveLength(_WaveLength), G(_G)
        , Exposure(_Exposure), NumberOfSamples(_NumerOfSamples) {}

    /** Constructor
        @remarks Copy Constructor */
    AtmosphereOptions(const AtmosphereOptions &opts)
        : InnerRadius(opts.InnerRadius), OuterRadius(opts.OuterRadius), HeightPosition(opts.HeightPosition)
        , RayleighMultiplier(opts.RayleighMultiplier), MieMultiplier(opts.MieMultiplier)
        , SunIntensity(opts.SunIntensity), WaveLength(opts.WaveLength), G(opts.G)
        , Exposure(opts.Exposure), NumberOfSamples(opts.NumberOfSamples) {}

    /// Inner atmosphere radius
    Real InnerRadius;
    /// Outer atmosphere radius
    Real OuterRadius;
    /// Height position, in [0, 1] range, 0=InnerRadius, 1=OuterRadius
    Real HeightPosition;
    /// Rayleigh multiplier
    Real RayleighMultiplier;
    /// Mie multiplier
    Real MieMultiplier;
    /// Sun intensity
    Real SunIntensity;
    /// WaveLength for RGB channels
    Vector3 WaveLength;
    /// Phase function
    Real G;
    /// Exposure coeficient
    Real Exposure;
    /// Number of samples
    int NumberOfSamples;
};

struct SkySettings {
    /** Constructor
        @remarks Skydome settings */
    SkySettings(const Vector3 t, const Real& tm, const Real& mp,
                const AtmosphereOptions& atmOpt, const bool& lc)
        : time(t), timeMultiplier(tm), moonPhase(mp), atmosphereOpt(atmOpt),
          layeredClouds(lc), volumetricClouds(false), vcLightnings(false) {}

    /** Constructor
        @remarks Skydome + vol. clouds */
    SkySettings(const Vector3 t, const Real& tm, const Real& mp,
                const AtmosphereOptions& atmOpt, const bool& lc,
                const bool& vc, const Real& vcws, const bool& vcauto,
                const Radian& vcwd, const Vector3& vcac, const Vector4& vclr,
                const Vector4& vcaf, const Vector2& vcw)
        : time(t), timeMultiplier(tm), moonPhase(mp), atmosphereOpt(atmOpt),
          layeredClouds(lc), volumetricClouds(vc), vcWindSpeed(vcws), vcAutoupdate(vcauto),
          vcWindDir(vcwd), vcAmbientColor(vcac), vcLightResponse(vclr), vcAmbientFactors(vcaf),
          vcWheater(vcw), vcLightnings(false) {}

    /** Constructor
        @remarks Skydome + vol. clouds + lightning settings */
    SkySettings(const Vector3 t, const Real& tm, const Real& mp,
                const AtmosphereOptions& atmOpt, const bool& lc,
                const bool& vc, const Real& vcws, const bool& vcauto,
                const Radian& vcwd, const Vector3& vcac, const Vector4& vclr,
                const Vector4& vcaf, const Vector2& vcw, const bool& vcl,
                const Real& vclat, const Vector3& vclc, const Real& vcltm)
        : time(t), timeMultiplier(tm), moonPhase(mp), atmosphereOpt(atmOpt),
          layeredClouds(lc), volumetricClouds(vc), vcWindSpeed(vcws), vcAutoupdate(vcauto),
          vcWindDir(vcwd), vcAmbientColor(vcac), vcLightResponse(vclr), vcAmbientFactors(vcaf),
          vcWheater(vcw), vcLightnings(vcl), vcLightningsAT(vclat), vcLightningsColor(vclc),
          vcLightningsTM(vcltm) {}

    /** Constructor
        @remarks Copy Constructor */
    SkySettings(const SkySettings &set)
        : time(set.time), timeMultiplier(set.timeMultiplier), moonPhase(set.moonPhase),
          atmosphereOpt(set.atmosphereOpt), layeredClouds(set.layeredClouds),
          volumetricClouds(set.volumetricClouds), vcWindSpeed(set.vcWindSpeed),
          vcAutoupdate(set.vcAutoupdate), vcWindDir(set.vcWindDir), vcAmbientColor(set.vcAmbientColor),
          vcLightResponse(set.vcLightResponse), vcAmbientFactors(set.vcAmbientFactors),
          vcWheater(set.vcWheater), vcLightnings(set.vcLightnings), vcLightningsAT(set.vcLightningsAT),
          vcLightningsColor(set.vcLightningsColor), vcLightningsTM(set.vcLightningsTM) {}

    //-------------------------
    //    GENERAL OPTIONS
    //-------------------------
    /// Time
    Vector3 time;
    /// Time multiplier
    Real timeMultiplier;
    /// Moon phase
    Real moonPhase;
    /// Atmosphere options
    AtmosphereOptions atmosphereOpt;
    /// Layered clouds?
    bool layeredClouds;
    //-------------------------
    // VOLUMETRIC CLOUD OPTIONS
    //-------------------------
    /// Volumetric clouds?
    bool volumetricClouds;
    /// VClouds wind speed
    Real vcWindSpeed;
    /// VClouds autoupdate
    bool vcAutoupdate;
    /// VClouds wind direction
    Radian vcWindDir;
    /// VClouds ambient color
    Vector3 vcAmbientColor;
    /// VClouds light response
    Vector4 vcLightResponse;
    /// VClouds ambient factors
    Vector4 vcAmbientFactors;
    /// VClouds wheater
    Vector2 vcWheater;
    //-------------------------
    //    LIGNING OPTIONS
    //-------------------------
    /// VClouds lightnings?
    bool vcLightnings;
    /// VClouds lightnings average aparition time
    Real vcLightningsAT;
    /// VClouds lightnings color
    Vector3 vcLightningsColor;
    /// VClouds lightnings time multiplier
    Real vcLightningsTM;
};

static SkySettings mPresets[] = {
    // Desert
//    SkySettings(Vector3(8.85f, 7.5f, 20.5f),  -0.08f, 0, AtmosphereOptions(9.77501f, 10.2963f, 0.01f, 0.0022f, 0.000675f, 30, Vector3(0.57f, 0.52f, 0.44f), -0.991f, 3, 4), false, true, 300, false, Radian(270), Vector3(0.63f,0.63f,0.7f), Vector4(0.35, 0.2, 0.92, 0.1), Vector4(0.4, 0.7, 0, 0), Vector2(0.8,1)),
    SkySettings(Vector3(9.5f, 7.5f, 20.5f),  -0.08f, 0, AtmosphereOptions(9.77501f, 10.2963f, 0.01f, 0.0022f, 0.000675f, 30, Vector3(0.57f, 0.52f, 0.44f), -0.991f, 3, 4), false, true, 300, false, Radian(270), Vector3(0.63f,0.63f,0.7f), Vector4(0.35, 0.2, 0.92, 0.1), Vector4(0.4, 0.7, 0, 0), Vector2(0.2, 0.8)),
    // Clear
    SkySettings(Vector3(17.16f, 7.5f, 20.5f), 0, 0, AtmosphereOptions(9.77501f, 10.2963f, 0.01f, 0.0017f, 0.000675f, 30, Vector3(0.57f, 0.54f, 0.44f), -0.991f, 2.5f, 4), false),
    // Thunderstorm 1
    SkySettings(Vector3(12.23, 7.5f, 20.5f),  0, 0, AtmosphereOptions(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30, Vector3(0.55f, 0.54f, 0.52f), -0.991f, 1, 4), false, true, 300, false, Radian(0), Vector3(0.63f,0.63f,0.7f), Vector4(0.25, 0.4, 0.5, 0.1), Vector4(0.45, 0.3, 0.6, 0.1), Vector2(1,1), true, 0.5, Vector3(1,0.976,0.92), 2),
    // Thunderstorm 2
    SkySettings(Vector3(10.23, 7.5f, 20.5f),  0, 0, AtmosphereOptions(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30, Vector3(0.55f, 0.54f, 0.52f), -0.991f, 0.5, 4), false, true, 300, false, Radian(0), Vector3(0.63f,0.63f,0.7f), Vector4(0, 0.02, 0.34, 0.24), Vector4(0.29, 0.3, 0.6, 1), Vector2(1,1), true, 0.5, Vector3(0.95,1,1), 2),
    // Sunset
    SkySettings(Vector3(7.59f, 7.5f, 20.5f), 0, -0.8f, AtmosphereOptions(9.77501f, 10.2963f, 0.01f, 0.0072f, 0.000925f, 30, Vector3(0.71f, 0.59f, 0.53f), -0.997f, 2.5f, 1), true),
    // Night
    SkySettings(Vector3(21.5f, 7.5, 20.5), 0.03, -0.25, AtmosphereOptions(), true)
};

class RealisticSky : public FixedComponent {
  public:
    enum SKY_PRESET_ENUM {
        SKY_DESERT = 0,
        SKY_CLEAR,
        SKY_THUNDER1,
        SKY_THUNDER2,
        SKY_SUNSET,
        SKY_NIGHT,
        SKY_NUM_PRESETS
    };

    RealisticSky(const String& name,
                 RenderLayer* renderLayer);
    virtual ~RealisticSky();

    void setTimeMultiplier(Real timeMul);
    Real getTimeMultiplier();
    void setPreset(SKY_PRESET_ENUM presetIndex);
    void setSettings(const SkySettings& settings);

    void unregisterCamera(Camera *c);

    SkyX::SkyX* getSkyX() {return mSky;}
  private:
    RenderLayer* mLayer;
    SkyX::SkyX* mSky;
    SkyX::BasicController* mBasicController;
};

}

#endif  // RENDER_REALISTICSKY_H
