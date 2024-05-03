#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QtCore/QObject>

#include <memory>

namespace DEMO_NAMESPACE
{


class NetworkManager final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(NetworkManager)

public:
    explicit NetworkManager(QObject *_parent = nullptr);
    virtual ~NetworkManager();

public slots:
    void requestData(const QString &urlStr) noexcept;

signals:
    void dataReceived(const QByteArray &data);
    void errorOccured(const int statusCode, const QString &errorStr);

private:
    struct PrivateObject;
    const std::unique_ptr<PrivateObject> pPrivateObject;



};

} // DEMO_NAMESPACE

#endif // NETWORKMANAGER_H
