// Ultrasonix.Amplio.h

#pragma once

#include "Stdafx.h"

using namespace System;

namespace Ultrasonix
{
    namespace Amplio 
    {
	    public ref class Amplio
	    {
		    public: int InitRfToB(int freqA, int freqB, int freqcutoff, int freqSampling, int length, int useCompression, int dyn, int reject)
		    {
			    return amplioInitRfToB(freqA, freqB, freqcutoff, freqSampling, length, useCompression, dyn, reject);
		    }

		    public: array<Byte>^ ProcessRfToB(array<short>^ rfData, int decimation)
		    {
                pin_ptr<short> rfPtr = &rfData[0];
                short* rf = rfPtr;

                int size = (int)(rfData->Length / Math::Pow(2, decimation));

                array<Byte>^ arr = gcnew array<Byte>(size);
			    pin_ptr<Byte> p = &arr[0];
			    unsigned char* data = p;

                int result = amplioProcessRfToB(rf, decimation, data);

                // Check result?

                return arr;
		    }

            public: int InitScanConversion(int pixel_width, int pixel_height, int num_samples, int num_scanlines, int start_sample, int end_sample,
                int start_scanline, int end_scanline, int sampling_frequency, int line_density, int steer_angle, int extension_angle, int origin_x,
                int origin_y, int microns_x, int microns_y, int elements, int pitch, int radius, int probe_angle,
                int transmit_offset)
            {
                return amplioInitSC(pixel_width, pixel_height, num_samples, num_scanlines, start_sample, end_sample,
                                    start_scanline, end_scanline, sampling_frequency, line_density, steer_angle, extension_angle, origin_x,
                                    origin_y, microns_x, microns_y, elements, pitch, radius, probe_angle,
                                    transmit_offset);
            }

            public: array<Byte>^ ScanConvert(array<Byte>^ input)
            {
                pin_ptr<Byte> inputPtr = &input[0];
                const unsigned char* in = inputPtr;

                int size = input->Length;

                array<Byte>^ output = gcnew array<Byte>(size);
			    pin_ptr<Byte> outPtr = &output[0];
			    unsigned char* out = outPtr;

                int result = amplioScanConvert(in, out);

                return output;
            }

            public: array<Byte>^ Enhance(array<Byte>^ input, int width, int height, int edge, int smooth, int weight, int contrast, int lpcutoff)
            {
                pin_ptr<Byte> inputPtr = &input[0];
                unsigned char* in = inputPtr;

                int size = width * height;

                array<Byte>^ output = gcnew array<Byte>(size);
			    pin_ptr<Byte> outPtr = &output[0];
			    unsigned char* out = outPtr;

                int result = amplioEnhance(in, out, width, height, edge, smooth, weight, contrast, lpcutoff);

                return output;
            }

            public: void Destroy()
            {
                amplioDestroy();
            }
	    };
    }
}
