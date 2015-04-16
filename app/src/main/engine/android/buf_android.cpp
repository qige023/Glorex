#include <android/log.h>

//http://stackoverflow.com/questions/8870174/is-stdcout-usable-in-android-ndk
//
//To actually set up std::cout to write to this stream buffer, you would do something like this in your main() function:
//
//int main() {
//    std::cout.rdbuf(new androidbuf);
//    ...
//}
//This create a memory leak for the one androidbuf stream which is, however, somewhat intentional: the stream may be written to after main() is exited and it is flushed when when std::cout gets destroyed. If you don't want this, you could either restore std::cout's original stream buffer or set it to null and delete the return from rdbuf():
//
//   // avoid a one-time resource leak but don't get output afterwards:
//   delete std::cout.rdbuf(0);

class androidbuf: public std::streambuf {
public:
    enum { bufsize = 128 }; // ... or some other suitable buffer size
    androidbuf() { this->setp(buffer, buffer + bufsize - 1); }
private:
    int overflow(int c) {
        if (c == traits_type::eof()) {
            *this->pptr() = traits_type::to_char_type(c);
            this->sbumpc();
        }
        return this->sync()? traits_type::eof(): traits_type::not_eof(c);
    }
    int sync() {
        int rc = 0;
        if (this->pbase() != this->pptr()) {
            __android_log_print(ANDROID_LOG_INFO,
                               "Native",
                               "%s",
                               std::string(this->pbase(),
                                           this->pptr() - this->pbase()).c_str());
            rc = 0;
            this->setp(buffer, buffer + bufsize - 1);
        }
        return rc;
    }
    char buffer[bufsize];
};
