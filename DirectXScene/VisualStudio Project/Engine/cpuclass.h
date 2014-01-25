// Based on tutorials from http://www.rastertek.com
// For core module AG1101A - Programming for Games, DirectX Coursework, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
///////////////////////////////////////////////////////////////////////////////
// Filename: cpuclass.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _CPUCLASS_H_
#define _CPUCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "pdh.lib")


//////////////
// INCLUDES //
//////////////
#include <pdh.h>


///////////////////////////////////////////////////////////////////////////////
// Class name: CpuClass
///////////////////////////////////////////////////////////////////////////////
class CpuClass
{
public:
	CpuClass();
	CpuClass(const CpuClass&);
	~CpuClass();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();

private:
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};

#endif