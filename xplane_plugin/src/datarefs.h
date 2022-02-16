
#ifndef DATAREF_H_INCLUDED
#define DATAREF_H_INCLUDED

#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMPlugin.h"
#include "XPLMPlanes.h"

int registerDataRefs();

int initDataRefs();
void unregisterData();

XPLMDataRef* createIntDR(const char* name, int* valuepointer);
XPLMDataRef* createIntArrayDR(const char* name, int* valuepointer, const char* type);
int createFloatDR(const char* name);
int createDoubleDR(const char* name);

void SetIntCB(void* inRefcon, int inValue);
int GetIntCB(void* inRefcon);

void SetIntArrayCB(void* inRefcon, int* inValue, int index, int len);
int GetIntArrayCB(void* inRefcon, int* outValue, int index, int len);

void SetFloatCB(void* inRefcon, float inValue);
float GetFloatCB(void* inRefcon);

void SetDoubleCB(void* inRefcon, double inValue);
double GetDoubleCB(void* inRefcon);

void createArrays(int nroflines);
void createIOKompInt(const char* name, const char* type, const char* sys, const char* lp, const char* es);
void createIOKompButton(const char* name, const char* type, const char* sys, const char* or, const char* es);

void ioFlightLoop();

#endif
