/*
 * AppEventManager.h
 *
 *  Created on: Dec 13, 2016
 *      Author: gabin
 */

#ifndef _APP_EVENT_MANAGER_H_
#define _APP_EVENT_MANAGER_H_

#include "WDefine.h"
#include "WSignal.h"
#include "Singleton.h"

class WAPP_ASSIST_EXPORT AppEventManager
{
SINGLETON_IDIOM(AppEventManager);
public:
    enum class EventType
    {
        FONT_CHANGED = 0,
        COLOR_CHANGED,
        TIME_CHANGED,
        LANGUAGE_CHANGED,
        REGION_FORMAT_CHANGED,
        APP_PAUSED,
        APP_RESUMED,
    };
public:
    app_assist::WSignal_ListenerId addListener(const std::function<void(AppEventManager::EventType typ)>& listener);
    void removeListener(app_assist::WSignal_ListenerId id);
    void propagateEvent(AppEventManager::EventType eventType);

protected:
    AppEventManager();
    virtual ~AppEventManager();

private:
    WDISABLE_COPY_AND_ASSIGN(AppEventManager);
private:
    app_assist::WSignal<void(AppEventManager::EventType)> _eventSignal;
};

#endif /* _APP_EVENT_MANAGER_H_ */
