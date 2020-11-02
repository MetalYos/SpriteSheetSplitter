#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <map>
#include <unordered_map>
#include "utils.h"

class Settings
{
public:
    enum FieldCategories
    {
        FRAME_DETECTION,
        ANIMATION,
        TIMELINE,
        NONE_CATEGORY
    };

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
        ANIMATION_FPS,
        TIMELINE_TOP_SCALE,
        TIMELINE_PADDING_SCALE,
        TIMELINE_TOP_BG_COLOR,
        TIMELINE_BOTTOM_BG_COLOR,
        TIMELINE_SELECTED_FRAME_COLOR,
        NONE_FIELD
    };

    enum MouseButtons
    {
        LEFT_MOUSE_BUTTON,
        RIGHT_MOUSE_BUTTON,
        MIDDLE_MOUSE_BUTTON
    };

private:
    struct Types
    {
        static std::string Int;
        static std::string String;
        static std::string Bool;
        static std::string Float;
        static std::string UChar3;
    };

    struct FieldKey
    {
        std::string FieldName;
        Fields Field;
        FieldCategories Category;
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
        float Float;
        unsigned char UChar3[3];
    };

public:
    static bool LoadSettings();
    static bool SaveSettings();

    static int GetInt(Fields field);
    static bool GetBool(Fields field);
    static std::string GetStr(Fields field);
    static float GetFloat(Fields field);
    static unsigned char* GetUChar3(Fields field);

    static void Set(Fields field, int value);
    static void Set(Fields field, const std::string& value);
    static void Set(Fields field, bool value);
    static void Set(Fields field, float value);
    static void Set(Fields field, unsigned char value[3]);

private:
    static Fields StringToField(const std::string& fieldName);
    static FieldCategories StringToCategory(const std::string& category);
    static FieldValue GetValueFromString(const FieldKey& key, std::string value);
    static const FieldKey& GetFieldKey(Fields field);
    static FieldValue& GetFieldValue(Fields field);

private:
    static std::map<std::string, Fields> fieldNamesMap;
    static std::map<std::string, FieldCategories> categoryNamesMap;

    static const char* settingsFile;
    static std::unordered_map<FieldKey, FieldValue, FieldKeyHash> settings;
};

#endif // SETTINGS_H
