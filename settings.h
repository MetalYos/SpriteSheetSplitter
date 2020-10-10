#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <map>
#include <unordered_map>

class Settings
{
public:
    enum Fields
    {
        FRAME_INIT_WIDTH,
        FRAME_INIT_HEIGHT,
        FRAME_TOLERANCE_MIN,
        FRAME_TOLERANCE_MAX,
        FRAME_TOLERANCE_INIT,
        FRAME_COLOR_TOLERANCE,
        COLOR_PICKER_CURSOR_PATH,
        FRAME_DETECTION_ALGO_STEP,
        FRAME_DETECTION_ALGO_USE_ADAPTIVE_STEP,
        NONE
    };

    enum MouseButtons
    {
        LEFT_MOUSE_BUTTON,
        RIGHT_MOUSE_BUTTON,
        MIDDLE_MOUSE_BUTTON
    };

private:
    struct FieldKey
    {
        std::string FieldName;
        Fields Field;
        std::string Type;

        bool operator==(const FieldKey& f) const
        {
            return (f.Field == Field);
        }
    };

    struct FieldKeyHash
    {
        size_t operator()(const FieldKey& f) const noexcept
        {
            return std::hash<int>{}(f.Field);
        }
    };

    struct FieldValue
    {
        std::string Str;
        int Int;
        bool Bool;
    };

public:
    static bool LoadSettings();
    static bool SaveSettings();

    static int GetInt(Fields field);
    static bool GetBool(Fields field);
    static std::string GetStr(Fields field);

    static void Set(Fields field, int value);
    static void Set(Fields field, const std::string& value);
    static void Set(Fields field, bool value);

private:
    static Fields StringToField(const std::string& fieldName);
    static FieldValue GetValueFromString(const FieldKey& key, std::string value);
    static FieldValue& GetFieldValue(Fields field);

private:
    static std::map<std::string, Fields> fieldNamesMap;

    static const char* settingsFile;
    static std::unordered_map<FieldKey, FieldValue, FieldKeyHash> settings;
};

#endif // SETTINGS_H
