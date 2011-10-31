#pragma once

#ifdef EXPORT_TEXO
    #define texolinkage __declspec(dllexport)
#elif defined IMPORT_TEXO
    #define texolinkage __declspec(dllimport)
#else
    #define texolinkage
#endif

#include "texo_def.h"

////////////////////////////////////////////////////////////////////////////////
/// texo is an API that can build a custom sequence of individually programmable
/// scanlines.
///
/// Features:
/// - Takes full advantage of Ultrasonix programmable ultrasound engine
/// - Support for all Ultrasonix transducers
/// - 63 MB of cine storage
/// - Programmable with up to 2048 scanlines
/// - Blank lines supported for full PRF control
/// - Acquisition Control:
///   - Start / Stop imaging
///   - Access to realtime incoming data while imaging
///   - Access to cine buffer data once frozen
/// - I/O synchronization control
/// - Data formats include:
///   - RF (16 bit @ 40 MHz)
///   - Envelope Detected (16 bit @ 40 MHz)
///   - Envelope Detected (8 bit after custom compression table applied)
/// - Global Parameters:
///   - Up to TGC curves
///   - Positive and negative transmit power values
///   - Programmable compression table for 8 bit envelope detected data
/// - Transmit Scanline Parameters:
///   - Center element
///   - Aperture
///   - Focus distance
///   - Angle
///   - Frequency
///   - Pulse shape
///   - Support for manually entered time delays
/// - Receive Scanline Parameters:
///   - Center element
///   - Aperture
///   - Focus distance (*)
///   - Aperture opening maximum depth (*)
///   - Speed of sound (*)
///   - Acquisition depth
///   - Channel masking within aperture
///   - Boolean option to turn off time delays
///   - Support for manually entered time delays
///   - Custom line duration
///   - Scanline digital gain
///   - TGC selection
///
/// (*) for time delay calculations
////////////////////////////////////////////////////////////////////////////////
class texolinkage texo
{
public:
    texo();
    ~texo();

    bool init(const char* firmwarePath, int pci, int usm, int hv, int channels, int tx = 3, int szCine = 128);
    bool shutdown();

    bool isInitialized() const;
    bool isImaging() const;

    bool activateProbeConnector(int connector);
    bool getProbeName(int connector, char* name, int len);
    int getProbeCode(int connector);
    int getProbeNumElements();
    int getProbeCenterFreq();
    int getProbeFOV();

    bool beginSequence();
    int addLine(texoDataFormat format, texoTransmitParams txPrms, texoReceiveParams rxPrms);
    int endSequence();

    void clearTGCs();
    bool addTGC(double percent);
    bool addTGC(CURVE* tgc, int depth);
    bool addReceive(texoReceiveParams rxPrms);
    bool addTransmit(texoTransmitParams txPrms);
    bool loadCompressionTable(char* data, int size);
    bool setPower(int power, int maxPositive, int maxNegative);
    void setVCAInfo(VCAInfo vcaInfo);

    bool runImage();
    bool stopImage();
    void setCallback(TEXO_CALLBACK fn, void* prm);

    double getFrameRate();
    int getFrameSize();
    int getMaxFrameCount();
    int getCollectedFrameCount();
    unsigned char* getCineStart(unsigned int blockid);

    bool setDelayReadBack(const char* file);
    void closeDelayReadBack();

    bool setSyncSignals(int input, int output, int output2);
    bool enableSyncNotify(bool enable);

    bool setupMotor(bool enable, int fpv, int spf);
    double goToPosition(double angle);
    double stepMotor(bool cw, int steps);

    void forceConnector(int conn);
};
