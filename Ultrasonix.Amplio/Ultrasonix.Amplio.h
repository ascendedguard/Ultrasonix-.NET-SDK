// Ultrasonix.Amplio.h

#pragma once

#include "amplio.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Ultrasonix 
{
	namespace Amplio
	{
		public ref class Amplio
		{
			private: amplio* amp;

			public: Amplio()
			{
				amp = new amplio();
			}

			public: ~Amplio()
			{
			    delete amp;
			}

            public: bool Init(array<double>^ filterA, int numCoeffA, array<double>^ filterB, int numCoeffB, 
                              array<double>^ filterC, int numCoeffC, array<Byte>^ compressionTable, int comprSz)
            {
                // We can likely just read the size of the arrays for parameters 2 4 6 and 8... need to verify though.

                pin_ptr<double> pFA = &filterA[0];
				double* fA = pFA;
                
                pin_ptr<double> pFB = &filterB[0];
				double* fB = pFB;
                
                pin_ptr<double> pFC = &filterC[0];
				double* fC = pFC;

                pin_ptr<Byte> compTable = &compressionTable[0];
				unsigned char* comp = compTable;

                return amp->init(fA, numCoeffA, fB, numCoeffB, fC, numCoeffC, comp, comprSz);
            }

            public: bool Init(String^ path, int indexA, int indexB, int indexC)
            {
                IntPtr pPath = Marshal::StringToHGlobalAnsi(path);

                bool result = amp->init((char*)pPath.ToPointer(), indexA, indexB, indexC);

                Marshal::FreeHGlobal(pPath);

                return result;
            }

            public: bool InitIQ(int freqA, int freqB, int freqcutoff, int freqSampling, int length)
            {
                return amp->initIQ(freqA, freqB, freqcutoff, freqSampling, length);
            }

            public: bool Apply(array<short>^ rfData, bool sumAfterEnvelope, int useCompression, int decimation, [Out] array<Byte>^% envData)
            {
                int length = rfData->Length;
                pin_ptr<short> pRF = &rfData[0];
				short* rf = pRF;

                array<Byte>^ out = gcnew array<Byte>(length / decimation);
				pin_ptr<Byte> o = &out[0];
				unsigned char* outputArray = o;

                bool result = amp->apply(rf, length, sumAfterEnvelope, useCompression, decimation, outputArray);

                envData = out;

                return result;
            }

            public: bool ApplyRF(array<short>^ rfData, int decimation, [Out] array<short>^% filteredRfData)
            {
                int length = rfData->Length;
                pin_ptr<short> pRF = &rfData[0];
				short* rf = pRF;

                array<short>^ out = gcnew array<short>(length / decimation);
				pin_ptr<short> o = &out[0];
				short* outputArray = o;

                bool result = amp->applyRF(pRF, length, decimation, outputArray);

                filteredRfData = out;

                return result;
            }

            public: bool ApplyIQ(array<short>^ rfData, int useCompression, int scale, int decimation, [Out] array<Byte>^% envData)
            {
                int length = rfData->Length;
                pin_ptr<short> pRF = &rfData[0];
				short* rf = pRF;

                array<Byte>^ out = gcnew array<Byte>(length / decimation);
				pin_ptr<Byte> o = &out[0];
				unsigned char* outputArray = o;

                // why is length missing from this function?!
                bool results = amp->applyIQ(pRF, useCompression, scale, decimation, outputArray);

                envData = out;

                return results;
            }
		};
	}
}
