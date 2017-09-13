/*
 * SaMessageEventListener.h
 *
 *  Created on: Sep 12, 2017
 *      Author: devbin
 */

#ifndef SAMESSAGEEVENTLISTENER_H_
#define SAMESSAGEEVENTLISTENER_H_

#include "WSignal.h"
#include "WDefine.h"

class WAPP_ASSIST_EXPORT SaMessageEventListener
{
public:
    SaMessageEventListener();
    virtual ~SaMessageEventListener();

protected:
    virtual void onMessageReceived(const std::string& key, const std::string& code, const std::string& result, const std::string& errorMessage) = 0;
    app_assist::WSignal_ListenerId _signalId;

};

#endif /* SAMESSAGEEVENTLISTENER_H_ */
