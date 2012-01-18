#pragma once

#ifdef EXPORT_PANDO
    #define pandolinkage __declspec(dllexport)
#elif defined IMPORT_PANDO
    #define pandolinkage __declspec(dllimport)
#else
    #define pandolinkage
#endif

////////////////////////////////////////////////////////////////////////////////
/// Pando scan converts raw ultrasound data into a readable ultrasound image.
/// The calls work best for B mode images, however M mode spectrums, color Doppler
/// data and elastography data can also be used as input for generating an
/// interpolated image. Note that the API only works on 8 bit data.
////////////////////////////////////////////////////////////////////////////////
class pandolinkage pando
{
public:
    pando();
    virtual ~pando();

    bool init(int pixel_width, int pixel_height, int num_samples, int num_scanlines, int start_sample, int end_sample, int start_scanline,
        int end_scanline, int sampling_frequency, int line_density, int steer_angle, int extension_angle, int origin_x, int origin_y,
        int microns_x, int microns_y, int elements, int pitch, int radius, int probe_angle,
        int transmit_offset);

    bool scanConvert(const unsigned char* input, unsigned char* output);
};
