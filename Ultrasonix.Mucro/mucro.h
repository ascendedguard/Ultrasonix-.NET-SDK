#pragma once

#ifdef EXPORT_MUCRO
    #define mucrolinkage __declspec(dllexport)
#elif defined IMPORT_MUCRO
    #define mucrolinkage __declspec(dllimport)
#else
    #define mucrolinkage
#endif

struct FilterParams
{
    int asrSmooth;
    int asrWeight;
    int asrContrast;
    int asrLPCut;
    int asrLineStrength;
};

////////////////////////////////////////////////////////////////////////////////
/// Mucro processes B scan ultrasound images, by running a intense filtering
/// algorithms including edge enhancement and smoothing for better border and
/// structure definition and resolution.
////////////////////////////////////////////////////////////////////////////////
class mucrolinkage mucro
{
public:
    mucro();
    virtual ~mucro();
    int apply(unsigned char* in, unsigned char* out, int w, int h, FilterParams params);
};
