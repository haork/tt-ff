#ifndef BASERESPONDER_H
#define BASERESPONDER_H

#include <QObject>

class BaseResponder : public QObject
{
    Q_OBJECT
public:
    BaseResponder(QObject *parent = 0);

    BaseResponder(QHttpRequest *request, QHttpResponse *response);

signals:

public slots:
};

#endif // BASERESPONDER_H
