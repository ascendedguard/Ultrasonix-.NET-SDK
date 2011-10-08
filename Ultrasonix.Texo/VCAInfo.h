#pragma once

public ref class VCAInformation
{
private:
    int amplification;
    int activetermination;
    int inclamp;
    int lpf;
    int lnaIntegratorEnable;
    int pgaIntegratorEnable;
    int hpfDigitalEnable;
    int hpfDigitalValue;

public: 
	VCAInformation()
    {
        amplification = 0;
        activetermination = 0;
        inclamp = 0;
        lpf = 0;
        lnaIntegratorEnable = 0;
        pgaIntegratorEnable = 0;
        hpfDigitalEnable = 0;
        hpfDigitalValue = 0;
    }

	property int Amplification
	{
		int get() { return this->amplification; }
		void set(int value) { this->amplification = value; }
	}

	property int ActiveTermination
	{
		int get() { return this->activetermination; }
		void set(int value) { this->activetermination = value; }
	}

	property int InClamp
	{
		int get() { return this->inclamp; }
		void set(int value) { this->inclamp = value; }
	}

	property int LPF
	{
		int get() { return this->lpf; }
		void set(int value) { this->lpf = value; }
	}

	property int LnaIntegratorEnable
	{
		int get() { return this->lnaIntegratorEnable; }
		void set(int value) { this->lnaIntegratorEnable = value; }
	}

	property int PgaIntegratorEnable
	{
		int get() { return this->pgaIntegratorEnable; }
		void set(int value) { this->pgaIntegratorEnable = value; }
	}

	property int HpfDigitalEnable
	{
		int get() { return this->hpfDigitalEnable; }
		void set(int value) { this->hpfDigitalEnable = value; }
	}

	property int HpfDigitalValue
	{
		int get() { return this->hpfDigitalValue; }
		void set(int value) { this->hpfDigitalValue = value; }
	}
};