// Ultrasonix.Porta.h

#pragma once

#include "Stdafx.h"
#include "ProbeInfo.h"
#include "ImagingMode.h"
#include "ParamTypes.h"
#include "RawDataEventArgs.h"
#include "DisplayCallbackEventArgs.h"

#include "PortaImagingException.h"

#include "ImagingParameter.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

bool PreRunCallback(void*);
bool RawDataCallback(void* param, unsigned char* data, int cineBlock, int header);
bool DisplayCallback(void* prm, int id, int header);

namespace Ultrasonix
{
	namespace Porta
	{
		public ref class Porta
		{
			private:
				porta* po;
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
					this->po = new porta();
					displayDimensions = gcnew Dictionary<int, Size^>();
				}
			
				~Porta()
				{
					delete po;

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
						result = po->init(cineSize,
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

				bool Shutdown()
				{
					return po->shutdown();
				}

				bool IsConnected()
				{
					return po->isConnected();
				}

				String^ GetLastError()
				{
					char* err = new char[128];

					bool result = po->getLastError(err, 128);

					// Throw exception if result == false

					String^ errStr = Marshal::PtrToStringAnsi(IntPtr(err));

					delete[] err;

					return errStr;
				}

				String^ GetSystemId()
				{
					char* sysId = new char[128];

					bool result = po->getLastError(sysId, 128);

					// Throw exception if result == false

					String^ sysIdStr = Marshal::PtrToStringAnsi(IntPtr(sysId));

					delete[] sysId;

					return sysIdStr;
				}

				bool SignalBnc()
				{
					return po->signalBnc();
				}

				bool PollBnc()
				{
					return po->pollBnc();
				}

				bool AckBnc()
				{
					return po->ackBnc();
				}

				array<char>^ ReadEPROM(int size)
				{
					char* d = new char[size];

					int result = po->readEPROM(d, size);

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

					int result = po->writeEPROM(d, size);

					delete[] d;

					return result;
				}

				bool SelectProbe(int id)
				{
					return po->selectProbe(id);
				}

				int GetCurrentProbeId()
				{
					return po->getCurrentProbeID();
				}

				int GetProbeId(int connector)
				{
					return po->getProbeID(connector);
				}

				bool ActivateProbeConnector(int connector)
				{
					return po->activateProbeConnector(connector);
				}

				String^ GetProbeName(int id)
				{
					char* probeName = new char[128];
					bool result = po->getProbeName(probeName, 128, id);

					// Throw exception if result is false.

					String^ probeNameStr = Marshal::PtrToStringAnsi(IntPtr(probeName));

					delete[] probeName;

					return probeNameStr;
				}

				ProbeInfo^ GetProbeInfo()
				{
					probeInfo info;

					po->getProbeInfo(info);

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
					return po->testElectronicComponent(id);
				}
			
				// TODO: Implement testProbeElements
				// TODO: Implement setPowerTestParams
				// TODO: Implement readPowerValues

				bool RunImage()
				{
					return po->runImage();
				}

				bool StopImage()
				{
					return po->stopImage();
				}

				bool IsImaging()
				{
					return po->isImaging();
				}

				bool SetWatchdog(int timeout)
				{
					return po->setWatchdog(timeout);
				}

				bool SetCustomDMA(bool enable, int physicaladdress, int size)
				{
					return po->setCustomDMA(enable, physicaladdress, size);
				}

				bool SetSleepDelay(int delay)
				{
					return po->setSleepDelay(delay);
				}

				bool InitImagingMode(ImagingMode mode)
				{
					return po->initImagingMode((imagingMode)mode);
				}

				ImagingMode GetCurrentMode()
				{
					return (ImagingMode)po->getCurrentMode();
				}
				
				void StartRawDataCallback()
				{
					IntPtr ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(objHandle);

					this->po->setRawDataCallback(RawDataCallback, ptr.ToPointer());
				}

				void StartPreRunCallback()
				{
					IntPtr ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(objHandle);

					this->po->setPreRunCallback(PreRunCallback, ptr.ToPointer());
				}

				bool StartDisplayCallback(int frameIndex)
				{
					IntPtr ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(objHandle);

					return po->setDisplayCallback(frameIndex, DisplayCallback, ptr.ToPointer());
				}

				// TODO: Implement event for DisplayCallback

				int GetFrameCount(int index)
				{
					return po->getFrameCount(index);
				}

				bool LoadMasterPreset()
				{
					return po->loadMasterPreset();
				}

				bool LoadPreset(String^ path)
				{
					IntPtr pPath = Marshal::StringToHGlobalAnsi(path);

					bool result = po->loadPreset((char*)pPath.ToPointer());

					Marshal::FreeHGlobal(pPath);

					return result;
				}

				bool SavePreset(String^ path, bool overwrite)
				{
					IntPtr pPath = Marshal::StringToHGlobalAnsi(path);

					bool result = po->savePreset((char*)pPath.ToPointer(), overwrite);

					Marshal::FreeHGlobal(pPath);

					return result;
				}

				bool IsMasterPreset()
				{
					return po->isMasterPreset();
				}

				bool IsFactoryPreset()
				{
					return po->isFactoryPreset();
				}

				String^ FindMasterPreset(int probe)
				{
					char* probePath = new char[512];

					po->findMasterPreset(probePath, 512, probe);

					String^ probePathStr = Marshal::PtrToStringAnsi(IntPtr(probePath));

					delete[] probePath;

					return probePathStr;
				}

				bool IsMasterPreset(String^ path)
				{
					IntPtr pPath = Marshal::StringToHGlobalAnsi(path);

					bool result = po->isMasterPreset((char*)pPath.ToPointer());

					Marshal::FreeHGlobal(pPath);

					return result;
				}

				bool GetPresetProbeID(String^ path, [Out] int% probeId1, [Out] int% probeId2, [Out] int% probeId3)
				{
					int id1, id2, id3;

					IntPtr pPath = Marshal::StringToHGlobalAnsi(path);

					bool result = po->getPresetProbeID((char*)pPath.ToPointer(), id1, id2, id3);

					Marshal::FreeHGlobal(pPath);

					probeId1 = id1;
					probeId2 = id2;
					probeId3 = id3;

					return result;
				}
				// TODO: Implement getPresetProbeID

				int GetFrameRate()
				{
					return po->getFrameRate();
				}

				int GetDataFrameRate()
				{
					return po->getDataFrameRate();
				}

				int GetParam(ImagingParameter param)
				{
					return po->getParam((imagingParams)param);
				}

				int GetParam(String^ param)
				{
					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);

					int result = po->getParam((char*)pParam.ToPointer());

					Marshal::FreeHGlobal(pParam);

					return result;
				}

				bool SetParam(String^ param, int value)
				{
					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);

					bool result = po->setParam((char*)pParam.ToPointer(), value);

					Marshal::FreeHGlobal(pParam);

					return result;
				}

				String^ GetParamString(String^ param)
				{
					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);

					char* buffer = new char[128];

					bool result = po->getParam((char*)pParam.ToPointer(), buffer, 128);

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

					bool result = po->setParam((char*)pParam.ToPointer(), (char*)pValue.ToPointer());

					Marshal::FreeHGlobal(pParam);
					Marshal::FreeHGlobal(pValue);

					return result;
				}

				Rect^ GetParamRect(String^ param)
				{
					URect r;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = po->getParam((char*)pParam.ToPointer(), r);
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
					URect r;

					r.top = value->Top;
					r.left = value->Left;
					r.right = value->Right;
					r.bottom = value->Bottom;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = po->setParam((char*)pParam.ToPointer(), r);
					Marshal::FreeHGlobal(pParam);

					return result;
				}

				Point^ GetParamPoint(String^ param)
				{
					UPoint p;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = po->getParam((char*)pParam.ToPointer(), p);
					Marshal::FreeHGlobal(pParam);

					Point^ point = gcnew Point();

					point->X = p.x;
					point->Y = p.y;

					return point;
				}

				bool SetParam(String^ param, Point^ value)
				{
					UPoint p;

					p.x = value->X;
					p.y = value->Y;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = po->setParam((char*)pParam.ToPointer(), p);
					Marshal::FreeHGlobal(pParam);

					return result;
				}

				Curve^ GetParamCurve(String^ param)
				{
					UCurve c;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = po->getParam((char*)pParam.ToPointer(), c);
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
					UCurve c;

					c.t = value->Top;
					c.m = value->Middle;
					c.b = value->Bottom;
					c.vm = value->VerticalMiddle;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = po->setParam((char*)pParam.ToPointer(), c);
					Marshal::FreeHGlobal(pParam);

					return result;
				}

				GainCurve^ GetParamGainCurve(String^ param)
				{
					UGainCurve c;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = po->getParam((char*)pParam.ToPointer(), c);
					Marshal::FreeHGlobal(pParam);

					GainCurve^ curve = gcnew GainCurve();

					curve->Values[0] = c.val0;
					curve->Values[1] = c.val1;
					curve->Values[2] = c.val2;
					curve->Values[3] = c.val3;
					curve->Values[4] = c.val4;
					curve->Values[5] = c.val5;
					curve->Values[6] = c.val6;
					curve->Values[7] = c.val7;

					return curve;
				}

				bool SetParam(String^ param, GainCurve^ value)
				{
					UGainCurve c;

					c.val0 = value->Values[0];
					c.val1 = value->Values[1];
					c.val2 = value->Values[2];
					c.val3 = value->Values[3];
					c.val4 = value->Values[4];
					c.val5 = value->Values[5];
					c.val6 = value->Values[6];
					c.val7 = value->Values[7];

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = po->setParam((char*)pParam.ToPointer(), c);
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
					bool result = po->cycleParam((char*)pParam.ToPointer(), forward, wrap);
					Marshal::FreeHGlobal(pParam);
				
					return result;
				}

				bool GetParamMinMax(String^ param, [Out] int% min, [Out] int% max)
				{
					int mn, mx;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = po->getParamMinMax((char*)pParam.ToPointer(), mn, mx);
					Marshal::FreeHGlobal(pParam);

					min = mn;
					max = mx;

					return result;
				}

				VariableType GetParamType(String^ param)
				{
					EVariableType t;

					IntPtr pParam = Marshal::StringToHGlobalAnsi(param);
					bool result = po->getParamType((char*)pParam.ToPointer(), t);
					Marshal::FreeHGlobal(pParam);

					return (VariableType)t;
				}

				String^ GetListParam(int prmNum)
				{
					char* name = new char[128];
					bool result = po->getListParam(name, 128, prmNum);

					String^ n = Marshal::PtrToStringAnsi(IntPtr(name));
					delete[] name;

					return n;
				}
		
				int GetNumParams()
				{
					return po->getNumParams();
				}

				bool SetDisplayDimensions(int index, int x, int y)
				{
					displayDimensions[index] = gcnew Size(x, y);
					return po->setDisplayDimensions(index, x, y);
				}

				bool GetDisplayDimensions(int index, [Out] int% x, [Out] int% y)
				{
					int xOut, yOut;

					bool result = po->getDisplayDimensions(index, xOut, yOut);

					x = xOut;
					y = yOut;

					return result;
				}

				bool GetPrescanBDimensions(int index, [Out] int% w, [Out] int% h)
				{
					int xOut, yOut;

					bool result = po->getPrescanBDimensions(index, xOut, yOut);

					w = xOut;
					h = yOut;

					return result;
				}

				bool GetBwImagePrescan(int index, IntPtr data)
				{
					unsigned char* d = (unsigned char*)data.ToPointer();
					return po->getBwImagePrescan(index, d);
				}

				array<Byte>^ GetBwImagePrescan(int index)
				{
					// This should be based on the set size, not necessarily hard-coded.
					int w, h;
					po->getPrescanBDimensions(index, w, h);
					
					int size = w * h;

					array<Byte>^ arr = gcnew array<Byte>(size);
					pin_ptr<Byte> p = &arr[0];
					unsigned char* data = p;

					bool result = po->getBwImagePrescan(index, data);

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

					bool result = po->getBwImage(index, data, useChroma);

					if (result == false)
					{
						throw gcnew PortaImagingException("GetBwImage failed to retrieve an image");
					}

					return arr;
				}

				bool GetBwImage(int index, bool useChroma, IntPtr data)
				{
					unsigned char* d = (unsigned char*)data.ToPointer();
					return po->getBwImage(index, d, useChroma);
				}

				array<Byte>^ GetColorImage(int index)
				{
					// This should be based on the set size, not necessarily hard-coded.
					Size^ s = displayDimensions[index];
					int size = s->Width * s->Height * 4;

					array<Byte>^ arr = gcnew array<Byte>(size);
					pin_ptr<Byte> p = &arr[0];
					unsigned char* data = p;

					bool result = po->getColorImage(index, data);

					if (result == false)
					{
						throw gcnew PortaImagingException("GetColorImage failed to retrieve an image");
					}

					return arr;
				}

				bool GetColorImage(int index, IntPtr data)
				{
					unsigned char* d = (unsigned char*)data.ToPointer();
					return po->getColorImage(index, d);
				}

				array<Byte>^ GetColorData(int index, bool velocity, bool prescan, bool copy)
				{
					Size^ s = displayDimensions[index];
					int size = s->Width * s->Height;

					array<Byte>^ arr = gcnew array<Byte>(size);
					pin_ptr<Byte> p = &arr[0];
					unsigned char* data = p;

					bool result = po->getColorData(index, data, velocity, prescan, copy);

					if (result == false)
					{
						throw gcnew PortaImagingException("GetColorImage failed to retrieve an image");
					}

					return arr;
				}

				bool GetColorData(int index, bool velocity, bool prescan, bool copy, IntPtr data)
				{
					unsigned char* d = (unsigned char*)data.ToPointer();
					return po->getColorData(index, d, velocity, prescan, copy);
				}

				array<Byte>^ GetColorVV(int index)
				{
					Size^ s = displayDimensions[index];
					int size = s->Width * s->Height * 4;

					array<Byte>^ arr = gcnew array<Byte>(size);
					pin_ptr<Byte> p = &arr[0];
					unsigned char* data = p;

					bool result = po->getColorVV(index, data);

					if (result == false)
					{
						throw gcnew PortaImagingException("GetColorImage failed to retrieve an image");
					}

					return arr;
				}

				bool GetColorVV(int index, IntPtr data)
				{
					unsigned char* d = (unsigned char*)data.ToPointer();
					return po->getColorVV(index, d);
				}

				int GetDisplayFrameCount(int index)
				{
					return po->getDisplayFrameCount(index);
				}

				bool ProcessCineImage(int index, int frame)
				{
					return po->processCineImage(index, frame);
				}

				bool ImportChromaMap(int index, array<int>^ lut)
				{
					int size = lut->Length;
					unsigned int* arr = new unsigned int[size];
					
					for(int i = 0; i < size; i++)
					{
						arr[i] = lut[i];
					}

					bool result = po->importChromaMap(index, arr);

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

					bool result = po->importColorMap(index, arr);

					delete[] arr;

					return result;
				}

				bool GetMicronsPerPixel(int index, [Out] int% mx, [Out] int% my)
				{
					int x, y;

					bool result = po->getMicronsPerPixel(index, x, y);

					mx = x;
					my = y;

					return result;
				}

				bool GetPixelCoordinates(int index, int line, int sample, [Out] int% xOut, [Out] int% yOut, bool bColor, int addAngle)
				{
					int x, y;

					bool result = po->getPixelCoordinates(index, line, sample, x, y, bColor, addAngle);

					xOut = x;
					yOut = y;

					return result;
				}

				bool GetUltrasoundCoordinates(int index, int x, int y, [Out] int% xOut, [Out] int% yOut, bool bColor, int addAngle)
				{
					int xo, yo;

					bool result = po->getUltrasoundCoordinates(index, x, y, xo, yo, bColor, addAngle);

					xOut = xo;
					yOut = yo;

					return result;
				}

				Rect^ GetHorizontalArcRect(int index, bool color, bool top, int sampleOverride)
				{
					URect rect;

					bool result = po->getHorizontalArcRect(index, rect, color, top, sampleOverride);

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

					bool result = po->getROI(index, &x, &y);

					xOut = x;
					yOut = y;

					return result;
				}

				bool GetColorBox(int index, [Out] int% xOut, [Out] int% yOut)
				{
					int x, y;

					bool result = po->getColorBox(index, &x, &y);

					xOut = x;
					yOut = y;

					return result;
				}

				Rect^ GetNewColorBox(int index, int x, int y, bool center)
				{
					URect rect;

					bool result = po->getNewColorBox(index, x, y, rect, center);

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
					return po->getLinePosition(index);
				}

				double GoToPosition(double angle)
				{
					return po->goToPosition(angle);
				}

				double StepMotor(bool cw)
				{
					return po->stepMotor(cw);
				}

				double StepMotor(bool cw, int steps)
				{
					return po->stepMotor(cw, steps);
				}

				void SetMotorHomeParams(int framesOffCenter)
				{
					return po->setMotorHomeParams(framesOffCenter);
				}

				void SetMotorActive(bool run)
				{
					return po->setMotorActive(run);
				}
		};
	}
}
