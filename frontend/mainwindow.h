/**
 * \class mainwindow
 *
 *
 * \brief Provide the main window of the whole app
 *
 * This is class is meant to be showe all functions in the app
 * it's devided by 2 tabs view one shows the over view
 * of the trainers in/out, the current time and the
 * number of the people in. The other tabe controls
 * all other functionalities
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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "json.hpp"
#include <stdio.h>
#include "request.h"
#include "helper.h"

using json = nlohmann::json;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    //!
    //! \brief Allows user to register new trainers
    //!
    void on_pushButton_register_clicked();
    //!
    //! \brief Allows user to delete existing trainers
    //!
    void on_pushButton_delete_user_clicked();
    //!
    //! \brief Allows user to set the max trainers allowed to be in
    //!
    void on_set_max_pushButton_clicked();
    //!
    //! \brief Allows user to get the max trainers allowed to be in
    //!
    void on_get_max_pushButton_clicked();
    //!
    //! \brief Updates the overview table every one sec
    //!
    void update_overview();
    //!
    //! \brief Allows user to see all trainers saved in the database
    //!
    void on_show_all_users_clicked();
    //!
    //! \brief Allows user to search for one trainer in the database
    //!
    void on_show_user_pushButton_clicked();
    //!
    //! \brief Allows user to show the trainers checked-in on specific date
    //!
    void on_show_users_onDate_pushButton_clicked();
    //!
    //! \brief Allows user to search for one or more trainers in the datatable
    //! \param arg1     text to search with
    //!
    void on_search_in_table_lineEdit_textChanged(const QString &arg1);
    //!
    //! \brief Allows user to reset the datatable view
    //!
    void on_reste_datatable_pushButton_clicked();
    //!
    //! \brief Allows user to sed waning emailsto list of chosen emails
    //!
    void on_create_email_pushButton_clicked();
    //!
    //! \brief Allows user to select all given trainers in the datatable view
    //! \param arg1     contains the checkbox status
    //!
    void on_selectAll_checkBox_stateChanged(int arg1);  //

private:
    Ui::MainWindow *ui;

    QTimer *timer;



};
#endif // MAINWINDOW_H
