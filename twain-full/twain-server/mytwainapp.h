#ifndef MYTWAINAPP_H
#define MYTWAINAPP_H

#include <QObject>
#include <twainapp.h>

class MyTwainApp : public TwainApp
{
public:
    MyTwainApp(HWND parent=NULL);

    ~MyTwainApp();

    virtual void fillIdentity(TW_IDENTITY& _identity);

    virtual void loadDS(const TW_INT32 _dsID);

    virtual void unloadDS();

    void initCaps();

    void uninitCaps();

    bool startScan(QString &msg);

    bool getICAP_XFERMECH(TW_UINT16& _val);

    bool getICAP_IMAGEFILEFORMAT(TW_UINT16& _val);

    QString getSavePath();

    void setSavePath(QString path);

    int m_DSMState;

    vector<TW_IDENTITY> getDataSources();

    #ifdef TWNDS_OS_LINUX
      sem_t m_TwainEvent;
    #endif

private:

    ////////////////
    // CAPABILITIES

    TW_CAPABILITY   m_CAP_XFERCOUNT;        /**< Number of images the application is willing to accept this session. */
    TW_CAPABILITY   m_ICAP_XFERMECH;        /**< Transfer mechanism - used to learn options and set-up for upcoming transfer. */
    TW_CAPABILITY   m_ICAP_IMAGEFILEFORMAT; /**< File format saved when using File Xfer Mechanism. */
    TW_CAPABILITY   m_ICAP_COMPRESSION;     /**< Compression method used for upcoming transfer. */
    TW_CAPABILITY   m_ICAP_UNITS;           /**< Unit of measure (inches, centimeters, etc). */
    TW_CAPABILITY   m_ICAP_PIXELTYPE;       /**< The type of pixel data (B/W, gray, color, etc). */
    TW_CAPABILITY   m_ICAP_BITDEPTH;        /**< Pixel bit depth for Current value of ICAP_PIXELTYPE. */
    TW_CAPABILITY   m_ICAP_XRESOLUTION;     /**< Current/Available optical resolutions for x-axis. */
    TW_CAPABILITY   m_ICAP_YRESOLUTION;     /**< Current/Available optical resolutions for y-axis */
    TW_CAPABILITY   m_ICAP_FRAMES;          /**< Size and location of frames on page. */

};

#endif // MYTWAINAPP_H
