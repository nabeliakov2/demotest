#ifndef APPLICATIONLAUNCHER_H
#define APPLICATIONLAUNCHER_H

#include <QtCore/QObject>

#include <memory>

namespace DEMO_NAMESPACE
{

class ApplicationLauncher final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ApplicationLauncher)

public:
    explicit ApplicationLauncher(QObject *parent_ = nullptr);
    virtual ~ApplicationLauncher() override final;

signals:

private:
    struct PrivateObject;
    const std::unique_ptr<PrivateObject> pPrivateObject;

};

} // DEMO_NAMESPACE

#endif // APPLICATIONLAUNCHER_H
