#ifndef FORMULA_HPP
#define FORMULA_HPP

#include <QApplication>
#include <QString>
#include <QWidget>

#include <QtMmlWidget>

class Formula
    : public QtMmlWidget
{
public:
    inline Formula(QWidget *parent = 0)
        : QtMmlWidget(parent)
    {
        this->setBaseFontPointSize(QApplication::font().pointSize());
        this->setFontName(QtMmlWidget::NormalFont, QApplication::font().family());
    };

    inline void setText(QString const &text)
    {
        this->setContent("<mi>" + text + "</mi>");
    }
};

#endif // FORMULA_HPP
