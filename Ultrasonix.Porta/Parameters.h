#pragma once

using namespace System;

namespace Ultrasonix
{
	namespace Porta
	{
		public ref class Parameters
		{
		public:
			static const String^ AcousticPower = prmAcousticPower;
			static const String^ BImageDepth = prmBImageDepth;
			static const String^ BBImageDepth = prmBBImageDepth;
			static const String^ B4ImageDepthA = prm4BImageDepthA;
			static const String^ B4ImageDepthB = prm4BImageDepthB;
			static const String^ Zoom = prmZoom;
			static const String^ TGC = prmTGC;
			static const String^ Brightness = prmBrightness;
			static const String^ Contrast = prmContrast;
			static const String^ Gamma = prmGamma;
			static const String^ BMap = prmBMap;
			static const String^ BSector = prmBSector;
			static const String^ BBSector = prmBBSector;
			static const String^ B4SectorA = prm4BSectorA;
			static const String^ B4SectorB = prm4BSectorB;
			static const String^ BPersistence = prmBPersist;
			static const String^ BDynRange = prmBDynRange;
			static const String^ BSteer = prmBSteer;
			static const String^ BGain = prmBGain;
			static const String^ BFocusDepth = prmBFocusDepth;
			static const String^ BFocusCount = prmBFocusCount;
			static const String^ BFocusSpacing = prmBFocusSpacing;
			static const String^ BImageOptimization = prmBImageOpt;
			static const String^ BMru = prmBMru;
			static const String^ BExtensionAngle = prmBExtensionAngle;
			static const String^ BxFocusCount = prmBxFocusCount;
			static const String^ DualActiveDisplay = prmDualActiveDisplay;
			static const String^ QuadActiveDisplay = prmQuadActiveDisplay;
			static const String^ MMode = prmMMode;
			static const String^ MZoom = prmMZoom;
			static const String^ MPosition = prmMPos;
			static const String^ MDepth = prmMDepth;
			static const String^ MSweep = prmMSweep;
			static const String^ MSteer = prmMSteer;
			static const String^ PwActiveDisplay = prmPwActiveDisplay;
			static const String^ PwGatePosition = prmPwGatePos;
			static const String^ PwGateDepth = prmPwGateDepth;
			static const String^ PwGateSize = prmPwGateSize;
			static const String^ PwGain = prmPwGain;
			static const String^ PwInvert = prmPwInvert;
			static const String^ PwSteer = prmPWSteer;
			static const String^ PwSweep = prmPwSweep;
			static const String^ PwTxFreq = prmPwTxFreq;
			static const String^ PwBaseline = prmPwBaseline;
			static const String^ PwPrp = prmPwPrp;
			static const String^ ColorBox = prmColorBox;
			static const String^ ColorSteer = prmColorSteer;
			static const String^ ColorPersist = prmColorPersist;
			static const String^ ColorGain = prmColorGain;
			static const String^ ColorInvert = prmColorInvert;
			static const String^ ColorTxFreq = prmColorTxFreq;
			static const String^ ColorTxFreqPulsed = prmColorTxFreqPulsed;
			static const String^ ColorEnsemble = prmColorEnsemble;
			static const String^ ColorMode = prmColorMode;
			static const String^ ColorPrp = prmColorPrp;
			static const String^ TriplexActiveDisplay = prmTriplexActiveDisplay;
			static const String^ BSamplingFreq = prmBSamplingFreq;
			static const String^ BLineDensity = prmBLineDensity;
			static const String^ BImageRect = prmBImageRect;
			static const String^ BBImageRect = prmBBImageRect;
			static const String^ B4ImageRectA = prm4BImageRectA;
			static const String^ B4ImageRectB = prm4BImageRectB;
			static const String^ PowerPositive = prmPowerPositive;
			static const String^ PowerNegative = prmPowerNegative;
			static const String^ BSampleSize = prmBSampleSize;
			static const String^ RfMode = prmRfMode;
			static const String^ RfDecimation = prmRfDecim;
			static const String^ MotorFrames = prmMotorFrames;
			static const String^ MotorSteps = prmMotorSteps;
			static const String^ MotorStatus = prmMotorStatus;
		};
	}
}