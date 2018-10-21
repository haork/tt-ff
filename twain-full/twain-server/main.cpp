#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <signal.h>

#include "requesthandler.h"

//////////////////////////////////////////////////////////////////////////////
/**
* Display exit message.
* @param[in] _sig not used.
*/
void onSigINT(int _sig)
{
    qDebug()<<"bye"<<_sig;
    exit(0);
}


//////////////////////////////////////////////////////////////////////////////
/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);
    QSettings setting("config.ini",QSettings::IniFormat);
    setting.beginGroup("Generic");
    int port = setting.value("port",8080).toInt();

    RequestHandler requestHander;
    requestHander.listen(port);

    signal(SIGINT, &onSigINT);

    return app.exec();
}


