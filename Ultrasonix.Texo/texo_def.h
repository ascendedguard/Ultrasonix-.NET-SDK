#pragma once

////////////////////////////////////////////////////////////////////////////////
/// Callback for when a frame has been captured.
////////////////////////////////////////////////////////////////////////////////
typedef int (*TEXO_CALLBACK)(void*, unsigned char*, int);

////////////////////////////////////////////////////////////////////////////////
/// Data definition for programming the TGC.
////////////////////////////////////////////////////////////////////////////////
struct _texoCurve
{
    _texoCurve() { top = mid = btm = vmid = 0; }
    int top;
    int mid;
    int btm;
    int vmid;
};

////////////////////////////////////////////////////////////////////////////////
/// Data definition for programming the VCA.
////////////////////////////////////////////////////////////////////////////////
struct _vcaInfo
{
    int amplification;
    int activetermination;
    int inclamp;
    int LPF;
    int lnaIntegratorEnable;
    int pgaIntegratorEnable;
    int hpfDigitalEnable;
    int hpfDigitalValue;
};

#define MAXPULSESHAPESZ 96

////////////////////////////////////////////////////////////////////////////////
/// Information returned once a scanline is added to the sequence.
////////////////////////////////////////////////////////////////////////////////
struct _texoLineInfo
{
    /// size of the line in bytes
    int lineSize;
    /// duration of the line in microseconds
    int lineDuration;
};

////////////////////////////////////////////////////////////////////////////////
/// Transmit parameters for a scanline.
////////////////////////////////////////////////////////////////////////////////
struct _texoTransmitParams
{
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
    int useManualDelays;
    /// time delays for up to 128 elements, with the maximum delay at the end
    int manualDelays[129];
    /// transmit table index
    int tableIndex;
    /// mask elements usage (1 = on, 0 = off)
    int useMask;
    /// array to mask elements (0 = dead, 1 = fire)
    int mask[128];
    /// enable or disable the line sync (0 = off, 1 = on). note that texoSetSyncSignals() must be set
    /// with an output trigger = 1 in order for this rayline sync to take effect
    int sync;
};

////////////////////////////////////////////////////////////////////////////////
/// Receive parameters for a scanline.
////////////////////////////////////////////////////////////////////////////////
struct _texoReceiveParams
{
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
    int applyFocus;
    /// manual time delay usage (true = on, false = off)
    int useManualDelays;
    /// time delays for up to 64 elements, with the maximum delay at the end
    int manualDelays[65];
    /// custom line duration in nanoseconds
    int customLineDuration;
    /// digital gain value applied to entire scanline (0 - 4095 (12 bits) for hw V2)
    ///                                               (0 - 16383 (14 bits) for hw V3 & V4)
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
    _texoCurve rxAprCrv;
    // the window type to use for the receive aperture
    int weightType;
    // custom apodization window (true = on, false = off)
    int useCustomWindow;
    // custom apodization window
    unsigned char window[64];
};
