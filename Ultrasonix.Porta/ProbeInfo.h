#pragma once

namespace Ultrasonix
{
	namespace Porta
	{
		public ref class ProbeInfo
		{
		private:
			/// The number of elements the probe has.
			int elements;
			/// The distance in microns between elements.
			int pitch;
			/// The radius of the probe if it is a convex or concave array.
			int radius;
			/// Specifies whether the probe has a motor or not.
			bool motorized;
			/// The field of view of the motor if applicable.
			int motorFov;
			/// The radius of the motor if applicable.
			int motorRadius;
			/// The number of steps in the motor sweep if applicable.
			int motorSteps;
			/// Specifies whether the motorized probe has a homing sensor
			bool motorHomeSensor;

		public:
			property int Elements
			{
				int get() { return elements; }
				void set(int value) { elements = value; }
			}

			property int Pitch
			{
				int get() { return pitch; }
				void set(int value) { pitch = value; }
			}

			property int Radius
			{
				int get() { return radius; }
				void set(int value) { radius = value; }
			}

			property bool Motorized
			{
				bool get() { return motorized; }
				void set(bool value) { motorized = value; }
			}

			property int MotorFieldOfView
			{
				int get() { return motorFov; }
				void set(int value) { motorFov = value; }
			}

			property int MotorRadius
			{
				int get() { return motorRadius; }
				void set(int value) { motorRadius = value; }
			}

			property int MotorSteps
			{
				int get() { return motorSteps; }
				void set(int value) { motorSteps = value; }
			}

			property bool MotorHasHomeSensor
			{
				bool get() { return motorHomeSensor; }
				void set(bool value) { motorHomeSensor = value; }
			}
		};
	}
}