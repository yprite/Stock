/*
 * SaWidget.h
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#ifndef SAWIDGET_H_
#define SAWIDGET_H_

#include "WWidget.h"
#include <system_settings.h>

class SaWidget : public app_assist::WWidget
{
public:
    SaWidget();
    virtual ~SaWidget();

private:
    virtual void onCreate() override;
    virtual void onTerminate() override;
    virtual app_assist::WWidgetInstance* onCreateWidgetInstance() override;

private:
    static void _onSystemSettingChanged(system_settings_key_e key, void *user_data);
};

#endif /* SAWIDGET_H_ */
