#include "esfile.h"

#ifdef ANDROID
    AAssetManager *ESFileWrapper::androidAssetManager = NULL;

    void ESFileWrapper::setAssetmanager(AAssetManager* manager) {
        androidAssetManager = manager;
    }

    int ESFileWrapper::android_read(void* cookie, char* buf, int size) {
      return AAsset_read((AAsset*)cookie, buf, size);
    }

    int ESFileWrapper::android_write(void* cookie, const char* buf, int size) {
      return EACCES; // can't provide write access to the apk
    }

    fpos_t ESFileWrapper::android_seek(void* cookie, fpos_t offset, int whence) {
      return AAsset_seek((AAsset*)cookie, offset, whence);
    }

    int ESFileWrapper::android_close(void* cookie) {
      AAsset_close((AAsset*)cookie);
      return 0;
    }


    FILE* ESFileWrapper::android_fopen(const char* fname, const char* mode, long *length) {
      if(mode[0] == 'w') return NULL;

      AAsset* asset = AAssetManager_open( androidAssetManager, fname, 0);
      if(!asset) return NULL;

      if(length != NULL) {
          *length = AAsset_getLength(asset);
      }

      return funopen(asset, android_read, android_write, android_seek, android_close);
    }
#endif


FILE *ESFileWrapper::esFopen(const char *fileName, const char* mode, int fOpenMode) {
    FILE *file;
#ifdef ANDROID

    if ( fOpenMode == FOPEN_DEFAULT_MODE && androidAssetManager != NULL ) {
        file = android_fopen(fileName, mode);
    } else if (fOpenMode == FOPEN_ABSOLUTE_MODE) {
        file = fopen(fileName, mode);
        return file;
    }
#else
#ifdef __APPLE__
    // iOS: Remap the filename to a path that can be opened from the bundle.
    fileName = GetBundleFileName ( fileName );
#endif
#endif
    return file;
}


///
// esFileOpen()
//
//    Wrapper for platform specific File open
//
esFile *ESFileWrapper::esFileOpen(const char *fileName) {
    esFile *pFile = NULL;

#ifdef ANDROID
    if ( androidAssetManager != NULL ) {
        pFile = AAssetManager_open ( androidAssetManager, fileName, AASSET_MODE_BUFFER );
    }
#else
#ifdef __APPLE__
    // iOS: Remap the filename to a path that can be opened from the bundle.
    fileName = GetBundleFileName ( fileName );
#endif
    pFile = fopen(fileName, "rb");
#endif
    return pFile;
}

///
// esFileClose()
//
//    Wrapper for platform specific File close
//
void ESFileWrapper::esFileClose(esFile *pFile) {
    if (pFile != NULL) {
#ifdef ANDROID
        AAsset_close ( pFile );
#else
        fclose(pFile);
        pFile = NULL;
#endif
    }
}

///
// esFileRead()
//
//    Wrapper for platform specific File read
//
int ESFileWrapper::esFileRead(esFile *pFile, int bytesToRead, void *buffer) {
    int bytesRead = 0;

    if (pFile == NULL) {
        return bytesRead;
    }

#ifdef ANDROID
    bytesRead = AAsset_read ( pFile, buffer, bytesToRead );
#else
    bytesRead = fread(buffer, bytesToRead, 1, pFile);
#endif

    return bytesRead;
}

///
// esFileRead()
//
//    Wrapper for platform specific File Get Length
//
long ESFileWrapper::esFileGetLength(esFile *pFile) {
    long length;
#ifdef ANDROID
    length = AAsset_getLength(pFile);
#else
    fseek(pFile, 0, SEEK_END);
    length = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
#endif
    return length;
}
