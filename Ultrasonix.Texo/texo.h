#pragma once

#ifdef EXPORT_TEXO
    #define texoL __declspec(dllexport)
#elif defined IMPORT_TEXO
    #define texoL __declspec(dllimport)
#else
    #define texoL
#endif

#include "texo_def.h"

#ifdef __cplusplus
extern "C" {
#endif

texoL int texoInit(const char* firmwarePath, int pci, int usm, int hv, int channels, int cw = 0, int szCine = 128);
texoL void texoShutdown();
texoL int texoIsInitialized();
texoL int texoIsImaging();

texoL int texoActivateProbeConnector(int connector);
texoL int texoSelectProbe(int id);
texoL int texoGetProbeName(int connector, char* name, int len);
texoL int texoGetProbeCode(int connector);
texoL int texoGetProbeNumElements();
texoL int texoGetProbeCenterFreq();
texoL int texoGetProbeHasMotor();
texoL int texoGetProbeFOV();

texoL int texoBeginSequence();
texoL int texoAddLine(_texoTransmitParams txPrms, _texoReceiveParams rxPrms, _texoLineInfo& lineInfo);
texoL int texoEndSequence();

texoL void texoClearTGCs();
texoL int texoAddTGCFixed(double percent);
texoL int texoAddTGC(_texoCurve* tgc, int depth);
texoL int texoAddReceive(_texoReceiveParams rxPrms);
texoL int texoAddTransmit(_texoTransmitParams txPrms);
texoL int texoSetPower(int power, int maxPositive, int maxNegative);
texoL void texoSetVCAInfo(_vcaInfo vcaInfo);

texoL int texoRunImage();
texoL int texoStopImage();
texoL void texoSetCallback(TEXO_CALLBACK fn, void* prm);

texoL double texoGetFrameRate();
texoL int texoGetFrameSize();
texoL int texoGetMaxFrameCount();
texoL int texoGetCollectedFrameCount();
texoL unsigned char* texoGetCineStart(unsigned int blockid);

texoL int texoSetDelayReadBack(const char* file);
texoL void texoCloseDelayReadBack();

texoL void texoSetSyncSignals(int input, int output, int output2);
texoL void texoEnableSyncNotify(int enable);

texoL int texoSetupMotor(int enable, int fpv, int spf);
texoL double texoGoToPosition(double angle);
texoL double texoStepMotor(int cw, int steps);

texoL void texoForceConnector(int conn);

#ifdef __cplusplus
}
#endif
