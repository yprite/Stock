/*
 * AppEventListener.h
 *
 *  Created on: Dec 13, 2016
 *      Author: gabin
 */

#ifndef _APP_EVENT_LISTENER_H_
#define _APP_EVENT_LISTENER_H_

#include "WDefine.h"
#include "AppEventManager.h"

class WAPP_ASSIST_EXPORT AppEventListener
{
public:
    AppEventListener();
    virtual ~AppEventListener();

protected:
    virtual void onEvent(AppEventManager::EventType eventType) = 0;
    app_assist::WSignal_ListenerId _signalId;
};

#endif /* _APP_EVENT_LISTENER_H_ */
