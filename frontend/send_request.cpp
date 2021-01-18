#include "send_request.h"
#include <curl/curl.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include <stdio.h>
#include<QDebug>
#include <QtNetwork>




using json = nlohmann::json;
using namespace std;


//!
//! \brief send_request::WriteCallback
//! \param contents
//! \param size
//! \param nmemb
//! \param userp
//! \return size * nmemb
//!
size_t send_request::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//!
//! \brief send_request::convert_to_char
//! \param input
//! \return
//!
const char* send_request::convert_to_char (QString input){
    QByteArray str = input.toLocal8Bit();
    const char*output = str.data();
    return output;
}

//!
//! \brief send_request::request_put
//!
//! \param url
//! \param new_trainer
//! \return
//!
 json send_request::request_put (const char *url , content new_trainer)
{
    string username = new_trainer.username.toStdString();
    string name = new_trainer.name.toStdString();
    string id = new_trainer.id.toStdString();
    string email = new_trainer.email.toStdString();
    string _content = "{""\"username\" : \"" + username +"\","" \"name\" : \"" + name + "\", ""\"email\" : \"" + email +"\", ""\"id\" : \"" + id +"\"""}";
    QString print = QString::fromStdString(_content);
    qDebug() << print;
    const char* content = _content.c_str();

    CURL *curl;
    string readBuffer;
    char frame[1000000];

    sprintf(frame, content);

    curl = curl_easy_init();

    if(curl) {
          struct curl_slist *headers = NULL;
          curl_slist_append(headers, "Accept: application/json");
          curl_slist_append(headers, "Content-Type: application/json");
          curl_slist_append(headers, "charset: utf-8");
          curl_easy_setopt(curl, CURLOPT_URL, url);
          curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
          curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
          curl_easy_setopt(curl, CURLOPT_POSTFIELDS, frame);
          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&readBuffer);
          curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");
          curl_easy_perform(curl);
          curl_easy_cleanup(curl);
     json response = json::parse(readBuffer);
     return response;
    }
    return "Server internal Error";
}

 //!
 //! \brief send_request::request_delete
 //! \param url
 //! \return
 //!
 json send_request::request_delete (QString url )
{
    CURL *curl;
    string readBuffer;
    const char *_url = convert_to_char(url);

    curl = curl_easy_init();
    if(curl) {
          struct curl_slist *headers = NULL;
          curl_slist_append(headers, "Accept: application/json");
          curl_slist_append(headers, "Content-Type: application/json");
          curl_slist_append(headers, "charset: utf-8");
          curl_easy_setopt(curl, CURLOPT_URL, _url); //
          curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
          curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&readBuffer);
          curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");
          curl_easy_perform(curl);
          curl_easy_cleanup(curl);
     json response = json::parse(readBuffer);
     return response;
    }
    return "Server internal Error";
}

 //!
 //! \brief send_request::request_get_oneUser
 //! \param url
 //! \return
 //!
 json send_request::request_get_oneUser (QString url)
{
    CURL *curl;
    string readBuffer;
    const char *_url = convert_to_char(url);

    curl = curl_easy_init();
    if(curl) {
          struct curl_slist *headers = NULL;
          curl_slist_append(headers, "Accept: application/json");
          curl_slist_append(headers, "Content-Type: application/json");
          curl_slist_append(headers, "charset: utf-8");
          curl_easy_setopt(curl, CURLOPT_URL, _url); //
          curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
          curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&readBuffer);
          curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");
          curl_easy_perform(curl);
          curl_easy_cleanup(curl);
     auto response = json::parse(readBuffer);
     return response;
    }
    return "Server internal Error";
}

 //!
 //! \brief send_request::request_get
 //! \param url
 //! \return
 //!
 json send_request::request_get (QString url)
{
    CURL *curl;
    string readBuffer;
    const char *_url = convert_to_char(url);

    curl = curl_easy_init();
    if(curl) {
          struct curl_slist *headers = NULL;
          curl_slist_append(headers, "Accept: application/json");
          curl_slist_append(headers, "Content-Type: application/json");
          curl_slist_append(headers, "charset: utf-8");
          curl_easy_setopt(curl, CURLOPT_URL, _url);
          curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
          curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&readBuffer);
          curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");
          curl_easy_perform(curl);
          curl_easy_cleanup(curl);
     auto response = json::parse(readBuffer);
     return response;
    }
    return "Server internal Error";
}

 //!
 //! \brief send_request::request_send_email
 //! \param url
 //! \param request_content
 //! \return
 //!
 QJsonDocument send_request::request_send_email (const char *url , QJsonObject request_content)
     {
         QNetworkRequest request(QUrl("http://192.168.178.82:8001/api/email"));
         request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
         QNetworkAccessManager nam;
         QNetworkReply *reply = nam.post(request, QJsonDocument(request_content).toJson());
         while (!reply->isFinished())
         {
             qApp->processEvents();
         }
         QByteArray response_data = reply->readAll();
         QJsonDocument json = QJsonDocument::fromJson(response_data);
         QJsonDocument Doc(json);
         return Doc;
 }

