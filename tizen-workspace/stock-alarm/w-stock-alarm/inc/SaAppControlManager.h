/*
 * SaAppControlManager.h
 *
 *  Created on: Sep 12, 2017
 *      Author: devbin
 */

#ifndef SAAPPCONTROLMANAGER_H_
#define SAAPPCONTROLMANAGER_H_

#include "Singleton.h"
#include "SaCompanyInfo.h"
#include <app_control.h>

class SaAppControlManager
{
    SINGLETON_IDIOM(SaAppControlManager);
public:
    bool initialize();
    bool finalize();

    bool isWidgetRequest() const;
    void setCallerControl(app_control_h appControl);
    app_control_h getCallerControl() const;
    bool sendSelectedInfo(const SaCompanyInfo& info);

protected:
    SaAppControlManager();
    virtual ~SaAppControlManager();

private:
    app_control_h _appControl;
    bool _isWidgetRequest;
};

#endif /* SAAPPCONTROLMANAGER_H_ */
