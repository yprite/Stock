/*
 * AppEventManager.h
 *
 *  Created on: Aug 21, 2017
 *      Author: devbin
 */

#ifndef APPEVENTMANAGER_H_
#define APPEVENTMANAGER_H_

#include "AppEventListener.h"
#include "Singleton.h"
#include "WDefine.h"

class WAPP_ASSIST_EXPORT AppEventManager
{
    SINGLETON_IDIOM(AppEventManager);

public:
    app_assist::WSignal_ListenerId addListener(
            const std::function<void(AppEventListener::EventType eventType)>& listener);
    void removeListener(app_assist::WSignal_ListenerId id);
    void propagateEvent(AppEventListener::EventType eventType);

protected:
    AppEventManager();
    virtual ~AppEventManager();

private:
    app_assist::WSignal<void(AppEventListener::EventType)> _eventSignal;
};

#endif /* APPEVENTMANAGER_H_ */
