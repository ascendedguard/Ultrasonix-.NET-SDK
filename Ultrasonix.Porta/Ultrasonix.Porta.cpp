#include "stdafx.h"
#include "Ultrasonix.Porta.h"

using namespace System;
using namespace Ultrasonix::Porta;

int PreRunCallback(void* param)
{
	Porta^ porta = static_cast<Porta^>(GCHandle::operator GCHandle(IntPtr(param)).Target);
	porta->RaisePreRunning();

	return 0;
}

int RawDataCallback(void* param, unsigned char* data, int cineBlock, int header)
{
	Porta^ porta = static_cast<Porta^>(GCHandle::operator GCHandle(IntPtr(param)).Target);

	RawDataEventArgs^ args = gcnew RawDataEventArgs(IntPtr(data), cineBlock, header);
	porta->RaiseRawDataCallback(args);

	return 0;
}

int DisplayCallback(void* param, int id, int header)
{
	Porta^ porta = static_cast<Porta^>(GCHandle::operator GCHandle(IntPtr(param)).Target);

	DisplayCallbackEventArgs^ args = gcnew DisplayCallbackEventArgs(id, header);
	porta->RaiseDisplayImageReceived(args);

	return 0;
}