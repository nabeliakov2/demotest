#include "include/gui/colorcombobox.h"

#include <QtCore/QSet>
#include <QtCore/QHash>
#include <QtCore/QDebug>

#include <QtWidgets/QAbstractItemView>

#include <iostream>

#include "include/core/utility.h"

namespace DEMO_NAMESPACE
{

struct ColorComboBox::PrivateObject final  // private
{
    QComboBox *const pObj {nullptr};
    QHash<QString, int> indexForColor;

    explicit PrivateObject(QComboBox *const pObj_)
        : pObj {pObj_} {}
    ~PrivateObject() = default;

    void updateColors();
};


ColorComboBox::ColorComboBox(const Type type_, QWidget *parent_ /* = nullptr */)  // explicit public
    : QComboBox {parent_},
    pPrivateObject {new PrivateObject(this)}
{
    static const QSet<QString> excludedColorSet {
        "transparent", "darkgrey", "lightslategrey", "lightgrey", "darkslategrey",
        "slategrey"
    };

    static const QHash<QString, QString> colorTaranslation {
        std::make_pair("aliceblue", tr("aliceblue")),
        std::make_pair("antiquewhite", tr("antiquewhite")),
        std::make_pair("aqua", tr("aqua")),
        std::make_pair("aquamarine", tr("aquamarine")),
        std::make_pair("azure", tr("azure")),
        std::make_pair("beige", tr("beige")),
        std::make_pair("bisque", tr("bisque")),
        std::make_pair("black", tr("black")),
        std::make_pair("blanchedalmond", tr("blanchedalmond")),
        std::make_pair("blue", tr("blue")),
        std::make_pair("blueviolet", tr("blueviolet")),
        std::make_pair("brown", tr("brown")),
        std::make_pair("burlywood", tr("burlywood")),
        std::make_pair("cadetblue", tr("cadetblue")),
        std::make_pair("chartreuse", tr("chartreuse")),
        std::make_pair("chocolate", tr("chocolate")),
        std::make_pair("coral", tr("coral")),
        std::make_pair("cornflowerblue", tr("cornflowerblue")),
        std::make_pair("cornsilk", tr("cornsilk")),
        std::make_pair("crimson", tr("crimson")),
        std::make_pair("cyan", tr("cyan")),
        std::make_pair("darkblue", tr("darkblue")),
        std::make_pair("darkcyan", tr("darkcyan")),
        std::make_pair("darkgoldenrod", tr("darkgoldenrod")),
        std::make_pair("darkgray", tr("darkgray")),
        std::make_pair("darkgreen", tr("darkgreen")),
        std::make_pair("darkgrey", tr("darkgrey")),
        std::make_pair("darkkhaki", tr("darkkhaki")),
        std::make_pair("darkmagenta", tr("darkmagenta")),
        std::make_pair("darkolivegreen", tr("darkolivegreen")),
        std::make_pair("darkorange", tr("darkorange")),
        std::make_pair("darkorchid", tr("darkorchid")),
        std::make_pair("darkred", tr("darkred")),
        std::make_pair("darksalmon", tr("darksalmon")),
        std::make_pair("darkseagreen", tr("darkseagreen")),
        std::make_pair("darkslateblue", tr("darkslateblue")),
        std::make_pair("darkslategray", tr("darkslategray")),
        std::make_pair("darkslategrey", tr("darkslategrey")),
        std::make_pair("darkturquoise", tr("darkturquoise")),
        std::make_pair("darkviolet", tr("darkviolet")),
        std::make_pair("deeppink", tr("deeppink")),
        std::make_pair("deepskyblue", tr("deepskyblue")),
        std::make_pair("dimgray", tr("dimgray")),
        std::make_pair("dimgrey", tr("dimgrey")),
        std::make_pair("dodgerblue", tr("dodgerblue")),
        std::make_pair("firebrick", tr("firebrick")),
        std::make_pair("floralwhite", tr("floralwhite")),
        std::make_pair("forestgreen", tr("forestgreen")),
        std::make_pair("fuchsia", tr("fuchsia")),
        std::make_pair("gainsboro", tr("gainsboro")),
        std::make_pair("ghostwhite", tr("ghostwhite")),
        std::make_pair("gold", tr("gold")),
        std::make_pair("goldenrod", tr("goldenrod")),
        std::make_pair("gray", tr("gray")),
        std::make_pair("green", tr("green")),
        std::make_pair("greenyellow", tr("greenyellow")),
        std::make_pair("grey", tr("grey")),
        std::make_pair("honeydew", tr("honeydew")),
        std::make_pair("hotpink", tr("hotpink")),
        std::make_pair("indianred", tr("indianred")),
        std::make_pair("indigo", tr("indigo")),
        std::make_pair("ivory", tr("ivory")),
        std::make_pair("khaki", tr("khaki")),
        std::make_pair("lavender", tr("lavender")),
        std::make_pair("lavenderblush", tr("lavenderblush")),
        std::make_pair("lawngreen", tr("lawngreen")),
        std::make_pair("lemonchiffon", tr("lemonchiffon")),
        std::make_pair("lightblue", tr("lightblue")),
        std::make_pair("lightcoral", tr("lightcoral")),
        std::make_pair("lightcyan", tr("lightcyan")),
        std::make_pair("lightgoldenrodyellow", tr("lightgoldenrodyellow")),
        std::make_pair("lightgray", tr("lightgray")),
        std::make_pair("lightgreen", tr("lightgreen")),
        std::make_pair("lightgrey", tr("lightgrey")),
        std::make_pair("lightpink", tr("lightpink")),
        std::make_pair("lightsalmon", tr("lightsalmon")),
        std::make_pair("lightseagreen", tr("lightseagreen")),
        std::make_pair("lightskyblue", tr("lightskyblue")),
        std::make_pair("lightslategray", tr("lightslategray")),
        std::make_pair("lightslategrey", tr("lightslategrey")),
        std::make_pair("lightsteelblue", tr("lightsteelblue")),
        std::make_pair("lightyellow", tr("lightyellow")),
        std::make_pair("lime", tr("lime")),
        std::make_pair("limegreen", tr("limegreen")),
        std::make_pair("linen", tr("linen")),
        std::make_pair("magenta", tr("magenta")),
        std::make_pair("maroon", tr("maroon")),
        std::make_pair("mediumaquamarine", tr("mediumaquamarine")),
        std::make_pair("mediumblue", tr("mediumblue")),
        std::make_pair("mediumorchid", tr("mediumorchid")),
        std::make_pair("mediumpurple", tr("mediumpurple")),
        std::make_pair("mediumseagreen", tr("mediumseagreen")),
        std::make_pair("mediumslateblue", tr("mediumslateblue")),
        std::make_pair("mediumspringgreen", tr("mediumspringgreen")),
        std::make_pair("mediumturquoise", tr("mediumturquoise")),
        std::make_pair("mediumvioletred", tr("mediumvioletred")),
        std::make_pair("midnightblue", tr("midnightblue")),
        std::make_pair("mintcream", tr("mintcream")),
        std::make_pair("mistyrose", tr("mistyrose")),
        std::make_pair("moccasin", tr("moccasin")),
        std::make_pair("navajowhite", tr("navajowhite")),
        std::make_pair("navy", tr("navy")),
        std::make_pair("oldlace", tr("oldlace")),
        std::make_pair("olive", tr("olive")),
        std::make_pair("olivedrab", tr("olivedrab")),
        std::make_pair("orange", tr("orange")),
        std::make_pair("orangered", tr("orangered")),
        std::make_pair("orchid", tr("orchid")),
        std::make_pair("palegoldenrod", tr("palegoldenrod")),
        std::make_pair("palegreen", tr("palegreen")),
        std::make_pair("paleturquoise", tr("paleturquoise")),
        std::make_pair("palevioletred", tr("palevioletred")),
        std::make_pair("papayawhip", tr("papayawhip")),
        std::make_pair("peachpuff", tr("peachpuff")),
        std::make_pair("peru", tr("peru")),
        std::make_pair("pink", tr("pink")),
        std::make_pair("plum", tr("plum")),
        std::make_pair("powderblue", tr("powderblue")),
        std::make_pair("purple", tr("purple")),
        std::make_pair("red", tr("red")),
        std::make_pair("rosybrown", tr("rosybrown")),
        std::make_pair("royalblue", tr("royalblue")),
        std::make_pair("saddlebrown", tr("saddlebrown")),
        std::make_pair("salmon", tr("salmon")),
        std::make_pair("sandybrown", tr("sandybrown")),
        std::make_pair("seagreen", tr("seagreen")),
        std::make_pair("seashell", tr("seashell")),
        std::make_pair("sienna", tr("sienna")),
        std::make_pair("silver", tr("silver")),
        std::make_pair("skyblue", tr("skyblue")),
        std::make_pair("slateblue", tr("slateblue")),
        std::make_pair("slategray", tr("slategray")),
        std::make_pair("slategrey", tr("slategrey")),
        std::make_pair("snow", tr("snow")),
        std::make_pair("springgreen", tr("springgreen")),
        std::make_pair("steelblue", tr("steelblue")),
        std::make_pair("tan", tr("tan")),
        std::make_pair("teal", tr("teal")),
        std::make_pair("thistle", tr("thistle")),
        std::make_pair("tomato", tr("tomato")),
        std::make_pair("transparent", tr("transparent")),
        std::make_pair("turquoise", tr("turquoise")),
        std::make_pair("violet", tr("violet")),
        std::make_pair("wheat", tr("wheat")),
        std::make_pair("white", tr("white")),
        std::make_pair("whitesmoke", tr("whitesmoke")),
        std::make_pair("yellow", tr("yellow")),
        std::make_pair("yellowgreen", tr("yellowgreen")),
        std::make_pair("aliceblue", tr("aliceblue")),
        std::make_pair("antiquewhite", tr("antiquewhite")),
        std::make_pair("aqua", tr("aqua")),
        std::make_pair("aquamarine", tr("aquamarine")),
        std::make_pair("azure", tr("azure")),
        std::make_pair("beige", tr("beige")),
        std::make_pair("bisque", tr("bisque")),
        std::make_pair("black", tr("black")),
        std::make_pair("blanchedalmond", tr("blanchedalmond")),
        std::make_pair("blue", tr("blue")),
        std::make_pair("blueviolet", tr("blueviolet")),
        std::make_pair("brown", tr("brown")),
        std::make_pair("burlywood", tr("burlywood")),
        std::make_pair("cadetblue", tr("cadetblue")),
        std::make_pair("chartreuse", tr("chartreuse")),
        std::make_pair("chocolate", tr("chocolate")),
        std::make_pair("coral", tr("coral")),
        std::make_pair("cornflowerblue", tr("cornflowerblue")),
        std::make_pair("cornsilk", tr("cornsilk")),
        std::make_pair("crimson", tr("crimson")),
        std::make_pair("cyan", tr("cyan")),
        std::make_pair("darkblue", tr("darkblue")),
        std::make_pair("darkcyan", tr("darkcyan")),
        std::make_pair("darkgoldenrod", tr("darkgoldenrod")),
        std::make_pair("darkgray", tr("darkgray")),
        std::make_pair("darkgreen", tr("darkgreen")),
        std::make_pair("darkgrey", tr("darkgrey")),
        std::make_pair("darkkhaki", tr("darkkhaki")),
        std::make_pair("darkmagenta", tr("darkmagenta")),
        std::make_pair("darkolivegreen", tr("darkolivegreen")),
        std::make_pair("darkorange", tr("darkorange")),
        std::make_pair("darkorchid", tr("darkorchid")),
        std::make_pair("darkred", tr("darkred")),
        std::make_pair("darksalmon", tr("darksalmon")),
        std::make_pair("darkseagreen", tr("darkseagreen")),
        std::make_pair("darkslateblue", tr("darkslateblue")),
        std::make_pair("darkslategray", tr("darkslategray")),
        std::make_pair("darkslategrey", tr("darkslategrey")),
        std::make_pair("darkturquoise", tr("darkturquoise")),
        std::make_pair("darkviolet", tr("darkviolet")),
        std::make_pair("deeppink", tr("deeppink")),
        std::make_pair("deepskyblue", tr("deepskyblue")),
        std::make_pair("dimgray", tr("dimgray")),
        std::make_pair("dimgrey", tr("dimgrey")),
        std::make_pair("dodgerblue", tr("dodgerblue")),
        std::make_pair("firebrick", tr("firebrick")),
        std::make_pair("floralwhite", tr("floralwhite")),
        std::make_pair("forestgreen", tr("forestgreen")),
        std::make_pair("fuchsia", tr("fuchsia")),
        std::make_pair("gainsboro", tr("gainsboro")),
        std::make_pair("ghostwhite", tr("ghostwhite")),
        std::make_pair("gold", tr("gold")),
        std::make_pair("goldenrod", tr("goldenrod")),
        std::make_pair("gray", tr("gray")),
        std::make_pair("green", tr("green")),
        std::make_pair("greenyellow", tr("greenyellow")),
        std::make_pair("grey", tr("grey")),
        std::make_pair("honeydew", tr("honeydew")),
        std::make_pair("hotpink", tr("hotpink")),
        std::make_pair("indianred", tr("indianred")),
        std::make_pair("indigo", tr("indigo")),
        std::make_pair("ivory", tr("ivory")),
        std::make_pair("khaki", tr("khaki")),
        std::make_pair("lavender", tr("lavender")),
        std::make_pair("lavenderblush", tr("lavenderblush")),
        std::make_pair("lawngreen", tr("lawngreen")),
        std::make_pair("lemonchiffon", tr("lemonchiffon")),
        std::make_pair("lightblue", tr("lightblue")),
        std::make_pair("lightcoral", tr("lightcoral")),
        std::make_pair("lightcyan", tr("lightcyan")),
        std::make_pair("lightgoldenrodyellow", tr("lightgoldenrodyellow")),
        std::make_pair("lightgray", tr("lightgray")),
        std::make_pair("lightgreen", tr("lightgreen")),
        std::make_pair("lightgrey", tr("lightgrey")),
        std::make_pair("lightpink", tr("lightpink")),
        std::make_pair("lightsalmon", tr("lightsalmon")),
        std::make_pair("lightseagreen", tr("lightseagreen")),
        std::make_pair("lightskyblue", tr("lightskyblue")),
        std::make_pair("lightslategray", tr("lightslategray")),
        std::make_pair("lightslategrey", tr("lightslategrey")),
        std::make_pair("lightsteelblue", tr("lightsteelblue")),
        std::make_pair("lightyellow", tr("lightyellow")),
        std::make_pair("lime", tr("lime")),
        std::make_pair("limegreen", tr("limegreen")),
        std::make_pair("linen", tr("linen")),
        std::make_pair("magenta", tr("magenta")),
        std::make_pair("maroon", tr("maroon")),
        std::make_pair("mediumaquamarine", tr("mediumaquamarine")),
        std::make_pair("mediumblue", tr("mediumblue")),
        std::make_pair("mediumorchid", tr("mediumorchid")),
        std::make_pair("mediumpurple", tr("mediumpurple")),
        std::make_pair("mediumseagreen", tr("mediumseagreen")),
        std::make_pair("mediumslateblue", tr("mediumslateblue")),
        std::make_pair("mediumspringgreen", tr("mediumspringgreen")),
        std::make_pair("mediumturquoise", tr("mediumturquoise")),
        std::make_pair("mediumvioletred", tr("mediumvioletred")),
        std::make_pair("midnightblue", tr("midnightblue")),
        std::make_pair("mintcream", tr("mintcream")),
        std::make_pair("mistyrose", tr("mistyrose")),
        std::make_pair("moccasin", tr("moccasin")),
        std::make_pair("navajowhite", tr("navajowhite")),
        std::make_pair("navy", tr("navy")),
        std::make_pair("oldlace", tr("oldlace")),
        std::make_pair("olive", tr("olive")),
        std::make_pair("olivedrab", tr("olivedrab")),
        std::make_pair("orange", tr("orange")),
        std::make_pair("orangered", tr("orangered")),
        std::make_pair("orchid", tr("orchid")),
        std::make_pair("palegoldenrod", tr("palegoldenrod")),
        std::make_pair("palegreen", tr("palegreen")),
        std::make_pair("paleturquoise", tr("paleturquoise")),
        std::make_pair("palevioletred", tr("palevioletred")),
        std::make_pair("papayawhip", tr("papayawhip")),
        std::make_pair("peachpuff", tr("peachpuff")),
        std::make_pair("peru", tr("peru")),
        std::make_pair("pink", tr("pink")),
        std::make_pair("plum", tr("plum")),
        std::make_pair("powderblue", tr("powderblue")),
        std::make_pair("purple", tr("purple")),
        std::make_pair("red", tr("red")),
        std::make_pair("rosybrown", tr("rosybrown")),
        std::make_pair("royalblue", tr("royalblue")),
        std::make_pair("saddlebrown", tr("saddlebrown")),
        std::make_pair("salmon", tr("salmon")),
        std::make_pair("sandybrown", tr("sandybrown")),
        std::make_pair("seagreen", tr("seagreen")),
        std::make_pair("seashell", tr("seashell")),
        std::make_pair("sienna", tr("sienna")),
        std::make_pair("silver", tr("silver")),
        std::make_pair("skyblue", tr("skyblue")),
        std::make_pair("slateblue", tr("slateblue")),
        std::make_pair("slategray", tr("slategray")),
        std::make_pair("slategrey", tr("slategrey")),
        std::make_pair("snow", tr("snow")),
        std::make_pair("springgreen", tr("springgreen")),
        std::make_pair("steelblue", tr("steelblue")),
        std::make_pair("tan", tr("tan")),
        std::make_pair("teal", tr("teal")),
        std::make_pair("thistle", tr("thistle")),
        std::make_pair("tomato", tr("tomato")),
        std::make_pair("transparent", tr("transparent")),
        std::make_pair("turquoise", tr("turquoise")),
        std::make_pair("violet", tr("violet")),
        std::make_pair("wheat", tr("wheat")),
        std::make_pair("white", tr("white")),
        std::make_pair("whitesmoke", tr("whitesmoke")),
        std::make_pair("yellow", tr("yellow")),
        std::make_pair("yellowgreen", tr("yellowgreen")),
        std::make_pair("aliceblue", tr("aliceblue")),
        std::make_pair("antiquewhite", tr("antiquewhite")),
        std::make_pair("aqua", tr("aqua")),
        std::make_pair("aquamarine", tr("aquamarine")),
        std::make_pair("azure", tr("azure")),
        std::make_pair("beige", tr("beige")),
        std::make_pair("bisque", tr("bisque")),
        std::make_pair("black", tr("black")),
        std::make_pair("blanchedalmond", tr("blanchedalmond")),
        std::make_pair("blue", tr("blue")),
        std::make_pair("blueviolet", tr("blueviolet")),
        std::make_pair("brown", tr("brown")),
        std::make_pair("burlywood", tr("burlywood")),
        std::make_pair("cadetblue", tr("cadetblue")),
        std::make_pair("chartreuse", tr("chartreuse")),
        std::make_pair("chocolate", tr("chocolate")),
        std::make_pair("coral", tr("coral")),
        std::make_pair("cornflowerblue", tr("cornflowerblue")),
        std::make_pair("cornsilk", tr("cornsilk")),
        std::make_pair("crimson", tr("crimson")),
        std::make_pair("cyan", tr("cyan")),
        std::make_pair("darkblue", tr("darkblue")),
        std::make_pair("darkcyan", tr("darkcyan")),
        std::make_pair("darkgoldenrod", tr("darkgoldenrod")),
        std::make_pair("darkgray", tr("darkgray")),
        std::make_pair("darkgreen", tr("darkgreen")),
        std::make_pair("darkgrey", tr("darkgrey")),
        std::make_pair("darkkhaki", tr("darkkhaki")),
        std::make_pair("darkmagenta", tr("darkmagenta")),
        std::make_pair("darkolivegreen", tr("darkolivegreen")),
        std::make_pair("darkorange", tr("darkorange")),
        std::make_pair("darkorchid", tr("darkorchid")),
        std::make_pair("darkred", tr("darkred")),
        std::make_pair("darksalmon", tr("darksalmon")),
        std::make_pair("darkseagreen", tr("darkseagreen")),
        std::make_pair("darkslateblue", tr("darkslateblue")),
        std::make_pair("darkslategray", tr("darkslategray")),
        std::make_pair("darkslategrey", tr("darkslategrey")),
        std::make_pair("darkturquoise", tr("darkturquoise")),
        std::make_pair("darkviolet", tr("darkviolet")),
        std::make_pair("deeppink", tr("deeppink")),
        std::make_pair("deepskyblue", tr("deepskyblue")),
        std::make_pair("dimgray", tr("dimgray")),
        std::make_pair("dimgrey", tr("dimgrey")),
        std::make_pair("dodgerblue", tr("dodgerblue")),
        std::make_pair("firebrick", tr("firebrick")),
        std::make_pair("floralwhite", tr("floralwhite")),
        std::make_pair("forestgreen", tr("forestgreen")),
        std::make_pair("fuchsia", tr("fuchsia")),
        std::make_pair("gainsboro", tr("gainsboro")),
        std::make_pair("ghostwhite", tr("ghostwhite")),
        std::make_pair("gold", tr("gold")),
        std::make_pair("goldenrod", tr("goldenrod")),
        std::make_pair("gray", tr("gray")),
        std::make_pair("green", tr("green")),
        std::make_pair("greenyellow", tr("greenyellow")),
        std::make_pair("grey", tr("grey")),
        std::make_pair("honeydew", tr("honeydew")),
        std::make_pair("hotpink", tr("hotpink")),
        std::make_pair("indianred", tr("indianred")),
        std::make_pair("indigo", tr("indigo")),
        std::make_pair("ivory", tr("ivory")),
        std::make_pair("khaki", tr("khaki")),
        std::make_pair("lavender", tr("lavender")),
        std::make_pair("lavenderblush", tr("lavenderblush")),
        std::make_pair("lawngreen", tr("lawngreen")),
        std::make_pair("lemonchiffon", tr("lemonchiffon")),
        std::make_pair("lightblue", tr("lightblue")),
        std::make_pair("lightcoral", tr("lightcoral")),
        std::make_pair("lightcyan", tr("lightcyan")),
        std::make_pair("lightgoldenrodyellow", tr("lightgoldenrodyellow")),
        std::make_pair("lightgray", tr("lightgray")),
        std::make_pair("lightgreen", tr("lightgreen")),
        std::make_pair("lightgrey", tr("lightgrey")),
        std::make_pair("lightpink", tr("lightpink")),
        std::make_pair("lightsalmon", tr("lightsalmon")),
        std::make_pair("lightseagreen", tr("lightseagreen")),
        std::make_pair("lightskyblue", tr("lightskyblue")),
        std::make_pair("lightslategray", tr("lightslategray")),
        std::make_pair("lightslategrey", tr("lightslategrey")),
        std::make_pair("lightsteelblue", tr("lightsteelblue")),
        std::make_pair("lightyellow", tr("lightyellow")),
        std::make_pair("lime", tr("lime")),
        std::make_pair("limegreen", tr("limegreen")),
        std::make_pair("linen", tr("linen")),
        std::make_pair("magenta", tr("magenta")),
        std::make_pair("maroon", tr("maroon")),
        std::make_pair("mediumaquamarine", tr("mediumaquamarine")),
        std::make_pair("mediumblue", tr("mediumblue")),
        std::make_pair("mediumorchid", tr("mediumorchid")),
        std::make_pair("mediumpurple", tr("mediumpurple")),
        std::make_pair("mediumseagreen", tr("mediumseagreen")),
        std::make_pair("mediumslateblue", tr("mediumslateblue")),
        std::make_pair("mediumspringgreen", tr("mediumspringgreen")),
        std::make_pair("mediumturquoise", tr("mediumturquoise")),
        std::make_pair("mediumvioletred", tr("mediumvioletred")),
        std::make_pair("midnightblue", tr("midnightblue")),
        std::make_pair("mintcream", tr("mintcream")),
        std::make_pair("mistyrose", tr("mistyrose")),
        std::make_pair("moccasin", tr("moccasin")),
        std::make_pair("navajowhite", tr("navajowhite")),
        std::make_pair("navy", tr("navy")),
        std::make_pair("oldlace", tr("oldlace")),
        std::make_pair("olive", tr("olive")),
        std::make_pair("olivedrab", tr("olivedrab")),
        std::make_pair("orange", tr("orange")),
        std::make_pair("orangered", tr("orangered")),
        std::make_pair("orchid", tr("orchid")),
        std::make_pair("palegoldenrod", tr("palegoldenrod")),
        std::make_pair("palegreen", tr("palegreen")),
        std::make_pair("paleturquoise", tr("paleturquoise")),
        std::make_pair("palevioletred", tr("palevioletred")),
        std::make_pair("papayawhip", tr("papayawhip")),
        std::make_pair("peachpuff", tr("peachpuff")),
        std::make_pair("peru", tr("peru")),
        std::make_pair("pink", tr("pink")),
        std::make_pair("plum", tr("plum")),
        std::make_pair("powderblue", tr("powderblue")),
        std::make_pair("purple", tr("purple")),
        std::make_pair("red", tr("red")),
        std::make_pair("rosybrown", tr("rosybrown")),
        std::make_pair("royalblue", tr("royalblue")),
        std::make_pair("saddlebrown", tr("saddlebrown")),
        std::make_pair("salmon", tr("salmon")),
        std::make_pair("sandybrown", tr("sandybrown")),
        std::make_pair("seagreen", tr("seagreen")),
        std::make_pair("seashell", tr("seashell")),
        std::make_pair("sienna", tr("sienna")),
        std::make_pair("silver", tr("silver")),
        std::make_pair("skyblue", tr("skyblue")),
        std::make_pair("slateblue", tr("slateblue")),
        std::make_pair("slategray", tr("slategray")),
        std::make_pair("slategrey", tr("slategrey")),
        std::make_pair("snow", tr("snow")),
        std::make_pair("springgreen", tr("springgreen")),
        std::make_pair("steelblue", tr("steelblue")),
        std::make_pair("tan", tr("tan")),
        std::make_pair("teal", tr("teal")),
        std::make_pair("thistle", tr("thistle")),
        std::make_pair("tomato", tr("tomato")),
        std::make_pair("transparent", tr("transparent")),
        std::make_pair("turquoise", tr("turquoise")),
        std::make_pair("violet", tr("violet")),
        std::make_pair("wheat", tr("wheat")),
        std::make_pair("white", tr("white")),
        std::make_pair("whitesmoke", tr("whitesmoke")),
        std::make_pair("yellow", tr("yellow")),
        std::make_pair("yellowgreen", tr("yellowgreen")),
        std::make_pair("aliceblue", tr("aliceblue")),
        std::make_pair("antiquewhite", tr("antiquewhite")),
        std::make_pair("aqua", tr("aqua")),
        std::make_pair("aquamarine", tr("aquamarine")),
        std::make_pair("azure", tr("azure")),
        std::make_pair("beige", tr("beige")),
        std::make_pair("bisque", tr("bisque")),
        std::make_pair("black", tr("black")),
        std::make_pair("blanchedalmond", tr("blanchedalmond")),
        std::make_pair("blue", tr("blue")),
        std::make_pair("blueviolet", tr("blueviolet")),
        std::make_pair("brown", tr("brown")),
        std::make_pair("burlywood", tr("burlywood")),
        std::make_pair("cadetblue", tr("cadetblue")),
        std::make_pair("chartreuse", tr("chartreuse")),
        std::make_pair("chocolate", tr("chocolate")),
        std::make_pair("coral", tr("coral")),
        std::make_pair("cornflowerblue", tr("cornflowerblue")),
        std::make_pair("cornsilk", tr("cornsilk")),
        std::make_pair("crimson", tr("crimson")),
        std::make_pair("cyan", tr("cyan")),
        std::make_pair("darkblue", tr("darkblue")),
        std::make_pair("darkcyan", tr("darkcyan")),
        std::make_pair("darkgoldenrod", tr("darkgoldenrod")),
        std::make_pair("darkgray", tr("darkgray")),
        std::make_pair("darkgreen", tr("darkgreen")),
        std::make_pair("darkgrey", tr("darkgrey")),
        std::make_pair("darkkhaki", tr("darkkhaki")),
        std::make_pair("darkmagenta", tr("darkmagenta")),
        std::make_pair("darkolivegreen", tr("darkolivegreen")),
        std::make_pair("darkorange", tr("darkorange")),
        std::make_pair("darkorchid", tr("darkorchid")),
        std::make_pair("darkred", tr("darkred")),
        std::make_pair("darksalmon", tr("darksalmon")),
        std::make_pair("darkseagreen", tr("darkseagreen")),
        std::make_pair("darkslateblue", tr("darkslateblue")),
        std::make_pair("darkslategray", tr("darkslategray")),
        std::make_pair("darkslategrey", tr("darkslategrey")),
        std::make_pair("darkturquoise", tr("darkturquoise")),
        std::make_pair("darkviolet", tr("darkviolet")),
        std::make_pair("deeppink", tr("deeppink")),
        std::make_pair("deepskyblue", tr("deepskyblue")),
        std::make_pair("dimgray", tr("dimgray")),
        std::make_pair("dimgrey", tr("dimgrey")),
        std::make_pair("dodgerblue", tr("dodgerblue")),
        std::make_pair("firebrick", tr("firebrick")),
        std::make_pair("floralwhite", tr("floralwhite")),
        std::make_pair("forestgreen", tr("forestgreen")),
        std::make_pair("fuchsia", tr("fuchsia")),
        std::make_pair("gainsboro", tr("gainsboro")),
        std::make_pair("ghostwhite", tr("ghostwhite")),
        std::make_pair("gold", tr("gold")),
        std::make_pair("goldenrod", tr("goldenrod")),
        std::make_pair("gray", tr("gray")),
        std::make_pair("green", tr("green")),
        std::make_pair("greenyellow", tr("greenyellow")),
        std::make_pair("grey", tr("grey")),
        std::make_pair("honeydew", tr("honeydew")),
        std::make_pair("hotpink", tr("hotpink")),
        std::make_pair("indianred", tr("indianred")),
        std::make_pair("indigo", tr("indigo")),
        std::make_pair("ivory", tr("ivory")),
        std::make_pair("khaki", tr("khaki")),
        std::make_pair("lavender", tr("lavender")),
        std::make_pair("lavenderblush", tr("lavenderblush")),
        std::make_pair("lawngreen", tr("lawngreen")),
        std::make_pair("lemonchiffon", tr("lemonchiffon")),
        std::make_pair("lightblue", tr("lightblue")),
        std::make_pair("lightcoral", tr("lightcoral")),
        std::make_pair("lightcyan", tr("lightcyan")),
        std::make_pair("lightgoldenrodyellow", tr("lightgoldenrodyellow")),
        std::make_pair("lightgray", tr("lightgray")),
        std::make_pair("lightgreen", tr("lightgreen")),
        std::make_pair("lightgrey", tr("lightgrey")),
        std::make_pair("lightpink", tr("lightpink")),
        std::make_pair("lightsalmon", tr("lightsalmon")),
        std::make_pair("lightseagreen", tr("lightseagreen")),
        std::make_pair("lightskyblue", tr("lightskyblue")),
        std::make_pair("lightslategray", tr("lightslategray")),
        std::make_pair("lightslategrey", tr("lightslategrey")),
        std::make_pair("lightsteelblue", tr("lightsteelblue")),
        std::make_pair("lightyellow", tr("lightyellow")),
        std::make_pair("lime", tr("lime")),
        std::make_pair("limegreen", tr("limegreen")),
        std::make_pair("linen", tr("linen")),
        std::make_pair("magenta", tr("magenta")),
        std::make_pair("maroon", tr("maroon")),
        std::make_pair("mediumaquamarine", tr("mediumaquamarine")),
        std::make_pair("mediumblue", tr("mediumblue")),
        std::make_pair("mediumorchid", tr("mediumorchid")),
        std::make_pair("mediumpurple", tr("mediumpurple")),
        std::make_pair("mediumseagreen", tr("mediumseagreen")),
        std::make_pair("mediumslateblue", tr("mediumslateblue")),
        std::make_pair("mediumspringgreen", tr("mediumspringgreen")),
        std::make_pair("mediumturquoise", tr("mediumturquoise")),
        std::make_pair("mediumvioletred", tr("mediumvioletred")),
        std::make_pair("midnightblue", tr("midnightblue")),
        std::make_pair("mintcream", tr("mintcream")),
        std::make_pair("mistyrose", tr("mistyrose")),
        std::make_pair("moccasin", tr("moccasin")),
        std::make_pair("navajowhite", tr("navajowhite")),
        std::make_pair("navy", tr("navy")),
        std::make_pair("oldlace", tr("oldlace")),
        std::make_pair("olive", tr("olive")),
        std::make_pair("olivedrab", tr("olivedrab")),
        std::make_pair("orange", tr("orange")),
        std::make_pair("orangered", tr("orangered")),
        std::make_pair("orchid", tr("orchid")),
        std::make_pair("palegoldenrod", tr("palegoldenrod")),
        std::make_pair("palegreen", tr("palegreen")),
        std::make_pair("paleturquoise", tr("paleturquoise")),
        std::make_pair("palevioletred", tr("palevioletred")),
        std::make_pair("papayawhip", tr("papayawhip")),
        std::make_pair("peachpuff", tr("peachpuff")),
        std::make_pair("peru", tr("peru")),
        std::make_pair("pink", tr("pink")),
        std::make_pair("plum", tr("plum")),
        std::make_pair("powderblue", tr("powderblue")),
        std::make_pair("purple", tr("purple")),
        std::make_pair("red", tr("red")),
        std::make_pair("rosybrown", tr("rosybrown")),
        std::make_pair("royalblue", tr("royalblue")),
        std::make_pair("saddlebrown", tr("saddlebrown")),
        std::make_pair("salmon", tr("salmon")),
        std::make_pair("sandybrown", tr("sandybrown")),
        std::make_pair("seagreen", tr("seagreen")),
        std::make_pair("seashell", tr("seashell")),
        std::make_pair("sienna", tr("sienna")),
        std::make_pair("silver", tr("silver")),
        std::make_pair("skyblue", tr("skyblue")),
        std::make_pair("slateblue", tr("slateblue")),
        std::make_pair("slategray", tr("slategray")),
        std::make_pair("slategrey", tr("slategrey")),
        std::make_pair("snow", tr("snow")),
        std::make_pair("springgreen", tr("springgreen")),
        std::make_pair("steelblue", tr("steelblue")),
        std::make_pair("tan", tr("tan")),
        std::make_pair("teal", tr("teal")),
        std::make_pair("thistle", tr("thistle")),
        std::make_pair("tomato", tr("tomato")),
        std::make_pair("transparent", tr("transparent")),
        std::make_pair("turquoise", tr("turquoise")),
        std::make_pair("violet", tr("violet")),
        std::make_pair("wheat", tr("wheat")),
        std::make_pair("white", tr("white")),
        std::make_pair("whitesmoke", tr("whitesmoke")),
        std::make_pair("yellow", tr("yellow")),
        std::make_pair("yellowgreen", tr("yellowgreen")),
    };

    Q_ASSERT(pPrivateObject && model() && view());

    const QStringList &colorNames {QColor::colorNames()};
    int i {0};

    view()->setSelectionMode(QAbstractItemView::NoSelection);
    for (const QString &colorName : colorNames)
    {
        if (excludedColorSet.contains(colorName))
        {
            continue;
        }

        const QColor color(colorName);
        addItem(colorTaranslation.value(colorName, colorName), color);

        pPrivateObject->indexForColor.insert(color.name(), i);

        Q_ASSERT(model());

        const QColor fontColor {Utility::isColorBright(color) ? Qt::black : Qt::white};

        switch (type_)
        {
        case Type::Backround:
            model()->setData(model()->index(i, 0), color, Qt::BackgroundRole);
            model()->setData(model()->index(i, 0), fontColor, Qt::ForegroundRole);
            break;
        case Type::Text:
            model()->setData(model()->index(i, 0), color, Qt::ForegroundRole);
            model()->setData(model()->index(i, 0), fontColor, Qt::BackgroundRole);
            break;
        default:
            Q_ASSERT(false);
        }
        model()->setData(model()->index(i++, 0), color, Qt::UserRole);
    }


    if (!connect(this, qOverload<int>(&QComboBox::currentIndexChanged), this,
                 [this](int i)
                 {
                    if (i > -1)
                    {
                        Q_ASSERT(pPrivateObject);

                        pPrivateObject->updateColors();
                    }
                 })
        )
    {
        qCritical() << "Cannot connect signal and slot";
    }
}

ColorComboBox::~ColorComboBox() = default; // virtual public override final;

void ColorComboBox::setCurrentColor(const QColor &color_)   // public
{
    Q_ASSERT(pPrivateObject);

    if (!color_.isValid()) return;

    if (pPrivateObject->indexForColor.contains(color_.name()))
    {
        const int newIndex {pPrivateObject->indexForColor.value(color_.name())};

        Q_ASSERT(newIndex < count());

        setCurrentIndex(newIndex);
    }
}

QColor ColorComboBox::currentColor() const  // public
{
    Q_ASSERT(model());

    if (currentIndex() < 0) return {};

    const QModelIndex modelIndex {model()->index(currentIndex(), 0)};
    return model()->data(modelIndex, Qt::UserRole).value<QColor>();
}

QColor ColorComboBox::colorAt(const int i_) const   // public
{
    Q_ASSERT(model());

    if (i_ < 0 || i_ >= count()) return {};

    const QModelIndex modelIndex {model()->index(i_, 0)};
    return model()->data(modelIndex, Qt::UserRole).value<QColor>();
}

/////// ColorComboBox::PrivateObject ///////
void ColorComboBox::PrivateObject::updateColors()   // private::public
{
    Q_ASSERT(pObj && pObj->model());

    const QModelIndex modelIndex {pObj->model()->index(pObj->currentIndex(), 0)};
    const QColor bkgColor {pObj->model()->data(modelIndex, Qt::BackgroundRole).value<QColor>()};
    const QColor textColor {pObj->model()->data(modelIndex, Qt::ForegroundRole).value<QColor>()};

    QPalette p {pObj->palette()};
    p.setColor(QPalette::Normal, QPalette::Button, bkgColor);
    p.setColor(QPalette::Inactive, QPalette::Button, bkgColor);
    p.setColor(QPalette::Disabled, QPalette::Button, bkgColor);
    p.setColor(QPalette::Normal, QPalette::ButtonText, textColor);
    p.setColor(QPalette::Inactive, QPalette::ButtonText, textColor);
    p.setColor(QPalette::Disabled, QPalette::ButtonText, textColor);
    pObj->setPalette(p);
}


} // DEMO_NAMESPACE
