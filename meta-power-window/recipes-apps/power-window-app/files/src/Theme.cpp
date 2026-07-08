#include <cstring>
#include <array>
#include <algorithm>
#include <QSettings>
#include <QColor>
#include <QString>
#include <QStringList>
#include "Theme.hpp"

#define SET_PATH        "Theme/"
#define SET_MODE_PATH   SET_PATH "theme"

struct ThemeColors { const char * name, * bg, * fg, * accent; };

constexpr std::array<ThemeColors, 8> THEMES {{
        ThemeColors{ "Cyberpunk Oasis", "#1E112A", "#E0F2FE", "#10B981" },
        ThemeColors{ "Retro Sunset",    "#FFF7ED", "#431407", "#EA580C" },
        ThemeColors{ "Nordic Forest",   "#064E3B", "#ECFDF5", "#F59E0B" },
        ThemeColors{ "Electric Berry",  "#312E81", "#FCE7F3", "#F43F5E" },
        ThemeColors{ "Tropical Punch",  "#581C87", "#FDF4FF", "#FB923C" },
        ThemeColors{ "Desert Mirage",   "#FFF7ED", "#9A3412", "#14B8A6" },
        ThemeColors{ "Ocean Depth",     "#1E3A8A", "#E0F7FA", "#FACC15" },
        ThemeColors{ "Berry Harvest",   "#FFFAF5", "#831843", "#D946EF" }
}};

Theme::Theme(QObject * parent) : QObject(parent)
{
        QSettings settings;
        QString saved = settings.value(SET_MODE_PATH, THEMES[0].name).toString();
        applyTheme(themeData(saved));
        mCurrentTheme = saved;
}

QColor Theme::bgColor() const { return mBgColor; }
QColor Theme::fgColor() const { return mFgColor; }
QColor Theme::accentColor() const { return mAccentColor; }
QString Theme::currentTheme() const { return mCurrentTheme; }

void Theme::setThemeByIndex(int idx)
{
        if (idx >= 0 && idx < THEMES.size()) { setTheme(THEMES[idx].name); }
}

QStringList Theme::themeNames() const
{
        QStringList names;
        names.reserve(THEMES.size());
        for (const auto &theme : THEMES) { names.append(QString::fromUtf8(theme.name)); }
        return names;
}

void Theme::setTheme(const QString &name)
{
        if (name == mCurrentTheme) { return; }
        applyTheme(themeData(name));
        mCurrentTheme = name;
        QSettings settings;
        settings.setValue(SET_MODE_PATH, name);
        emit themeChanged();
}

Theme::ThemeData Theme::themeData(const QString &name) const
{
        const char * themeName = name.toStdString().c_str();
        for (const auto &theme : THEMES) {
                if (std::strcmp(theme.name, themeName) == 0) {
                        return ThemeData{ QColor(theme.bg), QColor(theme.fg), QColor(theme.accent) };
                }
        }
        // default if no theme was found
        return ThemeData{ QColor("#0D0E12"), QColor("#E4E4E7"), QColor("#F43F5E") };
}

void Theme::applyTheme(const ThemeData &data)
{
        mBgColor = data.bg;
        mFgColor = data.fg;
        mAccentColor = data.accent;
}