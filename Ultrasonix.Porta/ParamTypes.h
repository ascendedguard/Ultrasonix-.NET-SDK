#pragma once

using namespace System;

namespace Ultrasonix
{
	namespace Porta
	{
		public ref class Rect
		{
			private:
				int left;
				int top;
				int right;
				int bottom;

			public:
				property int Left
				{
					int get() { return left; }
					void set(int value) { left = value; }
				}

				property int Top
				{
					int get() { return top; }
					void set(int value) { top = value; }
				}

				property int Right
				{
					int get() { return right; }
					void set(int value) { right = value; }
				}

				property int Bottom
				{
					int get() { return bottom; }
					void set(int value) { bottom = value; }
				}
		};

		public ref class Curve
		{
			private:
				int top;
				int middle;
				int bottom;
				int verticalMiddle;

			public:
				property int Top
				{
					int get() { return top; }
					void set(int value) { top = value; }
				}

				property int Middle
				{
					int get() { return middle; }
					void set(int value) { middle = value; }
				}

				property int Bottom
				{
					int get() { return bottom; }
					void set(int value) { bottom = value; }
				}

				property int VerticalMiddle
				{
					int get() { return verticalMiddle; }
					void set(int value) { verticalMiddle = value; }
				}
		};

		public ref class GainCurve
		{
			private:
				array<int>^ values;

			public:
				GainCurve()
				{
					values = gcnew array<int>(8);
				}

				property array<int>^ Values
				{
					array<int>^ get() { return values; }
				}

				property int Value0
				{
					int get() { return values[0]; }
					void set(int value) { values[0] = value; }
				}

				property int Value1
				{
					int get() { return values[1]; }
					void set(int value) { values[1] = value; }
				}

				property int Value2
				{
					int get() { return values[2]; }
					void set(int value) { values[2] = value; }
				}

				property int Value3
				{
					int get() { return values[3]; }
					void set(int value) { values[3] = value; }
				}

				property int Value4
				{
					int get() { return values[4]; }
					void set(int value) { values[4] = value; }
				}

				property int Value5
				{
					int get() { return values[5]; }
					void set(int value) { values[5] = value; }
				}

				property int Value6
				{
					int get() { return values[6]; }
					void set(int value) { values[6] = value; }
				}

				property int Value7
				{
					int get() { return values[7]; }
					void set(int value) { values[7] = value; }
				}
		};

		private ref class Size
		{
			private:
				int width;
				int height;

			public:
				property int Width
				{
					int get() { return width; }
					void set(int value) { width = value; }
				}

				property int Height
				{
					int get() { return height; }
					void set(int value) { height = value; }
				}

				Size()
				{
				}

				Size(int width, int height)
				{
					this->width = width;
					this->height = height;
				}
		};

		public ref class Point
		{
			private:
				int x;
				int y;

			public:
				property int X
				{
					int get() { return x; }
					void set(int value) { x = value; }
				}

				property int Y
				{
					int get() { return y; }
					void set(int value) { y = value; }
				}

				Point()
				{
				}

				Point(int x, int y)
				{
					this->x = x;
					this->y = y;
				}
		};

		public enum class VariableType
		{
			Unknown = -1,
			Integer = 0,
			Float = 1,
			String = 2,
			GainCurve = 3,
			Rectangle = 4,
			Curve = 5,
			Color = 6,
			Boolean = 7,
			Array = 8,
			Point = 9,
			ScanConvert = 10,
			VariableEnd
		};
	}
}