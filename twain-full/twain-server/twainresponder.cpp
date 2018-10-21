#include "twainresponder.h"
#include <QJsonArray>
#include <QUrlQuery>

TwainResponder::TwainResponder(QHttpRequest *request, QHttpResponse *response, MyTwainApp *twain):
    Responder(request,response),m_twain(twain)
{

}

void TwainResponder::doResponse()
{
    params = new QMap<QString,QString>();

    if(m_request->method()==QHttpRequest::HTTP_GET)
    {
        QUrlQuery query(m_request->url());
        for(QPair<QString,QString> pair : query.queryItems())
        {
            params->insert(pair.first,pair.second);
        }

        requestMapping();
    }
    else if(m_request->method()==QHttpRequest::HTTP_POST)
    {
        m_request->storeBody();
        connect(m_request,SIGNAL(end()),this,SLOT(onRequestComplete()));
    }
    else
    {
        error_405();
    }
}

void TwainResponder::requestMapping()
{
    QString path = m_request->path();
    qDebug(QString("TwainResponder: %1").arg(path).toLatin1());
    if(path.startsWith("/setScanFilePath")) {
        setScanFilePath();
    } else if(path.startsWith("/startScan")) {
        startScan();
    } else if(path.startsWith("/stopScan")) {
        stopScan();
    } else if(path.startsWith("/listDS")) {
        listDS();
    } else if(path.startsWith("/setDS")) {
        setDS();
    } else {
        Responder::requestMapping();
    }
}

void TwainResponder::listDS()
{
    QJsonObject json;
    if(m_twain->m_DSMState<3)
    {
        json.insert("code",QHttpResponse::STATUS_INTERNAL_SERVER_ERROR);
        json.insert("msg",QString("The DSM has not been open yet,please open it first!"));
        qDebug("The DSM has not been open yet,please open it first!");
    }
    else
    {
        QJsonArray array;
        vector<TW_IDENTITY> ds = m_twain->getDataSources();
        for(int i=0;i<ds.size();++i)
        {
            QJsonObject obj;
            obj.insert("Id",(qint64)ds[i].Id);
            obj.insert("ProductName",QString(ds[i].ProductName));
            obj.insert("Manufacturer",QString(ds[i].Manufacturer));
            array.append(obj);
        }
        json.insert("code",QHttpResponse::STATUS_OK);
        json.insert("msg",QString("success"));
        json.insert("data",array);
    }
    reply(json);
}

void TwainResponder::setDS()
{
    QJsonObject json;
    if(params->contains("dsid"))
    {
        int dsid = params->value("dsid","0").toInt();
        m_twain->loadDS(dsid);
        if(m_twain->m_DSMState==4)
        {
            m_twain->initCaps();
        }
    }
    else
    {
        json.insert("code",400);
        json.insert("msg",QString("Parameter 'dsid' can not be empty!"));
    }
    reply(json);
}

void TwainResponder::setScanFilePath()
{
    QJsonObject json;
    if(params->contains("path"))
    {
        QString path = params->value("path","");

        m_twain->setSavePath(path);

        json.insert("code",200);
        json.insert("msg",QString("success"));
        json.insert("data",path);
    }
    else
    {
        json.insert("code",400);
        json.insert("msg",QString("Parameter 'path' can not be empty!"));
    }
    reply(json);
}

void TwainResponder::startScan()
{
    m_response->writeHead(QHttpResponse::STATUS_OK);
    m_response->setHeader("Content-Type","application/json");

    QJsonObject obj;

    if(m_twain->m_DSMState<3)
    {
        obj.insert("code",QHttpResponse::STATUS_INTERNAL_SERVER_ERROR);
        obj.insert("msg",QString("You need to select a source first!"));
    }
    else
    {
        m_twain->m_DSMessage=0;

        if(!m_twain->enableDS(0,FALSE))
        {
            obj.insert("code",QHttpResponse::STATUS_INTERNAL_SERVER_ERROR);
            obj.insert("msg",QString("You need to select a source first!"));
        }

        #ifdef TWNDS_OS_WIN
        while(!m_twain->m_DSMessage)
        {
            TW_EVENT twEvent = {0};
            MSG Msg;
            if(!GetMessage((LPMSG)&Msg, NULL,0,0))
            {
                break;
            }
            twEvent.pEvent = (TW_MEMREF)&Msg;
            twEvent.TWMessage - MSG_NULL;
            TW_UINT16 twRC = TWRC_NOTDSEVENT;
            twRC = _DSM_Entry(m_twain->getAppIdentity(),
                              m_twain->getDataSource(),
                              DG_CONTROL,
                              DAT_EVENT,
                              MSG_PROCESSEVENT,
                              (TW_MEMREF)&twEvent);
            if(!m_twain->gUSE_CALLBACKS && TWRC_DSEVENT)
            {
                switch(twEvent.TWMessage)
                {
                case MSG_XFERREADY:
                case MSG_CLOSEDSREQ:
                case MSG_CLOSEDSOK:
                case MSG_NULL:
                    m_twain->m_DSMessage = twEvent.TWMessage;
                    break;
                default:
                    qDebug()<<"Error - Unknown message in MSG_PROCESSEVENT loop";
                    break;
                }
            }
            if(!twRC!=TWRC_DSEVENT)
            {
                TranslateMessage ((LPMSG)&Msg);
                DispatchMessage ((LPMSG)&Msg);
            }
        }
#elif defined(TWNDS_OS_LINUX)
        sem_wait(&(m_twain->m_TwainEvent)); // event semaphore handle
#endif
        if(m_twain->m_DSMessage == MSG_XFERREADY)
        {
            m_twain->m_DSMState = 6;
            QString error;
            if(m_twain->startScan(error))
            {
                obj.insert("code",QHttpResponse::STATUS_OK);
                obj.insert("msg",QString("success"));
            }
            else
            {
                obj.insert("code",QHttpResponse::STATUS_BAD_REQUEST);
                obj.insert("msg",error);
            }
        }
        else
        {
            obj.insert("code",QHttpResponse::STATUS_BAD_REQUEST);
            obj.insert("msg",QString("MSG status error"));
        }
        m_twain->disableDS();
    }
    reply(obj);
}

void TwainResponder::stopScan()
{
    QJsonObject json;
    json.insert("code",200);
    json.insert("msg",QString("success"));
    reply(json);
}

void TwainResponder::reply(QJsonObject json)
{
    m_response->writeHead(QHttpResponse::STATUS_OK);
    m_response->setHeader("Content-Type","application/json");
    m_response->writeJson(json);
    m_response->end();
}
