// Ultrasonix.Pando.h

#pragma once

#include "pando.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Ultrasonix 
{
	namespace Pando
	{
		public ref class Pando
		{
			private: pando* pan;	

			public: Pando()
			{
				pan = new pando();
			}

			public: ~Pando()
			{
				delete pan;
			}

			public: bool Init(int pixel_width, int pixel_height, int num_samples, int num_scanlines, int start_sample, int end_sample, int start_scanline,
							int end_scanline, int sampling_frequency, int line_density, int steer_angle, int extension_angle, int origin_x, int origin_y,
							int microns_x, int microns_y, int elements, int pitch, int radius, int probe_angle,
							int transmit_offset)
			{
				return pan->init(pixel_width, pixel_height, num_samples, num_scanlines, start_sample, end_sample, start_scanline,
							end_scanline, sampling_frequency, line_density, steer_angle, extension_angle, origin_x, origin_y,
							microns_x, microns_y, elements, pitch, radius, probe_angle,
							transmit_offset);
			}

			public: bool ScanConvert(array<System::Byte>^ input, [Out] array<System::Byte>^% output, int outputSize)
			{
				// OutputSize can likely be a stored value based on pixel_width and pixel_height. For now, it's just being passed in
				// Since i'm not sure the size of the output pixels.

				pin_ptr<Byte> p = &input[0];
				unsigned char* in = p;

				array<Byte>^ out = gcnew array<Byte>(outputSize);
				pin_ptr<Byte> o = &out[0];
				unsigned char* outputArray = o;

				bool result = pan->scanConvert(in, outputArray);

				output = out;

				return result;
			}
		};
	}
}
