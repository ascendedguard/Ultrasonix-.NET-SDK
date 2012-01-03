// This is the main DLL file.

#include "stdafx.h"

#include "Ultrasonix.Texo.h"

using namespace Ultrasonix::Texo;

int DataCallback(void* sender, unsigned char* data, int index)
{
	// Need to verify what the final int is used for.
	// It shouldn't be a frame index in Texo, and may not be necessary.
	// If it is necessary, the field should be added to TexoDataEventArgs.

	Texo^ texo = static_cast<Texo^>(GCHandle::operator GCHandle(IntPtr(sender)).Target);
	texo->RaiseDataReceived(IntPtr(data));

	return 1;
}