#include "include/gui/mainwidget.h"

#include <QtCore/QSettings>

#include <QtGui/QResizeEvent>
#include <QtGui/QFont>
//#include <QtGui/QFontDatabase>

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>

#include <vector>

#include "include/core/utility.h"

namespace DEMO_NAMESPACE
{

namespace
{
QGridLayout *createGridLayout(QWidget *const parentWidget_ = nullptr);

}

struct MainWidget::PrivateObject final  // private
{
    static const int RowCount {15};
    static const int Margin {10};
    static const QString controlButtonStyleSheet;

    QColor normalColor {Qt::white};
    QColor normalTextColor {Qt::black};
    QColor invertedColor {Qt::black};
    QColor invertedTextColor {Qt::white};

    QVector<QLabel*> labels;
    std::vector<bool> isInverted;
    QAbstractButton *settingsButton;
    QAbstractButton *updateButton;

    explicit PrivateObject(QWidget *const parentWidget_);
    ~PrivateObject() = default;

    void updateColors();
    void updateButtons();
    QAbstractButton *createButton(QWidget *const parentWidget_ = nullptr);


};

const QString MainWidget::PrivateObject::controlButtonStyleSheet {
                "QPushButton:flat {background-color: transparent; color: %1;}"
                "QPushButton:flat:pressed {background-color: #ff6700; border: none;}"}; // static private::public


MainWidget::MainWidget(QWidget *parent_ /* = nullptr */) // explicit public
    : QWidget {parent_},
      pPrivateObject {new PrivateObject {this}}
{
    Q_ASSERT(pPrivateObject && pPrivateObject->updateButton && pPrivateObject->settingsButton);

    if (!connect(pPrivateObject->settingsButton, &QPushButton::clicked, this,
                 [this]()
                 {
                    emit showSettingsWidgetRequest();
                 }))
    {
        qCritical("Cannot connect signal and slot");
    }
    if (!connect(pPrivateObject->updateButton, &QPushButton::clicked, this,
                 [this]()
                 {
                     emit updateRequest();
                 }))
    {
        qCritical("Cannot connect signal and slot");
    }
}

MainWidget::~MainWidget() = default;    // public override final


void MainWidget::initColors(const QColor &normalColor, const QColor &normalTextColor,
                            const QColor &invertedColor, const QColor &invertedTextColor)   // public
{
    pPrivateObject->normalColor = normalColor;
    pPrivateObject->normalTextColor = normalTextColor;
    pPrivateObject->invertedColor = invertedColor;
    pPrivateObject->invertedTextColor = invertedTextColor;
    pPrivateObject->updateColors();
}

void MainWidget::setNormalColor(const QColor &color)    // public
{
    Q_ASSERT(pPrivateObject);

    pPrivateObject->normalColor = color;
    pPrivateObject->updateColors();
}

const QColor &MainWidget::normalColor() const  // public
{
    Q_ASSERT(pPrivateObject);

    return pPrivateObject->normalColor;
}

void MainWidget::setNormalTextColor(const QColor &color)    // public
{
    pPrivateObject->normalTextColor = color;
    pPrivateObject->updateColors();
}

const QColor &MainWidget::normalTextColor() const  // public
{
    Q_ASSERT(pPrivateObject);

    return pPrivateObject->normalTextColor;
}

void MainWidget::setInvertedColor(const QColor &color)  // public
{
    pPrivateObject->invertedColor = color;
    pPrivateObject->updateColors();
}

const QColor &MainWidget::invertedColor() const    // public
{
    Q_ASSERT(pPrivateObject);

    return pPrivateObject->invertedColor;
}

void MainWidget::setInvertedTextColor(const QColor &color)  // public
{
    pPrivateObject->invertedTextColor = color;
    pPrivateObject->updateColors();
}

const QColor &MainWidget::invertedTextColor() const    // public
{
    Q_ASSERT(pPrivateObject);

    return pPrivateObject->invertedTextColor;
}

void MainWidget::updateLine(const int line_, const LineMode mode_, const QString &text_)    // public
{
    Q_ASSERT(pPrivateObject);

    const int n {static_cast<int>(pPrivateObject->labels.size())};
    if (line_ < 0 || line_ >= n || mode_ == LineMode::Unknown)
    {
        // handle error
        return;
    }

    QLabel *const pLabel {pPrivateObject->labels.at(line_)};

    Q_ASSERT(pLabel);

    pPrivateObject->isInverted[line_] = mode_==LineMode::Inverted;
    if (mode_ == LineMode::Normal)
    {
        Utility::setBackgroundColor(pLabel, pPrivateObject->normalColor);
        Utility::setTextColor(pLabel, pPrivateObject->normalTextColor);
    }
    else if (mode_ == LineMode::Inverted)
    {
        Utility::setBackgroundColor(pLabel, pPrivateObject->invertedColor);
        Utility::setTextColor(pLabel, pPrivateObject->invertedTextColor);
    }
    pLabel->setText(text_);
    if (!line_)
    {
        pPrivateObject->updateButtons();
    }
}

void MainWidget::clear()    // public
{
    Q_ASSERT(pPrivateObject);

    for (int i = 0; i < pPrivateObject->RowCount; ++i)
    {
        QLabel *const pLabel {pPrivateObject->labels.at(i)};

        Q_ASSERT(pLabel);

        Utility::setBackgroundColor(pLabel, pPrivateObject->normalColor);
        Utility::setTextColor(pLabel, pPrivateObject->normalTextColor);
        pPrivateObject->isInverted[i] = false;
        pLabel->clear();
    }
}

void MainWidget::resizeEvent(QResizeEvent *event_)  // virtual protected override final
{
    QWidget::resizeEvent(event_);

    Q_ASSERT(pPrivateObject);

    const int fontPixelSize {event_->size().height() / PrivateObject::RowCount - (PrivateObject::Margin << 1)};


    QFont currentFont(font());
    currentFont.setPixelSize(fontPixelSize);
    setFont(currentFont);

    currentFont = pPrivateObject->settingsButton->font();
    currentFont.setPixelSize(fontPixelSize);
    pPrivateObject->settingsButton->setFont(currentFont);

    currentFont = pPrivateObject->updateButton->font();
    currentFont.setPixelSize(fontPixelSize);
    pPrivateObject->updateButton->setFont(currentFont);

    pPrivateObject->settingsButton->setFixedSize(pPrivateObject->labels.at(0)->height(),pPrivateObject->labels.at(0)->height());
    pPrivateObject->updateButton->setFixedSize(pPrivateObject->labels.at(0)->height(),pPrivateObject->labels.at(0)->height());

    const QSize iconSize {pPrivateObject->updateButton->size() - QSize(PrivateObject::Margin << 1, PrivateObject::Margin << 1)};

    pPrivateObject->settingsButton->setIconSize(iconSize);
    pPrivateObject->updateButton->setIconSize(iconSize);

}

/////// MainWidget::PrivateObject ///////
MainWidget::PrivateObject::PrivateObject(QWidget *const parentWidget_)  // explicit private::public
    : isInverted(RowCount, false)
{
    QGridLayout *widgetLayout(createGridLayout());
    for (int i = 0; i < RowCount; ++i)
    {
        QLabel *const pLabel {new QLabel(parentWidget_)};
        pLabel -> setAutoFillBackground(true);
        pLabel -> setAlignment(Qt::AlignCenter);
        labels.push_back(pLabel);
        widgetLayout->addWidget(pLabel, i, 0);
    }

    Q_ASSERT(!labels.empty());

    QLabel *const pTopLabel {labels.at(0)};

    Q_ASSERT(pTopLabel);

    settingsButton = createButton(pTopLabel);
    //settingsButton->setText("⋯");

    updateButton = createButton(pTopLabel);
    //updateButton->setText("⟳");
    //updateButton->setIcon(QPixmap(":/icons/res/view-refresh.png"));


    QHBoxLayout *topLabelLayout {new QHBoxLayout};
    topLabelLayout->setContentsMargins(0, 0, 0, 0);
    topLabelLayout->setSpacing(0);
    topLabelLayout->addWidget(settingsButton);
    topLabelLayout->addStretch();
    topLabelLayout->addWidget(updateButton);

    pTopLabel->setLayout(topLabelLayout);

    updateColors();

    parentWidget_->setLayout(widgetLayout);
}



void MainWidget::PrivateObject::updateColors()  // private::public
{
    Q_ASSERT(!labels.empty());

    for (int i = 0; i < RowCount; ++i)
    {
        Q_ASSERT(labels.at(i));

        if (!isInverted.at(i))
        {
            Utility::setBackgroundColor(labels.at(i), normalColor);
            Utility::setTextColor(labels.at(i), normalTextColor);
        }
        else
        {
            Utility::setBackgroundColor(labels.at(i), invertedColor);
            Utility::setTextColor(labels.at(i), invertedTextColor);
        }
    }

    updateButtons();
}

void MainWidget::PrivateObject::updateButtons() // private::public
{
    const QColor fontColor {isInverted.at(0) ? invertedTextColor : normalTextColor};
    const QColor bacgroundColor {isInverted.at(0) ? invertedColor : normalColor};
    const QString fontColorName {fontColor.name()};

    const QString settingsIconPixmapPath {Utility::isColorBright(bacgroundColor) ? ":/icons/res/dots-black.png" : ":/icons/res/dots-white.png"};
    const QString updateIconPixmapPath {Utility::isColorBright(bacgroundColor) ? ":/icons/res/refresh-black.png" : ":/icons/res/refresh-white.png"};

    settingsButton->setStyleSheet(controlButtonStyleSheet.arg(fontColorName));
    updateButton->setStyleSheet(controlButtonStyleSheet.arg(fontColorName));
    settingsButton->setIcon(QPixmap(settingsIconPixmapPath));
    updateButton->setIcon(QPixmap(updateIconPixmapPath));
}

QAbstractButton *MainWidget::PrivateObject::createButton(QWidget *const parentWidget_ /* = nullptr */)  // private::public
{
    QPushButton *const button {new QPushButton(parentWidget_)};
    button->setFlat(true);
    const QString fontColorName {isInverted.at(0) ? invertedTextColor.name() : normalTextColor.name()};

    button->setStyleSheet(controlButtonStyleSheet.arg(fontColorName));
    return button;
}

namespace
{

QGridLayout *createGridLayout(QWidget *const parentWidget_ /* = nullptr */)
{
    QGridLayout *const gridLayout {new QGridLayout(parentWidget_)};
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);

    return gridLayout;
}




} // namespace


} // DEMO_NAMESPACE
