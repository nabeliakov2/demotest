#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QtCore/QPointer>

#include <QtWidgets/QWidget>

class QSettings;

namespace DEMO_NAMESPACE
{

class SettingsWidget final : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(SettingsWidget)

public:
    enum SettingsKey
    {
        ServerURL = 0,
        NormalColor,
        InvertedColor,
        NormalTextColor,
        InvertedTextColor,
        SettingsKeyCount
    };

    enum ModifiedAttribute {
        Unmodified = 0x0,
        URL = 0x1,
        Colors = 0x2
    };
    Q_DECLARE_FLAGS(ModifiedAttributes, ModifiedAttribute)

    explicit SettingsWidget(QWidget *parent_ = nullptr);
    virtual ~SettingsWidget() override final;

    QVariant settingsValue(const SettingsKey key_) const;
    void setConnectionOk();
    void setConnectionError(const QString &errorStr);

    void reset();
    ModifiedAttributes modifiedAttributes() const;

signals:
    void cancelled();
    void accepted();
    void checkConnectionRequest(const QString&);

private:
    struct PrivateObject;
    const std::unique_ptr<PrivateObject> pPrivateObject;

    /// From QWidget
    virtual void resizeEvent(QResizeEvent *event_) override final;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(SettingsWidget::ModifiedAttributes)

} // DEMO_NAMESPACE

#endif // SETTINGSWIDGET_H
