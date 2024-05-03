#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <QtWidgets/QComboBox>

#include <memory>

namespace DEMO_NAMESPACE
{

class ColorComboBox final : public QComboBox
{
    Q_OBJECT
    Q_DISABLE_COPY(ColorComboBox);

public:
    enum class Type {Backround, Text};

    explicit ColorComboBox(const Type type_, QWidget *parent_ = nullptr);
    virtual ~ColorComboBox() override final;

    void setCurrentColor(const QColor &color_);
    QColor currentColor() const;
    QColor colorAt(const int i_) const;

signals:

private:
    struct PrivateObject;
    const std::unique_ptr<PrivateObject> pPrivateObject;

};

} // DEMO_NAMESPACE

#endif // COLORCOMBOBOX_H
