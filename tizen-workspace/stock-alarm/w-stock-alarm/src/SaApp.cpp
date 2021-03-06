/*
 * SaApp.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: devbin
 */

#include "SaApp.h"

#include "WWindowController.h"
#include "WNaviframeController.h"
#include "SaNoContentViewController.h"
#include "SaDataConsumer.h"
#include "SaCompanyDBManager.h"
#include "RotaryManager.h"
#include "SaBaseListViewController.h"
#include "SaCompanyListViewController.h"
#include "SaAppControlManager.h"
#include "SaDebug.h"

#include <Elementary.h>

using namespace app_assist;

static const char PACKAGE_NAME[] = "team.tizenia.w-stock-alarm";

SaApp::SaApp()
{
    // TODO Auto-generated constructor stub

}

SaApp::~SaApp()
{
    // TODO Auto-generated destructor stub
}

void foo(char **str)
{
}

bool SaApp::onCreate()
{
    WENTER();

    elm_config_accel_preference_set("opengl");

    attachWindowController(new WWindowController(PACKAGE_NAME, ELM_WIN_BASIC));

    WNaviframeController* navi = new WNaviframeController();

    // Add following statements, if you want to terminate application instead of going background

    navi->setOnLastItemPop(
            [] (bool* popOut)
            {
                *popOut = true;
                elm_exit();
            });

    getWindowController()->attachBaseViewController(navi);

//    ScRotaryManager::getInstance()->initialize(getWindowController()->getConformantEvasObject());
//
//    // init service application
    app_control_h appControl = nullptr;
    app_control_create(&appControl);
    app_control_set_operation(appControl, APP_CONTROL_OPERATION_DEFAULT);
    app_control_set_app_id(appControl, "team.tizenia.w-stock-alarm-service");
    app_control_send_launch_request(appControl, NULL, NULL);
    app_control_destroy(appControl);
//
//    ScDataExchangeConsumer::getInstance().init();

    // initialize singleton object
    SaDataConsumer::getInstance()->initialize();
    RotaryManager::getInstance()->initialize(getWindowController()->getConformantEvasObject());
    SaCompanyDBManager::getInstance()->initialize();
    SaAppControlManager::getInstance()->initialize();

//    SaCompanyInfo info = {"aaa", "bbb", "ccc"};
//    SaCompanyDBManager::getInstance()->add(info);
//    SaCompanyDBManager::getInstance()->add(info);
//    SaCompanyDBManager::getInstance()->add(info);

    return true;
}

void SaApp::onTerminate()
{
    //  ScRotaryManager::getInstance()->finalize();
    SaDataConsumer::getInstance()->finalize();
    SaCompanyDBManager::getInstance()->finalize();
    SaAppControlManager::getInstance()->finalize();
}

void SaApp::onAppControl(app_control_h request, bool firstLaunch)
{
    WENTER();
    SaAppControlManager::getInstance()->setCallerControl(request);
    if (firstLaunch)
    {
        auto navi = (WNaviframeController *)(getWindowController()->getBaseViewController());
        navi->push(new SaBaseListViewController());

        //navi->push(new SaNoContentViewController());
/*
        auto companyListView = new SaCompanyListViewController();
        companyListView->setOnItemClicked(
            []()
            {
                WHIT();
            });
        navi->push(companyListView);
*/
    }
}

void SaApp::onPause()
{

}

void SaApp::onResume()
{

}
