#pragma once

using namespace System;

namespace Ultrasonix
{
	namespace Porta
	{
		public ref class PortaImagingException : Exception
		{
		public:
			PortaImagingException() 
			{ 
			}

			PortaImagingException(String^ message) : Exception(message)
			{ 
			}
		};
	}
}