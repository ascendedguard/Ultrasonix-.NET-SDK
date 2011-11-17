#pragma once

#include "porta_params_def.h"

using namespace System;

namespace Ultrasonix
{
	namespace Porta
	{
		public enum class ImagingParameter
		{
			BNumLines = prmBNumLines,
			BNumSamples = prmBNumSamples,
			RfNumLines = prmRfNumLines,
			RfNumSamples = prmRfNumSamples,
			ColorNumLines = prmColorNumLines,
			ColorNumSamples = prmColorNumSamples,
			VoltageLevelPos = prmVoltageLevelPos,
			VoltageLevelNeg = prmVoltageLevelNeg
		};
	}
}