#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include "qhttpserverfwd.h"
#include "mytwainapp.h"

class RequestHandler : public TwainApp
{
    Q_OBJECT
public:
    RequestHandler();

    ~RequestHandler();

    void listen(int port);

    void close();

    void error_400(QHttpResponse *response);

    void error_404(QHttpRequest *request, QHttpResponse *response);

    void index(QHttpRequest *request,QHttpResponse *response);

    void setScanFilePath(QHttpRequest *request,QHttpResponse *response);

    void startScan(QHttpRequest *request,QHttpResponse *response);

    void stopScan(QHttpRequest *request,QHttpResponse *response);

    void about(QHttpRequest *request,QHttpResponse *response);

signals:

public slots:
    void requestMapper(QHttpRequest *request, QHttpResponse *response);

private:
    QHttpServer *server;
    MyTwainApp *twain;
    int port;
};

#endif // REQUESTHANDLER_H
