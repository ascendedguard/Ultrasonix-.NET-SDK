#include "stdafx.h"
#include "Ultrasonix.Porta.h"

using namespace System;
using namespace Ultrasonix::Porta;

bool PreRunCallback(void* param)
{
	Porta^ porta = static_cast<Porta^>(GCHandle::operator GCHandle(IntPtr(param)).Target);
	porta->RaisePreRunning();

	return true;
}

bool RawDataCallback(void* param, unsigned char* data, int cineBlock, int header)
{
	Porta^ porta = static_cast<Porta^>(GCHandle::operator GCHandle(IntPtr(param)).Target);

	RawDataEventArgs^ args = gcnew RawDataEventArgs(IntPtr(data), cineBlock, header);
	porta->RaiseRawDataCallback(args);

	return true;
}