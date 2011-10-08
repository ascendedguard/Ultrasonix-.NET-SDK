#pragma once

using namespace System;

namespace Ultrasonix
{
	namespace Texo
	{
		public ref class ReceiveParameters
		{
			private:
				int centerElement;
				int aperture;
				int angle;
				int maxApertureDepth;
				int acquisitionDepth;
				int saveDelay;
				int speedOfSound;
				array<int>^ channelMask; // 2 elements
				bool applyFocus;
				bool useManualDelays;
				array<int>^ manualDelays; // 65 elements
				int customLineDuration;
				int lgcValue;
				int tgcSel;
				int tableIndex;
				int decimation;
				int numChannels;
				Curve^ rxAprCrv;

			public:
				ReceiveParameters()
				{
					channelMask = gcnew array<int>(2);
					manualDelays = gcnew array<int>(65);
					rxAprCrv = gcnew Curve();
				}

				property int CenterElement
				{
					int get() { return this->centerElement; }
					void set(int value) { this->centerElement = value; }
				}

				property int Aperture
				{
					int get() { return this->aperture; }
					void set(int value) { this->aperture = value; }
				}

				property int Angle
				{
					int get() { return this->angle; }
					void set(int value) { this->angle = value; }
				}

				property int MaxApertureDepth
				{
					int get() { return this->maxApertureDepth; }
					void set(int value) { this->maxApertureDepth = value; }
				}

				property int AcquisitionDepth
				{
					int get() { return this->acquisitionDepth; }
					void set(int value) { this->acquisitionDepth = value; }
				}

				property int SaveDelay
				{
					int get() { return this->saveDelay; }
					void set(int value) { this->saveDelay = value; }
				}

				property int SpeedOfSound
				{
					int get() { return this->speedOfSound; }
					void set(int value) { this->speedOfSound = value; }
				}

				property array<int>^ ChannelMask
				{
					array<int>^ get() { return this->channelMask; }
				}

				property bool ApplyFocus
				{
					bool get() { return this->applyFocus; }
					void set(bool value) { this->applyFocus = value; }
				}

				property bool UseManualDelays
				{
					bool get() { return this->useManualDelays; }
					void set(bool value) { this->useManualDelays = value; }
				}

				property array<int>^ ManualDelays
				{
					array<int>^ get() { return this->manualDelays; }
				}

				property int CustomLineDuration
				{
					int get() { return this->customLineDuration; }
					void set(int value) { this->customLineDuration = value; }
				}

				property int LgcValue
				{
					int get() { return this->lgcValue; }
					void set(int value) { this->lgcValue = value; }
				}

				property int TGCSelection
				{
					int get() { return this->tgcSel; }
					void set(int value) { this->tgcSel = value; }
				}

				property int TableIndex
				{
					int get() { return this->tableIndex; }
					void set(int value) { this->tableIndex = value; }
				}

				property int Decimation
				{
					int get() { return this->decimation; }
					void set(int value) { this->decimation = value; }
				}

				property int NumChannels
				{
					int get() { return this->numChannels; }
					void set(int value) { this->numChannels = value; }
				}

				property Curve^ ReceiveApertureCurve
				{
					Curve^ get() { return this->rxAprCrv; }
				}

			internal:

				texoReceiveParams ConvertToC()
				{
					texoReceiveParams rx;

					rx.centerElement = this->CenterElement;
					rx.aperture = this->Aperture;
					rx.angle = this->Angle;
					rx.maxApertureDepth = this->MaxApertureDepth;
					rx.acquisitionDepth = this->AcquisitionDepth;
					rx.saveDelay = this->SaveDelay;
					rx.speedOfSound = this->SpeedOfSound;
			
					array<int>^ cm = this->ChannelMask;

					for(int i = 0; i < cm->Length; i++)
					{
						rx.channelMask[i] = cm[i];
					}

					rx.applyFocus = this->ApplyFocus;
					rx.useManualDelays = this->UseManualDelays;

					array<int>^ md = this->ManualDelays;

					for(int i = 0; i < md->Length; i++)
					{
						rx.manualDelays[i] = md[i];
					}

					rx.customLineDuration = this->CustomLineDuration;
					rx.lgcValue = this->LgcValue;
					rx.tgcSel = this->TGCSelection;
					rx.tableIndex = this->TableIndex;
					rx.decimation = this->Decimation;
					rx.numChannels = this->NumChannels;

					rx.rxAprCrv.top = this->ReceiveApertureCurve->Top;
					rx.rxAprCrv.mid = this->ReceiveApertureCurve->Middle;
					rx.rxAprCrv.btm = this->ReceiveApertureCurve->Bottom;
					rx.rxAprCrv.vmid = this->ReceiveApertureCurve->VerticalMiddle;
				
					return rx;
				}
		};
	}
}