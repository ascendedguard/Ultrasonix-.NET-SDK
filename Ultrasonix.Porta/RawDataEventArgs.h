#pragma once

using namespace System;

namespace Ultrasonix
{
	namespace Porta
	{
		public ref class RawDataEventArgs : EventArgs
		{
		private:
			IntPtr data;
			int cineBlock;
			int header;

		public: 
			RawDataEventArgs(IntPtr data, int cineBlock, int header)
			{
				this->data = data;
				this->cineBlock = cineBlock;
				this->header = header;
			}

			property IntPtr Data
			{
				IntPtr get() { return data; }
			}

			property int CineBlock
			{
				int get() { return cineBlock; }
			}

			property int Header
			{
				int get() { return header; }
			}
		};
	}
}