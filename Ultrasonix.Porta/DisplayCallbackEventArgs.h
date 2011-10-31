#pragma once

using namespace System;

namespace Ultrasonix
{
	namespace Porta
	{
		public ref class DisplayCallbackEventArgs : EventArgs
		{
		private:
			int cineBlock;
			int header;

		public: 
			DisplayCallbackEventArgs(int cineBlock, int header)
			{
				this->cineBlock = cineBlock;
				this->header = header;
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