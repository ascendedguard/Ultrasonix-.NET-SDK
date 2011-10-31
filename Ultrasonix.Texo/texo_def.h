#pragma once

////////////////////////////////////////////////////////////////////////////////
/// Callback for when a frame has been captured.
////////////////////////////////////////////////////////////////////////////////
typedef bool (*TEXO_CALLBACK)(void*, unsigned char*, int);

////////////////////////////////////////////////////////////////////////////////
/// Data definition for programming the TGC.
////////////////////////////////////////////////////////////////////////////////
class CURVE
{
public:
    CURVE() { top = mid = btm = vmid = 0; }
    int top;
    int mid;
    int btm;
    int vmid;
};

////////////////////////////////////////////////////////////////////////////////
/// Data definition for programming the VCA.
////////////////////////////////////////////////////////////////////////////////
class VCAInfo
{
public:
    int amplification;
    int activetermination;
    int inclamp;
    int LPF;
    int lnaIntegratorEnable;
    int pgaIntegratorEnable;
    int hpfDigitalEnable;
    int hpfDigitalValue;

    VCAInfo()
    {
        amplification = 0;
        activetermination = 0;
        inclamp = 0;
        LPF = 0;
        lnaIntegratorEnable = 0;
        pgaIntegratorEnable = 0;
        hpfDigitalEnable = 0;
        hpfDigitalValue = 0;
    }
};

#define MAXPULSESHAPESZ 96

////////////////////////////////////////////////////////////////////////////////
/// The available data formats.
////////////////////////////////////////////////////////////////////////////////
enum texoDataFormat
{
    /// RF (16 bits @ 40MHz with no decimation)
    rfData = 0,
    /// envelope detected data with compression (8 bits)
    envData = 1,
    /// envelope detected data without compression (16 bits)
    envNoCompression,
    /// IQ data (32 bits I=16 bits, Q=16 bits)
    iqData,
    /// no data sent
    noSendData
};

////////////////////////////////////////////////////////////////////////////////
/// Transmit parameters for a scanline.
////////////////////////////////////////////////////////////////////////////////
class texoTransmitParams
{
public:
    /// center element in 1/10th of an element
    int centerElement;
    /// aperture size in elements
    int aperture;
    /// focus distance in microns
    int focusDistance;
    /// angle in 1/1000th of a degree
    int angle;
    /// frequency in Hz
    int frequency;
    /// pulse shape in codes of '+', '-', or '0'
    char pulseShape[MAXPULSESHAPESZ];
    /// speed of sound
    int speedOfSound;
    /// manual time delay usage (true = on, false = off)
    bool useManualDelays;
    /// time delays for up to 128 elements, with the maximum delay at the end
    int manualDelays[129];
    /// transmit table index
    int tableIndex;
    /// dead elements usage (true = on, false = off)
    bool useDeadElements;
    /// array of dead elements (0 = dead, 1 = fire)
    int deadElements[128];
    // use transmit extension, which enables continuous transmit over a certain line duration
    // no receive is used for this case.
    bool trex;
};

////////////////////////////////////////////////////////////////////////////////
/// Receive parameters for a scanline.
////////////////////////////////////////////////////////////////////////////////
class texoReceiveParams
{
public:
    /// center element in 1/10th of an element
    int centerElement;
    /// aperture size in elements
    int aperture;
    /// angle in 1/1000th of a degree
    int angle;
    /// maximum aperture curve depth in microns
    int maxApertureDepth;
    /// acquisition depth in microns
    int acquisitionDepth;
    /// depth in microns to delay the saving of data
    int saveDelay;
    /// speed of sound in m/s
    int speedOfSound;
    /// channel masking for upto 64 elements (0 = on, 1 = off)
    int channelMask[2];
    /// computed time delay usage (true = on, false = off)
    bool applyFocus;
    /// manual time delay usage (true = on, false = off)
    bool useManualDelays;
    /// time delays for up to 64 elements, with the maximum delay at the end
    int manualDelays[65];
    /// custom line duration in ns
    int customLineDuration;
    /// digital gain value applied to entire scanline (0 - 4095 (12 bits) for hw V2)
    ///                                                  (0 - 16383 (14 bits) for hw V3 & V4)
    int lgcValue;
    /// tgc selection by index
    int tgcSel;
    /// receive table index
    int tableIndex;
    /// decimation value for RF or B mode data
    int decimation;
    /// number of channels (32 or 64)
    int numChannels;
    /// recieve aperture curve
    CURVE rxAprCrv;
    // the window type to use for the receive aperture
    int weightType;
    // custom apodization window (true = on, false = off)
    bool useCustomWindow;
    // custom apodization window
    unsigned char window[64];
};
