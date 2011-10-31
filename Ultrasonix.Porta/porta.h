#pragma once

#ifdef EXPORT_PORTA
    #define portalinkage __declspec(dllexport)
#elif defined IMPORT_PORTA
    #define portalinkage __declspec(dllimport)
#else
    #define portalinkage
#endif

#include "porta_std_includes.h"

class URect;
class UPoint;
class UCurve;
class UGainCurve;
enum EVariableType;
////////////////////////////////////////////////////////////////////////////////
/// Porta is an API that provides access to the native imaging modes
/// that run on the Ultrasonix ultrasound imaging electronics.
///
/// Features:
/// \li Simple communication with the ultrasound electronics
/// \li Full suite of clinical imaging modes
/// \li Support for all Ultrasonix transducers
/// \li Access to Ultrasonix clinical presets
/// \li Access to all imaging parameters
/// \li Up to 63 MB of cine storage
/// \li Manage and collect images in memory
/// \li Multiple data retrieval formats
///
/// Acquisition Control:
/// \li Start / stop imaging
/// \li Access to realtime data while imaging
/// \li Access to cine buffer data once frozen
/// \li I/O synchronization control
///
/// Data formats include:
/// \li Scan converted B (all modes)
/// \li Pre-scan converted B (all modes)
/// \li Scan converted Color (color/triplex modes)
/// \li Pre-scan converted Color (color/triplex modes)
/// \li M mode data (m mode)
/// \li Spectral Doppler data (PW/CW/triplex modes)
/// \li RF data (RF mode)
////////////////////////////////////////////////////////////////////////////////
class portalinkage porta
{
public:
    porta();
    ~porta();

    // initialization functions
    bool init(int cineSize, const char* firmwarePath, const char* settingsPath, const char* licensePath, const char* lutPath, int usm,
        int pci, int hv, int ddr,
        int channels);
    bool shutdown();
    bool isConnected() const;
    bool getLastError(char* err, unsigned int sz);
    bool getSystemId(char* sysid, unsigned int sz);

    // bnc functions
    bool signalBnc();
    bool pollBnc();
    bool ackBnc();

    // eprom functions
    int readEPROM(char* data, int sz);
    int writeEPROM(char* data, int sz);

    // probe functions
    bool selectProbe(int id) const;
    int getCurrentProbeID() const;
    int getProbeID(int connector) const;
    bool activateProbeConnector(int connector);
    bool getProbeName(char* name, int sz, int id) const;
    bool getProbeInfo(probeInfo& nfo);

    // testing functions
    bool testElectronicComponent(int id);
    bool testProbeElements(int connector, double* elementData, int probe = -1, int freq = 0, int depth = 20000, double gain = 0.80,
        int aquisitionTime = 2000);
    void setPowerTestParams(int tmout, int numQuickLevels, int* levels);
    bool readPowerValues(double* pv, double* nv, double* pav, double* nav, int& tmout, double& tol);

    // acquisition functions
    bool runImage();
    bool stopImage();
    bool isImaging() const;
    void setPreRunCallback(UTX_PRERUN_CALLBACK fn, void* prm);

    bool setWatchdog(int timeout);
    bool setCustomDMA(bool enable, int physicaladdress, int size);
    bool setSleepDelay(int delay);

    // imaging mode functions
    bool initImagingMode(imagingMode mode);
    imagingMode getCurrentMode() const;

    // data acquisition functions
    bool setDisplayCallback(int index, UTX_IMG_CALLBACK fn, void* prm);
    void setRawDataCallback(UTX_ACQ_CALLBACK fn, void* prm);
    int getFrameCount(unsigned int index);
    const unsigned char* getFrameAddress(int index, int frame);
    unsigned char* getCineStart(unsigned int blockid);
    void clearCine();
    int getFrameSize(int displayIndex);

    // preset functions
    bool loadMasterPreset();
    bool loadPreset(const char* path);
    bool savePreset(const char* path, bool overwrite);
    bool isMasterPreset() const;
    bool isFactoryPreset() const;
    bool findMasterPreset(char* path, int sz, int probe) const;
    bool isMasterPreset(const char* path) const;
    bool getPresetProbeID(const char* path, int& probeid1, int& probeid2, int& probeid3) const;

    // frame rate functions
    int getFrameRate() const;
    int getDataFrameRate() const;

    // imaging parameter functions
    int getParam(imagingParams prm) const;
    int getParam(const char* prm) const;
    bool getParam(const char* prm, char* val, int sz) const;
    bool setParam(const char* prm, const char* val) const;
    bool getParam(const char* prm, int& val) const;
    bool setParam(const char* prm, int val);
    bool getParam(const char* prm, UPoint& val) const;
    bool setParam(const char* prm, UPoint val);
    bool getParam(const char* prm, URect& val) const;
    bool setParam(const char* prm, URect val);
    bool getParam(const char* prm, UCurve& val) const;
    bool setParam(const char* prm, UCurve val);
    bool getParam(const char* prm, UGainCurve& val) const;
    bool setParam(const char* prm, UGainCurve val);
    bool cycleParam(const char* prm, bool fwd, bool wrap = false);
    bool getParamMinMax(const char* prm, int& min, int& max) const;
    bool getParamType(const char* prm, EVariableType& type) const;
    bool getListParam(char* prm, unsigned int sz, int prmNum);
    int getNumParams();

    // display setup functions
    bool setDisplayDimensions(int index, int x, int y);
    bool getDisplayDimensions(int index, int& x, int& y) const;

    // processed data retrieval functions
    bool getPrescanBDimensions(int index, int &w, int &h) const;
    bool getBwImage(int index, unsigned char*& img, bool useChroma) const;
    bool getBwImagePrescan(int index, unsigned char*& img) const;
    bool getColorImage(int index, unsigned char*& img) const;
    bool getColorData(int index, unsigned char*& img, bool velocity, bool prescan, bool copy) const;
    bool getColorVV(int index, unsigned char*& img) const;
    int getDisplayFrameCount(int index);
    bool processCineImage(int index, int frame);

    // mapping functions
    bool importChromaMap(int index, const unsigned int* lut);
    bool importColorMap(int index, const unsigned int* lut);

    // image geometry functions
    bool getMicronsPerPixel(int index, int& mx, int& my) const;
    bool getPixelCoordinates(int index, int line, int sample, int& xOut, int& yOut, bool bColor, int addAngle = 0) const;
    bool getUltrasoundCoordinates(int index, int x, int y, int& xOut, int& yOut, bool bColor, int addAngle = 0) const;

    // graphical image functions
    bool getHorizontalArcRect(int index, URect& arcRect, bool color, bool top, int sampleOverride = 0) const;
    bool getROI(int index, int* xOut, int* yOut) const;
    bool getColorBox(int index, int* xOut, int* yOut) const;
    bool getNewColorBox(int index, int x, int y, URect& colorBox, bool center);
    int getLinePosition(int index) const;

    // motor functions
    double goToPosition(double angle);
    double stepMotor(bool cw, int steps = 8);
    void setMotorHomeParams(int framesOffCenter);
    void setMotorActive(bool run);
};
