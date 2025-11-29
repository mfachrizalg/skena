/**
 * @file FileManager.cpp
 * @brief Implementation of FileManager class
 */

#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

FileManager::FileManager(const std::string& dataPath)
    : m_dataPath(dataPath)
{
    // Ensure data directory exists
    if (!fs::exists(m_dataPath)) {
        fs::create_directories(m_dataPath);
    }
}

std::string FileManager::getDataPath() const {
    return m_dataPath;
}

void FileManager::setDataPath(const std::string& path) {
    m_dataPath = path;
    // Ensure new path exists
    if (!fs::exists(m_dataPath)) {
        fs::create_directories(m_dataPath);
    }
}

std::vector<std::string> FileManager::readLines(const std::string& filename) const {
    std::vector<std::string> lines;
    std::string fullPath = m_dataPath + filename;

    std::ifstream file(fullPath);
    if (!file.is_open()) {
        return lines;  // Return empty vector if file doesn't exist
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments (lines starting with #)
        std::string trimmed = trim(line);
        if (!trimmed.empty() && trimmed[0] != '#') {
            lines.push_back(trimmed);
        }
    }

    file.close();
    return lines;
}

bool FileManager::writeLines(const std::string& filename, const std::vector<std::string>& lines) const {
    std::string fullPath = m_dataPath + filename;

    std::ofstream file(fullPath, std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& line : lines) {
        file << line << "\n";
    }

    file.close();
    return true;
}

bool FileManager::appendLine(const std::string& filename, const std::string& line) const {
    std::string fullPath = m_dataPath + filename;

    std::ofstream file(fullPath, std::ios::app);
    if (!file.is_open()) {
        return false;
    }

    file << line << "\n";
    file.close();
    return true;
}

bool FileManager::fileExists(const std::string& filename) const {
    std::string fullPath = m_dataPath + filename;
    return fs::exists(fullPath);
}

bool FileManager::ensureFileExists(const std::string& filename) const {
    if (fileExists(filename)) {
        return true;
    }

    std::string fullPath = m_dataPath + filename;
    std::ofstream file(fullPath);
    return file.is_open();
}

std::vector<std::string> FileManager::splitLine(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;

    while (std::getline(ss, field, delimiter)) {
        fields.push_back(trim(field));
    }

    return fields;
}

std::string FileManager::joinFields(const std::vector<std::string>& fields, char delimiter) {
    if (fields.empty()) {
        return "";
    }

    std::string result = fields[0];
    for (size_t i = 1; i < fields.size(); ++i) {
        result += delimiter + fields[i];
    }

    return result;
}

std::string FileManager::trim(const std::string& str) {
    const auto start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }

    const auto end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}
