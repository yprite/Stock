/*
 * SaAppControlManager.cpp
 *
 *  Created on: Sep 12, 2017
 *      Author: devbin
 */

#include "SaAppControlManager.h"
#include "SaDebug.h"
#include <widget_service.h>
SINGLETON_INSTANCE(SaAppControlManager);

SaAppControlManager::SaAppControlManager()
{
    // TODO Auto-generated constructor stub
    _appControl = nullptr;
    _isWidgetRequest = false;
}

SaAppControlManager::~SaAppControlManager()
{
    // TODO Auto-generated destructor stub
}


bool SaAppControlManager::initialize()
{
    return true;
}
bool SaAppControlManager::finalize()
{
    if (_appControl)
        app_control_destroy(_appControl);
    return false;
}

bool SaAppControlManager::isWidgetRequest() const
{
    return _isWidgetRequest;
}

void SaAppControlManager::setCallerControl(app_control_h appControl)
{
    if (_appControl)
        app_control_destroy(_appControl);
    app_control_clone(&_appControl, appControl);

    char *caller = nullptr;
    app_control_get_caller(appControl, &caller);
    if (caller)
        WINFO("caller : %s", caller);

    if (caller)
    {
        if (!strcmp(caller, "team.tizenia.w-stock-alarm-widget"))
            _isWidgetRequest = true;
        else
            _isWidgetRequest = false;
        free(caller);
    }
}

app_control_h SaAppControlManager::getCallerControl() const
{
    return _appControl;
}

bool SaAppControlManager::sendSelectedInfo(const SaCompanyInfo& info)
{
    WENTER();
    if (_isWidgetRequest)
    {
        int ret = APP_CONTROL_ERROR_NONE;
        WDEBUG("sending data : %s", info.code.c_str());
        app_control_h reply = nullptr;
        app_control_create(&reply);
        app_control_add_extra_data(reply, "code", info.code.c_str());
        app_control_reply_to_launch_request(reply, _appControl, APP_CONTROL_RESULT_SUCCEEDED);
        app_control_destroy(reply);

        return true;
    }
    return false;
}
