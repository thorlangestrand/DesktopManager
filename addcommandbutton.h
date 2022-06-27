#ifndef ADDCOMMANDBUTTON_H
#define ADDCOMMANDBUTTON_H

#include <QPushButton>
#include <QEvent>
#include <QDebug>
#include <QKeyEvent>



/**
 * @brief The AddCommandButton class
 *
 * Inherit basic button and "promote" button in ui to AddCommandButton class
 * in order for it to ignore keypresses (spacebar in particular)
 * as this was messing with the search functionality i wanted
 */
class AddCommandButton : public QPushButton
{
    Q_OBJECT;
public:
    AddCommandButton(const QString& text, QWidget * parent = nullptr);
    AddCommandButton(QWidget * parent = nullptr);
    ~AddCommandButton(){};
protected:
    void keyPressEvent(QKeyEvent* e) override;
};

#endif // ADDCOMMANDBUTTON_H
