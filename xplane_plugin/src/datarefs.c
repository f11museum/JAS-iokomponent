#include "jasiokomp.h"

#include "datarefs.h"
#include <time.h>

XPLMDataRef drPitch;

int nrInList = 0;
int nrInButtonList = 0;

int iokompArraySize = 0;
int* iokomp_lo;
int* iokomp_lamp;
int* iokomp_di;
int* iokomp_knapp;
XPLMDataRef* iokomp_lpDR;
XPLMDataRef* iokomp_esDR;
XPLMDataRef* iokomp_orDR;

XPLMDataRef* createIntDR(const char* name, int* valuepointer) {

    // int* valuepointer;
    // valuepointer = malloc(sizeof(int));

    *valuepointer = 0;
    debugLog("pointer create .%s. value %d mem %d\n", name, nrInList, valuepointer);
    XPLMDataRef customDR = XPLMRegisterDataAccessor(name,
                                                    xplmType_Int, // The types we support
                                                    1,            // Writable
                                                    GetIntCB,
                                                    SetIntCB, // Integer accessors
                                                    NULL,
                                                    NULL, // Float accessors
                                                    NULL,
                                                    NULL, // Doubles accessors
                                                    NULL,
                                                    NULL, // Int array accessors
                                                    NULL,
                                                    NULL, // Float array accessors
                                                    NULL,
                                                    NULL, // Raw data accessors
                                                    valuepointer,
                                                    valuepointer); // Refcons
    // Find and intialize our dataref

    customDR = XPLMFindDataRef(name);
    if (customDR == NULL) {
        return NULL;
    } else {
        XPLMSetDatai(customDR, 0);
        debugLog("Created int DR: %s\n", name);
        return customDR;
    }
}

int createFloatDR(const char* name) {

    float* valuepointer;
    valuepointer = malloc(sizeof(float));
    *valuepointer = 0.0;
    XPLMDataRef customDR = XPLMRegisterDataAccessor(name,
                                                    xplmType_Float, // The types we support
                                                    1,              // Writable
                                                    NULL,
                                                    NULL, // Integer accessors
                                                    GetFloatCB,
                                                    SetFloatCB, // Float accessors
                                                    NULL,
                                                    NULL, // Doubles accessors
                                                    NULL,
                                                    NULL, // Int array accessors
                                                    NULL,
                                                    NULL, // Float array accessors
                                                    NULL,
                                                    NULL, // Raw data accessors
                                                    valuepointer,
                                                    valuepointer); // Refcons
    // Find and intialize our dataref
    customDR = XPLMFindDataRef(name);
    if (customDR == NULL)
        return -1;
    else {
        XPLMSetDataf(customDR, 0.0);
        debugLog("Created float DR: %s\n", name);
        return 1;
    }
}

int initDataRefs() {

    // createIntDR("drdb/test/working0");
    //
    // createFloatDR("drdb/test/working2");

    return 1;
}

void unregisterData() {
    //XPLMUnregisterDataAccessor(hueDR);
}

void SetIntCB(void* inRefcon, int inValue) {

    *((int*)inRefcon) = inValue;
}
int GetIntCB(void* inRefcon) {

    // debugLog("pointer get value %d mem %d\n", *((int*)inRefcon), inRefcon);
    // int out = *((int*)inRefcon);
    return *((int*)inRefcon);
}

void SetFloatCB(void* inRefcon, float inValue) {
    *((float*)inRefcon) = inValue;
}
float GetFloatCB(void* inRefcon) {
    return *((float*)inRefcon);
}

void createIOKompInt(const char* name, const char* type, const char* sys, const char* lp, const char* es) {
    char ioname[512];
    sprintf(ioname, "JAS/io/%s/lo/%s", sys, name);
    createIntDR(ioname, &iokomp_lo[nrInList]);
    char lampname[512];
    sprintf(lampname, "JAS/io/%s/lamp/%s", sys, name);
    createIntDR(lampname, &iokomp_lamp[nrInList]);

    //XPLMDataRef lpDR =
    iokomp_lpDR[nrInList] = XPLMFindDataRef(lp);
    if (iokomp_lpDR[nrInList] == NULL) {
        debugLog("ERROR: did not find lp %s\n", lp);
    } else {
        debugLog("found lp: %s\n", lp);
    }

    iokomp_esDR[nrInList] = XPLMFindDataRef(es);
    if (iokomp_esDR[nrInList] == NULL) {
        debugLog("ERROR: did not find es %s\n", es);
    } else {
        debugLog("found es: %s\n", lp);
    }
    nrInList++;
}

void createIOKompButton(const char* name, const char* type, const char* sys, const char* or, const char* es) {
    debugLog("createIOKompButton %s\n", name);
    char ioname[512];
    sprintf(ioname, "JAS/io/%s/di/%s", sys, name);
    createIntDR(ioname, &iokomp_di[nrInButtonList]);
    char knappname[512];
    sprintf(knappname, "JAS/io/%s/knapp/%s", sys, name);
    createIntDR(knappname, &iokomp_knapp[nrInButtonList]);

    //XPLMDataRef lpDR =
    iokomp_orDR[nrInButtonList] = XPLMFindDataRef(or);
    if (iokomp_lpDR[nrInButtonList] == NULL) {
        debugLog("ERROR: did not find or %s\n", or);
    } else {
        debugLog("found or: %s\n", or);
    }
    nrInButtonList++;
}

void createArrays(int nroflines) {
    iokompArraySize = nroflines;

    iokomp_lo = malloc(sizeof(int) * iokompArraySize);
    memset(iokomp_lo, 0, sizeof(int) * iokompArraySize);

    iokomp_lamp = malloc(sizeof(int) * iokompArraySize);
    memset(iokomp_lamp, 0, sizeof(int) * iokompArraySize);

    iokomp_di = malloc(sizeof(int) * iokompArraySize);
    memset(iokomp_di, 0, sizeof(int) * iokompArraySize);

    iokomp_knapp = malloc(sizeof(int) * iokompArraySize);
    memset(iokomp_knapp, 0, sizeof(int) * iokompArraySize);

    iokomp_lpDR = malloc(sizeof(XPLMDataRef) * iokompArraySize);
    memset(iokomp_lpDR, 0, sizeof(XPLMDataRef) * iokompArraySize);
    for (int i = 0; i < iokompArraySize; i++) {
        iokomp_lpDR[i] = NULL;
    }

    iokomp_esDR = malloc(sizeof(XPLMDataRef) * iokompArraySize);
    memset(iokomp_esDR, 0, sizeof(XPLMDataRef) * iokompArraySize);
    for (int i = 0; i < iokompArraySize; i++) {
        iokomp_esDR[i] = NULL;
    }

    iokomp_orDR = malloc(sizeof(XPLMDataRef) * iokompArraySize);
    memset(iokomp_orDR, 0, sizeof(XPLMDataRef) * iokompArraySize);
    for (int i = 0; i < iokompArraySize; i++) {
        iokomp_orDR[i] = NULL;
    }
}

void ioFlightLoop() {
    // debugLog(".flightloopstart %d.\n", nrInList);
    for (int i = 0; i < nrInList; i++) {
        // debugLog(".loop %d.", i);
        if (iokomp_lpDR[i] != NULL) {
            // debugLog(".ifnull true %d.", i);
            if (XPLMGetDatai(iokomp_lpDR[i]) == 1) {

                // debugLog(".flightloop lampprov %d.", i);
                iokomp_lo[i] = 1;
            } else {
                iokomp_lo[i] = iokomp_lamp[i];
            }
        } else {
            // debugLog(".ifnull false %d.", i);
            iokomp_lo[i] = iokomp_lamp[i];
        }
        if (iokomp_esDR[i] != NULL) {
            // debugLog(".ifnull true %d.", i);
            if (XPLMGetDatai(iokomp_esDR[i]) == 0) {
                iokomp_lo[i] = 0;
            }
        }
        // debugLog(".loop klart %d.\n", i);
    }

    for (int i = 0; i < nrInButtonList; i++) {
        if (iokomp_orDR[i] != NULL) {
            if (XPLMGetDatai(iokomp_orDR[i]) == 1) {
                // Gör ingen uppdatering med knapparna när OR är aktiv
            } else {
                iokomp_knapp[i] = iokomp_di[i];
            }
        } else {
            iokomp_knapp[i] = iokomp_di[i];
        }
    }
}