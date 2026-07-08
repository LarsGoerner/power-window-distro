#ifndef THEME_HPP_
#define THEME_HPP_

#include <QObject>
#include <QColor>
#include <QStringList>

class Theme : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QColor bgColor READ bgColor NOTIFY themeChanged)
        Q_PROPERTY(QColor fgColor READ fgColor NOTIFY themeChanged)
        Q_PROPERTY(QColor accentColor READ accentColor NOTIFY themeChanged)
        Q_PROPERTY(QString currentTheme READ currentTheme NOTIFY themeChanged)
        Q_PROPERTY(QStringList themes READ themes CONSTANT)
        Q_PROPERTY(QStringList themeDisplayNames READ themeDisplayNames CONSTANT)

        Q_PROPERTY(int fontSizeS READ fontSizeS CONSTANT)
        Q_PROPERTY(int fontSizeM READ fontSizeM CONSTANT)
        Q_PROPERTY(int fontSizeL READ fontSizeL CONSTANT)
        Q_PROPERTY(int fontSizeXL READ fontSizeXL CONSTANT)
        
        Q_PROPERTY(int spacingS READ spacingS CONSTANT)
        Q_PROPERTY(int spacingM READ spacingM CONSTANT)
        Q_PROPERTY(int spacingL READ spacingL CONSTANT)

public:
        explicit Theme(QObject * parent = nullptr);

        QColor bgColor() const;
        QColor fgColor() const;
        QColor accentColor() const;
        QString currentTheme() const;
        QStringList themes() const;

        int fontSizeS() const { return 18; }
        int fontSizeM() const { return 24; }
        int fontSizeL() const { return 30; }
        int fontSizeXL() const { return 36; }

        int spacingS() const { return 6; }
        int spacingM() const { return 12; }
        int spacingL() const { return 24; }

        Q_INVOKABLE void setTheme(const QString &name);
        Q_INVOKABLE void setThemeByIndex(int idx);
        QStringList themeDisplayNames() const;

signals:
        void themeChanged();

private:
        struct ThemeData { QColor bg, fg, accent; };
        ThemeData themeData(const QString &name) const;
        void applyTheme(const ThemeData &data);

        QString mCurrentTheme;
        QColor mBgColor;
        QColor mFgColor;
        QColor mAccentColor;
};

#endif // THEME_HPP_