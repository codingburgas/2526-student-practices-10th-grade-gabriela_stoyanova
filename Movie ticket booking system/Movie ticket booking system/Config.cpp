#include "Config.h"
#include <filesystem>
#include <iostream>

namespace Config {

bool ensureDataDirectory()
{
    try {
        std::filesystem::create_directory(DATA_DIR);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating data directory: " << e.what() << std::endl;
        return false;
    }
}

}
