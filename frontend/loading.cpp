#include "send_request.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QJsonValue>
#include <iostream>
#include <string>
#include <stdio.h>
#include <loading.h>

Loading::Loading(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Loading)
{
    ui->setupUi(this);
}

Loading::~Loading()
{
    delete ui;
}
//!
//! \brief Loading::show_loading_screen
//! \param email_list
//! \param name_list
//! \return
//!
QStringList Loading::show_loading_screen(QStringList email_list, QStringList name_list){

    send_request send;
    Loading loading_screen;
    QStringList bool_response;
    QDateTime today = QDateTime::currentDateTime();
    QString date_today = today.toString("dd-MM-yy");
    const char *url = "http://192.168.178.82:8001/api/email";
    ui->progressBar->setMaximum(email_list.size());
    ui->progressBar->setMinimum(0);


    QJsonObject email_content;
    for(int i = 0; i < email_list.size(); i++){
        email_content.insert("email", QJsonValue::fromVariant(email_list[i]));
        email_content.insert("subject", QJsonValue::fromVariant("Corona Warning"));
        email_content.insert("name", QJsonValue::fromVariant(name_list[i]));
        email_content.insert("content", QJsonValue::fromVariant(date_today));
        QJsonDocument content_document(email_content);
        QJsonDocument response_document = send.request_send_email(url, email_content);
        QJsonObject response = response_document.object();
        if (bool boo = response["status"] == "Email Sent"){
            bool_response.append("1");
            ui->progressBar->setValue(i + 1);
        }
        else {
            bool_response.append("0");
        }

    }
    if (bool_response.size() == email_list.size()){
        ui->status_lable->setText("Email/s sending is done!");
    }
    return bool_response;

}
