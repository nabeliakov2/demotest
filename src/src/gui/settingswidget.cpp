#include "include/gui/settingswidget.h"

#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtCore/QPointer>
#include <QtCore/QDebug>

#include <QtGui/QColor>
#include <QtGui/QResizeEvent>

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QAbstractItemView>
#include <QtWidgets/QScroller>

#include "include/core/utility.h"
#include "include/gui/colorcombobox.h"

namespace DEMO_NAMESPACE
{

namespace
{
QString settingsKeyValue(const SettingsWidget::SettingsKey key_);
ColorComboBox *createColorComboBox(ColorComboBox::Type type_, QWidget *const parent_ = nullptr);
}   // namespace


struct SettingsWidget::PrivateObject final  // private
{
    enum ComboBoxID {
        NormalColorComboBox = 0,
        NormalTextColorComboBox,
        InvertedColorComboBox,
        InvertedTextColorComboBox,
        ComboBoxCount
    };

    static const int RowCount {30};
    static const int Margin {2};
    static const QHash<SettingsWidget::SettingsKey, QVariant> DefaultSettingsValues;

    ModifiedAttributes modified {ModifiedAttribute::Unmodified};

    SettingsWidget *const pObj {nullptr};

    QSettings *pSettings {nullptr};
    QLineEdit *urlLineEdit {nullptr};
    QPushButton *connectionCheckButton {nullptr};
    QLabel *connectionCheckLabel {nullptr};
    QVector<QPointer<ColorComboBox>> comboBoxes;

    explicit PrivateObject(SettingsWidget *const parentWidget_);
    ~PrivateObject() = default;

    void updateColors();
    void loadSettings();
    void saveSettings();

};

const QHash<SettingsWidget::SettingsKey, QVariant> SettingsWidget::PrivateObject::DefaultSettingsValues {
    std::make_pair(SettingsWidget::SettingsKey::ServerURL, QVariant::fromValue(QString("qrc:///data/res/data_sample.json"))),
    //std::make_pair(SettingsWidget::SettingsKey::ServerURL, QVariant::fromValue(QString("http://httpbin.org/ip"))),
    //std::make_pair(SettingsWidget::SettingsKey::ServerURL, QVariant::fromValue(QString("http://95.171.8.60:20800"))),
    std::make_pair(SettingsWidget::SettingsKey::NormalColor, QVariant::fromValue(Qt::white)),
    std::make_pair(SettingsWidget::SettingsKey::NormalTextColor, QVariant::fromValue(Qt::black)),
    std::make_pair(SettingsWidget::SettingsKey::InvertedColor, QVariant::fromValue(Qt::black)),
    std::make_pair(SettingsWidget::SettingsKey::InvertedTextColor, QVariant::fromValue(Qt::white)),
};  // static private::public

SettingsWidget::SettingsWidget(QWidget *parent_ /* = nullptr */) // explicit public
    : QWidget{parent_},
      pPrivateObject {new PrivateObject {this}}
{
}

SettingsWidget::~SettingsWidget() = default;   // virtual public override final;

QVariant SettingsWidget::settingsValue(const SettingsKey key_) const    // public
{
    Q_ASSERT(pPrivateObject && pPrivateObject->pSettings);
    Q_ASSERT(key_ >= 0 && key_ <= SettingsKeyCount);

    return pPrivateObject->pSettings->value(settingsKeyValue(key_), pPrivateObject->DefaultSettingsValues.value(key_));
}

void SettingsWidget::setConnectionOk()  // public
{
    Q_ASSERT(pPrivateObject);

    QPalette p(pPrivateObject->connectionCheckLabel->palette());
    p.setColor(QPalette::Normal, QPalette::WindowText, Qt::green);
    p.setColor(QPalette::Inactive, QPalette::WindowText, Qt::green);
    p.setColor(QPalette::Disabled, QPalette::WindowText, Qt::green);
    pPrivateObject->connectionCheckLabel->setPalette(p);

    pPrivateObject->connectionCheckLabel->setText(tr("OK"));
}

void SettingsWidget::setConnectionError(const QString &errorStr)    // public
{
    Q_ASSERT(pPrivateObject);

    QPalette p(pPrivateObject->connectionCheckLabel->palette());
    p.setColor(QPalette::Normal, QPalette::WindowText, Qt::red);
    p.setColor(QPalette::Inactive, QPalette::WindowText, Qt::red);
    p.setColor(QPalette::Disabled, QPalette::WindowText, Qt::red);
    pPrivateObject->connectionCheckLabel->setPalette(p);

    pPrivateObject->connectionCheckLabel->setText(QString("%1: %2").arg(tr("Error"), errorStr));
}

void SettingsWidget::reset()  // public
{
    Q_ASSERT(pPrivateObject);

    pPrivateObject->modified = ModifiedAttribute::Unmodified;
    pPrivateObject->connectionCheckLabel->clear();
}

SettingsWidget::ModifiedAttributes SettingsWidget::modifiedAttributes() const   // public
{
    Q_ASSERT(pPrivateObject);

    return pPrivateObject->modified;
}

void SettingsWidget::resizeEvent(QResizeEvent *event_)  // virtual protected override final
{
    QWidget::resizeEvent(event_);

    Q_ASSERT(pPrivateObject);

    const int fontPixelSize {event_->size().height() / PrivateObject::RowCount - (PrivateObject::Margin << 1)};

    QFont currentFont(font());
    currentFont.setPixelSize(fontPixelSize);
    setFont(currentFont);

    currentFont = pPrivateObject->connectionCheckButton->font();
    currentFont.setPixelSize(fontPixelSize);
    pPrivateObject->connectionCheckButton->setFont(currentFont);

}

/////// SettingsWidget::PrivateObject ///////
SettingsWidget::PrivateObject::PrivateObject(SettingsWidget *const parentWidget_)  // explicit private::public
    : pObj(parentWidget_),
      pSettings {new QSettings(parentWidget_)},
      urlLineEdit {new QLineEdit(parentWidget_)},
      connectionCheckButton {new QPushButton(parentWidget_)},
      connectionCheckLabel {new QLabel(parentWidget_)}
{
    pSettings->setIniCodec(Utility::defaultTextCodec());

    parentWidget_->setAutoFillBackground(true);

    const QVariant nColorVar {pSettings->value(settingsKeyValue(SettingsKey::NormalColor),
                                         DefaultSettingsValues.value(SettingsKey::NormalColor))};
    const QVariant ntColorVar {pSettings->value(settingsKeyValue(SettingsKey::NormalTextColor),
                                         DefaultSettingsValues.value(SettingsKey::NormalTextColor))};


    // Doesn't work here, gives an invalid QColor may be bug, Qt5.15.2
    // Utility::setBackgroundColor(parentWidget_, nColorVar.value<QColor>());

    // workaround
    Utility::setBackgroundColor(parentWidget_, QColor(nColorVar.toString()));
    Utility::setTextColor(parentWidget_, QColor(ntColorVar.toString()));

    QVBoxLayout *const widgetLayout {new QVBoxLayout};

    QLabel *const urlLabel {new QLabel(tr("URL:"), parentWidget_)};

    connectionCheckButton->setText(tr("Check connection"));
    connectionCheckLabel->setAlignment(Qt::AlignCenter);
    connectionCheckLabel->setWordWrap(true);

    QHBoxLayout *const connectionCheckLayout {new QHBoxLayout};
    connectionCheckLayout->addWidget(connectionCheckButton, 1);
    connectionCheckLayout->addWidget(connectionCheckLabel, 2);

    QPushButton *const okButton {new QPushButton(parentWidget_)};
    okButton->setText(tr("Apply"));

    QPushButton *const cancelButton {new QPushButton(parentWidget_)};
    cancelButton->setText(tr("Cancel"));

    QHBoxLayout *const buttonBoxLayout {new QHBoxLayout};
    buttonBoxLayout->addWidget(okButton);
    buttonBoxLayout->addWidget(cancelButton);

    QLabel *const normalColorLabel {new QLabel(tr("Normal color:"), parentWidget_)};
    QLabel *const normalTextColorLabel {new QLabel(tr("Normal text color:"), parentWidget_)};
    QLabel *const invertedColorLabel {new QLabel(tr("Inverted color:"), parentWidget_)};
    QLabel *const invertedTextColorLabel {new QLabel(tr("Inverted text color:"), parentWidget_)};

    for (int i = ComboBoxID::NormalColorComboBox; i < ComboBoxID::ComboBoxCount; ++i)
    {
        const ColorComboBox::Type t {i & 1 ? ColorComboBox::Type::Text : ColorComboBox::Type::Backround};

        comboBoxes.push_back(QPointer<ColorComboBox>(createColorComboBox(t, parentWidget_)));
    }

    widgetLayout->addWidget(urlLabel);
    widgetLayout->addWidget(urlLineEdit);
    widgetLayout->addStretch();
    widgetLayout->addWidget(connectionCheckButton);
    widgetLayout->addWidget(connectionCheckLabel);
    widgetLayout->addStretch();
    widgetLayout->addWidget(normalColorLabel);
    widgetLayout->addWidget(comboBoxes.at(ComboBoxID::NormalColorComboBox));
    widgetLayout->addStretch();
    widgetLayout->addWidget(normalTextColorLabel);
    widgetLayout->addWidget(comboBoxes.at(ComboBoxID::NormalTextColorComboBox));
    widgetLayout->addStretch();
    widgetLayout->addWidget(invertedColorLabel);
    widgetLayout->addWidget(comboBoxes.at(ComboBoxID::InvertedColorComboBox));
    widgetLayout->addStretch();
    widgetLayout->addWidget(invertedTextColorLabel);
    widgetLayout->addWidget(comboBoxes.at(ComboBoxID::InvertedTextColorComboBox));
    widgetLayout->addStretch();
    widgetLayout->addLayout(buttonBoxLayout);
    parentWidget_->setLayout(widgetLayout);
    loadSettings();

    if (!QObject::connect(comboBoxes.at(ComboBoxID::NormalColorComboBox).data(), qOverload<int>(&QComboBox::currentIndexChanged), parentWidget_,
                 [this](int i)
                 {
                    ColorComboBox *const pCombobox {comboBoxes.at(ComboBoxID::NormalColorComboBox).data()};

                    Q_ASSERT(pCombobox && pObj);

                    if (i > -1 && i < pCombobox->count())
                    {
                        Utility::setBackgroundColor(pObj, pCombobox->colorAt(i));
                        modified |= ModifiedAttribute::Colors;
                    }
                 })
        )
    {
        qCritical() << "Cannot connect signal and slot";
    }

    if (!QObject::connect(comboBoxes.at(ComboBoxID::NormalTextColorComboBox).data(), qOverload<int>(&QComboBox::currentIndexChanged), parentWidget_,
                 [this](int i)
                 {
                    ColorComboBox *const pCombobox {comboBoxes.at(ComboBoxID::NormalTextColorComboBox).data()};

                    Q_ASSERT(pCombobox && pObj);

                    if (i > -1 && i < pCombobox->count())
                    {
                        Utility::setTextColor(pObj, pCombobox->colorAt(i));
                        modified |= ModifiedAttribute::Colors;
                    }
                 })
        )
    {
        qCritical() << "Cannot connect signal and slot";
    }

    if (!QObject::connect(comboBoxes.at(ComboBoxID::InvertedColorComboBox).data(), qOverload<int>(&QComboBox::currentIndexChanged), parentWidget_,
                 [this](int)
                 {
                    modified |= ModifiedAttribute::Colors;
                 })
        )
    {
        qCritical() << "Cannot connect signal and slot";
    }

    if (!QObject::connect(comboBoxes.at(ComboBoxID::InvertedTextColorComboBox).data(), qOverload<int>(&QComboBox::currentIndexChanged), parentWidget_,
                 [this](int)
                 {
                    modified |= ModifiedAttribute::Colors;
                 })
        )
    {
        qCritical() << "Cannot connect signal and slot";
    }

    if (!QObject::connect(okButton, &QPushButton::clicked, parentWidget_,
                 [this, parentWidget_]()
                 {
                    saveSettings();
                    emit parentWidget_->accepted();
                 })
        )
    {
        qCritical() << "Cannot connect signal and slot";
    }

    if (!QObject::connect(cancelButton, &QPushButton::clicked, parentWidget_,
                 [parentWidget_]()
                 {
                    emit parentWidget_->cancelled();
                 })
        )
    {
        qCritical() << "Cannot connect signal and slot";
    }


    if (!QObject::connect(connectionCheckButton, &QPushButton::clicked, parentWidget_,
                  [this, parentWidget_]()
                  {
                    emit parentWidget_->checkConnectionRequest(this->urlLineEdit->text());
                  })
        )
    {
        qCritical() << "Cannot connect signal and slot";
    }

    if (!QObject::connect(urlLineEdit, &QLineEdit::textEdited, parentWidget_,
                  [this](const QString&)
                  {
                    modified |= ModifiedAttribute::URL;
                  })
        )
    {
        qCritical() << "Cannot connect signal and slot";
    }
}

void SettingsWidget::PrivateObject::loadSettings()  // explicit private::public
{
    Q_ASSERT(pSettings);

    urlLineEdit->setText(pSettings->value(settingsKeyValue(SettingsKey::ServerURL), DefaultSettingsValues.value(SettingsKey::ServerURL)).toString());

    Q_ASSERT(comboBoxes.value(ComboBoxID::NormalColorComboBox));

    comboBoxes.value(ComboBoxID::NormalColorComboBox)->setCurrentColor(QColor(pSettings->value(settingsKeyValue(SettingsKey::NormalColor),
                                                                                               DefaultSettingsValues.value(SettingsKey::NormalColor)).toString()).name());
    comboBoxes.value(ComboBoxID::NormalTextColorComboBox)->setCurrentColor(QColor(pSettings->value(settingsKeyValue(SettingsKey::NormalTextColor),
                                                                                               DefaultSettingsValues.value(SettingsKey::NormalTextColor)).toString()).name());
    comboBoxes.value(ComboBoxID::InvertedColorComboBox)->setCurrentColor(QColor(pSettings->value(settingsKeyValue(SettingsKey::InvertedColor),
                                                                                               DefaultSettingsValues.value(SettingsKey::InvertedColor)).toString()).name());
    comboBoxes.value(ComboBoxID::InvertedTextColorComboBox)->setCurrentColor(QColor(pSettings->value(settingsKeyValue(SettingsKey::InvertedTextColor),
                                                                                               DefaultSettingsValues.value(SettingsKey::InvertedTextColor)).toString()).name());

}

void SettingsWidget::PrivateObject::saveSettings()  // explicit private::public
{
    Q_ASSERT(pSettings);

    pSettings->setValue(settingsKeyValue(SettingsKey::ServerURL), QVariant::fromValue(urlLineEdit->text()));
    pSettings->setValue(settingsKeyValue(SettingsKey::NormalColor), QVariant::fromValue(comboBoxes.value(ComboBoxID::NormalColorComboBox)->currentColor()));
    pSettings->setValue(settingsKeyValue(SettingsKey::NormalTextColor), QVariant::fromValue(comboBoxes.value(ComboBoxID::NormalTextColorComboBox)->currentColor()));
    pSettings->setValue(settingsKeyValue(SettingsKey::InvertedColor), QVariant::fromValue(comboBoxes.value(ComboBoxID::InvertedColorComboBox)->currentColor()));
    pSettings->setValue(settingsKeyValue(SettingsKey::InvertedTextColor), QVariant::fromValue(comboBoxes.value(ComboBoxID::InvertedTextColorComboBox)->currentColor()));

    urlLineEdit->setText(pSettings->value(settingsKeyValue(SettingsKey::ServerURL), DefaultSettingsValues.value(SettingsKey::ServerURL)).toString());
}

namespace
{
QString settingsKeyValue(const SettingsWidget::SettingsKey key_)
{
    static const QHash<SettingsWidget::SettingsKey, QString> keyToValue {
        std::make_pair(SettingsWidget::SettingsKey::ServerURL, "Core/serverURL"),
        std::make_pair(SettingsWidget::SettingsKey::NormalColor, "GUI/normalColor"),
        std::make_pair(SettingsWidget::SettingsKey::NormalTextColor, "GUI/normalTextColor"),
        std::make_pair(SettingsWidget::SettingsKey::InvertedColor, "GUI/invertedColor"),
        std::make_pair(SettingsWidget::SettingsKey::InvertedTextColor, "GUI/invertedTextColor"),
    };

    return keyToValue.value(key_);
}

ColorComboBox *createColorComboBox(const ColorComboBox::Type type_, QWidget *const parent_ /* = nullptr */)
{
    ColorComboBox *const pCombobox {new ColorComboBox(type_, parent_)};
    QScroller::grabGesture(pCombobox->view(), QScroller::LeftMouseButtonGesture);
    return pCombobox;
}

} //namespace

} // DEMO_NAMESPACE


