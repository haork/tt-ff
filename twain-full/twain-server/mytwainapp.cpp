#include "mytwainapp.h"


MyTwainApp::MyTwainApp(HWND parent)
{

}

MyTwainApp::~MyTwainApp()
{

}

void MyTwainApp::fillIdentity(TW_IDENTITY &_identity)
{
    TwainApp::fillIdentity(_identity);
    SSTRCPY(_identity.Manufacturer,sizeof(_identity.Manufacturer),"haork");
    SSTRCPY(_identity.ProductFamily, sizeof(_identity.ProductFamily),"Twain Server");
    SSTRCPY(_identity.ProductName,sizeof(_identity.ProductName),"Twain Server");
}

void MyTwainApp::loadDS(const TW_INT32 _dsID)
{
    TwainApp::loadDS(_dsID);

    if(m_DSMState == 4)
    {
       initCaps();
    }
}

void MyTwainApp::unloadDS()
{
    uninitCaps();
    TwainApp::unloadDS();
}

void MyTwainApp::initCaps()
{
    if(m_DSMState < 3)
    {
      PrintCMDMessage("The DSM needs to be opened first.\n");
      return;
    }
    else if(m_DSMState < 4)
    {
      PrintCMDMessage("A Data Source needs to be opened first.\n");
      return;
    }

    // get the default pixel type
    memset(&m_CAP_XFERCOUNT,0,sizeof(TW_CAPABILITY));
    m_CAP_XFERCOUNT.Cap = CAP_XFERCOUNT;
    get_CAP(m_CAP_XFERCOUNT);

    memset(&m_ICAP_PIXELTYPE,0,sizeof(TW_CAPABILITY));
    m_ICAP_PIXELTYPE.Cap = ICAP_PIXELTYPE;
    get_CAP(m_ICAP_PIXELTYPE);

    memset(&m_ICAP_XFERMECH,0,sizeof(TW_CAPABILITY));
    m_ICAP_XFERMECH.Cap = ICAP_XFERMECH;
    get_CAP(m_ICAP_XFERMECH);

    memset(&m_ICAP_IMAGEFILEFORMAT,0,sizeof(TW_CAPABILITY));
    m_ICAP_IMAGEFILEFORMAT.Cap = ICAP_IMAGEFILEFORMAT;
    get_CAP(m_ICAP_IMAGEFILEFORMAT);

    memset(&m_ICAP_COMPRESSION,0,sizeof(TW_CAPABILITY));
    m_ICAP_COMPRESSION.Cap = ICAP_COMPRESSION;
    get_CAP(m_ICAP_COMPRESSION);

    memset(&m_ICAP_UNITS,0,sizeof(TW_CAPABILITY));
    m_ICAP_UNITS.Cap = ICAP_UNITS;
    get_CAP(m_ICAP_UNITS);

    memset(&m_ICAP_BITDEPTH,0,sizeof(TW_CAPABILITY));
    m_ICAP_BITDEPTH.Cap = ICAP_BITDEPTH;
    get_CAP(m_ICAP_BITDEPTH);

    memset(&m_ICAP_XRESOLUTION,0,sizeof(TW_CAPABILITY));
    m_ICAP_XRESOLUTION.Cap = ICAP_XRESOLUTION;
    get_CAP(m_ICAP_XRESOLUTION);

    memset(&m_ICAP_YRESOLUTION,0,sizeof(TW_CAPABILITY));
    m_ICAP_YRESOLUTION.Cap = ICAP_YRESOLUTION;
    get_CAP(m_ICAP_YRESOLUTION);

    memset(&m_ICAP_FRAMES,0,sizeof(TW_CAPABILITY));
    m_ICAP_FRAMES.Cap = ICAP_FRAMES;
    get_CAP(m_ICAP_FRAMES);
}

void MyTwainApp::uninitCaps()
{
    if(m_DSMState < 3)
    {
        PrintCMDMessage("The DSM needs to be opened first.\n");
        return;
    }
    else if(m_DSMState < 4)
    {
        PrintCMDMessage("A Data Source needs to be opened first.\n");
        return;
    }

    if(m_CAP_XFERCOUNT.hContainer)
    {
        _DSM_Free(m_CAP_XFERCOUNT.hContainer);
        m_CAP_XFERCOUNT.hContainer=0;
    }
    if(m_ICAP_PIXELTYPE.hContainer)
    {
        _DSM_Free(m_ICAP_PIXELTYPE.hContainer);
        m_ICAP_PIXELTYPE.hContainer=0;
    }
    if(m_ICAP_XFERMECH.hContainer)
    {
        _DSM_Free(m_ICAP_XFERMECH.hContainer);
        m_ICAP_XFERMECH.hContainer=0;
    }
    if(m_ICAP_IMAGEFILEFORMAT.hContainer)
    {
        _DSM_Free(m_ICAP_IMAGEFILEFORMAT.hContainer);
        m_ICAP_IMAGEFILEFORMAT.hContainer=0;
    }
    if(m_ICAP_COMPRESSION.hContainer)
    {
        _DSM_Free(m_ICAP_COMPRESSION.hContainer);
        m_ICAP_COMPRESSION.hContainer=0;
    }
    if(m_ICAP_UNITS.hContainer)
    {
        _DSM_Free(m_ICAP_UNITS.hContainer);
        m_ICAP_UNITS.hContainer=0;
    }
    if(m_ICAP_BITDEPTH.hContainer)
    {
        _DSM_Free(m_ICAP_BITDEPTH.hContainer);
        m_ICAP_BITDEPTH.hContainer=0;
    }
    if(m_ICAP_XRESOLUTION.hContainer)
    {
        _DSM_Free(m_ICAP_XRESOLUTION.hContainer);
        m_ICAP_XRESOLUTION.hContainer=0;
    }
    if(m_ICAP_YRESOLUTION.hContainer)
    {
        _DSM_Free(m_ICAP_YRESOLUTION.hContainer);
        m_ICAP_YRESOLUTION.hContainer=0;
    }
    if(m_ICAP_FRAMES.hContainer)
    {
        _DSM_Free(m_ICAP_FRAMES.hContainer);
        m_ICAP_FRAMES.hContainer=0;
    }
}

bool MyTwainApp::getICAP_XFERMECH(TW_UINT16 &_val)
{
    TW_UINT32 val;
    bool rtn = getCurrent(&m_ICAP_XFERMECH, val);
    _val = (TW_UINT16)val;
    return rtn;
}

bool MyTwainApp::getICAP_IMAGEFILEFORMAT(TW_UINT16 &_val)
{
    TW_UINT32 val;
    bool rtn = getCurrent(&m_ICAP_IMAGEFILEFORMAT, val);
    _val = (TW_UINT16)val;
    return rtn;
}

QString MyTwainApp::getSavePath()
{
    return QString::fromStdString(m_strSavePath);
}

void MyTwainApp::setSavePath(QString path)
{
    this->m_strSavePath = path.toStdString();
}

vector<TW_IDENTITY> MyTwainApp::getDataSources()
{
    return this->m_DataSources;
}

bool MyTwainApp::startScan(QString &msg)
{
    if(m_DSMState != 6)
    {
        msg="Error: fax state invalid";
        return false;
    }
    TW_UINT16 mech;
    if(!getICAP_XFERMECH(mech))
    {
        msg="Error: counld not get the transfer mechanism";
        return false;
    }

    switch(mech)
    {
    case TWSX_NATIVE:
        initiateTransfer_Native();
        break;
    case TWSX_FILE:
    {
        TW_UINT16 fileformat = TWFF_TIFF;
        if(!getICAP_IMAGEFILEFORMAT(fileformat))
        {
          // Default back to TIFF
          fileformat = TWFF_TIFF;
        }
        initiateTransfer_File(fileformat);
    }
    break;
    case TWSX_MEMORY:
        initiateTransfer_Memory();
        break;
    }
    return true;
}
