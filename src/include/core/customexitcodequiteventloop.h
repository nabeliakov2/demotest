#ifndef CUSTOMEXITCODEQUITEVENTLOOP_H
#define CUSTOMEXITCODEQUITEVENTLOOP_H

#include <QtCore/QEventLoop>

namespace DEMO_NAMESPACE
{

class CustomExitCodeQuitEventLoop Q_DECL_FINAL : public QEventLoop
{
    Q_OBJECT
    Q_DISABLE_COPY(CustomExitCodeQuitEventLoop)

public:
    explicit CustomExitCodeQuitEventLoop(QObject *_parent = Q_NULLPTR)
            : QEventLoop(_parent) {}
    explicit CustomExitCodeQuitEventLoop(const int _exitCode, QObject *_parent = Q_NULLPTR)
        : QEventLoop(_parent), i_exitCode(_exitCode) {}
    virtual ~CustomExitCodeQuitEventLoop() Q_DECL_OVERRIDE Q_DECL_FINAL = default;

    inline void setCustomExitCode(const int _exitCode) Q_DECL_NOEXCEPT
    {
        i_exitCode = _exitCode;
    }
    inline int customExitCde() const Q_DECL_NOEXCEPT
    {
        return i_exitCode;
    }

public slots:
    void customQuit()
    {
        exit(i_exitCode);
    }
    void additionalCustomQuit()
    {
        exit(i_exitCode + 1);
    }

private:
    int i_exitCode {0};
};

} // DEMO_NAMESPACE

#endif // CUSTOMEXITCODEQUITEVENTLOOP_H
