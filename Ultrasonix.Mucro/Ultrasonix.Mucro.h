// Ultrasonix.Mucro.h

#pragma once

#include "mucro.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Ultrasonix 
{
	namespace Mucro
	{
		public ref class FilterParameters
		{
			private: int smooth;

			public: property int Smooth
			{
				void set(int value) { smooth = value; }
				int get() { return smooth; }
			}

			private: int weight;

			public: property int Weight
			{
				void set(int value) { weight = value; }
				int get() { return weight; }
			}			
		
			private: int contrast;

			public: property int Contrast
			{
				void set(int value) { contrast = value; }
				int get() { return contrast; }
			}	

			private: int lpCut;

			public: property int LPCut
			{
				void set(int value) { lpCut = value; }
				int get() { return lpCut; }
			}

			private: int lineStrength;

			public: property int LineStrength
			{
				void set(int value) { lineStrength = value; }
				int get() { return lineStrength; }
			}
		};

		public ref class Mucro
		{
			private: mucro* muc;

			public: Mucro()
			{
				muc = new mucro();
			}

			public: ~Mucro()
			{
				delete muc;
			}

			public: int Apply(array<Byte>^ input, [Out] array<Byte>^% output, int w, int h, FilterParameters^ parameters)
			{
				pin_ptr<Byte> p = &input[0];
				unsigned char* in = p;

				array<Byte>^ out = gcnew array<Byte>(input->Length);
				pin_ptr<Byte> o = &out[0];
				unsigned char* outputArray = o;

				FilterParams para;
				para.asrSmooth =		parameters->Smooth;
				para.asrWeight =		parameters->Weight;
				para.asrContrast =		parameters->Contrast;
				para.asrLPCut =			parameters->LPCut;
				para.asrLineStrength =	parameters->LineStrength;

				int result = muc->apply(in, outputArray, w, h, para);

				output = out;

				return result;
			}
		};


	}
}
