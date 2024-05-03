#include "include/core/utility.h"

#include <QtCore/QTextCodec>

#include <QtWidgets/QWidget>

namespace DEMO_NAMESPACE
{

void Utility::setTextColor(QWidget *const pWidget_, const QColor &fontColor_)   // static public
{
    QPalette widgetPalette(pWidget_->palette());
    widgetPalette.setColor(QPalette::Normal, QPalette::WindowText, fontColor_);
    widgetPalette.setColor(QPalette::Inactive, QPalette::WindowText, fontColor_);
    widgetPalette.setColor(QPalette::Disabled, QPalette::WindowText, fontColor_);
    widgetPalette.setColor(QPalette::Normal, QPalette::ButtonText, fontColor_);
    widgetPalette.setColor(QPalette::Inactive, QPalette::ButtonText, fontColor_);
    widgetPalette.setColor(QPalette::Disabled, QPalette::ButtonText, fontColor_);
    widgetPalette.setColor(QPalette::Normal, QPalette::Text, fontColor_);
    widgetPalette.setColor(QPalette::Inactive, QPalette::Text, fontColor_);
    widgetPalette.setColor(QPalette::Disabled, QPalette::Text, fontColor_);
    pWidget_->setPalette(widgetPalette);
}

void Utility::setBackgroundColor(QWidget *const pWidget_, const QColor &bkgColor_)  // static public
{
    QPalette widgetPalette(pWidget_->palette());
    widgetPalette.setColor(QPalette::Normal, QPalette::Window, bkgColor_);
    widgetPalette.setColor(QPalette::Inactive, QPalette::Window, bkgColor_);
    widgetPalette.setColor(QPalette::Disabled, QPalette::Window, bkgColor_);
    widgetPalette.setColor(QPalette::Normal, QPalette::Button, bkgColor_);
    widgetPalette.setColor(QPalette::Inactive, QPalette::Button, bkgColor_);
    widgetPalette.setColor(QPalette::Disabled, QPalette::Button, bkgColor_);
    widgetPalette.setColor(QPalette::Normal, QPalette::Base, bkgColor_);
    widgetPalette.setColor(QPalette::Inactive, QPalette::Base, bkgColor_);
    widgetPalette.setColor(QPalette::Disabled, QPalette::Base, bkgColor_);
    pWidget_->setPalette(widgetPalette);
}

QTextCodec *Utility::defaultTextCodec()  // static public
{
    static const QByteArray DefaultTextCodecName {"UTF-8"};
    static QTextCodec *const PDefaultTextCodec {QTextCodec::codecForName(DefaultTextCodecName)};

    return PDefaultTextCodec;
}

bool Utility::isColorBright(const QColor &color_) // static public
{
    return color_.red() * 0.299 + color_.green() * 0.587 + color_.blue() * 0.114 > 186.0;
}

} // DEMO_NAMESPACE
