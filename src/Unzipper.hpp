#pragma once

#include <stdio.h>
#include <fstream>
#include <string>
#include <utility>
#include <regex>
#include <filesystem>

#include "../src/vendor/11Zip/include/elzip/elzip.hpp"
#include "../src/vendor/xzDecompressor/xz/decode.hpp"

#define BUFF_CAP 128

//TODO: use std::fs::path instead of plain strings. windows reasons
void extractSyslogsFrom(const std::string& unzippedSnapshot, const std::string& parentZip,
    const std::string& childName, const std::string& outputTarget)
{
    printf("Trying to extract syslogs\n");

    std::string tempOutput = outputTarget + "/temp";
    std::string toUnzipFrom = unzippedSnapshot + "/" + parentZip;
    if (!std::filesystem::exists(toUnzipFrom))
    {
        fprintf(stderr, "Parent zip %s doesnt exist!\n", toUnzipFrom.c_str());
        return;
    }

    // get syslog.zip from part_x.zip
    try
    {
        elz::extractFile(toUnzipFrom, childName, tempOutput);
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "Something happened unzipping:\n -> \"%s\"\n--from-- \n\"%s\"\n -> what(): %s\n",
            childName.c_str(), toUnzipFrom.c_str(), e.what());
        return;
    }

    // get syslog.xz from syslog.zip
    std::string syslogHint = "NO_HINT";
    if (childName.find("startup") != std::string::npos)
    {
        syslogHint = "startup_BTSOM.log.xz";
    }
    else if (childName.find("runtime") != std::string::npos)
    {
        syslogHint = "runtime_BTSOM.log.xz";
    }

    const std::string extractXZFrom = tempOutput + "/" + childName;
    try
    {
        elz::extractFile(extractXZFrom, syslogHint, tempOutput);
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "Something happened unzipping LOG.XZ:\n -> \"%s\"\n--from-- \n\"%s\"\n -> what(): %s\n",
            syslogHint.c_str(), extractXZFrom.c_str(), e.what());
        return;
    }

    // get syslog.txt from syslog.xz
    const std::string xzInFilePath = tempOutput + "/" + syslogHint;
    const std::string xzOutFilePath = outputTarget + "/" + syslogHint.substr(0, syslogHint.size() - 7) + ".txt";
    std::ifstream inFile(xzInFilePath);
    std::ofstream outFile(xzOutFilePath);

    if (!xz::decompress(inFile, outFile) != EXIT_SUCCESS)
    {
        fprintf(stderr, "Faied to decompress XZ: %s\n", xzInFilePath.c_str());
        return;
    }

    // clean up
    std::filesystem::remove_all(tempOutput);
    printf("%s\n%s\n", parentZip.c_str(), childName.c_str());
}

//TODO: use std::fs::path instead of plain strings. windows reasons
void extractIMS2From(const std::string& unzippedSnapshot, const std::string& parentZip,
    const std::string& childName, const std::string& outputTarget)
{
    printf("Trying to extract IMS2\n");

    std::string toUnzipFrom = unzippedSnapshot + "/" + parentZip;
    if (!std::filesystem::exists(toUnzipFrom))
    {
        fprintf(stderr, "Parent zip %s doesnt exist!\n", toUnzipFrom.c_str());
        return;
    }

    // get file.ims2 from part_x.zip
    try
    {
        elz::extractFile(toUnzipFrom, childName, outputTarget);
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "Something happened unzipping:\n -> \"%s\"\n--from-- \n\"%s\"\n -> what(): %s\n",
            childName.c_str(), toUnzipFrom.c_str(), e.what());
        return;
    }
    printf("%s\n%s\n", parentZip.c_str(), childName.c_str());

    // no cleanup necessarry
}

void unzip(const std::string& inPath, const std::string& outPath)
{
    /*
    unzip snapshot.zip/rar
    find snapshot_file_list.txt

    for syslogs:
        find patters ['.*?([1-9])011_(.*?)im_snapshot.ims2', '.*?BTS[0-9]{1,9}_([1-9])011_(startup|runtime).zip'] (ex BTS12345_1011_runtime.zip)
            for each pattern go up until you find 1st X_Line line containing ":"
            unzip X_Line.zip -> get X_line_runtime.zip
            unzip X_line_runtime.zip -> runtime.log.xz
            xz runtime.log.xz -> get runtime.txt
            move runtime.txt to output folder
    */
    std::ifstream snapshotFileList("src/unzip_test/extracted/snapshot_file_list.txt");

    if (snapshotFileList.fail() || snapshotFileList.bad())
    {
        fprintf(stderr, "File couldn't be found/loaded %s\n", "THE PATH");
        return;
    }

    // testing only, rm later
    std::string unzippedSnapshot{ "src/unzip_test/extracted/Snapshot_MRBTS-25162516_MRBTS-2516_SBTS24R2_ENB_9999_231206_1092066_20231207-0307" };
    std::string syslogsOutputPath{ "src/unzip_test/extracted" };

    std::smatch match;
    std::regex ims2_reg("(BTS[0-9]{1,9}_([1-9])011_(pm_[1-9]_)*im_snapshot.ims2)");
    std::regex syslog_reg("(BTS[0-9]{1,9}_([1-9])011_(startup|runtime).zip)");

    char readbuffer[BUFF_CAP];
    std::string maybeParentLine;
    std::string maybePatternMatch;

    maybeParentLine.reserve(BUFF_CAP);
    maybePatternMatch.reserve(BUFF_CAP);

    // read file line by line
    while (!snapshotFileList.eof())
    {
        snapshotFileList.getline(readbuffer, BUFF_CAP, '\n');
        maybeParentLine = readbuffer;

        // if line ends with : means it's parent to possible regex pattern
        if (maybeParentLine.ends_with(":"))
        {
            // go down the lines to find a pattern
            while (!snapshotFileList.eof())
            {
                snapshotFileList.getline(readbuffer, BUFF_CAP, '\n');
                maybePatternMatch = readbuffer;

                // if the line is empty, means we are at a border between end of parent contents and start of next parent contents
                // so it's impossible to have a match here. Exit
                if (maybePatternMatch.empty())
                {
                    break;
                }

                // extract the syslog file from the /parent/zip/ location
                if (std::regex_search(maybePatternMatch, match, syslog_reg))
                {
                    extractSyslogsFrom(unzippedSnapshot, maybeParentLine.substr(0, maybeParentLine.size() - 1), match[0].str(), syslogsOutputPath);
                }

                // extract the ims2 file from the /parent/zip/ location
                if (std::regex_search(maybePatternMatch, match, ims2_reg))
                {
                    extractIMS2From(unzippedSnapshot, maybeParentLine.substr(0, maybeParentLine.size() - 1), match[0].str(), syslogsOutputPath);
                }
            }
        }
    }

}