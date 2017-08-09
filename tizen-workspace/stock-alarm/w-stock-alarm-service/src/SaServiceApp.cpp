/*
 * SaServiceApp.cpp
 *
 *  Created on: Jul 25, 2017
 *      Author: devbin
 */

#include "SaServiceApp.h"
#include "SaServiceDebug.h"

#include <app_manager.h>
#include <app_preference.h>

#define STOCK_ALARM_APP_ID          "team.tizenia.w-stock-alarm"
#define STOCK_ALARM_WIDGET_ID   "team.tizenia.w-stock-alarm-widget"

SaServiceApp::SaServiceApp()
{
    // TODO Auto-generated constructor stub
//    _isAppAlive = false;
//    _isWidgetAlive = false;
}

SaServiceApp::~SaServiceApp()
{
    // TODO Auto-generated destructor stub
}

bool SaServiceApp::onCreate()
{
    WENTER();

    int ret = app_manager_set_app_context_event_cb(_onAppContextEventChanged, this);
    if (ret != APP_MANAGER_ERROR_NONE)
    {
        WERROR("app_manager_set_app_context_event_cb failed. (%d)", ret);
    }

    return true;
}

void SaServiceApp::onTerminate()
{
    WENTER();

    app_manager_unset_app_context_event_cb();
}

void SaServiceApp::onAppControl(app_control_h request, bool firstLaunch)
{
    WENTER();

//    //yql_test();
//    {
//        char *appId = nullptr;
//        app_control_get_app_id(request, &appId);
//
//        if(appId)
//        {
//            if (!strcmp(appId, STOCK_ALARM_APP_ID))
//                _isAppAlive = true;
//            if (!strcmp(appId, STOCK_ALARM_WIDGET_ID))
//                _isWidgetAlive = true;
//            free(appId);
//        }
//    }

    if (firstLaunch)
    {

    }
}

void SaServiceApp::_onAppContextEventChanged(app_context_h appContext, app_context_event_e event, void *userData)
{
    WENTER();
    auto self = (SaServiceApp *)userData;

    char *appId = nullptr;
    int ret = APP_MANAGER_ERROR_NONE;
#if 0
    ret = app_context_get_app_id(appContext, &appId);
    //int app_context_get_app_state(app_context_h app_context, app_state_e *state);
    if (ret != APP_MANAGER_ERROR_NONE)
    {
        WERROR("app_context_get_app_id failed. (%d)", ret);

        if (appId)
            free(appId);

        return;
    }

    if (appId == nullptr)
    {
        WERROR("appId is nullptr. failed.");
        return;
    }

    if (!strcmp(appId, STOCK_ALARM_APP_ID) && event == APP_CONTEXT_EVENT_TERMINATED)
    {
        free(appId);
        self->_isAppAlive = false;

        bool isWidgetRunning = false;
        app_manager_is_running(STOCK_ALARM_WIDGET_ID, &isWidgetRunning);
        if (isWidgetRunning == false)
            service_app_exit();
    }

    if (!strcmp(appId, STOCK_ALARM_WIDGET_ID) && event == APP_CONTEXT_EVENT_TERMINATED)
    {
        free(appId);
        self->_isAppAlive = false;

        bool isWidgetRunning = false;
        app_manager_is_running(STOCK_ALARM_APP_ID, &isWidgetRunning);
        if (isWidgetRunning == false)
            service_app_exit();
    }
#endif
}
