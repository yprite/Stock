/*
 * SaMessageEventListener.cpp
 *
 *  Created on: Sep 12, 2017
 *      Author: devbin
 */

#include "SaMessageEventListener.h"
#include "SaMessageEventManager.h"

using namespace app_assist;

SaMessageEventListener::SaMessageEventListener() : _signalId(WSIGNAL_INVALID_ID)
{
    // TODO Auto-generated constructor stub
    _signalId = SaMessageEventManager::getInstance()->addListener(
        [this](const std::string& key, const std::string& code, const std::string& result, const std::string& errorMessage)
        {
            onMessageReceived(key, code, result, errorMessage);
        });
}

SaMessageEventListener::~SaMessageEventListener()
{
    // TODO Auto-generated destructor stub
    if (_signalId != WSIGNAL_INVALID_ID)
    {
        SaMessageEventManager::getInstance()->removeListener(_signalId);
        _signalId = WSIGNAL_INVALID_ID;
    }
}

