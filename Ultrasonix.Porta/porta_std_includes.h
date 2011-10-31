////////////////////////////////////////////////////////////////////////////////
///@file porta_std_includes.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

/// Callback used when an image has finished processing.
///
///@param           prm Custom user parameter that is passed from the callback.
///@param           id The frame identifer originally from the cine buffer.
///@return          A boolean value. Reserved for future use.
///@sa              porta::setDisplayCallback()
typedef bool (*UTX_IMG_CALLBACK)(void* prm, int id, int header);

/// Callback used when raw ultrasound data has been acquired in cine memory.
///
///@param           prm Custom user parameter that is passed from the callback.
///@param           data Pointer to the cine data.
///@param           cineBlock The cine block index where the data came from.
///                 The number of imaging blocks depends on the current imaging mode.
///@param           header The data information header.
///@return          A boolean value. Reserved for future use.
///@sa              porta::setRawDataCallback()
///@sa              imagingMode for information on cine block indexing.
typedef bool (*UTX_ACQ_CALLBACK)(void* prm, unsigned char* data, int cineBlock, int header);

/// Callback used prior to running the image when Porta makes internal calls to the function.
///
typedef bool (*UTX_PRERUN_CALLBACK)(void* prm);

#ifdef COMPILE_PORTA_DEMO

class URect
{
public:

    int left;
    int top;
    int right;
    int bottom;
};

class UPoint
{
public:

    int x;
    int y;
};

class UCurve
{
public:
    // horizontal position of top point
    int t;
    // horizontal position of middle point
    int m;
    // horizontal position of bottom point
    int b;
    // vertical position of middle point
    int vm;
};

class UGainCurve
{
public:

    int val0;
    int val1;
    int val2;
    int val3;
    int val4;
    int val5;
    int val6;
    int val7;
};

enum EVariableType
{
    eVariableUnknown = -1,
    eInteger = 0,
    eFloat = 1,
    eString = 2,
    eGainCurve = 3,
    eRectangle = 4,
    eCurve = 5,
    eColor = 6,
    eBoolean = 7,
    eArray = 8,
    ePoint = 9,
    eScanConvert = 10,
    eVariableEnd
};

#endif
