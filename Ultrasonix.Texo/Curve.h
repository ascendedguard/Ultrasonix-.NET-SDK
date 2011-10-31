#pragma once

namespace Ultrasonix
{
	namespace Texo
	{
		public ref class Curve
		{
			private:
				int top;
				int mid;
				int btm;
				int vmid;

			public:
				property int Top
				{
					int get() { return this->top; }
					void set(int value) { this->top = value; }
				}

				property int Middle
				{
					int get() { return this->mid; }
					void set(int value) { this->mid = value; }
				}

				property int Bottom
				{
					int get() { return this->btm; }
					void set(int value) { this->btm = value; }
				}

				property int VerticalMiddle
				{
					int get() { return this->vmid; }
					void set(int value) { this->vmid = value; }
				}
		};
	}
}