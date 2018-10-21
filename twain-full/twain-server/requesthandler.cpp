#include "requesthandler.h"

#include <QCoreApplication>
#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>
#include <QUrlQuery>
#include <twainresponder.h>

RequestHandler::RequestHandler()
{
    twain = new MyTwainApp();
    server = new QHttpServer(this);
    connect(server,SIGNAL(newRequest(QHttpRequest*,QHttpResponse*)),
            this,SLOT(requestMapper(QHttpRequest*,QHttpResponse*)));
}

RequestHandler::~RequestHandler()
{
    delete twain;
    delete server;
}

void RequestHandler::listen(int port)
{
    this->port = port;
    twain->connectDSM();
    server->listen(QHostAddress::Any,port);
}

void RequestHandler::close()
{
    if(server!=NULL) {
        server->close();
        twain->disconnectDSM();
    }
}

void RequestHandler::requestMapper(QHttpRequest *request, QHttpResponse *response)
{
    Q_UNUSED(request);

    TwainResponder *responder = new TwainResponder(request,response,twain);
    responder->setPort(this->port);
    responder->doResponse();
}
