#include "include/core/applicationlauncher.h"

#include <QtCore/QFile>
#include <QtCore/QTranslator>

#include <QtWidgets/QApplication>

#include "include/core/applicationmediator.h"
#include "include/core/utility.h"

namespace DEMO_NAMESPACE
{

namespace
{
bool initialized {false};

void configApplication(QApplication *const _pApp);
} // namespace



struct ApplicationLauncher::PrivateObject final  // private
{
    qobject_delete_later_unique_ptr<ApplicationMediator> pApplicationMediator;

    void init(QObject *const _pContext); // Initial configuring and request for invoking initAsync() in the Event Loop
    void initAsync() Q_DECL_NOEXCEPT; // Invoked in the Event Loop ASAP
};



ApplicationLauncher::ApplicationLauncher(QObject *parent_ /* = nullptr */)  // explicit public
    : QObject {parent_},
    pPrivateObject(new PrivateObject)
{
  Q_ASSERT(pPrivateObject);

  pPrivateObject->init(this);
}

ApplicationLauncher::~ApplicationLauncher() = default;  // virtual public override final


/////// ApplicationLauncher::PrivateObject ///////
void ApplicationLauncher::PrivateObject::init(QObject *const _pContext) // private::public
{
    if (initialized)
    {
        qFatal("Already initialized");
    }
    initialized = true;

    QCoreApplication::setApplicationName("Demo_Test_App");
    QCoreApplication::setOrganizationName("DemoCorp");
    QCoreApplication::setOrganizationDomain("demo.test.com");
    QCoreApplication::setApplicationVersion(QString(APPLICATION_VERSION));

    Q_ASSERT(qApp);

    configApplication(qApp);

    pApplicationMediator.reset(new ApplicationMediator());

    if (!QMetaObject::invokeMethod(_pContext,
                                   [this] {initAsync();},
                                   Qt::QueuedConnection))
    {
        qFatal("Cannot start application");
    }
}

void ApplicationLauncher::PrivateObject::initAsync() Q_DECL_NOEXCEPT // private::public
{
    try
    {
        Q_ASSERT(pApplicationMediator);

        pApplicationMediator->run();
    }
    catch (...)
    {
        qCritical("Unhandled exception");
    }
}

namespace
{
void configApplication(QApplication *const pApp_)
{
    Q_ASSERT(pApp_);

    pApp_->setQuitOnLastWindowClosed(true);

    pApp_->setEffectEnabled(Qt::UI_AnimateCombo, true);
    pApp_->setEffectEnabled(Qt::UI_AnimateTooltip, true);
    pApp_->setEffectEnabled(Qt::UI_AnimateMenu, true);
}

}   // namespace

} // DEMO_NAMESPACE
