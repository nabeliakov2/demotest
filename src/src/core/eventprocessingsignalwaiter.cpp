#include "include/core/eventprocessingsignalwaiter.h"

#include <QtCore/QTimer>

#include "include/core/customexitcodequiteventloop.h"

namespace DEMO_NAMESPACE
{

namespace
{
EventProcessingSignalWaiter::ExitResult start(QObject *const _sender,
                                              const char *const _normalSignal,
                                              const QEventLoop::ProcessEventsFlags &_flags,
                                              const int _timeout,
                                              CustomExitCodeQuitEventLoop &_eventLoop)
{
    QTimer timeoutTimer;
    if (_timeout >= 0)
    {
        timeoutTimer.setSingleShot(true);
        if (!QObject::connect(&timeoutTimer, &QTimer::timeout, &_eventLoop,
                             &CustomExitCodeQuitEventLoop::customQuit))
        {
            throw std::runtime_error("Signal connection error");   // less likely
        }
    }
    if (!QObject::connect(_sender, _normalSignal, &_eventLoop, SLOT(quit())))
    {
        throw std::runtime_error("Signal connection error");
    }

    if (_timeout >= 0)
    {
        timeoutTimer.start(_timeout);
    }
    const int exitCode {_eventLoop.exec(_flags)};

    return static_cast<EventProcessingSignalWaiter::ExitResult>(exitCode);
}

}  // namespace


bool EventProcessingSignalWaiter::wait(QObject *const _sender,
                                       const char *const _normalSignal,
                                       const QEventLoop::ProcessEventsFlags &_flags /* = QEventLoop::AllEvents */,
                                       const int _timeout /* = -1 */)  // static public
{
    CustomExitCodeQuitEventLoop localEventLoop(ExitResult::Timeout);

    return !static_cast<bool>(start(_sender, _normalSignal, _flags, _timeout, localEventLoop));
}

EventProcessingSignalWaiter::ExitResult EventProcessingSignalWaiter::waitErrorChecked(QObject *const _sender,
                                                                                      const char *const _normalSignal,
                                                                                      const char *const _errorSignal,
                                                                                      const QEventLoop::ProcessEventsFlags &_flags /* = QEventLoop::AllEvents */,
                                                                                      const int _timeout /* = -1 */)    // static public
{
    CustomExitCodeQuitEventLoop localEventLoop(ExitResult::Timeout);

    if (!QObject::connect(_sender, _errorSignal, &localEventLoop, SLOT(additionalCustomQuit())))
    {
        throw std::runtime_error("Signal connection error");
    }
    return start(_sender, _normalSignal, _flags, _timeout, localEventLoop);
}

} // DEMO_NAMESPACE
