/*
 * AppEventManager.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: gabin
 */

#include "AppEventManager.h"
#include <algorithm>

using namespace app_assist;

SINGLETON_INSTANCE(AppEventManager);

WSignal_ListenerId AppEventManager::addListener(const std::function<void(AppEventManager::EventType)>& listener)
{
    return _eventSignal.addListener(listener);
}

void AppEventManager::removeListener(WSignal_ListenerId id)
{
    return _eventSignal.removeListener(id);
}

void AppEventManager::propagateEvent(AppEventManager::EventType type)
{
    _eventSignal.emit(type);
}

AppEventManager::AppEventManager()
{

}

AppEventManager::~AppEventManager()
{

}
