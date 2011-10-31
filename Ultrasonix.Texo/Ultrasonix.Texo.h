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

bool DataCallback(void* sender, unsigned char* data, int index);

namespace Ultrasonix 
{
	namespace Texo
	{
		public ref class Texo
		{
			private: texo* tex;
			private: GCHandle objHandle;

			public: virtual event EventHandler<DataEventArgs^>^ DataReceived;

			/// <summary> Creates a new instance of a Texo object. </summary>
			public: Texo()
			{
				tex = new texo();

				// We must hold the reference to be able to free after Texo is deallocated.
				objHandle = System::Runtime::InteropServices::GCHandle::Alloc(this);
				IntPtr ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(objHandle);

				tex->setCallback(DataCallback, ptr.ToPointer());
			}

			/// <summary> Deletes the texo instance and frees up object resources. </summary>
			~Texo()
			{
				delete tex;

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

				bool result = tex->init((char*)pFirmwarePath.ToPointer(), pci, usm, hv, channels);

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
					result = tex->init((char*)pFirmwarePath.ToPointer(), pci, usm, hv, channels, tx);
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

				bool result = tex->init((char*)pFirmwarePath.ToPointer(), pci, usm, hv, channels, tx, sizeCine);

				Marshal::FreeHGlobal(pFirmwarePath);

				return result;
			}

			public: bool Shutdown()
			{
				return tex->shutdown();
			}

			public: bool IsInitialized()
			{
				return tex->isInitialized();
			}

			public: bool IsImaging()
			{
				return tex->isImaging();
			}

			public: bool ActivateProbeConnector(int connector)
			{
				return tex->activateProbeConnector(connector);
			}

			public: String^ GetProbeName(int connector)
			{
				char* name = new char[20];
				bool result = tex->getProbeName(connector, name, 20);

				String^ nameStr = Marshal::PtrToStringAnsi(IntPtr(name));

				delete[] name;

				//TODO: Throw an exception if result was false

				return nameStr;
			}

			public: int GetProbeCode(int connector)
			{
				return tex->getProbeCode(connector);
			}

			public: int GetProbeNumElements()
			{
				return tex->getProbeNumElements();
			}

			public: int GetProbeCenterFreq()
			{
				return tex->getProbeCenterFreq();
			}

			public: int GetProbeFOV()
			{
				return tex->getProbeFOV();
			}

			public: bool BeginSequence()
			{
				return tex->beginSequence();
			}

			public: int AddLine(DataFormat format, TransmitParameters^ txPrms, ReceiveParameters^ rxPrms)
			{
				texoTransmitParams tx = txPrms->ConvertToC();
				texoReceiveParams rx = rxPrms->ConvertToC();

				return tex->addLine((texoDataFormat)format, tx, rx);
			}

			public: bool EndSequence()
			{
				int result = tex->endSequence();

				if (result == -1)
				{
					return false;
				}

				return true;
			}

			public: void ClearTGCs()
			{
				tex->clearTGCs();
			}

			public: bool AddTGC(double percent)
			{
				return tex->addTGC(percent);
			}

			public: bool AddTGC(Curve^ tgc, int depth)
			{
				CURVE* cv = new CURVE();

				cv->top = tgc->Top;
				cv->mid = tgc->Middle;
				cv->btm = tgc->Bottom;
				cv->vmid = tgc->VerticalMiddle;

				bool result = tex->addTGC(cv, depth);

				// Hopefully this is alright.
				delete cv;

				return result;
			}

			public: bool AddReceive(ReceiveParameters^ param)
			{
				if(param == nullptr)
				{
					throw gcnew ArgumentNullException("param");
				}

				texoReceiveParams rx = param->ConvertToC();

				return tex->addReceive(rx);
			}

			public: bool AddTransmit(TransmitParameters^ param)
			{
				if(param == nullptr)
				{
					throw gcnew ArgumentNullException("param");
				}

				texoTransmitParams tx = param->ConvertToC();

				return tex->addTransmit(tx);
			}

			public: bool SetPower(int power, int maxPositive, int maxNegative)
			{
				return tex->setPower(power, maxPositive, maxNegative);
			}

			public: void SetVCAInfo(VCAInformation^ information)
			{
				if(information == nullptr)
				{
					throw gcnew ArgumentNullException("information");
				}

				VCAInfo info;

				info.amplification = information->Amplification;
				info.activetermination = information->ActiveTermination;
				info.inclamp = information->InClamp;
				info.LPF = information->LPF;
				info.lnaIntegratorEnable = information->LnaIntegratorEnable;
				info.pgaIntegratorEnable = information->PgaIntegratorEnable;
				info.hpfDigitalEnable = information->HpfDigitalEnable;
				info.hpfDigitalValue = information->HpfDigitalValue;

				tex->setVCAInfo(info);
			}

			public: bool RunImage()
			{
				return tex->runImage();
			}

			public: bool StopImage()
			{
				return tex->stopImage();
			}

			// TODO: Implement SetCallback as a .NET Event

			public: double GetFrameRate()
			{
				return tex->getFrameRate();
			}

			public: int GetFrameSize()
			{
				return tex->getFrameSize();
			}

			public: int GetMaxFrameCount()
			{
				return tex->getMaxFrameCount();
			}

			public: int GetCollectedFrameCount()
			{
				return tex->getCollectedFrameCount();
			}

			public: IntPtr GetCineStart(int blockid)
			{
				unsigned char* c = tex->getCineStart(blockid);
				return IntPtr(c);
			}

			public: bool SetDelayReadBack(String^ file)
			{
				IntPtr pFile = Marshal::StringToHGlobalAnsi(file);

				bool result = tex->setDelayReadBack((char*)pFile.ToPointer());

				Marshal::FreeHGlobal(pFile);

				return result;
			}

			public: void CloseDelayReadBack()
			{
				tex->closeDelayReadBack();
			}

			public: bool SetSyncSignals(int input, int output, int output2)
			{
				return tex->setSyncSignals(input, output, output2);
			}

			public: bool EnableSyncNotify(bool enable)
			{
				return tex->enableSyncNotify(enable);
			}

			public: bool SetupMotor(bool enable, int fpv, int spf)
			{
				return tex->setupMotor(enable, fpv, spf);
			}

			public: double GoToPosition(double angle)
			{
				return tex->goToPosition(angle);
			}

			public: double StepMotor(bool cw, int steps)
			{
				return tex->stepMotor(cw, steps);
			}

			public: void ForceConnector(int conn)
			{
				return tex->forceConnector(conn);
			}
		};
	}
}
