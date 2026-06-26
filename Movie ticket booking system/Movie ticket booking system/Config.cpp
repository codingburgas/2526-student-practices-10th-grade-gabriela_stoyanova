#include "Config.h"
#include <filesystem>
#include <iostream>

namespace Config {

bool ensureDataDirectory()
{
    try {
        // Check if path exists and is a directory
        if (std::filesystem::exists(DATA_DIR)) {
            if (!std::filesystem::is_directory(DATA_DIR)) {
                std::cerr << "Error: " << DATA_DIR << " exists but is not a directory" << std::endl;
                return false;
            }
            return true; // Directory already exists
        }

        // Create the directory
        if (std::filesystem::create_directory(DATA_DIR)) {
            return true; // Successfully created
        }

        std::cerr << "Error: Failed to create directory " << DATA_DIR << std::endl;
        return false;

    } catch (const std::exception& e) {
        std::cerr << "Error creating data directory: " << e.what() << std::endl;
        return false;
    }
}

}
