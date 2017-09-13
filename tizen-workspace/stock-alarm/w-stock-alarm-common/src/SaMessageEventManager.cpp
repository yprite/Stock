/*
 * SaMessageEventManager.cpp
 *
 *  Created on: Sep 12, 2017
 *      Author: devbin
 */

#include "SaMessageEventManager.h"

SINGLETON_INSTANCE(SaMessageEventManager);

SaMessageEventManager::SaMessageEventManager()
{
    // TODO Auto-generated constructor stub

}

SaMessageEventManager::~SaMessageEventManager()
{
    // TODO Auto-generated destructor stub
}

app_assist::WSignal_ListenerId SaMessageEventManager::addListener(
        const std::function<void(const std::string& key, const std::string& code, const std::string& result, const std::string& errorMessage)>& listener)
{
    return _eventSignal.addListener(listener);
}
void SaMessageEventManager::removeListener(app_assist::WSignal_ListenerId id)
{
    _eventSignal.removeListener(id);
}
void SaMessageEventManager::propagateEvent(const std::string& key, const std::string& code, const std::string& result, const std::string& errorMessage)
{
    _eventSignal.emit(key, code, result, errorMessage);
}


