/*
 * AppEventListener.h
 *
 *  Created on: Aug 21, 2017
 *      Author: devbin
 */

#ifndef APPEVENTLISTENER_H_
#define APPEVENTLISTENER_H_

#include "WSignal.h"
#include "WDefine.h"

class WAPP_ASSIST_EXPORT AppEventListener
{
public:
    enum class EventType
    {
        APP_PAUSE,
        APP_RESUME,
        FONT_CHANGED,
        COLOR_CHANGED,
        TIME_CHANGED,
        LANGUAGE_CHANGED,
        REGION_FORMAT_CHANGED,
    };

public:
    AppEventListener();
    virtual ~AppEventListener();

protected:
    virtual void onEventOccured(AppEventListener::EventType eventType) = 0;
    app_assist::WSignal_ListenerId _signalId;
};

#endif /* APPEVENTLISTENER_H_ */
