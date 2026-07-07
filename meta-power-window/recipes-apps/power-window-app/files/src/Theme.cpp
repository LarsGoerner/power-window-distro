#include <QSettings>
#include "Theme.hpp"

Theme::Theme(QObject * parent) : QObject(parent)
{
        QSettings settings;
        QString saved = settings.value("theme", "dark_blue").toString();
        applyTheme(themeData(saved));
        mCurrentTheme = saved;
}

QColor Theme::bgColor() const { return mBgColor; }
QColor Theme::fgColor() const { return mFgColor; }
QColor Theme::accentColor() const { return mAccentColor; }
QString Theme::currentTheme() const { return mCurrentTheme; }
QStringList Theme::themes() const { return { "dark_blue", "light_blue", "dark_red", "light_red", "dark_green", "light_green" }; }

void Theme::setThemeByIndex(int idx) { setTheme(themes().value(idx)); }
QStringList Theme::themeDisplayNames() const { return { "Dark Blue", "Light Blue", "Dark Red", "Light Red", "Dark Green", "Light Green" }; }

void Theme::setTheme(const QString &name)
{
        if (name == mCurrentTheme) { return; }
        applyTheme(themeData(name));
        mCurrentTheme = name;
        QSettings settings;
        settings.setValue("theme", name);
        emit themeChanged();
}

Theme::ThemeData Theme::themeData(const QString &name) const
{
        if (name == "light_blue") { return { "#e0fbfc", "#0d1b2a", "#98c1d9" }; }
        if (name == "dark_red") { return { "#1a0a0a", "#f0e0e0", "#4a1515" }; }
        if (name == "light_red") { return { "#fce4e4", "#1a0a0a", "#e57373" }; }
        if (name == "dark_green") { return { "#0a1a0a", "#e0f0e0", "#1a4a1a" }; }
        if (name == "light_green") { return { "#e4fce4", "#0a1a0a", "#73e573" }; }
        /* default: dark_blue */ return { "#0d1b2a", "#e0e1dd", "#1b263b" };
}

void Theme::applyTheme(const ThemeData &data)
{
        mBgColor = data.bg;
        mFgColor = data.fg;
        mAccentColor = data.accent;
}