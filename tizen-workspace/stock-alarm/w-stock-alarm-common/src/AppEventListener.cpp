/*
 * AppEventListener.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: gabin
 */

#include "AppEventListener.h"

using namespace app_assist;

AppEventListener::AppEventListener() : _signalId(WSIGNAL_INVALID_ID)
{
    _signalId = AppEventManager::getInstance()->addListener(
        [this](AppEventManager::EventType type)
        {
            onEvent(type);
        });
}

AppEventListener::~AppEventListener()
{
    if (_signalId != WSIGNAL_INVALID_ID)
    {
        AppEventManager::getInstance()->removeListener(_signalId);
        _signalId = WSIGNAL_INVALID_ID;
    }
}


