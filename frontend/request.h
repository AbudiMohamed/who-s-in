#ifndef REQUEST_H
#define REQUEST_H

#include <QMainWindow>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QJsonValue>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include <stdio.h>


using json = nlohmann::json;
using namespace std;

struct content {
  QString username;
  QString name;
  QString email;
  QString id;
};


class request {

public:
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    const char* convert_to_char (QString input);
    json request_put (const char *url, content new_trainer);
};








#endif // REQUEST_H
