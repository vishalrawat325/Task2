#pragma once
#include "Library.h"
#include <string>

class Storage {
public:
    // The storage object does not own the Library; it reads/writes its value data.
    static bool save(const Library& library, const std::string& path,
                     std::string& message);
    static bool load(Library& library, const std::string& path,
                     std::string& message);
};
