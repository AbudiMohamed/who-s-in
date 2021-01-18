/**
 * \class Loading
 *
 *
 * \brief Provide a loading screen while warning emails are being sent
 *
 * This is class is meant to be showen after the user selects
 * the emails list from the datatable view to whom the user
 * wants to send the warning emails to.
 *
 *
 * \author $Author: Abdelrahman mohamed, Ahmed Mahmoud$
 *
 * \version $Revision: latest $
 *
 *
 *
 *
 *
 */

#ifndef LOADING_H
#define LOADING_H


#include <mainwindow.h>
#include <QMainWindow>
#include "ui_loading.h"
#include <mainwindow.h>
#include <send_request.h>
#include "ui_mainwindow.h"






namespace Ui {
class Loading;
}

class Loading : public QDialog
{
    Q_OBJECT

public:
    explicit Loading(QWidget *parent = nullptr);
    ~Loading();

    //!
    //! \brief show loading screen while sending the emails
    //! \param email_list   list of emails to whom the emails should be sent
    //! \param name_list    list of names to whom the emails should be sent
    //! \return bool_response   bool array contains all response
    //!
    QStringList show_loading_screen(QStringList email_list, QStringList name_list);


private:
    Ui::Loading *ui; // Ui method of the progress bar

};

#endif // LOADING_H
