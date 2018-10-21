#include "Common.h"
#include "CommonTWAIN.h"

#include <QDebug>

#ifdef TWNDS_OS_LINUX
  #include <semaphore.h>
#endif

//////////////////////////////////////////////////////////////////////////////
/**
* Callback funtion for DS.  This is a callback function that will be called by
* the source when it is ready for the application to start a scan. This
* callback needs to be registered with the DSM before it can be called.
* It is important that the application returns right away after recieving this
* message.  Set a flag and return.  Do not process the callback in this function.
*/
#ifdef TWH_CMP_MSC
TW_UINT16 FAR PASCAL
#else
FAR PASCAL TW_UINT16
#endif

DSMCallback(pTW_IDENTITY _pOrigin,
            pTW_IDENTITY _pDest,
            TW_UINT32    _DG,
            TW_UINT16    _DAT,
            TW_UINT16    _MSG,
            TW_MEMREF    _pData)
{
  UNUSEDARG(_pDest);
  UNUSEDARG(_DG);
  UNUSEDARG(_DAT);
  UNUSEDARG(_pData);

  TW_UINT16 twrc = TWRC_SUCCESS;

  // we are only waiting for callbacks from our datasource, so validate
  // that the originator.
//  if(0 == _pOrigin)
//          ||_pOrigin->Id != gpTwainApplicationCMD->getDataSource()->Id)
//  {
//    return TWRC_FAILURE;
//  }
  switch (_MSG)
  {
    case MSG_XFERREADY:
    case MSG_CLOSEDSREQ:
    case MSG_CLOSEDSOK:
    case MSG_NULL:
//      gpTwainApplicationCMD->m_DSMessage = _MSG;
      // now signal the event semaphore
    #ifdef TWNDS_OS_LINUX
      {
      int test=12345;
//      sem_post(&(gpTwainApplicationCMD->m_TwainEvent));    // Event semaphore Handle
  }
    #endif
      break;

    default:
      qDebug() << "Error - Unknown message in callback routine";
      twrc = TWRC_FAILURE;
      break;
  }

  return twrc;
}
