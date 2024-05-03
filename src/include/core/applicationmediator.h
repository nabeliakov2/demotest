#ifndef APPLICATIONMEDIATOR_H
#define APPLICATIONMEDIATOR_H

#include <QtCore/QObject>

#include <memory>

namespace DEMO_NAMESPACE
{

class ApplicationMediator final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ApplicationMediator)

public:
    explicit ApplicationMediator(QObject *parent_ = nullptr);
    virtual ~ApplicationMediator();

    void run();

signals:

private:
    struct PrivateObject;
    const std::unique_ptr<PrivateObject> pPrivateObject;


};

} // DEMO_NAMESPACE

#endif // APPLICATIONMEDIATOR_H
