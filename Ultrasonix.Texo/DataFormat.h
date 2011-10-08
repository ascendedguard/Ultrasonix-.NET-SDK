#pragma once

using namespace System;

namespace Ultrasonix
{
	namespace Texo
	{
		public enum class DataFormat
		{
			rfData = 0,
			envData = 1,
			envNoCompression,
			iqData,
			noSendData
		};
	}
}