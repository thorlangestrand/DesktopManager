#include "addcommandbutton.h"

/**
 * @brief AddCommandButton::AddCommandButton
 *
 * Simple overload which inherits constructor from
 * QPushButton
 *
 * @param text      Text in button
 * @param parent    Used for positioning and cleanup
 */
AddCommandButton::AddCommandButton(const QString& text, QWidget * parent) : QPushButton(text, parent)
{

}

/**
 * @brief AddCommandButton::AddCommandButton
 *
 * Simple overload which inherits constructor from
 * QPushButton
 *
 * @param parent    Used for positioning and cleanup
 */
AddCommandButton::AddCommandButton(QWidget * parent) : QPushButton(parent)
{

}

/**
 * @brief AddCommandButton::keyPressEvent
 *
 * This is just a subclass where we overwrite the
 * keyPressEvent of the QPushButton class in order
 * for the button to ignore keypresses and avoid having
 * to lock keyboard functionality to MainWindow using
 * ui->centralwidget->grabKeyboard();
 *
 * @param e
 */
void AddCommandButton::keyPressEvent(QKeyEvent* e)
{
    e->ignore();
}
