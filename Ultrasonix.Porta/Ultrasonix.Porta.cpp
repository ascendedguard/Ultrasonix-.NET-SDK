#include "stdafx.h"
#include "Ultrasonix.Porta.h"

using namespace System;
using namespace Ultrasonix::Porta;

bool PreRunCallback(void* param)
{
	Porta^ texo = static_cast<Porta^>(GCHandle::operator GCHandle(IntPtr(param)).Target);
	texo->RaisePreRunning();

	return true;
}