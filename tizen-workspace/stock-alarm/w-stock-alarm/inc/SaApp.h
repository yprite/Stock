/*
 * SaApp.h
 *
 *  Created on: Jul 24, 2017
 *      Author: devbin
 */

#ifndef _SA_APP_H_
#define _SA_APP_H_

#include "WApp.h"

class SaApp: public app_assist::WApp
{
    public:
        SaApp();
        virtual ~SaApp();

    private:
        virtual bool onCreate() override;
        virtual void onTerminate() override;
        virtual void onAppControl(app_control_h request, bool firstLaunch) override;
        virtual void onPause() override;
        virtual void onResume() override;
};

#endif /* _SA_APP_H_ */
