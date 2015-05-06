/// \file esfile.h
/// \brief A utility library for File.
//
#ifndef ESFILE_H
#define ESFILE_H

#include <esdef.h>
#include <cstdio>
#include <errno.h>

#ifdef ANDROID
#include <android/asset_manager.h>
typedef AAsset esFile;
#else
typedef FILE esFile;
#endif

#ifdef __APPLE__
#include "FileWrapper.h"
#endif

class ESFileWrapper {
private:
#ifdef ANDROID
    static int android_read(void* cookie, char* buf, int size);
    static int android_write(void* cookie, const char* buf, int size);
    static fpos_t android_seek(void* cookie, fpos_t offset, int whence);
    static int android_close(void* cookie);
#endif

public:
    // return file* or asset* according to platforms
    const static int FOPEN_DEFAULT_MODE = 0;
    const static int FOPEN_ABSOLUTE_MODE = 1;

#ifdef ANDROID
    // must be established when app init...
    static AAssetManager *androidAssetManager;
    static void setAssetmanager(AAssetManager* manager);
    static FILE* android_fopen(const char *fname, const char *mode, long *length = NULL);
#endif
    static FILE *esFopen(const char *fileName, const char* mode, int fOpenMode = FOPEN_DEFAULT_MODE);
    static esFile *esFileOpen(const char *fileName);
    static void esFileClose(esFile *pFile);
    static int esFileRead(esFile *pFile, int bytesToRead, void *buffer);
    static long esFileGetLength(esFile *pFile);
};

#endif
