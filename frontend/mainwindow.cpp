#include "send_request.h"
#include "mainwindow.h"
#include "helper.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QNetworkReply>
#include <QDateTime>
#include <QJsonValue>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include <stdio.h>
#include <QtWidgets>
#include <QDialog>
#include <QApplication>
#include <ctime>
#include <QRegExp>
#include <loading.h>





using json = nlohmann::json;
using namespace std;


//!
//! \brief MainWindow::MainWindow
//! \param parent
//!
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QPixmap logo ("/home/abudi/Desktop/ATD/logo.png");
    QPixmap circle ("/home/abudi/Desktop/ATD/circle.png");

    ui->setupUi(this);
    //Logo
    ui->logo->setPixmap(logo.scaled(220,220,Qt::KeepAspectRatio));
    ui->logo_2->setPixmap(logo.scaled(220,220,Qt::KeepAspectRatio));
    ui->circle_blue->setPixmap(circle.scaled(220,220,Qt::KeepAspectRatio));
    ui->circle_blue_2->setPixmap(circle.scaled(220,220,Qt::KeepAspectRatio));
    //Overview Table
    ui->overview_table->setColumnWidth(0, 170);
    ui->overview_table->setColumnWidth(1, 185);
    ui->overview_table->setColumnWidth(2, 160);
    ui->overview_table->setAlternatingRowColors(true);
    ui->overview_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->overview_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->overview_table->setTextElideMode(Qt::ElideMiddle);
    //Data table
    ui->data_table->setColumnWidth(0, 10);
    ui->data_table->setColumnWidth(1, 160);
    ui->data_table->setColumnWidth(2, 160);
    ui->data_table->setColumnWidth(3, 160);
    ui->data_table->setColumnWidth(4, 160);
    ui->data_table->setAlternatingRowColors(true);
    ui->data_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->data_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->data_table->setTextElideMode(Qt::ElideMiddle);

    ui->trainers_in->setAttribute(Qt::WA_TranslucentBackground);



    //Update overview screen every second
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_overview()));
    timer ->start(1000);
    //Doxygen
}

MainWindow::~MainWindow()
{
    delete ui;
}

//!
//! \brief MainWindow::update_overview
//!
//!
void MainWindow::update_overview(){

    send_request send;

    //Update the watch
    QTime time_now = QTime::currentTime();
    ui->time->setText(time_now.toString());

    //Clean table before update
    ui->trainers_in->clear();
    ui->overview_table->setRowCount(0);

    //Get the data
    QString url = "http://192.168.178.82:8000/api/whoisin";
    auto response = send.request_get(url);
    int trainers_in_date = response.size();
    int counter = 0;

    //Desplay data
    ui->overview_table->setRowCount(trainers_in_date);
    int row = 0;
        for (int i = 0; i < trainers_in_date; i++) {
            counter += 1;

            //id formatting
            string id = response[i]["id"].dump();
            ui->overview_table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(id)));

            //name formating
            string name = response[i]["name"].dump().erase(0, 1);
            name.erase(name.end() - 1, name.end());
            ui->overview_table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(name))  );

            //time formating
            string time = response[i]["checkin time"].dump().erase(0, 1);
            time.erase(time.end() - 1, time.end());

            ui->overview_table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(time)));
            row += 1;

            ui->trainers_in->setText(QString::number(counter));
        }
}


//!
//! \brief MainWindow::on_pushButton_register_clicked
//!
void MainWindow::on_pushButton_register_clicked()
{
   //Get data from the user
   content new_trainer;
   send_request send;
   const char *url = "http://192.168.178.82:8000/api/add_user";
   new_trainer.username = ui->lineEdit_trainer_username->text();
   new_trainer.name = ui->lineEdit_trainer_name->text();
   new_trainer.email = ui->lineEdit_trainer_email->text();
   new_trainer.id = ui->lineEdit_RFID_ID->text();

   //Validate input !=0
   if (new_trainer.username.length() < 1 || new_trainer.name.length() < 1 || new_trainer.email.length() < 1 || new_trainer.id.length() < 1){
       QMessageBox::warning(this, "Registration Failed", "Please check your entries and try again!");
   }
   else{
   //Save user in database
       auto response = send.request_put( url, new_trainer);
       qDebug()<< QString::fromStdString( response.dump());

   //Validate creation
       if (response["msg"] == "recored created"){
         QMessageBox::information(this, "Registration Succeeded", "New trainer has been successfully added!");
         ui->lineEdit_trainer_username->clear();
         ui->lineEdit_trainer_name->clear();
         ui->lineEdit_trainer_email->clear();
         ui->lineEdit_RFID_ID->clear();
       }
       else if (response["detail"] == "Email or id not valid"){
         QMessageBox::warning(this, "Registration Failed", "Email or id not valid or duplicated!");

       }
   }
}


//!
//! \brief MainWindow::on_pushButton_delete_user_clicked
//!
void MainWindow::on_pushButton_delete_user_clicked()
{
    //Get id from user
    QString trainer_id = ui->lineEdit_RFID_ID_detete->text();
    send_request send;
    QString url_get = "http://192.168.178.82:8000/api/get?id=" + trainer_id;
    QString url_delete = "http://192.168.178.82:8000/api/delete_user?id=" + trainer_id;

    //Validate input !=0
    if (trainer_id.length() < 1){
        QMessageBox::warning(this, "Input Error", "Please check your entries and try again!");
    }
    else{
        //Validate user exists
        auto get_response = send.request_get_oneUser(url_get);
        qDebug()<< QString::fromStdString( get_response.dump());
        if (get_response["Error_msg"] == "Id does not exist!"){
            QMessageBox::warning(this, "Delete Failed", "Id does not exist!");
        }
        else{
        //Send confirmation message
        QString username = QString::fromStdString( get_response["username"].dump()).remove(QChar('"'), Qt::CaseInsensitive);
        QString name = QString::fromStdString(get_response["email"].dump()).remove(QChar('"'), Qt::CaseInsensitive);
        QString email = QString::fromStdString(get_response["name"].dump()).remove(QChar('"'), Qt::CaseInsensitive);

        QMessageBox::StandardButton reply;
        QString question = "Are you sure you want to delete user?\n\nUsername: " + username + "\nName: " + name + "\nEmail: " + email;
        reply = QMessageBox::question(this, "Delete", question
                                      ,QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            qDebug() << "Yes was clicked";

            //Delete user from database
            auto delete_response = send.request_delete(url_delete);
            qDebug()<< QString::fromStdString(delete_response.dump());

            //Delete validation
            if (delete_response["msg"] == "recored deleted"){
              QMessageBox::information(this, "Delete Succeeded", "The trainer has been successfully deleted!");
              ui->lineEdit_RFID_ID_detete->clear();
            }
            else {
                QMessageBox::warning(this, "Delete Failed", QString::fromStdString(delete_response["Error_msg:"].dump()));
            }

          } else {
            qDebug() << "Yes was *not* clicked";
          }
        }
    }
}

//!
//! \brief MainWindow::on_set_max_pushButton_clicked
//!
void MainWindow::on_set_max_pushButton_clicked()
{
    //Get max number from user
    send_request send;
    QString max = ui->set_max_lineEdit->text();
    QString url = "http://192.168.178.82:8000/api/set_max?max=" + max;

    //Set max number
    auto response = send.request_get(url);
    qDebug()<< QString::fromStdString(response.dump());
    string response_msg = "Max have been changed to " + max.toStdString();

    //Set validation
    if (response["msg"] == response_msg){
        QMessageBox::information(this, "Max successfully set", "The new maximum number of trainers allowed in is: " + max);
    } else {
        QMessageBox::warning(this, "Failed", "Please check your entries and try again!");
    }
}

//!
//! \brief MainWindow::on_get_max_pushButton_clicked
//!
void MainWindow::on_get_max_pushButton_clicked()
{
    //Get max number
    send_request send;
    QString url = "http://192.168.178.82:8000/api/get_max";
    auto response = send.request_get(url);
    QString max = QString::fromStdString(response.dump());
    qDebug()<< QString::fromStdString(response.dump());

    //Request validation
    if (max.size() > 0){
        ui->view_max_label->setText(max);
    } else {
        QMessageBox::warning(this, "Failed", "Server Error!");
    }

}

//!
//! \brief MainWindow::on_show_all_users_clicked
//!
void MainWindow::on_show_all_users_clicked()
{
    //Get all users
    send_request send;
    QString url = "http://192.168.178.82:8000/api/users";
    auto response = send.request_get(url);
    qDebug()<< QString::fromStdString(response.dump());
    int users = response.size();
    int counter = 0;

    //Request validation
    if (response.size() > 0){
        ui->data_table->setRowCount(0);
        //Desplay data
        ui->data_table->setRowCount(users);
        int row = 0;
            for (int i = 0; i < users; i++) {
                counter += 1;

                //id formatting
                string id = response[i]["id"].dump();
                ui->data_table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(id)));


                //username formating
                string username = response[i]["username"].dump().erase(0, 1);
                username.erase(username.end() - 1, username.end());
                ui->data_table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(username)));


                //name formating
                string name = response[i]["name"].dump().erase(0, 1);
                name.erase(name.end() - 1, name.end());
                ui->data_table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(name)));

                //email formating
                string email = response[i]["email"].dump().erase(0, 1);
                email.erase(email.end() - 1, email.end());
                ui->data_table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(email)));
                 row += 1;

                // Create a widget that will contain a checkbox
                    QWidget *checkBoxWidget = new QWidget();
                    QCheckBox *checkBox = new QCheckBox();                          // declare and initialize the checkbox
                    QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget); // create a layer with reference to the widget
                    layoutCheckBox->addWidget(checkBox);                          // Set the checkbox in the layer
                    layoutCheckBox->setAlignment(Qt::AlignCenter);               // Center the checkbox
                    layoutCheckBox->setContentsMargins(0,0,0,0);                // Set the zero padding

                ui->data_table->setCellWidget(i,0, checkBoxWidget);
            }

            ui->data_table->resizeColumnsToContents();
    } else {
        QMessageBox::warning(this, "Failed", "Server Error!");
    }
}
//!
//! \brief MainWindow::on_show_user_pushButton_clicked
//!
void MainWindow::on_show_user_pushButton_clicked()
{
    QString trainer_id = ui->get_user_lineEdit->text();
    ui->data_table->setRowCount(0);
    send_request send;
    QString url = "http://192.168.178.82:8000/api/get?id=" + trainer_id;
    if (trainer_id.length() < 1){
        QMessageBox::warning(this, "Input Error", "Please check your entries and try again!");
    }
    else{
        auto response = send.request_get_oneUser(url);
        qDebug()<< QString::fromStdString(response.dump());

        //Validate user exists
        if (response["Error_msg"] == "Id does not exist!"){
            QMessageBox::warning(this, "Delete Failed", "Id does not exist!");
        }
        else{
            //Desplay data
            ui->data_table->setRowCount(1);

                    //id formatting
                    string id = response["id"].dump();
                    ui->data_table->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(id)));

                    //username formating
                    string username = response["username"].dump().erase(0, 1);
                    username.erase(username.end() - 1, username.end());
                    ui->data_table->setItem(0, 2, new QTableWidgetItem(QString::fromStdString(username)));


                    //name formating
                    string name = response["name"].dump().erase(0, 1);
                    name.erase(name.end() - 1, name.end());
                    ui->data_table->setItem(0, 3, new QTableWidgetItem(QString::fromStdString(name)));

                    //email formating
                    string email = response["email"].dump().erase(0, 1);
                    email.erase(email.end() - 1, email.end());
                    ui->data_table->setItem(0, 4, new QTableWidgetItem(QString::fromStdString(email)));

                    //initiate checkboxs
                    QWidget *checkBoxWidget = new QWidget();
                    QCheckBox *checkBox = new QCheckBox();                          // declare and initialize the checkbox
                    QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget); // create a layer with reference to the widget
                    layoutCheckBox->addWidget(checkBox);                          // Set the checkbox in the layer
                    layoutCheckBox->setAlignment(Qt::AlignCenter);               // Center the checkbox
                    layoutCheckBox->setContentsMargins(0,0,0,0);                // Set the zero padding
                    ui->data_table->setCellWidget(0,0, checkBoxWidget);        // Desplay checkboxs
        }
    }
}


//!
//! \brief MainWindow::on_show_users_onDate_pushButton_clicked
//!
void MainWindow::on_show_users_onDate_pushButton_clicked()
{
    QString date = ui->get_trainers_onDate_dateEdit->text();

    //Get all users on given date
    send_request send;
    QString url = "http://192.168.178.82:8000/api/get_ondate?date=" + date;
    auto response = send.request_get(url);
    qDebug()<< QString::fromStdString(response.dump());
    int users = response.size();
    int counter = 0;

    //Request validation
    if (response.size() > 0){
        ui->data_table->setRowCount(0);
        //Desplay data
        ui->data_table->setRowCount(users);
        int row = 0;
            for (int i = 0; i < users; i++) {
                counter += 1;

                //id formatting
                string id = response[i]["id"].dump();
                ui->data_table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(id)));


                //username formating
                string username = response[i]["username"].dump().erase(0, 1);
                username.erase(username.end() - 1, username.end());
                ui->data_table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(username)));


                //name formating
                string name = response[i]["name"].dump().erase(0, 1);
                name.erase(name.end() - 1, name.end());
                ui->data_table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(name)));

                //email formating
                string email = response[i]["email"].dump().erase(0, 1);
                email.erase(email.end() - 1, email.end());
                ui->data_table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(email)));
                row += 1;

                //initiate checkboxs
                QWidget *checkBoxWidget = new QWidget();
                QCheckBox *checkBox = new QCheckBox();                          // declare and initialize the checkbox
                QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget); // create a layer with reference to the widget
                layoutCheckBox->addWidget(checkBox);                          // Set the checkbox in the layer
                layoutCheckBox->setAlignment(Qt::AlignCenter);               // Center the checkbox
                layoutCheckBox->setContentsMargins(0,0,0,0);                // Set the zero padding
                ui->data_table->setCellWidget(i,0, checkBoxWidget);        // Desplay checkboxs
            }
            ui->data_table->resizeColumnsToContents();

            } else if (response.size() == 0){
                QMessageBox::information(this, "No Users", "No trainers cheched-in on the given date!");
           }else {
                QMessageBox::warning(this, "Failed", "Server Error!");
           }
}

//!
//! \brief MainWindow::on_search_in_table_lineEdit_textChanged
//! \param arg1
//!
void MainWindow::on_search_in_table_lineEdit_textChanged(const QString &arg1)
{
    int max_col = ui->data_table->columnCount();
    for (int col=0; col< max_col; col++){
        // return a list of all matching results
        QModelIndexList results = ui->data_table->model()->match(
            ui->data_table->model()->index(0, col),
            Qt::DisplayRole,
            arg1,
            -1,
            Qt::MatchContains
        );
        // for each result, select the row
        for (QModelIndex idx : results)
           ui->data_table->selectRow(idx.row()) ;
    }
}

//!
//! \brief MainWindow::on_reste_datatable_pushButton_clicked
//!
void MainWindow::on_reste_datatable_pushButton_clicked()
{
    ui->data_table->setRowCount(0);
}

//!
//! \brief MainWindow::on_create_email_pushButton_clicked
//!
void MainWindow::on_create_email_pushButton_clicked()
{
    Loading loading_screen;
    QStringList email_list;
    QStringList name_list;
    QJsonObject email_content;
    for(int i = 0; i < ui->data_table->rowCount(); i++){
            // take the widget from the cell
            QWidget *item = ( ui->data_table->cellWidget(i,0));
            // take the widget from the layout and cast it to QCheckBox
            QCheckBox *checkbox = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
            //get emails from table
            if( checkbox->isChecked()){
                ui->data_table->selectRow(i);
                QString email = ui->data_table->model()->data(ui->data_table->model()->index(i,4)).toString();
                QString name =  ui->data_table->model()->data(ui->data_table->model()->index(i,3)).toString();
              email_list.push_back(email);
              name_list.push_back(name);
            }
        }
    if( email_list.size() > 0){
        loading_screen.setModal(true);
        loading_screen.show_loading_screen(email_list, name_list);
        loading_screen.exec();
    }
    else {
        QMessageBox::warning(this, "No Trainers selected", "There was no trainers selected, Please select the trainers from the table to whom you want to send the warning and try again!");
    }
}

//!
//! \brief MainWindow::on_selectAll_checkBox_stateChanged
//! \param arg1
//!
void MainWindow::on_selectAll_checkBox_stateChanged(int arg1)
{
    for(int i = 0; i < ui->data_table->rowCount(); i++){
            // take the widget from the cell
            QWidget *item = ( ui->data_table->cellWidget(i,0));
            // take the widget from the layout and cast it to QCheckBox
            QCheckBox *checkbox = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
            //select all on checkbox checked
            if( arg1 == 2){
                checkbox->setChecked(true);
            } else {
                checkbox->setChecked(false);
            }
        }
}
