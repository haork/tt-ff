#ifndef RESPONDER_H
#define RESPONDER_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <qhttprequest.h>
#include <qhttpresponse.h>

class Responder : public QObject
{
    Q_OBJECT
public:
    Responder(QHttpRequest *request, QHttpResponse *response);

    virtual void requestMapping();

    virtual void doResponse();

    void error_400();
    void error_404();
    void error_405();

    void index();

    void status();

    void logo();


    void render_header(QString title);

    void render_footer();

    int getPort();
    void setPort(int port);

signals:

public slots:
    void onRequestComplete();
protected:
    QHttpRequest *m_request;
    QHttpResponse *m_response;
    QMap<QString,QString> *params;
    int port;
};

#endif // RESPONDER_H
