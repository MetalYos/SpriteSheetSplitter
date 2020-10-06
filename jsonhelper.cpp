#include "jsonhelper.h"
#include "utils.h"
#include <iostream>
#include <fstream>

bool JsonHelper::ImportJson(const std::string& filepath, std::pair<std::string, std::vector<Frame*>>& output)
{
    std::ifstream inFile;
    inFile.open(filepath.c_str());
    if (!inFile.is_open())
        return false;

    std::string line;

    // Open curly braces at start of file
    getline(inFile, line);
    // Name, TODO: save it
    getline(inFile, line);
    // Relative Image Name
    getline(inFile, line);
    output.first = GetStringValue(line);
    // Total number of frames
    getline(inFile, line);
    int numFrames = GetIntValue(line);

    // Start of Frames array
    getline(inFile, line);

    for (int i = 0; i < numFrames; i++)
    {
        // Open curly braces
        getline(inFile, line);
        // Index (ignore, TODO: maybe create an array with a size of numFrames, and place frames according to Index)
        getline(inFile, line);
        // Get Frame parameters
        getline(inFile, line);
        int top = GetIntValue(line);
        getline(inFile, line);
        int left = GetIntValue(line);
        getline(inFile, line);
        int width = GetIntValue(line);
        getline(inFile, line);
        int height = GetIntValue(line);
        getline(inFile, line);
        int originX = GetIntValue(line);
        getline(inFile, line);
        int originY = GetIntValue(line);
        // Close curly braces
        getline(inFile, line);

        Frame* frame = new Frame(top, left, width, height);
        frame->Origin({originX, originY});

        output.second.push_back(frame);
    }
    // End of Frames array
    getline(inFile, line);
    // Close curly braces at end of file
    getline(inFile, line);

    inFile.close();

    return true;
}

bool JsonHelper::ExportJson(const std::string& sSheetPath, const std::vector<Frame*>& frames)
{
    if (frames.size() == 0)
        return false;

    auto sSheetFilename = StringUtils::GetFilename(sSheetPath);
    auto sSheetName = StringUtils::GetFilenameWithoutExt(sSheetFilename);
    auto outputPath = StringUtils::GetFilepathWithoutFile(sSheetPath) + sSheetName + ".json";

    std::ofstream outFile;
    outFile.open(outputPath.c_str(), std::ios::out);
    if (!outFile.is_open())
        return false;

    outFile << "{" << std::endl;
    outFile << "\tName: " << "\"" << sSheetName << "\"," << std::endl;
    outFile << "\tRelativeImageName: " << "\"" << sSheetFilename << "\"," << std::endl;
    outFile << "\tTotalNumFrames: " << frames.size() << "," << std::endl;

    outFile << "\tFrames: [" << std::endl;
    for (size_t i = 0; i < frames.size(); i++)
    {
        auto frame = frames[i];
        outFile << "\t\t{" << std::endl;
        outFile << "\t\t\tIndex: " << i << "," << std::endl;
        outFile << "\t\t\tTop: " << frame->Top() << "," << std::endl;
        outFile << "\t\t\tLeft: " << frame->Left() << "," << std::endl;
        outFile << "\t\t\tWidth: " << frame->Width() << "," << std::endl;
        outFile << "\t\t\tHeight: " << frame->Height() << "," << std::endl;
        outFile << "\t\t\tOriginX: " << frame->Origin().first << "," << std::endl;
        outFile << "\t\t\tOriginY: " << frame->Origin().second << std::endl;
        outFile << "\t\t}";

        if (i < frames.size() - 1)
            outFile << ",";
        outFile << std::endl;
    }
    outFile << "\t]" << std::endl;

    outFile << "}";

    outFile.close();

    return true;
}

std::string JsonHelper::GetStringValue(const std::string& line)
{
    int startIndex;
    for (startIndex = line.size() - 1; startIndex >= 0; startIndex--)
    {
        if (line[startIndex] == ':')
            break;
    }
    startIndex++;

    while (line[startIndex] == ' ')
        startIndex++;

    if (line[startIndex] == '"')
        startIndex++;

    int endIndex = startIndex;
    while (line[endIndex] != '"' && line[endIndex] != '\r' && line[endIndex] != '\n' && endIndex < (int)line.size())
        endIndex++;

    return line.substr(startIndex, endIndex - startIndex);
}

int JsonHelper::GetIntValue(const std::string& line)
{
    std::string value = GetStringValue(line);

    try
    {
        return std::stoi(value);
    }  catch (...)
    {
        return 0;
    }
}
