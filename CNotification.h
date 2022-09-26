#ifndef CNOTIFICATION_H
#define CNOTIFICATION_H

#include <QDialog>
#include <QMainWindow>

enum CNotificationType
{
    Information,
    Success,
    Error,
    Alert,
    Secondary
};

enum CNotificationPosition
{
    TopRight,
    TopLeft,
    TopCenter,
    BottomRight,
    BottomLeft,
    BottomCenter,
    Center
};

struct CNotificationOptions
{
    CNotificationPosition position = CNotificationPosition::TopRight;
    bool fullWidth = false;
    int fullWidthMargin = 10;
    int width = 400;
    int height = 60;
    int spacing = 10;
    int duration = 1500;
    QString secondaryForeground = "#41464B";
    QString secondaryBackground = "#E2E3E5";
    QString successForeground = "#0F5132";
    QString successBackground = "#D1E7DD";
    QString alertForeground = "#664D03";
    QString alertBackground = "#FFF3CD";
    QString errorForeground = "#842029";
    QString errorBackground = "#F8D7DA";
    QString informationForeground = "#084298";
    QString informationBackground = "#CFE2FF";

};

class CNotification: public QDialog
{
public:
    explicit CNotification(QMainWindow* parent, const QString& message, const CNotificationType& type = CNotificationType::Information);

    static QList<CNotification*> Items;
    static CNotificationOptions Options;

    static void Show(const QString& message);
    static void Error(const QString& message);
    static void Alert(const QString& message);
    static void Information(const QString& message);
    static void Secondary(const QString& message);
    static void Success(const QString& message);

    static QMainWindow* GetMainWindow();

private:
    int position;
};

#endif // CNOTIFICATION_H
