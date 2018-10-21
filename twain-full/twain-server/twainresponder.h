#ifndef TWAINRESPONDER_H
#define TWAINRESPONDER_H

#include <responder.h>
#include <mytwainapp.h>

class TwainResponder : public Responder
{
public:
    TwainResponder(QHttpRequest *request, QHttpResponse *response, MyTwainApp *twain);

    virtual void doResponse() override;
    virtual void requestMapping() override;

    void listDS();
    void setDS();
    void setScanFilePath();
    void startScan();
    void stopScan();

    void reply(QJsonObject json);

private:
    MyTwainApp *m_twain;
};

#endif // TWAINRESPONDER_H
