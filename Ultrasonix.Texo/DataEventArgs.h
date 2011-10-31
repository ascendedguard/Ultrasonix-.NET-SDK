#pragma once

namespace Ultrasonix
{
	namespace Texo
	{
		public ref class DataEventArgs : EventArgs
		{
			private:
				IntPtr data;

			public:
				DataEventArgs(IntPtr d)
				{
					this->data = d;
				}
			
				property IntPtr Data
				{
					IntPtr get() { return data; }
				}
		};
	}
}