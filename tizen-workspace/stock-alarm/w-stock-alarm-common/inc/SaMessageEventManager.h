/*
 * SaMessageEventManager.h
 *
 *  Created on: Sep 12, 2017
 *      Author: devbin
 */

#ifndef SAMESSAGEEVENTMANAGER_H_
#define SAMESSAGEEVENTMANAGER_H_

#include "Singleton.h"
#include "WDefine.h"
#include "SaMessageEventListener.h"

class WAPP_ASSIST_EXPORT SaMessageEventManager
{
    SINGLETON_IDIOM(SaMessageEventManager);
public:

    app_assist::WSignal_ListenerId addListener(
            const std::function<void(const std::string& key, const std::string& code, const std::string& result, const std::string& errorMessage)>& listener);
    void removeListener(app_assist::WSignal_ListenerId id);
    void propagateEvent(const std::string& key, const std::string& code, const std::string& result, const std::string& errorMessage);

protected:
    SaMessageEventManager();
    virtual ~SaMessageEventManager();

private:
    app_assist::WSignal<void(const std::string& key, const std::string& code, const std::string& result, const std::string& errorMessage)> _eventSignal;
};

#endif /* SAMESSAGEEVENTMANAGER_H_ */
