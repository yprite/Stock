/*
 * AppEventManager.cpp
 *
 *  Created on: Aug 21, 2017
 *      Author: devbin
 */

#include "AppEventManager.h"

SINGLETON_INSTANCE(AppEventManager);

AppEventManager::AppEventManager()
{
    // TODO Auto-generated constructor stub

}

AppEventManager::~AppEventManager()
{
    // TODO Auto-generated destructor stub
}

app_assist::WSignal_ListenerId AppEventManager::addListener(
        const std::function<void(AppEventListener::EventType eventType)>& listener)
{
    return _eventSignal.addListener(listener);
}

void AppEventManager::removeListener(app_assist::WSignal_ListenerId id)
{
    _eventSignal.removeListener(id);
}

void AppEventManager::propagateEvent(AppEventListener::EventType eventType)
{
    _eventSignal.emit(eventType);
}
