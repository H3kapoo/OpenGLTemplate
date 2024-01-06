#pragma once

#include <stdio.h>
#include <fstream>
#include <string>
#include <utility>
#include <regex>
#include <filesystem>
#include <functional>

#include "../src/vendor/11Zip/include/elzip/elzip.hpp"
#include "../src/vendor/xzDecompressor/xz/decode.hpp"

namespace unsnapshot
{
#define BUFF_CAP 128

class UnSnapshot
{
    typedef std::function<void(const bool status, const std::string& err)> UnSnapshotDoneCb;
    typedef std::function<void(const float progress)> UnSnapshotProgressCb;
    typedef std::filesystem::path Path;
public:
    UnSnapshot() = default;
    void unSnap(const Path& inPath, const Path& outPath, const UnSnapshotProgressCb progressCb,
        const UnSnapshotDoneCb doneCb);

private:
    //TODO: use std::fs::path instead of plain strings. windows reasons
    bool handleParentLine(std::ifstream& snapshotFileList, const std::string& maybeParentLine, char* readBuffer);
    void extractSyslogsFrom(const Path& unzippedSnapshotPath, const std::string& parentName,
        const std::string& childName, const Path& outputPath);
    void extractIMS2From(const Path& unzippedSnapshotPath, const std::string& parentZip,
        const std::string& childName, const Path& outputPath);

    void okBye();
    void failBye(const std::string& err);
    void reportProgress(const float progress);

    Path gUnzippedSnapshotPath;
    Path gSyslogsOutputPath;
    Path gTempPath;

    UnSnapshotDoneCb gDoneCallback{ nullptr };
    UnSnapshotProgressCb gProgressCallback{ nullptr };
};
}