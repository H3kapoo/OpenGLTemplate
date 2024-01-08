#include "Unzipper.hpp"

namespace unsnapshot
{

void UnSnapshot::extractSyslogsFrom(const Path& unzippedSnapshotPath, const std::string& parentName,
    const std::string& childName, const Path& outputPath)
{
    printf("Trying to extract syslogs\n");

    Path parentNamePath = unzippedSnapshotPath / parentName;
    if (!std::filesystem::exists(parentNamePath))
    {
        fprintf(stderr, "Parent zip %s doesnt exist!\n", parentNamePath.string().c_str());
        return failBye("Unlikely FiNoFound failure");
    }

    /* Get syslog.zip from part_x.zip */
    try
    {
        elz::extractFile(parentNamePath, childName, gTempPath);
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "Something happened unzipping:  \"%s\" FROM \"%s\"\n -> what(): %s\n",
            childName.c_str(), parentNamePath.string().c_str(), e.what());
        return failBye("Weird failure of unzip");
    }

    /* Get syslog.xz from syslog.zip */
    std::string syslogHint = "NO_HINT";
    if (childName.find("startup") != std::string::npos)
    {
        syslogHint = "startup_BTSOM.log.xz";
    }
    else if (childName.find("runtime") != std::string::npos)
    {
        syslogHint = "runtime_BTSOM.log.xz";
    }

    const Path extractXZFromPath = gTempPath / childName;
    try
    {
        elz::extractFile(extractXZFromPath, syslogHint, gTempPath);
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "Something happened unzipping LOG.XZ: -> \"%s\" FROM \"%s\"\n -> what(): %s\n",
            syslogHint.c_str(), extractXZFromPath.string().c_str(), e.what());
        return failBye("Weird failure of unzip");
    }

    /* Get syslog.txt from syslog.xz */
    const Path xzInFilePath = gTempPath / syslogHint;
    const Path xzOutFilePath = outputPath / (syslogHint.substr(0, syslogHint.size() - 7) + ".txt");
    std::ifstream inFile(xzInFilePath);
    std::ofstream outFile(xzOutFilePath);

    printf("Paths: in: %s \n out: %s\n", xzInFilePath.string().c_str(), xzOutFilePath.string().c_str());
    if (!xz::decompress(inFile, outFile) != EXIT_SUCCESS)
    {
        fprintf(stderr, "Faied to decompress XZ: %s\n", xzInFilePath.string().c_str());
        inFile.close();
        outFile.close();
        return failBye("Weird failure of unzip");
    }

    inFile.close();
    outFile.close();
    printf("%s\n%s\n", parentName.c_str(), childName.c_str());
}

void UnSnapshot::extractIMS2From(const Path& unzippedSnapshotPath, const std::string& parentName,
    const std::string& childName, const Path& outputPath)
{
    printf("Trying to extract IMS2\n");

    Path parentNamePath = unzippedSnapshotPath / parentName;
    if (!std::filesystem::exists(parentNamePath))
    {
        fprintf(stderr, "Parent zip %s doesnt exist!\n", parentNamePath.string().c_str());
        return failBye("Weird failure of unzip");
    }

    /* Get file.ims2 from part_x.zip */
    try
    {
        elz::extractFile(parentNamePath, childName, outputPath);
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "Something happened unzipping: \"%s\" FROM \"%s\"\n -> what(): %s\n",
            childName.c_str(), parentNamePath.string().c_str(), e.what());
        return failBye("Weird failure of unzip");
    }

    printf("%s\n%s\n", parentName.c_str(), childName.c_str());
}

bool UnSnapshot::handleParentLine(std::ifstream& snapshotFileList, const std::string& maybeParentLine, char* readBuffer)
{
    std::smatch match;
    std::regex ims2_reg("(BTS[0-9]{1,9}_([1-9])011_(pm_[1-9]_)*im_snapshot.ims2)");
    std::regex syslog_reg("(BTS[0-9]{1,9}_([1-9])011_(startup|runtime).zip)");

    std::string maybePatternMatch;
    maybePatternMatch.reserve(BUFF_CAP);

    /* Go down the lines to find a pattern */
    while (!snapshotFileList.eof())
    {
        snapshotFileList.getline(readBuffer, BUFF_CAP, '\n');
        maybePatternMatch = readBuffer;

        /* If the line is empty, means we are at a border between end of parent contents and start of next parent contents
           so it's impossible to have a match here. Exit */
        if (maybePatternMatch.empty()) { return false; }

        /* Extract the ims2 file from the /parent/zip/ location */
        if (std::regex_search(maybePatternMatch, match, syslog_reg))
        {
            extractSyslogsFrom(gUnzippedSnapshotPath, maybeParentLine.substr(0, maybeParentLine.size() - 1), match[0].str(), gSyslogsOutputPath);
            return true;
        }

        /* Extract the ims2 file from the /parent/zip/ location */
        if (std::regex_search(maybePatternMatch, match, ims2_reg))
        {
            extractIMS2From(gUnzippedSnapshotPath, maybeParentLine.substr(0, maybeParentLine.size() - 1), match[0].str(), gSyslogsOutputPath);
            return true;
        }
    }

    return false;
}

void UnSnapshot::okBye()
{
    if (gDoneCallback)
    {
        /* Clean up */
        std::filesystem::remove_all(gTempPath);

        gDoneCallback(true, "");
        gDoneCallback = nullptr;
    }
}

void UnSnapshot::failBye(const std::string& err)
{
    if (gDoneCallback)
    {
        /* Clean up */
        std::filesystem::remove_all(gTempPath);

        gDoneCallback(false, err);
        gDoneCallback = nullptr;
        gProgressCallback = nullptr;
    }
}

void UnSnapshot::reportProgress(const float progress)
{
    if (gProgressCallback)
    {
        gProgressCallback(progress);
    }
}

void UnSnapshot::unSnap(const Path& inPath, const Path& outPath, const UnSnapshotProgressCb progressCb,
    const UnSnapshotDoneCb doneCb)
{
    gDoneCallback = doneCb;
    gProgressCallback = progressCb;

    /* Extract big snapshot. */
    gTempPath = outPath / "temp";
    try
    {
        elz::extractZip(inPath, gTempPath);
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "Something happened unzipping big snapshot: \"%s\"\n -> what(): %s\n",
            inPath.string().c_str(), e.what());
        return failBye("Unzip failure");
    }
    reportProgress(10.0f);

    gUnzippedSnapshotPath = gTempPath;
    gSyslogsOutputPath = outPath;
    Path snapshotFileListPath = gTempPath / "snapshot_file_list.txt";

    std::ifstream snapshotFileList(snapshotFileListPath);
    if (snapshotFileList.fail() || snapshotFileList.bad())
    {
        fprintf(stderr, "File couldn't be found at 1st level of zip or loaded %s\n", snapshotFileListPath.string().c_str());
        snapshotFileList.close();
        return failBye("snapshot_file_list missing");
    }
    reportProgress(20.0f);

    std::string maybeParentLine;
    maybeParentLine.reserve(BUFF_CAP);
    char readBuffer[BUFF_CAP];
    int32_t filesFound = 0;
    float fakeIncrementFactor = 5.0f;

    while (!snapshotFileList.eof())
    {
        snapshotFileList.getline(readBuffer, BUFF_CAP, '\n');
        maybeParentLine = readBuffer;

        /* If line ends with : means it's parent to possible regex pattern */
        if (maybeParentLine.ends_with(":"))
        {
            if (handleParentLine(snapshotFileList, maybeParentLine, readBuffer))
            {
                filesFound++;
                reportProgress(50.0f + filesFound * fakeIncrementFactor);
            }
        }
    }

    snapshotFileList.close();
    reportProgress(100.0f);
    okBye();
}

}