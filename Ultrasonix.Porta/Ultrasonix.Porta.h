// Ultrasonix.Porta.h

#pragma once

#include "Stdafx.h"
#include "ProbeInfo.h"
#include "ImagingMode.h"
#include "ParamTypes.h"
#include "RawDataEventArgs.h"
#include "DisplayCallbackEventArgs.h"
#include "PortaImagingException.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

int PreRunCallback(void*);
int RawDataCallback(void* param, unsigned char* data, int cineBlock, int header);
int DisplayCallback(void* prm, int id, int header);

namespace Ultrasonix
{
	namespace Porta
	{
		public ref class Porta
		{
			private:
				GCHandle objHandle;
				
				// Used to cache the display dimensions when calling local imaging functions.
				Dictionary<int, Size^>^ displayDimensions;

			internal:
				void RaisePreRunning()
				{
					this->PreRunning(this, EventArgs::Empty);
				}

				void RaiseRawDataCallback(RawDataEventArgs^ args)
				{
					this->RawDataReceived(this, args);
				}

				void RaiseDisplayImageReceived(DisplayCallbackEventArgs^ args)
				{
					this->DisplayImageReceived(this, args);
				}

			public:
				virtual event EventHandler<EventArgs^>^ PreRunning;
				virtual event EventHandler<RawDataEventArgs^>^ RawDataReceived;
				virtual event EventHandler<DisplayCallbackEventArgs^>^ DisplayImageReceived;

				Porta()
				{
					displayDimensions = gcnew Dictionary<int, Size^>();
				}
			
				~Porta()
				{
					objHandle.Free();
				}

				bool Initialize(int cineSize, String^ firmwarePath, String^ settingsPath, String^ licensePath, String^ lutPath, int usm, int pci, int hv, int ddr, int channels)
				{
					IntPtr pFirmwarePath = Marshal::StringToHGlobalAnsi(firmwarePath);
					IntPtr pSettingsPath = Marshal::StringToHGlobalAnsi(settingsPath);
					IntPtr pLicensePath = Marshal::StringToHGlobalAnsi(licensePath);
					IntPtr pLutPath = Marshal::StringToHGlobalAnsi(lutPath);

					bool result;

					try
					{
						result = portaInit(cineSize,
								(char*)pFirmwarePath.ToPointer(),
								(char*)pSettingsPath.ToPointer(),
								(char*)pLicensePath.ToPointer(),
								(char*)pLutPath.ToPointer(),
								usm, pci, hv, ddr, channels);
					}
					finally
					{
						// Ensure the memory is always freed.
						Marshal::FreeHGlobal(pFirmwarePath);
						Marshal::FreeHGlobal(pSettingsPath);
						Marshal::FreeHGlobal(pLicensePath);
						Marshal::FreeHGlobal(pLutPath);
					}

					// Once the system is initialized, we can hook our events.
					objHandle = System::Runtime::InteropServices::GCHandle::Alloc(this);

					return result;
				}

				void Shutdown()
				{
					portaShutdown();
				}

				bool IsConnected()
				{
					return portaIsConnected();
				}

				/* Function is gone from latest SDK
				String^ GetLastError()
				{
					char* err = new char[128];

					bool result = portaGetLastError(err, 128);

					// Throw exception if result == false

					String^ errStr = Marshal::PtrToStringAnsi(IntPtr(err));

					delete[] err;

					return errStr;
				}
				*/

				String^ GetSystemId()
				{
					char* sysId = new char[128];

					bool result = portaGetSystemId(sysId, 128);

					// Throw exception if result == false

					String^ sysIdStr = Marshal::PtrToStringAnsi(IntPtr(sysId));

					delete[] sysId;

					return sysIdStr;
				}

				bool SignalBnc()
				{
					return portaSignalBnc();
				}

				bool PollBnc()
				{
					return portaPollBnc();
				}

				bool AckBnc()
				{
					return portaAckBnc();
				}

				/* Missing from latest SDK
				array<char>^ ReadEPROM(int size)
				{
					char* d = new char[size];

					int result = portaReadEPROM(d, size);

					array<char>^ data = gcnew array<char>(size);

					for(int i = 0; i < size; i++)
					{
						data[i] = d[i];
					}

					delete[] d;

					return data;
				}

				int WriteEPROM(array<char>^ data)
				{
					int size = data->Length;
					char* d = new char[size];

					for(int i = 0; i < size; i++)
					{
						d[i] = data[i];
					}

					int result = portaWriteEPROM(d, size);

					delete[] d;

					return result;
				}
				*/

				bool SelectProbe(int id)
				{
					return portaSelectProbe(id);
				}

				int GetCurrentProbeId()
				{
					return portaGetCurrentProbeID();
				}

				int GetProbeId(int connector)
				{
					return portaGetProbeID(connector);
				}

				bool ActivateProbeConnector(int connector)
				{
					return portaActivateProbeConnector(connector);
				}

				String^ GetProbeName(int id)
				{
					char* probeName = new char[128];
					bool result = portaGetProbeName(probeName, 128, id);

					// Throw exception if result is false.

					String^ probeNameStr = Marshal::PtrToStringAnsi(IntPtr(probeName));

					delete[] probeName;

					return probeNameStr;
				}

				ProbeInfo^ GetProbeInfo()
				{
					probeInfo info;

					portaGetProbeInfo(info);

					ProbeInfo^ p = gcnew ProbeInfo();
					p->Elements = info.elements;
					p->Pitch = info.pitch;
					p->Radius = info.radius;
					p->Motorized = info.motorized;
					p->MotorFieldOfView = info.motorFov;
					p->MotorRadius = info.motorRadius;
					p->MotorSteps = info.motorSteps;
					p->MotorHasHomeSensor = info.motorHomeSensor;

					return p;
				}

				bool TestElectronicComponent(int id)
				{
					return portaTestElectronicComponent(id);
				}
			
				// TODO: Implement testProbeElements
				// TODO: Implement setPowerTestParams
				// TODO: Implement readPowerValues

				bool RunImage()
				{
					return portaRunImage();
				}

				bool StopImage()
				{
					return portaStopImage();
				}

				bool IsImaging()
				{
					return portaIsImaging();
				}

				bool SetWatchdog(int timeout)
				{
					return portaSetWatchdog(timeout);
				}

				bool SetCustomDMA(bool enable, int physicaladdress, int size)
				{
					return portaSetCustomDMA(enable, physicaladdress, size);
				}

				void SetSleepDelay(int delay)
				{
					portaSetSleepDelay(delay);
				}

				bool InitImagingMode(ImagingMode mode)
				{
					return portaInitMode((imagingMode)mode);
				}

				ImagingMode GetCurrentMode()
				{
					return (ImagingMode)portaGetCurrentMode();
				}
				
				void StartRawDataCallback()
				{
					IntPtr ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(objHandle);

					portaSetRawDataCallback(RawDataCallback, ptr.ToPointer());
				}

				void StartPreRunCallback()
				{
					IntPtr ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(objHandle);

					portaSetPreRunCallback(PreRunCallback, ptr.ToPointer());
				}

				void StartDisplayCallback(int frameIndex)
				{
					IntPtr ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(objHandle);

					portaSetDisplayCallback(frameIndex, DisplayCallback, ptr.ToPointer());
				}

				int GetFrameCount(int index)
				{
					return portaGetFrameCount(index);
				}

				bool LoadMasterPreset()
				{
					return portaLoadMasterPreset();
				}

				bool LoadPreset(String^ path)
				{
					IntPtr pPath = Marshal::StringToHGlobalAnsi(path);

					bool result = portaLoadPreset((char*)pPath.ToPointer());

					Marshal::FreeHGlobal(pPath);

					return result;
				}

				bool SavePreset(String^ path, bool overwrite)
				{
					IntPtr pPath = Marshal::StringToHGlobalAnsi(path);

					bool result = portaSavePreset((char*)pPath.ToPointer(), overwrite);

					Marshal::FreeHGlobal(pPath);

					return result;
				}

				/* Missing from latest SDK
				bool IsFactoryPreset()
				{
					return portaIsFactoryPreset();
				}
				*/

				String^ FindMasterPreset(int probe)
				{
					char* probePath = new char[512];

					portaFindMasterPreset(probePath, 512, probe);

					String^ probePathStr = Marshal::PtrToStringAnsi(IntPtr(probePath));

					delete[] probePath;

					return probePathStr;
				}

				bool IsMasterPreset(String^ path)
				{
					IntPtr pPath = Marshal::StringToHGlobalAnsi(path);

					bool result = portaIsMasterPreset((char*)pPath.ToPointer());

					Marshal::FreeHGlobal(pPath);

					return result;
				}

				bool GetPresetProbeID(String^ path, [Out] int% probeId1, [Out] int% probeId2, [Out] int% probeId3)
				{
					int id1, id2, id3;

					IntPtr pPath = Marshal::StringToHGlobalAnsi(path);

					bool result = portaGetPresetProbeID((char*)pPath.ToPointer(), id1, id2, id3);

					Marshal::FreeHGlobal(pPath);

					probeId1 = id1;
					probeId2 = id2;
					probeId3 = id3;

					return result;
				}

				int GetFrameRate()
				{
					return portaGetFrameRate();
				}

				int GetDataFrameRate()
				{
					return portaGetDataFrameRate();
				}

				int GetParam(String^ param)
				{
					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);

					int result;

					portaGetParamI((char*)pParam.ToPointer(), result);

					Marshal::FreeHGlobal(pParam);

					return result;
				}

				bool SetParam(String^ param, int value)
				{
					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);

					bool result = portaSetParamI((char*)pParam.ToPointer(), value);

					Marshal::FreeHGlobal(pParam);

					return result;
				}

				String^ GetParamString(String^ param)
				{
					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);

					char* buffer = new char[128];

					bool result = portaGetParamS((char*)pParam.ToPointer(), buffer, 128);

					Marshal::FreeHGlobal(pParam);

					String^ buf = Marshal::PtrToStringAnsi(IntPtr(buffer));

					delete[] buffer;

					// Throw exception if result == false

					return buf;
				}
			
				bool SetParam(String^ param, String^ value)
				{
					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					IntPtr pValue = Marshal::StringToHGlobalAnsi(value);

					bool result = portaSetParamS((char*)pParam.ToPointer(), (char*)pValue.ToPointer());

					Marshal::FreeHGlobal(pParam);
					Marshal::FreeHGlobal(pValue);

					return result;
				}

				Rect^ GetParamRect(String^ param)
				{
					portaRect r;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = portaGetParamR((char*)pParam.ToPointer(), r);
					Marshal::FreeHGlobal(pParam);

					Rect^ rect = gcnew Rect();

					rect->Top = r.top;
					rect->Left = r.left;
					rect->Right = r.right;
					rect->Bottom = r.bottom;

					return rect;
				}

				bool SetParam(String^ param, Rect^ value)
				{
					portaRect r;

					r.top = value->Top;
					r.left = value->Left;
					r.right = value->Right;
					r.bottom = value->Bottom;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = portaSetParamR((char*)pParam.ToPointer(), r);
					Marshal::FreeHGlobal(pParam);

					return result;
				}

				Point^ GetParamPoint(String^ param)
				{
					portaPoint p;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = portaGetParamP((char*)pParam.ToPointer(), p);
					Marshal::FreeHGlobal(pParam);

					Point^ point = gcnew Point();

					point->X = p.x;
					point->Y = p.y;

					return point;
				}

				bool SetParam(String^ param, Point^ value)
				{
					portaPoint p;

					p.x = value->X;
					p.y = value->Y;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = portaSetParamP((char*)pParam.ToPointer(), p);
					Marshal::FreeHGlobal(pParam);

					return result;
				}

				Curve^ GetParamCurve(String^ param)
				{
					portaCurve c;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = portaGetParamC((char*)pParam.ToPointer(), c);
					Marshal::FreeHGlobal(pParam);

					Curve^ curve = gcnew Curve();

					curve->Top = c.t;
					curve->Middle = c.m;
					curve->Bottom = c.b;
					curve->VerticalMiddle = c.vm;

					return curve;
				}

				bool SetParam(String^ param, Curve^ value)
				{
					portaCurve c;

					c.t = value->Top;
					c.m = value->Middle;
					c.b = value->Bottom;
					c.vm = value->VerticalMiddle;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = portaSetParamC((char*)pParam.ToPointer(), c);
					Marshal::FreeHGlobal(pParam);

					return result;
				}

				GainCurve^ GetParamGainCurve(String^ param)
				{
					portaGainCurve c;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = portaGetParamGC((char*)pParam.ToPointer(), c);
					Marshal::FreeHGlobal(pParam);

					GainCurve^ curve = gcnew GainCurve();

					curve->Values[0] = c.val[0];
					curve->Values[1] = c.val[1];
					curve->Values[2] = c.val[2];
					curve->Values[3] = c.val[3];
					curve->Values[4] = c.val[4];
					curve->Values[5] = c.val[5];
					curve->Values[6] = c.val[6];
					curve->Values[7] = c.val[7];

					return curve;
				}

				bool SetParam(String^ param, GainCurve^ value)
				{
					portaGainCurve c;

					c.val[0] = value->Values[0];
					c.val[1] = value->Values[1];
					c.val[2] = value->Values[2];
					c.val[3] = value->Values[3];
					c.val[4] = value->Values[4];
					c.val[5] = value->Values[5];
					c.val[6] = value->Values[6];
					c.val[7] = value->Values[7];

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = portaSetParamGC((char*)pParam.ToPointer(), c);
					Marshal::FreeHGlobal(pParam);

					return result;
				}

				bool CycleParam(String^ param, bool forward)
				{
					return CycleParam(param, forward, false);
				}

				bool CycleParam(String^ param, bool forward, bool wrap)
				{
					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = portaCycleParam((char*)pParam.ToPointer(), forward, wrap);
					Marshal::FreeHGlobal(pParam);
				
					return result;
				}

				bool GetParamMinMax(String^ param, [Out] int% min, [Out] int% max)
				{
					int mn, mx;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = portaGetParamMinMax((char*)pParam.ToPointer(), mn, mx);
					Marshal::FreeHGlobal(pParam);

					min = mn;
					max = mx;

					return result;
				}

				VariableType GetParamType(String^ param)
				{
					portaVarType t;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = portaGetParamType((char*)pParam.ToPointer(), t);
					Marshal::FreeHGlobal(pParam);

					return (VariableType)t;
				}

				String^ GetListParam(int prmNum)
				{
					char* name = new char[128];
					bool result = portaGetListParam(name, 128, prmNum);

					String^ n = Marshal::PtrToStringAnsi(IntPtr(name));
					delete[] name;

					return n;
				}
		
				int GetNumParams()
				{
					return portaGetNumParams();
				}

				bool SetDisplayDimensions(int index, int x, int y)
				{
					displayDimensions[index] = gcnew Size(x, y);
					return portaSetDisplayDimensions(index, x, y);
				}

				bool GetDisplayDimensions(int index, [Out] int% x, [Out] int% y)
				{
					int xOut, yOut;

					bool result = portaGetDisplayDimensions(index, xOut, yOut);

					x = xOut;
					y = yOut;

					return result;
				}

				bool GetPrescanBDimensions(int index, [Out] int% w, [Out] int% h)
				{
					int xOut, yOut;

					bool result = portaGetPrescanBDimensions(index, xOut, yOut);

					w = xOut;
					h = yOut;

					return result;
				}

				bool GetBwImagePrescan(int index, IntPtr data)
				{
					unsigned char* d = (unsigned char*)data.ToPointer();
					return portaGetBwImagePrescan(index, d);
				}

				array<Byte>^ GetBwImagePrescan(int index)
				{
					// This should be based on the set size, not necessarily hard-coded.
					int w, h;
					portaGetPrescanBDimensions(index, w, h);
					
					int size = w * h;

					array<Byte>^ arr = gcnew array<Byte>(size);
					pin_ptr<Byte> p = &arr[0];
					unsigned char* data = p;

					bool result = portaGetBwImagePrescan(index, data);

					if (result == false)
					{
						throw gcnew PortaImagingException("GetBwImagePrescan failed to retrieve an image");
					}

					return arr;
				}

				array<Byte>^ GetBwImage(int index, bool useChroma)
				{
					// This should be based on the set size, not necessarily hard-coded.
					Size^ s = displayDimensions[index];
					int size = s->Width * s->Height;

					if (useChroma)
					{
						size *= 4;
					}

					array<Byte>^ arr = gcnew array<Byte>(size);
					pin_ptr<Byte> p = &arr[0];
					unsigned char* data = p;

					bool result = portaGetBwImage(index, data, useChroma);

					if (result == false)
					{
						throw gcnew PortaImagingException("GetBwImage failed to retrieve an image");
					}

					return arr;
				}

				bool GetBwImage(int index, bool useChroma, IntPtr data)
				{
					unsigned char* d = (unsigned char*)data.ToPointer();
					return portaGetBwImage(index, d, useChroma);
				}

				array<Byte>^ GetColorImage(int index)
				{
					// This should be based on the set size, not necessarily hard-coded.
					Size^ s = displayDimensions[index];
					int size = s->Width * s->Height * 4;

					array<Byte>^ arr = gcnew array<Byte>(size);
					pin_ptr<Byte> p = &arr[0];
					unsigned char* data = p;

					bool result = portaGetColorImage(index, data);

					if (result == false)
					{
						throw gcnew PortaImagingException("GetColorImage failed to retrieve an image");
					}

					return arr;
				}

				bool GetColorImage(int index, IntPtr data)
				{
					unsigned char* d = (unsigned char*)data.ToPointer();
					return portaGetColorImage(index, d);	
				}

				array<Byte>^ GetColorData(int index, bool velocity, bool prescan, bool copy)
				{
					Size^ s = displayDimensions[index];
					int size = s->Width * s->Height;

					array<Byte>^ arr = gcnew array<Byte>(size);
					pin_ptr<Byte> p = &arr[0];
					unsigned char* data = p;

					bool result = portaGetColorData(index, data, velocity, prescan, copy);

					if (result == false)
					{
						throw gcnew PortaImagingException("GetColorImage failed to retrieve an image");
					}

					return arr;
				}

				bool GetColorData(int index, bool velocity, bool prescan, bool copy, IntPtr data)
				{
					unsigned char* d = (unsigned char*)data.ToPointer();
					return portaGetColorData(index, d, velocity, prescan, copy);
				}

				array<Byte>^ GetColorVV(int index)
				{
					Size^ s = displayDimensions[index];
					int size = s->Width * s->Height * 4;

					array<Byte>^ arr = gcnew array<Byte>(size);
					pin_ptr<Byte> p = &arr[0];
					unsigned char* data = p;

					bool result = portaGetColorVV(index, data);

					if (result == false)
					{
						throw gcnew PortaImagingException("GetColorImage failed to retrieve an image");
					}

					return arr;
				}

				bool GetColorVV(int index, IntPtr data)
				{
					unsigned char* d = (unsigned char*)data.ToPointer();
					return portaGetColorVV(index, d);
				}

				int GetDisplayFrameCount(int index)
				{
					return portaGetDisplayFrameCount(index);
				}

				bool ProcessCineImage(int index, int frame)
				{
					return portaProcessCineImage(index, frame);
				}

				bool ImportChromaMap(int index, array<int>^ lut)
				{
					int size = lut->Length;
					unsigned int* arr = new unsigned int[size];
					
					for(int i = 0; i < size; i++)
					{
						arr[i] = lut[i];
					}

					bool result = portaImportChromaMap(index, arr);

					delete[] arr;

					return result;
				}

				bool ImportColorMap(int index, array<int>^ lut)
				{
					int size = lut->Length;
					unsigned int* arr = new unsigned int[size];
					
					for(int i = 0; i < size; i++)
					{
						arr[i] = lut[i];
					}

					bool result = portaImportColorMap(index, arr);

					delete[] arr;

					return result;
				}

				bool GetMicronsPerPixel(int index, [Out] int% mx, [Out] int% my)
				{
					int x, y;

					bool result = portaGetMicronsPerPixel(index, x, y);

					mx = x;
					my = y;

					return result;
				}

				bool GetPixelCoordinates(int index, int line, int sample, [Out] int% xOut, [Out] int% yOut, bool bColor, int addAngle)
				{
					int x, y;

					bool result = portaGetPixelCoordinates(index, line, sample, x, y, bColor, addAngle);

					xOut = x;
					yOut = y;

					return result;
				}

				bool GetUltrasoundCoordinates(int index, int x, int y, [Out] int% xOut, [Out] int% yOut, bool bColor, int addAngle)
				{
					int xo, yo;

					bool result = portaGetUltrasoundCoordinates(index, x, y, xo, yo, bColor, addAngle);

					xOut = xo;
					yOut = yo;

					return result;
				}

				Rect^ GetHorizontalArcRect(int index, bool color, bool top, int sampleOverride)
				{
					portaRect rect;

					bool result = portaGetHorizontalArcRect(index, rect, color, top, sampleOverride);

					// Throw exception if result == false

					Rect^ r = gcnew Rect();
					r->Top = rect.top;
					r->Left = rect.left;
					r->Right = rect.right;
					r->Bottom = rect.bottom;

					return r;
				}

				bool GetROI(int index, [Out] int% xOut, [Out] int% yOut)
				{
					int x, y;

					bool result = portaGetROI(index, &x, &y);

					xOut = x;
					yOut = y;

					return result;
				}

				bool GetColorBox(int index, [Out] int% xOut, [Out] int% yOut)
				{
					int x, y;

					bool result = portaGetColorBox(index, &x, &y);

					xOut = x;
					yOut = y;

					return result;
				}

				Rect^ GetNewColorBox(int index, int x, int y, bool center)
				{
					portaRect rect;

					bool result = portaGetNewColorBox(index, x, y, rect, center);

					// Throw exception if result == false

					Rect^ r = gcnew Rect();
					r->Top = rect.top;
					r->Left = rect.left;
					r->Right = rect.right;
					r->Bottom = rect.bottom;

					return r;
				}
			
				int GetLinePosition(int index)
				{
					return portaGetLinePosition(index);
				}

				double GoToPosition(double angle)
				{
					return portaGoToPosition(angle);
				}

				double StepMotor(bool cw)
				{
					return portaStepMotor(cw);
				}

				double StepMotor(bool cw, int steps)
				{
					return portaStepMotor(cw, steps);
				}

				void SetMotorHomeParams(int framesOffCenter)
				{
					return portaSetMotorHomeParams(framesOffCenter);
				}

				void SetMotorActive(bool run)
				{
					return portaSetMotorActive(run);
				}
		};
	}
}
