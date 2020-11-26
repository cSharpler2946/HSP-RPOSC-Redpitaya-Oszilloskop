#include <iostream>
#include <libsigrokdecode/libsigrokdecode.h>

int main() {
    const char* s = srd_lib_version_string_get();
    std::cout << "LibSigrokDecode version: " << s << std::endl;
    return 0;
}
