#include "include/core/applicationmediator.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QTextCodec>
#include <QtCore/QVector>
#include <QtCore/QJsonDocument>
#include <QtCore/QPointer>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QParallelAnimationGroup>

#include <QtGui/QScreen>
#include <QtGui/QGuiApplication>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStackedLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

#include "include/core/utility.h"
#include "include/core/networkmanager.h"
#include "include/core/eventprocessingsignalwaiter.h"
#include "include/gui/mainwidget.h"
#include "include/gui/settingswidget.h"

namespace DEMO_NAMESPACE
{

namespace
{
bool initialized {false};

void configureTopLevelWidget(QWidget *const _pWidget);
QVariant parseJSON(const QByteArray &JSONData);
} // namespace


struct ApplicationMediator::PrivateObject final  // private
{
    Q_DISABLE_COPY(PrivateObject);

    static const int OutputScreen {0};
    static const int AttributesCount {3};
    static const int baseAnimationDuration {500}; //ms

    qobject_delete_later_unique_ptr<QWidget> pBaseWidget;   // Top-level
    MainWidget *pMainWidget {nullptr};
    SettingsWidget *pSettingsWidget {nullptr};
    NetworkManager *pNetworkManager {nullptr};
    QVBoxLayout *pBaseWidgetLayout {nullptr};
    QPropertyAnimation *slideAnimationLeft {nullptr};
    QPropertyAnimation *slideAnimationRight {nullptr};
    QParallelAnimationGroup *animationGroup {nullptr};
    QPointer<QWidget> pCurrentWidget;

    bool onlyCheckConnectionFlag {false};


    PrivateObject() = default;
    ~PrivateObject() = default;

    void createAndConfigureBaseWidget(QWidget *const parent_ = nullptr);
    void createAndConfigureMainWidget(QWidget *const parent_ = nullptr);
    void createAndConfigureSettingsWidget(QWidget *const parent_ = nullptr);
    void createAndConfigureNetworkManager(QObject *const parent_ = nullptr);
    void createAnimations(QObject *const parent_ = nullptr);
    void updateMainWidget(const QVariant &parsedData);
    void reinitMainWidgetColors() const;
    void requestData() const;
    void checkConnection(const QString &urlStr);
    void showWidget(QWidget *const widget_);
    void handleError(const QString &errorString);
};

ApplicationMediator::ApplicationMediator(QObject *parent_ /* = nullptr */) // public explicit
    : QObject(parent_), pPrivateObject(new PrivateObject)
{
    Q_ASSERT(pPrivateObject);

    if (initialized) qFatal("Already initialized");
    initialized = true;

    QTextCodec::setCodecForLocale(Utility::defaultTextCodec());

    pPrivateObject->createAndConfigureNetworkManager(this);
    pPrivateObject->createAndConfigureBaseWidget();

    Q_ASSERT(pPrivateObject->pBaseWidget);

    pPrivateObject->createAndConfigureSettingsWidget(pPrivateObject->pBaseWidget.get());
    pPrivateObject->createAndConfigureMainWidget(pPrivateObject->pBaseWidget.get());

    pPrivateObject->createAnimations(this);

    Q_ASSERT(pPrivateObject->pNetworkManager);

    if (!connect(pPrivateObject->pNetworkManager, &NetworkManager::dataReceived, this,
                 [this](const QByteArray &data_)
                 {
                    Q_ASSERT(pPrivateObject);

                    if (pPrivateObject->onlyCheckConnectionFlag)
                    {
                        Q_ASSERT(pPrivateObject->pSettingsWidget);

                        pPrivateObject->onlyCheckConnectionFlag = false;
                        pPrivateObject->pSettingsWidget->setConnectionOk();
                    }
                    else
                    {
                        QVariant parsedData {parseJSON(data_)};
                        if (!parsedData.isValid())
                        {
                            pPrivateObject->handleError(tr("JSON Parse error"));

                        }
                        else
                        {
                            pPrivateObject->updateMainWidget(parsedData);
                        }
                    }
                 }))
    {
        qCritical("Cannot connect signal to slot");
    }
    if (!connect(pPrivateObject->pNetworkManager, &NetworkManager::errorOccured, this,
                 [this](const int, const QString &errorString_)
                 {
                    Q_ASSERT(pPrivateObject);

                    if (pPrivateObject->onlyCheckConnectionFlag)
                    {
                        Q_ASSERT(pPrivateObject->pSettingsWidget);

                        pPrivateObject->onlyCheckConnectionFlag = false;
                        pPrivateObject->pSettingsWidget->setConnectionError(errorString_);
                    }
                    else
                    {
                        pPrivateObject->handleError(errorString_);
                    }
                 }))
    {
        qCritical("Cannot connect signal to slot");
    }

    if (!connect(pPrivateObject->pMainWidget, &MainWidget::showSettingsWidgetRequest, this,
                 [this]()
                 {
                    Q_ASSERT(pPrivateObject && pPrivateObject->pSettingsWidget);

                    //pPrivateObject->pBaseWidgetLayout->setCurrentWidget(pPrivateObject->pSettingsWidget);
                    pPrivateObject->pSettingsWidget->reset();
                    pPrivateObject->showWidget(pPrivateObject->pSettingsWidget);
                 }))
    {
        qCritical("Cannot connect signal to slot");
    }
    if (!connect(pPrivateObject->pMainWidget, &MainWidget::updateRequest, this,
                 [this]()
                 {
                     Q_ASSERT(pPrivateObject);

                     pPrivateObject->requestData();
                 }))
    {
        qCritical("Cannot connect signal to slot");
    }
    if (!connect(pPrivateObject->pSettingsWidget, &SettingsWidget::accepted, this,
                 [this]()
                 {
                    Q_ASSERT(pPrivateObject && pPrivateObject->pSettingsWidget);

                    if (pPrivateObject->pSettingsWidget->modifiedAttributes() & SettingsWidget::ModifiedAttribute::URL)
                    {
                        pPrivateObject->requestData();
                    }
                    if (pPrivateObject->pSettingsWidget->modifiedAttributes() & SettingsWidget::ModifiedAttribute::Colors)
                    {
                        pPrivateObject->reinitMainWidgetColors();
                    }

                    pPrivateObject->showWidget(pPrivateObject->pMainWidget);
                 }))
    {
        qCritical("Cannot connect signal to slot");
    }
    if (!connect(pPrivateObject->pSettingsWidget, &SettingsWidget::cancelled, this,
                 [this]()
                 {
                    Q_ASSERT(pPrivateObject);

                    //pPrivateObject->pBaseWidgetLayout->setCurrentWidget(pPrivateObject->pMainWidget);
                    pPrivateObject->showWidget(pPrivateObject->pMainWidget);
                 }))
    {
        qCritical("Cannot connect signal to slot");
    }
    if (!connect(pPrivateObject->pSettingsWidget, &SettingsWidget::checkConnectionRequest, this,
                 [this](const QString &urlStr)
                 {
                    Q_ASSERT(pPrivateObject);

                    pPrivateObject->checkConnection(urlStr);
                 }))
    {
        qCritical("Cannot connect signal to slot");
    }

}

ApplicationMediator::~ApplicationMediator() = default;  // virtual public override final



void ApplicationMediator::run() // public
{
    Q_ASSERT(pPrivateObject && pPrivateObject->pBaseWidget &&
             pPrivateObject->pNetworkManager && pPrivateObject->pSettingsWidget &&
             pPrivateObject->pMainWidget && pPrivateObject->pBaseWidgetLayout);

    pPrivateObject->requestData();
    pPrivateObject->pSettingsWidget->hide();
    pPrivateObject->pMainWidget->show();
    pPrivateObject->pCurrentWidget = pPrivateObject->pMainWidget;
    //pPrivateObject->pBaseWidgetLayout->setCurrentWidget(pPrivateObject->pMainWidget);
    pPrivateObject->pBaseWidget->show();
}

/////// ApplicationMediator::PrivateObject ///////
void ApplicationMediator::PrivateObject::createAndConfigureNetworkManager(QObject *const parent_ /* = nullptr */) // private::public
{
    pNetworkManager = new NetworkManager(parent_);
}

void ApplicationMediator::PrivateObject::createAnimations(QObject * const parent_ /* = nullptr */) // private::public
{
    Q_ASSERT(pSettingsWidget && pMainWidget);

    slideAnimationLeft = new QPropertyAnimation(parent_);
    slideAnimationLeft->setTargetObject(pSettingsWidget);
    slideAnimationLeft->setDuration(baseAnimationDuration);
    slideAnimationLeft->setEasingCurve(QEasingCurve(QEasingCurve::OutBack));
    slideAnimationLeft->setPropertyName("pos");

    slideAnimationRight = new QPropertyAnimation(parent_);
    slideAnimationRight->setTargetObject(pMainWidget);
    slideAnimationRight->setDuration(baseAnimationDuration);
    slideAnimationRight->setEasingCurve(QEasingCurve(QEasingCurve::OutBack));
    slideAnimationRight->setPropertyName("pos");

    animationGroup = new QParallelAnimationGroup(parent_);
    animationGroup -> addAnimation(slideAnimationLeft);
    animationGroup -> addAnimation(slideAnimationRight);
}

void ApplicationMediator::PrivateObject::createAndConfigureBaseWidget(QWidget *const parent_ /* = nullptr */) // private::public
{
    pBaseWidget.reset(new QWidget(parent_));
    configureTopLevelWidget(pBaseWidget.get());

    QScreen *const pScreen {QGuiApplication::screens().at(OutputScreen)};

    Q_ASSERT(pScreen);

    const QRect screenGeometry{pScreen->availableGeometry()};
    pBaseWidget->move(screenGeometry.topLeft());
    pBaseWidget->setFixedSize(screenGeometry.size());

    pBaseWidgetLayout = new QVBoxLayout;
    pBaseWidgetLayout->setContentsMargins(0, 0, 0, 0);
    pBaseWidgetLayout->setSpacing(0);
    pBaseWidget->setLayout(pBaseWidgetLayout);
}

void ApplicationMediator::PrivateObject::createAndConfigureMainWidget(QWidget *const parent_ /* = nullptr */) // private::public
{
    pMainWidget = new MainWidget(parent_);

    Q_ASSERT(pSettingsWidget);

    if (parent_ && parent_->layout())
    {
        parent_->layout()->addWidget(pMainWidget);
    }

    reinitMainWidgetColors();
}

void ApplicationMediator::PrivateObject::createAndConfigureSettingsWidget(QWidget *const parent_ /* = nullptr */) // private::public
{
    pSettingsWidget = new SettingsWidget(parent_);
}

void ApplicationMediator::PrivateObject::updateMainWidget(const QVariant &parsedData)  // private::public
{
    static const QString ModeAttr {"MODE"};
    static const QString LineAttr {"LINE"};
    static const QString TextAttr {"TEXT"};

    static const QString NormalVal {"NORMAL"};
    static const QString InvertedVal {"INVERSE"};

    Q_ASSERT(pMainWidget);

    if (!parsedData.canConvert<QVariantList>())
    {
        handleError(tr("Invalid JSON"));
        return;
    }

    pMainWidget->clear();

    MainWidget::LineMode mode = MainWidget::LineMode::Unknown;
    int line = -1;
    QString text;
    bool gotText {false};
    QSequentialIterable it {parsedData.value<QSequentialIterable>()};
    for (const QVariant &attr : it)
    {
        if (attr.canConvert<QVariantMap>())
        {
            const QVariantMap attrMap(attr.toMap());

            if (attrMap.contains(ModeAttr))
            {
                const QString modeStr {attrMap.value(ModeAttr).toString()};
                if (modeStr == NormalVal) mode = MainWidget::LineMode::Normal;
                else if (modeStr == InvertedVal) mode = MainWidget::LineMode::Inverted;
                else
                {
                    handleError(tr("Invalid JSON Format"));
                    return;
                }
            }
            else if (attrMap.contains(LineAttr))
            {
                bool s;
                line = attrMap.value(LineAttr).toInt(&s);
                if (!s)
                {
                    handleError(tr("Invalid JSON Format"));
                    return;
                }
            }
            else if (attrMap.contains(TextAttr))
            {
                text = attrMap.value(TextAttr).toString();
                gotText = true;
            }
            else
            {
                qWarning("Invalid JSON Element");
            }
            if (mode != MainWidget::LineMode::Unknown && line >= 0 && gotText)
            {
                pMainWidget->updateLine(line, mode, text);

                mode = MainWidget::LineMode::Unknown;
                line = -1;
                gotText = false;
            }
        }
        else
        {
            handleError(tr("Invalid JSON Format"));
            return;
        }
    }
}

void ApplicationMediator::PrivateObject::reinitMainWidgetColors() const // private::public
{
    pMainWidget->initColors(QColor(pSettingsWidget->settingsValue(SettingsWidget::NormalColor).toString()),
                            QColor(pSettingsWidget->settingsValue(SettingsWidget::NormalTextColor).toString()),
                            QColor(pSettingsWidget->settingsValue(SettingsWidget::InvertedColor).toString()),
                            QColor(pSettingsWidget->settingsValue(SettingsWidget::InvertedTextColor).toString()));
    pMainWidget->update();
}

void ApplicationMediator::PrivateObject::requestData() const // private::public
{
    Q_ASSERT(pNetworkManager);


    pNetworkManager->requestData(pSettingsWidget->settingsValue(SettingsWidget::ServerURL).toString());
}

void ApplicationMediator::PrivateObject::checkConnection(const QString &urlStr) // private::public
{
    Q_ASSERT(pNetworkManager);

    onlyCheckConnectionFlag = true;
    pNetworkManager->requestData(urlStr);
}

void ApplicationMediator::PrivateObject::showWidget(QWidget *const widget_)    // private::public
{
    Q_ASSERT(pSettingsWidget && pMainWidget && pBaseWidgetLayout);

    if (widget_ == pSettingsWidget)
    {
        //pBaseWidgetLayout->removeWidget(pSettingsWidget);
        //pSettingsWidget->hide();
        pSettingsWidget->resize(pBaseWidget->size());
        pMainWidget->resize(pBaseWidget->size());
        pSettingsWidget->move(-pSettingsWidget->width(), pSettingsWidget->y());

        slideAnimationLeft->setStartValue(pSettingsWidget->pos());
        slideAnimationLeft->setEndValue(pMainWidget->pos());
        slideAnimationRight->setStartValue(pMainWidget->pos());
        slideAnimationRight->setEndValue(QPoint(pBaseWidget->width(), pMainWidget->y()));
        pBaseWidgetLayout->removeWidget(pMainWidget);
        pCurrentWidget = pSettingsWidget;
    }
    else if (widget_ == pMainWidget)
    {
        //pMainWidget->hide();
        pSettingsWidget->resize(pBaseWidget->size());
        pMainWidget->resize(pBaseWidget->size());
        pMainWidget->move(pBaseWidget->width(), pBaseWidget->y());

        slideAnimationLeft->setStartValue(pSettingsWidget->pos());
        slideAnimationLeft->setEndValue(QPoint(-pSettingsWidget->width(), pSettingsWidget->y()));
        slideAnimationRight->setStartValue(pMainWidget->pos());
        slideAnimationRight->setEndValue(pSettingsWidget->pos());
        pBaseWidgetLayout->removeWidget(pSettingsWidget);
        pCurrentWidget = pMainWidget;
    }
    else
    {
        Q_ASSERT(false);
    }

    pSettingsWidget->show();
    pMainWidget->show();
    animationGroup->start();
    if (animationGroup->state() == QAbstractAnimation::Running)
    {
        const bool wasSignal {EventProcessingSignalWaiter::wait(animationGroup,
                                                                SIGNAL(finished()),
                                                                QEventLoop::ExcludeUserInputEvents,
                                                                animationGroup->duration() + 1000)};
        if (!wasSignal)
        {
            qWarning() << "Animation error";
        }
    }
    if (pCurrentWidget != widget_) return;

    if (widget_ == pSettingsWidget)
    {
        pBaseWidgetLayout->addWidget(pSettingsWidget);
        pMainWidget->hide();
    }
    else if (widget_ == pMainWidget)
    {
        pBaseWidgetLayout->addWidget(pMainWidget);
        pSettingsWidget->hide();
    }
}

void ApplicationMediator::PrivateObject::handleError(const QString &errorString)
{
    QMessageBox msgBox(QMessageBox::Critical, {}, errorString, QMessageBox::NoButton, nullptr, Qt::Dialog | Qt::FramelessWindowHint);
    QAbstractButton *const toSettingsButton {msgBox.addButton(tr("Go to settings..."), QMessageBox::ActionRole)};
    QAbstractButton *const cancelButton {msgBox.addButton(QMessageBox::Cancel)};
    msgBox.setIcon(QMessageBox::Critical);

    //msgBox.setIconPixmap(QPixmap(":/icons/res/dialog-error.png"));
    //cancelButton->setIcon(QPixmap(":/icons/res/dialog-cancel.png"));

    Q_ASSERT(pSettingsWidget && pMainWidget);

    msgBox.setFont(pMainWidget->font());
    msgBox.setPalette(pSettingsWidget->palette());
    cancelButton->setIconSize({cancelButton->font().pixelSize(), cancelButton->font().pixelSize()});

    msgBox.exec();

    if (msgBox.clickedButton() == toSettingsButton)
    {
        Q_ASSERT(pBaseWidgetLayout && pSettingsWidget);

        //pBaseWidgetLayout->sho setCurrentWidget(pSettingsWidget);

        showWidget(pSettingsWidget);
    }
}

namespace
{
void configureTopLevelWidget(QWidget *const _pWidget)
{
    Q_ASSERT(_pWidget);

    _pWidget -> setWindowFlags(_pWidget -> windowFlags() | Qt::FramelessWindowHint);
}


QVariant parseJSON(const QByteArray &JSONData_)
{
    QJsonParseError parseError;
    QJsonDocument doc {QJsonDocument::fromJson(JSONData_, &parseError)};
    if (parseError.error == QJsonParseError::NoError)
    {
        return doc.toVariant();
    }
    qWarning() << "Data parsing error:" << parseError.errorString();
    return {};
}

}   // namespace


} // DEMO_NAMESPACE

