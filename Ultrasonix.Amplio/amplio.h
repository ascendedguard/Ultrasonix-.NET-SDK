#pragma once

#ifdef EXPORT_AMPLIO
    #define amplioL __declspec(dllexport)
#elif defined IMPORTAMPLIO
    #define amplioL __declspec(dllimport)
#else
    #define amplioL
#endif

#ifdef __cplusplus
extern "C" {
#endif

// rf to b conversion
amplioL int amplioInitRfToB(int freqA, int freqB, int freqcutoff, int freqSampling, int length, int useCompression, int dyn, int reject);
amplioL int amplioProcessRfToB(short* rfData, int decimation, unsigned char* envData);
// scan conversion
amplioL int amplioInitSC(int pixel_width, int pixel_height, int num_samples, int num_scanlines, int start_sample, int end_sample,
    int start_scanline, int end_scanline, int sampling_frequency, int line_density, int steer_angle, int extension_angle, int origin_x,
    int origin_y, int microns_x, int microns_y, int elements, int pitch, int radius, int probe_angle,
    int transmit_offset);
amplioL int amplioScanConvert(const unsigned char* input, unsigned char* output);
// enhancement filtering
amplioL int amplioEnhance(unsigned char* datain, unsigned char* dataout, int w, int h, int edge, int smooth, int weight, int contrast,
    int lpcutoff);
amplioL void amplioDestroy();

#ifdef __cplusplus
}
#endif
