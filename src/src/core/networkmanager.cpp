#include "include/core/networkmanager.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif // Q_OS_ANDROID

namespace DEMO_NAMESPACE
{

struct NetworkManager::PrivateObject final  // private
{
    Q_DISABLE_COPY(PrivateObject)

    QNetworkAccessManager *pNetworkAccessManager {nullptr};

    explicit PrivateObject(QObject *const pOwnerObject_);
    ~PrivateObject() = default;
};

NetworkManager::NetworkManager(QObject *_parent /* = nullptr */) // explicit public
    : QObject {_parent},
    pPrivateObject(new PrivateObject(this))
{
    Q_ASSERT(pPrivateObject && pPrivateObject->pNetworkAccessManager);

    if (!connect(pPrivateObject->pNetworkAccessManager, &QNetworkAccessManager::finished, this,
                 [this](QNetworkReply *const reply)
                 {
                    if (reply->error() == QNetworkReply::NoError)
                    {
                        emit dataReceived(reply->readAll());
                    }
                    else
                    {
                        emit errorOccured(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
                                          reply->errorString());
                    }

                 }))
    {
        qCritical("Cannot connect signal to slot");
    }
}

NetworkManager::~NetworkManager() = default;   // virtual public override final

void NetworkManager::requestData(const QString &urlStr) noexcept // public slot
{
    Q_ASSERT(pPrivateObject && pPrivateObject->pNetworkAccessManager);

    try
    {
#ifdef Q_OS_ANDROID
        if (urlStr.startsWith("file://"))
        {
            QtAndroid::PermissionResult pr {QtAndroid::checkPermission("android.permission.READ_EXTERNAL_STORAGE")};
            if (pr == QtAndroid::PermissionResult::Denied)
            {
                QtAndroid::requestPermissionsSync({"android.permission.READ_EXTERNAL_STORAGE"});
            }
        }
#endif // Q_OS_ANDROID
        QNetworkRequest request {QUrl(urlStr)};
        pPrivateObject->pNetworkAccessManager->get(request);
    }
    catch (...)
    {
        qCritical("Unhandled exception");
    }

}

//////////// NetworkManager::PrivateObject ////////////////
NetworkManager::PrivateObject::PrivateObject(QObject *const pOwnerObject_)  // explicit private::public
    : pNetworkAccessManager {new QNetworkAccessManager(pOwnerObject_)}
{}




} // DEMO_NAMESPACE
