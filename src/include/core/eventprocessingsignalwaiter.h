#ifndef EVENTPROCESSINGSIGNALWAITER_H
#define EVENTPROCESSINGSIGNALWAITER_H

#include <QtCore/QEventLoop>

class QObject;

namespace DEMO_NAMESPACE
{

class EventProcessingSignalWaiter final
{

public:
    enum ExitResult {NormalExit = 0, Timeout = 1, Error = 2};

/// Using string-based signal is easier and more universal here,
/// but it may cause run-time connection errors
    static bool wait(QObject *const _sender,
                     const char *const _normalSignal,
                     const QEventLoop::ProcessEventsFlags &_flags = QEventLoop::AllEvents,
                     const int _timeout = -1);
    static ExitResult waitErrorChecked(QObject *const _sender,
                                       const char *const _normalSignal,
                                       const char *const _errorSignal,
                                       const QEventLoop::ProcessEventsFlags &_flags = QEventLoop::AllEvents,
                                       const int _timeout = -1);

private:
    EventProcessingSignalWaiter() = delete;
    EventProcessingSignalWaiter(const EventProcessingSignalWaiter &_other) = delete;
    EventProcessingSignalWaiter &operator= (const EventProcessingSignalWaiter &_other) = delete;

};

} // DEMO_NAMESPACE

#endif // EVENTPROCESSINGSIGNALWAITER_H
