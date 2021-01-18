/**
 * \class send_request
 *
 *
 * \brief Manges all requests in the app
 *
 * This is class is meant to mange all http requests needed
 * in the app
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
#ifndef SEND_REQUEST_H
#define SEND_REQUEST_H

#ifndef REQUEST_H
#define REQUEST_H

#include <QMainWindow>
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

class send_request {


public:
    //!
    //! \brief Help function for all http requests
    //! \param contents
    //! \param size
    //! \param nmemb
    //! \param userp
    //! \return
    //!
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    //!
    //! \brief Converts QString variables to const char *
    //! \param QString input
    //! \return const char* output
    //!
    const char* convert_to_char (QString input);
    //!
    //! \brief Http put request
    //! \param url
    //! \param new_trainer
    //! \return response
    //!
    json request_put (const char *url, content new_trainer);
    //!
    //! \brief Http delete request
    //! \param url
    //! \return response
    //!
    json request_delete (QString url);
    //!
    //! \brief Http get request
    //! \param url
    //! \return one user
    //!
    json request_get_oneUser (QString url);
    //!
    //! \brief Http get request
    //! \param url
    //! \return response
    //!
    json request_get (QString url);
    //!
    //! \brief Sends emails
    //! \param url
    //! \param content
    //! \return response
    //!
    QJsonDocument request_send_email (const char *url , QJsonObject content);
};



#endif // REQUEST_H

#endif // SEND_REQUEST_H
