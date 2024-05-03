#ifndef UTILITY_H
#define UTILITY_H

#include <QtCore/QObject>

#include <memory>

class QTextCodec;

namespace DEMO_NAMESPACE
{

class Utility final
{
public:
    static QTextCodec *defaultTextCodec();

    static void setTextColor(QWidget *const pWidget_, const QColor &fontColor_);
    static void setBackgroundColor(QWidget *const pWidget_, const QColor &bkgColor_);
    static bool isColorBright(const QColor &color_);

private:
    Utility() = default;
    ~Utility() = default;
};

struct ObjectDeleteLater final
{
    void operator() (QObject *pObj_)
    {
        pObj_ -> deleteLater();
    }
};

inline void doDeleteLater(QObject *pObj_)
{
    pObj_ -> deleteLater();
}

template<typename T>
using qobject_delete_later_unique_ptr = std::unique_ptr<T, ObjectDeleteLater>;



} // DEMO_NAMESPACE

#endif // UTILITY_H
