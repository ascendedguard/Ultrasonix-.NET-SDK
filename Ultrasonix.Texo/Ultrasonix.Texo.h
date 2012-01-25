// Ultrasonix.Texo.h
// TODO: Implement object as an IDisposable to allow proper delete on texo*

#pragma once

#include "Stdafx.h"

#include <string.h>

#include "Curve.h"
#include "DataFormat.h"
#include "DataEventArgs.h"
#include "TransmitParameters.h"
#include "ReceiveParameters.h"

using namespace System;
using namespace System::Runtime::InteropServices;

int DataCallback(void* sender, unsigned char* data, int index);

namespace Ultrasonix 
{
	namespace Texo
	{
		public ref class Texo
		{
			private: GCHandle objHandle;

			public: virtual event EventHandler<DataEventArgs^>^ DataReceived;

			/// <summary> Creates a new instance of a Texo object. </summary>
			public: Texo()
			{
				// We must hold the reference to be able to free after Texo is deallocated.
				objHandle = System::Runtime::InteropServices::GCHandle::Alloc(this);
				IntPtr ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(objHandle);

				texoSetCallback(DataCallback, ptr.ToPointer());
			}

			/// <summary> Deletes the texo instance and frees up object resources. </summary>
			~Texo()
			{
				objHandle.Free();
			}

			internal:
				void RaiseDataReceived(IntPtr data)
				{
					// Checking against nullptr shouldn't be required in Managed C++ for events,
					// But I could be wrong.

					DataEventArgs^ args = gcnew DataEventArgs(data);
					this->DataReceived(this, args);
				}

			/// <summary> Initializes the ultrasound system. Must be called before all other calls. </summary>
			public: bool Init(String^ firmwarePath, int pci, int usm, int hv, int channels)
			{
				// TODO: Add final "\\" to path.

				IntPtr pFirmwarePath = Marshal::StringToHGlobalAnsi(firmwarePath);

				bool result = texoInit((char*)pFirmwarePath.ToPointer(), pci, usm, hv, channels);

				Marshal::FreeHGlobal(pFirmwarePath);

				return result;
			}
	
			/// <summary> Initializes the ultrasound system. Must be called before all other calls. </summary>
			public: bool Init(String^ firmwarePath, int pci, int usm, int hv, int channels, int tx)
			{
				// TODO: Add final "\\" to path.

				IntPtr pFirmwarePath = Marshal::StringToHGlobalAnsi(firmwarePath);

				bool result;
				try
				{
					result = texoInit((char*)pFirmwarePath.ToPointer(), pci, usm, hv, channels, tx);
				}
				finally
				{
					// Ensures the allocated memory is always freed.
					Marshal::FreeHGlobal(pFirmwarePath);
				}

				return result;
			}	    
	
			/// <summary> Initializes the ultrasound system. Must be called before all other calls. </summary>
			public: bool Init(String^ firmwarePath, int pci, int usm, int hv, int channels, int tx, int sizeCine)
			{
				// TODO: Add final "\\" to path.

				IntPtr pFirmwarePath = Marshal::StringToHGlobalAnsi(firmwarePath);

				bool result = texoInit((char*)pFirmwarePath.ToPointer(), pci, usm, hv, channels, tx, sizeCine);

				Marshal::FreeHGlobal(pFirmwarePath);

				return result;
			}

			public: void Shutdown()
			{
				texoShutdown();
			}

			public: bool IsInitialized()
			{
				return texoIsInitialized();
			}

			public: bool IsImaging()
			{
				return texoIsImaging();
			}

			public: bool ActivateProbeConnector(int connector)
			{
				return texoActivateProbeConnector(connector);
			}

			public: String^ GetProbeName(int connector)
			{
				char* name = new char[20];
				bool result = texoGetProbeName(connector, name, 20);

				String^ nameStr = Marshal::PtrToStringAnsi(IntPtr(name));

				delete[] name;

				//TODO: Throw an exception if result was false

				return nameStr;
			}

			public: int GetProbeCode(int connector)
			{
				return texoGetProbeCode(connector);
			}

			public: int GetProbeNumElements()
			{
				return texoGetProbeNumElements();
			}

			public: int GetProbeCenterFreq()
			{
				return texoGetProbeCenterFreq();
			}

			public: int GetProbeFOV()
			{
				return texoGetProbeFOV();
			}

			public: bool BeginSequence()
			{
				return texoBeginSequence();
			}

			public: int AddLine(TransmitParameters^ txPrms, ReceiveParameters^ rxPrms)
			{
				_texoTransmitParams tx = txPrms->ConvertToC();
				_texoReceiveParams rx = rxPrms->ConvertToC();

				_texoLineInfo format; // Output

				// TODO: Change output to return LineInfo object (need to convert to C#)
				return texoAddLine(tx, rx, format);
			}

			public: bool EndSequence()
			{
				int result = texoEndSequence();

				if (result == -1)
				{
					return false;
				}

				return true;
			}

			public: void ClearTGCs()
			{
				texoClearTGCs();
			}

			public: bool AddTGC(double percent)
			{
				return texoAddTGCFixed(percent);
			}

			public: bool AddTGC(Curve^ tgc, int depth)
			{
				_texoCurve cv;

				cv.top = tgc->Top;
				cv.mid = tgc->Middle;
				cv.btm = tgc->Bottom;
				cv.vmid = tgc->VerticalMiddle;

				bool result = texoAddTGC(&cv, depth);

				return result;
			}

			public: bool AddReceive(ReceiveParameters^ param)
			{
				if(param == nullptr)
				{
					throw gcnew ArgumentNullException("param");
				}

				_texoReceiveParams rx = param->ConvertToC();

				return texoAddReceive(rx);
			}

			public: bool AddTransmit(TransmitParameters^ param)
			{
				if(param == nullptr)
				{
					throw gcnew ArgumentNullException("param");
				}

				_texoTransmitParams tx = param->ConvertToC();

				return texoAddTransmit(tx);
			}

			public: bool SetPower(int power, int maxPositive, int maxNegative)
			{
				return texoSetPower(power, maxPositive, maxNegative);
			}

			public: void SetVCAInfo(VCAInformation^ information)
			{
				if(information == nullptr)
				{
					throw gcnew ArgumentNullException("information");
				}

				_vcaInfo info;

				info.amplification = information->Amplification;
				info.activetermination = information->ActiveTermination;
				info.inclamp = information->InClamp;
				info.LPF = information->LPF;
				info.lnaIntegratorEnable = information->LnaIntegratorEnable;
				info.pgaIntegratorEnable = information->PgaIntegratorEnable;
				info.hpfDigitalEnable = information->HpfDigitalEnable;
				info.hpfDigitalValue = information->HpfDigitalValue;

				texoSetVCAInfo(info);
			}

			public: bool RunImage()
			{
				return texoRunImage();
			}

			public: bool StopImage()
			{
				return texoStopImage();
			}

			// TODO: Implement SetCallback as a .NET Event

			public: double GetFrameRate()
			{
				return texoGetFrameRate();
			}

			public: int GetFrameSize()
			{
				return texoGetFrameSize();
			}

			public: int GetMaxFrameCount()
			{
				return texoGetMaxFrameCount();
			}

			public: int GetCollectedFrameCount()
			{
				return texoGetCollectedFrameCount();
			}

			public: IntPtr GetCineStart(int blockid)
			{
				unsigned char* c = texoGetCineStart(blockid);
				return IntPtr(c);
			}

			public: bool SetDelayReadBack(String^ file)
			{
				IntPtr pFile = Marshal::StringToHGlobalAnsi(file);

				bool result = texoSetDelayReadBack((char*)pFile.ToPointer());

				Marshal::FreeHGlobal(pFile);

				return result;
			}

			public: void CloseDelayReadBack()
			{
				texoCloseDelayReadBack();
			}

			public: void SetSyncSignals(int input, int output, int output2)
			{
				texoSetSyncSignals(input, output, output2);
			}

			public: void EnableSyncNotify(bool enable)
			{
				texoEnableSyncNotify(enable);
			}

			public: bool SetupMotor(bool enable, int fpv, int spf)
			{
				return texoSetupMotor(enable, fpv, spf);
			}

			public: double GoToPosition(double angle)
			{
				return texoGoToPosition(angle);
			}

			public: double StepMotor(bool cw, int steps)
			{
				return texoStepMotor(cw, steps);
			}

			public: void ForceConnector(int conn)
			{
				return texoForceConnector(conn);
			}
		};
	}
}
