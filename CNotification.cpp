#include "CNotification.h"

#include <QApplication>
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>

QList<CNotification*> CNotification::Items;
CNotificationOptions CNotification::Options;

CNotification::CNotification(QMainWindow* parent, const QString& message, const CNotificationType& type): QDialog(parent)
{
    if (parent == nullptr) return;

    QWidget* currentFocus = parent->focusWidget();

    int spacing = CNotification::Options.spacing;
    int duration = CNotification::Options.duration;

    int width = CNotification::Options.fullWidth ? parent->geometry().width() - 2 * CNotification::Options.fullWidthMargin : CNotification::Options.width;
    int height = CNotification::Options.height;

    int left = 0;
    if (CNotification::Options.fullWidth) {
        left = parent->geometry().left() + CNotification::Options.fullWidthMargin;
    } else if (CNotification::Options.position == CNotificationPosition::BottomLeft
            || CNotification::Options.position == CNotificationPosition::TopLeft) {
        left = parent->geometry().left();
    } else if (CNotification::Options.position == CNotificationPosition::BottomRight
            || CNotification::Options.position == CNotificationPosition::TopRight) {
        left = parent->geometry().right() - width;
    } else {
        left = parent->geometry().left() + (parent->geometry().width() - width) / 2;
    }

    int top = 0;
    if (CNotification::Options.position == CNotificationPosition::BottomLeft
     || CNotification::Options.position == CNotificationPosition::BottomCenter
     || CNotification::Options.position == CNotificationPosition::BottomRight) {
        this->position = -1;
        top = parent->geometry().bottom() - height - spacing - (spacing + height) * CNotification::Items.length();
    } else if (CNotification::Options.position == CNotificationPosition::TopLeft
            || CNotification::Options.position == CNotificationPosition::TopCenter
            || CNotification::Options.position == CNotificationPosition::TopRight) {
        this->position = 1;
        top = parent->geometry().top() + spacing + (spacing + height) * CNotification::Items.length();
    } else {
        this->position = 0;
        top = parent->geometry().top() + (parent->geometry().height() - height) / 2;
    }

    QString mdMessage = message;
    mdMessage.replace("\n", "<br />");

    QLabel* label = new QLabel(mdMessage);
    label->setWordWrap(true);
    label->setTextFormat(Qt::MarkdownText);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->addWidget(label);

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowDoesNotAcceptFocus);
    this->setModal(false);
    this->setGeometry(left, top, width, height);
    this->setFixedHeight(height);
    this->setLayout(layout);

    switch (type) {
    case CNotificationType::Secondary:
        label->setStyleSheet("QLabel{color:" + CNotification::Options.secondaryForeground + "}");
        this->setStyleSheet("QDialog{background-color:" + CNotification::Options.secondaryBackground + "}");
        break;
    case CNotificationType::Success:
        label->setStyleSheet("QLabel{color:" + CNotification::Options.successForeground + "}");
        this->setStyleSheet("QDialog{background-color:" + CNotification::Options.successBackground + "}");
        break;
    case CNotificationType::Alert:
        label->setStyleSheet("QLabel{color:" + CNotification::Options.alertForeground + "}");
        this->setStyleSheet("QDialog{background-color:" + CNotification::Options.alertBackground + "}");
        break;
    case CNotificationType::Error:
        label->setStyleSheet("QLabel{color:" + CNotification::Options.errorForeground + "}");
        this->setStyleSheet("QDialog{background-color:" + CNotification::Options.errorBackground + "}");
        break;
    default:
        label->setStyleSheet("QLabel{color:" + CNotification::Options.informationForeground + "}");
        this->setStyleSheet("QDialog{background-color:" + CNotification::Options.informationBackground + "}");
    }

    CNotification::Items.append(this);

    QTimer* timerStart = new QTimer();
    connect(timerStart, &QTimer::timeout, this, [=]()
    {
        this->show();
        timerStart->deleteLater();
    });
    timerStart->setSingleShot(true);
    timerStart->start(0);

    QTimer* timerEnd = new QTimer();
    connect(timerEnd, &QTimer::timeout, this, [=]()
    {
        CNotification::Items.removeOne(this);
        auto items = CNotification::Items;
        for (auto notification: items) {
            auto geometry = notification->geometry();
            geometry.setTop(geometry.top() - (spacing + height) * this->position);
            geometry.setBottom(geometry.bottom() - (spacing + height) * this->position);
            notification->setGeometry(geometry);
        }
        timerEnd->deleteLater();
        this->deleteLater();
    });
    timerEnd->setSingleShot(true);
    timerEnd->start(duration);

    if (currentFocus != nullptr) currentFocus->setFocus();
}


QMainWindow* CNotification::GetMainWindow()
{
    auto widgets = QApplication::topLevelWidgets();
    for(QWidget* widget: widgets) {
        QMainWindow* window = qobject_cast<QMainWindow*>(widget);
        if (window) return window;
    }
    return nullptr;
}

void CNotification::Show(const QString& message)
{
    new CNotification(CNotification::GetMainWindow(), message);
}

void CNotification::Error(const QString& message)
{
    new CNotification(CNotification::GetMainWindow(), message, CNotificationType::Error);
}

void CNotification::Alert(const QString& message)
{
    new CNotification(CNotification::GetMainWindow(), message, CNotificationType::Alert);
}

void CNotification::Information(const QString& message)
{
    new CNotification(CNotification::GetMainWindow(), message);
}

void CNotification::Secondary(const QString& message)
{
    new CNotification(CNotification::GetMainWindow(), message, CNotificationType::Secondary);
}

void CNotification::Success(const QString& message)
{
    new CNotification(CNotification::GetMainWindow(), message, CNotificationType::Success);
}
