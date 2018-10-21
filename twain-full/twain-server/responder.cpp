#include "responder.h"

#include <QUrlQuery>
#include <QFile>

Responder::Responder(QHttpRequest *request, QHttpResponse *response):
    m_request(request),
    m_response(response)
{

}

void Responder::onRequestComplete()
{
    qDebug()<<QString(m_request->body());

    QUrlQuery query(QString(m_request->body()));
    for(QPair<QString,QString> pair : query.queryItems())
    {
        params->insert(pair.first,pair.second);
    }
    requestMapping();
}

void Responder::requestMapping()
{
    QString path = m_request->path();
    qDebug(QString("BaseResponder :%1").arg(path).toLatin1());

    if(path.startsWith("/status")) {
        status();
    } else if(path.startsWith("/index")) {
        index();
    } else if(path.startsWith("/favicon.ico")) {
        logo();
    } else {
        error_404();
    }
}

void Responder::doResponse()
{

}

void Responder::error_400()
{
    m_response->writeHead(QHttpResponse::STATUS_BAD_REQUEST);
    m_response->setHeader("Content-Type","text/html");
    m_response->write("<html>");
    render_header("Method not support");
    m_response->write("<body style='color:#333; height: 100%; min-height: 100%;'>");
    m_response->write("<div style='min-height:80%;'>");
    m_response->write("<h1>Method not support!</h1>");
    m_response->write("</div>");
    render_footer();
    m_response->write("</body></html>");
    m_response->end();
}

void Responder::error_404()
{
    m_response->writeHead(QHttpResponse::STATUS_NOT_FOUND);
    m_response->setHeader("Content-Type","text/html");
    m_response->write("<html>");
    render_header("Not Found");
    m_response->write("<body style='color:#333; height: 100%; min-height: 100%;'>");
    m_response->write("<div style='min-height:80%;'>");
    m_response->write("<h1>Not Found</h1><p>");
    m_response->write("The requested URL ");
    m_response->write(m_request->path().toLatin1());
    m_response->write(" was not found on this server.");
    m_response->write("</div>");
    render_footer();
    m_response->write("</body></html>");
    m_response->end();
}

void Responder::error_405()
{
    m_response->writeHead(QHttpResponse::STATUS_METHOD_NOT_ALLOWED);
    m_response->setHeader("Content-Type","text/html");
    m_response->write("<html>");
    render_header("Method not allowed");
    m_response->write("<body style='color:#333; height: 100%; min-height: 100%;'>");
    m_response->write("<div style='min-height:80%;'>");
    m_response->write("<h1>Method not allowed</h1>");
    m_response->write("</div>");
    render_footer();
    m_response->write("</body></html>");
    m_response->end();
}

void Responder::status()
{
    m_response->writeHead(QHttpResponse::STATUS_OK);
    m_response->setHeader("Content-Type","text/html");
    m_response->write("<html>");
    render_header("Status");
    m_response->write("<body style='color:#333; height: 100%; min-height: 100%;'><h1>Twain Server is running</h1><p>");
    m_response->write("API list");
    m_response->write("<div style='min-height:80%;'>");
    m_response->write("<div style='background-color:#f3f3f3;'><ul>");
    m_response->write(QString("<li><a href='http://%1:%2/setScanFilePath'>setScanFilePath</a></li>").arg("127.0.0.1").arg(port).toLatin1());
    m_response->write(QString("<li><a href='http://%1:%2/startScan'>startScan</a></li>").arg("127.0.0.1").arg(port).toLatin1());
    m_response->write(QString("<li><a href='http://%1:%2/stopScan'>stopScan</a></li>").arg("127.0.0.1").arg(port).toLatin1());
    m_response->write(QString("<li><a href='http://%1:%2/listDS'>listDS</a></li>").arg("127.0.0.1").arg(port).toLatin1());
    m_response->write(QString("<li><a href='http://%1:%2/setDS'>setDS</a></li>").arg("127.0.0.1").arg(port).toLatin1());
    m_response->write("</ul>");
    m_response->write("</div>");
    m_response->write(QString("<a href='mailto:%1'>Contact us</a><p>").arg("hellohrk@163.com").toLatin1());
    m_response->write("</div>");

    render_footer();
    m_response->write("</body></html>");
    m_response->end();
}

void Responder::logo()
{
    QFile file(":/logo.ico");
    if(file.open(QFile::ReadOnly))
    {
        m_response->writeHead(QHttpResponse::STATUS_OK);
        m_response->setHeader("Content-Type","image/x-icon");
        m_response->write(file.readAll());
        m_response->end();
    }
    else
    {
        error_404();
    }
}

void Responder::render_header(QString title)
{
    m_response->write("<head>");
    m_response->write("<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>");
    m_response->write("<link rel='icon' href='/favicon.ico' type='image/x-icon' />");
    m_response->write("<link rel='shortcut icon' href='/favicon.ico' type='image/x-icon' />");
    m_response->write("<link rel='bookmark' href='/favicon.ico' type='image/x-icon' />");
    m_response->write(QString("<title>%1</title>").arg(title).toLatin1());
    m_response->write("</head>");
}

void Responder::render_footer()
{
    m_response->write(QString("<p><div style='color:#999; text-align:center;'>%1</div><p>").arg("Copyright 2018 by haork. All Rights Reserved").toLatin1());
}

int Responder::getPort()
{
    return this->port;
}

void Responder::setPort(int port)
{
    this->port = port;
}

void Responder::index()
{
    m_response->writeHead(QHttpResponse::STATUS_OK);
    m_response->setHeader("Content-Type","application/json"); 

    QJsonObject obj;
    obj.insert("code",QHttpResponse::STATUS_OK);
    obj.insert("msg",QString("success"));

    m_response->writeJson(obj);
    m_response->end();
}

