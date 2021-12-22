
#ifndef DATAREF_H_INCLUDED
#define DATAREF_H_INCLUDED

#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMPlugin.h"
#include "XPLMPlanes.h"

int registerDataRefs();

int initDataRefs();
void unregisterData();

int createIntDR(const char* name);
int createFloatDR(const char* name);

void SetIntCB(void* inRefcon, int inValue);
int GetIntCB(void* inRefcon);

void SetFloatCB(void* inRefcon, float inValue);
float GetFloatCB(void* inRefcon);

#endif
