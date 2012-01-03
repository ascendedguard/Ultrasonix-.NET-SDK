#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Ultrasonix
{
	namespace Texo
	{
		public ref class TransmitParameters
		{
			private:
				int centerElement;
				int aperture;
				int focusDistance;
				int angle;
				int frequency;
				String^ pulseShape;
				int speedOfSound;
				bool useManualDelays;
				array<int>^ manualDelays;
				int tableIndex;
				bool useMask;
				array<int>^ mask;
				bool sync;

			public:
				TransmitParameters()
				{
					manualDelays = gcnew array<int>(129);
					mask = gcnew array<int>(128);
				}

				/// <summary> Gets or sets the center element, in 1/10th of an element. </summary>
				property int CenterElement
				{
					int get() { return this->centerElement; }
					void set(int value) { this->centerElement = value; }
				}

				/// <summary> Gets or sets the aperture size, in elements. </summary>
				property int Aperture
				{
					int get() { return this->aperture; }
					void set(int value) { this->aperture = value; }
				}

				/// <summary> Gets or sets the focus distance, in microns. </summary>
				property int FocusDistance
				{
					int get() { return this->focusDistance; }
					void set(int value) { this->focusDistance = value; }
				}

				/// <summary> Gets or sets the angle, in 1/1000th of a degree. </summary>
				property int Angle
				{
					int get() { return this->angle; }
					void set(int value) { this->angle = value; }
				}

				/// <summary> Gets or sets the frequency, in Hz. </summary>
				property int Frequency
				{
					int get() { return this->frequency; }
					void set(int value) { this->frequency = value; }
				}

				/// <summary> Gets or sets the pulse shape in codes of '+', '-', or '0'. </summary>
				property String^ PulseShape
				{
					String^ get() { return this->pulseShape; }
					void set(String^ value) { this->pulseShape = value; }
				}

				property int SpeedOfSound
				{
					int get() { return this->speedOfSound; }
					void set(int value) { this->speedOfSound = value; }
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

				property int TableIndex
				{
					int get() { return this->tableIndex; }
					void set(int value) { this->tableIndex = value; }
				}

				property bool UseMask
				{
					bool get() { return this->useMask; }
					void set(bool value) { this->useMask = value; }
				}

				property array<int>^ Mask
				{
					array<int>^ get() { return this->mask; }
				}

				property bool Sync
				{
					bool get() { return this->sync; }
					void set(bool value) { this->sync = value; }
				}

			internal:

				_texoTransmitParams ConvertToC()
				{
					_texoTransmitParams tx;

					tx.centerElement = this->CenterElement;
					tx.aperture = this->Aperture;
					tx.focusDistance = this->FocusDistance;
					tx.angle = this->Angle;
					tx.frequency = this->Frequency;

					IntPtr pPulse = Marshal::StringToHGlobalAnsi(this->PulseShape);

					strcpy_s(tx.pulseShape, MAXPULSESHAPESZ, (char*)pPulse.ToPointer());

					Marshal::FreeHGlobal(pPulse);
			
					tx.speedOfSound = this->SpeedOfSound;
					tx.useManualDelays = this->UseManualDelays;

					array<int>^ delays = this->ManualDelays;

					// Incoming array must have at least 129 elements
					for(int i = 0; i < 129; i++)
					{
						tx.manualDelays[i] = delays[i];
					}

					tx.tableIndex = this->TableIndex;
					tx.useMask = this->UseMask;

					array<int>^ el = this->Mask;

					for(int i = 0 ; i < 128; i++)
					{
						tx.mask[i] = el[i];
					}

					tx.sync = this->Sync;

					return tx;
				}
		};
	}
}