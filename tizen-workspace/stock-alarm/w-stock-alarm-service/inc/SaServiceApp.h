/*
 * SaServiceApp.h
 *
 *  Created on: Jul 25, 2017
 *      Author: devbin
 */

#ifndef _SA_SERVICE_APP_H_
#define _SA_SERVICE_APP_H_

#include "WServiceApp.h"
#include <app_manager.h>

class SaServiceApp : public app_assist::WServiceApp
{
    public:
        SaServiceApp();
        virtual ~SaServiceApp();

    private:
        virtual bool onCreate() override;
        virtual void onTerminate() override;
        virtual void onAppControl(app_control_h request, bool firstLaunch) override;

    private:
        static void _onAppContextEventChanged(app_context_h appContext, app_context_event_e event, void *userData);
//
//    private:
//        bool _isAppAlive;
//        bool _isWidgetAlive;
};

#endif /* _SA_SERVICE_APP_H_ */
