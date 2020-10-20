#include "settings.h"
#include <iostream>
#include <fstream>
#include "jsonhelper.h"
#include "utils.h"

const char* Settings::settingsFile = "Settings.json";
std::map<std::string, Settings::Fields> Settings::fieldNamesMap = {
    { "FRAME_INIT_WIDTH", Settings::Fields::FRAME_INIT_WIDTH },
    { "FRAME_INIT_HEIGHT", Settings::Fields::FRAME_INIT_HEIGHT },
    { "FRAME_TOLERANCE_MIN", Settings::Fields::FRAME_TOLERANCE_MIN },
    { "FRAME_TOLERANCE_MAX", Settings::Fields::FRAME_TOLERANCE_MAX },
    { "FRAME_TOLERANCE_INIT", Settings::Fields::FRAME_TOLERANCE_INIT },
    { "FRAME_COLOR_TOLERANCE", Settings::Fields::FRAME_COLOR_TOLERANCE },
    { "COLOR_PICKER_CURSOR_PATH", Settings::Fields::COLOR_PICKER_CURSOR_PATH },
    { "FRAME_DETECTION_ALGO_STEP", Settings::Fields::FRAME_DETECTION_ALGO_STEP },
    { "FRAME_DETECTION_ALGO_USE_ADAPTIVE_STEP", Settings::Fields::FRAME_DETECTION_ALGO_USE_ADAPTIVE_STEP },
    { "ANIMATION_FPS", Settings::Fields::ANIMATION_FPS }
};
std::map<std::string, Settings::FieldCategories> Settings::categoryNamesMap = {
    { "FRAME_DETECTION", Settings::FieldCategories::FRAME_DETECTION },
    { "ANIMATION", Settings::FieldCategories::ANIMATION }
};

std::unordered_map<Settings::FieldKey, Settings::FieldValue, Settings::FieldKeyHash> Settings::settings;

bool Settings::LoadSettings()
{
    std::ifstream inFile;
    inFile.open(settingsFile);
    if (!inFile.is_open())
        return false;

    std::string line;

    // Open curly braces at start of file
    getline(inFile, line);
    // Total number of settings
    getline(inFile, line);
    int numSettings = JsonHelper::GetIntValue(line);

    // Start of Settings array
    getline(inFile, line);

    for (int i = 0; i < numSettings; i++)
    {
        // Open curly braces
        getline(inFile, line);
        // Get Settings parameters
        getline(inFile, line);
        std::string name = JsonHelper::GetStringValue(line);
        getline(inFile, line);
        std::string category = JsonHelper::GetStringValue(line);
        getline(inFile, line);
        std::string type = JsonHelper::GetStringValue(line);
        getline(inFile, line);
        std::string value = JsonHelper::GetStringValue(line);
        // Close curly braces
        getline(inFile, line);

        FieldKey key { name, StringToField(name), StringToCategory(category), type };
        if (key.Field != Fields::NONE_FIELD)
        {
            FieldValue fValue;
            if (StringUtils::ToLower(key.Type) == "int")
                fValue.Int = JsonHelper::GetIntValue(value);
            if (StringUtils::ToLower(key.Type) == "string")
                fValue.Str = JsonHelper::GetStringValue(value);
            if (StringUtils::ToLower(key.Type) == "bool")
                fValue.Bool = JsonHelper::GetBoolValue(value);

            settings[key] = fValue;
        }
    }
    // End of Settings array
    getline(inFile, line);
    // Close curly braces at end of file
    getline(inFile, line);

    inFile.close();

    return true;
}

bool Settings::SaveSettings()
{
    if (settings.size() == 0)
        return false;

    std::ofstream outFile;
    outFile.open(settingsFile, std::ios::out);
    if (!outFile.is_open())
        return false;

    outFile << "{" << std::endl;
    outFile << "\t\"NumSettings\": " << settings.size() << "," << std::endl;

    outFile << "\t\"Settings\": [" << std::endl;
    size_t i = 0;
    for (auto setting : settings)
    {
        outFile << "\t\t{" << std::endl;
        outFile << "\t\t\t\"Name\": \"" << setting.first.FieldName << "\"," << std::endl;
        outFile << "\t\t\t\"Category: \"" << setting.first.Category << "\"," << std::endl;
        outFile << "\t\t\t\"Type\": \"" << setting.first.Type << "\"," << std::endl;
        if (StringUtils::ToLower(setting.first.Type) == "bool")
        {
            const char* bVal = (setting.second.Bool) ? "true" : "false";
            outFile << "\t\t\t\"Value\": \"" << bVal << "\"" << std::endl;
        }
        else if (StringUtils::ToLower(setting.first.Type) == "int")
            outFile << "\t\t\t\"Value\": \"" << setting.second.Int << "\"" << std::endl;
        else if (StringUtils::ToLower(setting.first.Type) == "string")
            outFile << "\t\t\t\"Value\": \"" << setting.second.Str << "\"" << std::endl;
        outFile << "\t\t}";

        if (i++ < settings.size() - 1)
            outFile << ",";
        outFile << std::endl;
    }
    outFile << "\t]" << std::endl;

    outFile << "}";

    outFile.close();

    return true;
}

int Settings::GetInt(Fields field)
{
    try
    {
        auto& value = GetFieldValue(field);
        return value.Int;
    }
    catch (...)
    {
        return 0;
    }
}

bool Settings::GetBool(Fields field)
{
    try
    {
        auto& value = GetFieldValue(field);
        return value.Bool;
    }
    catch (...)
    {
        return false;
    }
}

std::string Settings::GetStr(Fields field)
{
    try
    {
        auto& value = GetFieldValue(field);
        return value.Str;
    }
    catch (...)
    {
        return "";
    }
}

Settings::Fields Settings::StringToField(const std::string& fieldName)
{
    try
    {
        return fieldNamesMap.at(fieldName);
    }
    catch (...)
    {
        return Settings::Fields::NONE_FIELD;
    }
}

Settings::FieldCategories Settings::StringToCategory(const std::string& category)
{
    try
    {
        return categoryNamesMap.at(category);
    }
    catch (...)
    {
        return Settings::FieldCategories::NONE_CATEGORY;
    }
}

Settings::FieldValue& Settings::GetFieldValue(Fields field)
{
    FieldKey key;
    key.Field = field;

    return settings.at(key);
}

void Settings::Set(Fields field, int value)
{
    if (field == Fields::NONE_FIELD)
        return;

    GetFieldValue(field).Int = value;
}

void Settings::Set(Fields field, const std::string& value)
{
    if (field == Fields::NONE_FIELD)
        return;

    GetFieldValue(field).Str = value;
}

void Settings::Set(Fields field, bool value)
{
    if (field == Fields::NONE_FIELD)
        return;

    GetFieldValue(field).Bool = value;
}
