/**
 * @file FileManager.h
 * @brief File I/O utility class for reading/writing data files
 *
 * SOLID Principles Applied:
 * - Single Responsibility: Only handles file I/O operations
 * - DRY: Reusable helper methods for parsing and formatting
 */

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

/**
 * @class FileManager
 * @brief Handles all file I/O operations for the POS application
 *
 * This class provides methods to read and write data files in CSV format
 * using '|' as the delimiter. It encapsulates file handling logic and
 * provides reusable parsing utilities.
 */
class FileManager {
private:
    std::string m_dataPath;  ///< Base path for data files

public:
    /**
     * @brief Constructs a FileManager with specified data path
     * @param dataPath Base directory for data files (default: "data/")
     */
    explicit FileManager(const std::string& dataPath = "data/");

    /**
     * @brief Gets the current data path
     * @return The base directory path
     */
    std::string getDataPath() const;

    /**
     * @brief Sets the data path
     * @param path New base directory path
     */
    void setDataPath(const std::string& path);

    // ============ File Operations ============

    /**
     * @brief Reads all lines from a file
     * @param filename Name of the file (relative to data path)
     * @return Vector of lines from the file
     */
    std::vector<std::string> readLines(const std::string& filename) const;

    /**
     * @brief Writes lines to a file (overwrites existing content)
     * @param filename Name of the file (relative to data path)
     * @param lines Vector of lines to write
     * @return true if successful, false otherwise
     */
    bool writeLines(const std::string& filename, const std::vector<std::string>& lines) const;

    /**
     * @brief Appends a single line to a file
     * @param filename Name of the file (relative to data path)
     * @param line Line to append
     * @return true if successful, false otherwise
     */
    bool appendLine(const std::string& filename, const std::string& line) const;

    /**
     * @brief Checks if a file exists
     * @param filename Name of the file (relative to data path)
     * @return true if file exists, false otherwise
     */
    bool fileExists(const std::string& filename) const;

    /**
     * @brief Creates an empty file if it doesn't exist
     * @param filename Name of the file (relative to data path)
     * @return true if file exists or was created, false on error
     */
    bool ensureFileExists(const std::string& filename) const;

    // ============ Parsing Utilities (DRY) ============

    /**
     * @brief Splits a line by delimiter
     * @param line The line to split
     * @param delimiter Character to split on (default: '|')
     * @return Vector of fields
     */
    static std::vector<std::string> splitLine(const std::string& line, char delimiter = '|');

    /**
     * @brief Joins fields with delimiter
     * @param fields Vector of fields to join
     * @param delimiter Character to join with (default: '|')
     * @return Joined string
     */
    static std::string joinFields(const std::vector<std::string>& fields, char delimiter = '|');

    /**
     * @brief Trims whitespace from both ends of a string
     * @param str String to trim
     * @return Trimmed string
     */
    static std::string trim(const std::string& str);
};

#endif // FILEMANAGER_H
