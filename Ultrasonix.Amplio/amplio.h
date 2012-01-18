#pragma once

#ifdef EXPORT_AMPLIO
    #define ampliolinkage __declspec(dllexport)
#elif defined IMPORTAMPLIO
    #define ampliolinkage __declspec(dllimport)
#else
    #define ampliolinkage
#endif

////////////////////////////////////////////////////////////////////////////////
/// Amplio is an API that runs a similar processing chain as the Sonix ultrasound
/// scanners to convert a beamformed RF signal into envelope detected B data.
////////////////////////////////////////////////////////////////////////////////
class ampliolinkage amplio
{
public:
    amplio();
    virtual ~amplio();

    bool init(double* filterA, int numCoeffA, double* filterB, int numCoeffB, double* filterC, int numCoeffC,
        unsigned char* compressionTable,
        int comprSz);
    bool init(const char* path, int indexA, int indexB, int indexC);
    bool initIQ(int freqA, int freqB, int freqcutoff, int freqSampling, int length);
    bool apply(short* rfData, int length, bool sumAfterEnvelope, int useCompression, int decimation, unsigned char* envData);
    bool amplio::applyRF(short* rfData, int length, int decimation, short* filteredRfData);
    bool applyIQ(short* rfData, int useCompression, int scale, int decimation, unsigned char* envData);
private:

//    bool readLineOfTextFromFile(FILE* fp, char* str, unsigned int sz);
    void applyFIRfilter(short* rfInput, int length, double* FIRcoefs, int numCoefs, short* rfOutput);
    void envelopeDetection(short* rf, int length, short* env);
    bool applyDecimation(short* rfInput, int length, int decimation, short* rfOutput);
    bool applyDecimation(unsigned char* rfInput, int length, int decimation, unsigned char* rfOutput);
    bool applyCompression(short* env, int length, unsigned char* compressionMap, int sz, unsigned char* cmpEnv);
    void sum(short* a, short* b, short* c, int length, short* out);
    int power(int base, int exp);
    void fillCompressionTable(unsigned char* compressionMap, int sz, int cfg, int scale = 1);
    void resetAll();
    void generateFIRCoefs(double wn, int numCoefs, int mode, double* filter);
    void calcWindow(double* win, int length);
    double sinc(double x);
    void fillTable(int freqA, int freqB, int freqSampling, double* sineTable, double* cosineTable, int tableSize);
    void quadratureDemodulation(short* rfData, short* I, short* Q);
    void calculateMagnitude(short* I, short* Q, short* env);

private:

    int m_numCoeffA;
    int m_numCoeffB;
    int m_numCoeffC;
    int m_numCoeffIQ;
    int m_tableSize;

    double* m_filterA;
    double* m_filterB;
    double* m_filterC;
    double* m_filterIQ;
    double* m_sineTable;
    double* m_cosineTable;

    short* m_I;
    short* m_Q;
    short* m_fI;
    short* m_fQ;
    short* m_env;
    unsigned char* m_final;

    int m_comprSz;
    unsigned char* m_compressionMap;
};
