/*
 * AppEventListener.cpp
 *
 *  Created on: Aug 21, 2017
 *      Author: devbin
 */

#include "AppEventListener.h"
#include "AppEventManager.h"

using namespace app_assist;

AppEventListener::AppEventListener() : _signalId(WSIGNAL_INVALID_ID)
{
    // TODO Auto-generated constructor stub
    _signalId = AppEventManager::getInstance()->addListener(
        [this](AppEventListener::EventType eventType)
        {
            onEventOccured(eventType);
        });
}

AppEventListener::~AppEventListener()
{
    // TODO Auto-generated destructor stub
    if (_signalId != WSIGNAL_INVALID_ID)
    {
        AppEventManager::getInstance()->removeListener(_signalId);
        _signalId = WSIGNAL_INVALID_ID;
    }
}
