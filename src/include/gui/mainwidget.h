#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>

#include <memory>

namespace DEMO_NAMESPACE
{

class MainWidget final : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(MainWidget)

public:
    enum class LineMode {Unknown = 0, Normal, Inverted};

    explicit MainWidget(QWidget *_parent = nullptr);
    virtual ~MainWidget() override final;

    void initColors(const QColor &normalColor_, const QColor &normalTextColor_,
                    const QColor &invertedColor_, const QColor &invertedTextColor_);

    void setNormalColor(const QColor &color_);
    const QColor &normalColor() const;

    void setNormalTextColor(const QColor &color_);
    const QColor &normalTextColor() const;

    void setInvertedColor(const QColor &color_);
    const QColor &invertedColor() const;

    void setInvertedTextColor(const QColor &color_);
    const QColor &invertedTextColor() const;

    void updateLine(const int line_, const LineMode mode_, const QString &text_);
    void clear();

signals:
    void showSettingsWidgetRequest();
    void updateRequest();

private:
    struct PrivateObject;
    const std::unique_ptr<PrivateObject> pPrivateObject;

    /// From QWidget
    virtual void resizeEvent(QResizeEvent *event_) override final;
};

} // DEMO_NAMESPACE

#endif // MAINWIDGET_H
