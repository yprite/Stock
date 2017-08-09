/*
 * WNaviframeControllerEx.h
 *
 *  Created on: Dec 13, 2016
 *      Author: gabin
 */

#ifndef _WNAVIFRAME_CONTROLLER_EX_H_
#define _WNAVIFRAME_CONTROLLER_EX_H_

#include <Eina.h>
#include <Elementary.h>
//#include <efl_assist.h> // efl_assist header is not included in public SDK.

#include "WDefine.h"
#include "WNaviframeController.h"

class WAPP_ASSIST_EXPORT WNaviframeControllerEx : public app_assist::WNaviframeController
{
public:
    WNaviframeControllerEx();

public:
    void deleteTop();
    virtual app_assist::WViewController* getTopView();
    virtual app_assist::WViewController* getBottomView();

protected:
    virtual ~WNaviframeControllerEx();
};

#endif /* _WNAVIFRAME_CONTROLLER_EX_H_ */
