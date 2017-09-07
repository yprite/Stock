/*
 * SaWidget.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#include "SaWidget.h"
#include "SaWidgetInstance.h"
#include "SaDataConsumer.h"
#include "SaCompanyDBManager.h"
#include "SaWidgetDebug.h"

#include <app.h>
#include <widget_service.h>
#include <feedback.h>
#include <Elementary.h>
#include <system_settings.h>

SaWidget::SaWidget()
{
    // TODO Auto-generated constructor stub

}

SaWidget::~SaWidget()
{
    // TODO Auto-generated destructor stub
}

void SaWidget::onCreate()
{
    WENTER();

    do {
        int ret = SYSTEM_SETTINGS_ERROR_NONE;

        ret = system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_FONT_SIZE, _onSystemSettingChanged, this);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_FONT_SIZE set_changed_cb failed.(%d)", ret);

        ret = system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_FONT_TYPE, _onSystemSettingChanged, this);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_FONT_TYPE set_changed_cb failed.(%d)", ret);

        ret = system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY, _onSystemSettingChanged, this);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY set_changed_cb failed.(%d)", ret);

        ret = system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, _onSystemSettingChanged, this);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE set_changed_cb failed.(%d)", ret);

        ret = system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_TIMEFORMAT_24HOUR, _onSystemSettingChanged, this);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_LOCALE_TIMEFORMAT_24HOUR set_changed_cb failed.(%d)", ret);

        ret = system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_TIMEZONE, _onSystemSettingChanged, this);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_LOCALE_TIMEZONE set_changed_cb failed.(%d)", ret);

        ret = system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_TIME_CHANGED, _onSystemSettingChanged, this);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_TIME_CHANGED set_changed_cb failed.(%d)", ret);

    } while (0);
    /*
    system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_3G_DATA_NETWORK_ENABLED,
        [](system_settings_key_e key, void *userData)
        {
            WINFO("SYSTEM_SETTINGS_KEY_3G_DATA_NETWORK_ENABLED changed callback!");
        }, this);
    */
    SaDataConsumer::getInstance()->initialize();
    SaCompanyDBManager::getInstance()->initialize();
    feedback_initialize();
}

void SaWidget::onTerminate()
{
    do {
        int ret = SYSTEM_SETTINGS_ERROR_NONE;

        ret = system_settings_unset_changed_cb(SYSTEM_SETTINGS_KEY_FONT_SIZE);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_FONT_SIZE unset_changed_cb failed.(%d)", ret);

        ret = system_settings_unset_changed_cb(SYSTEM_SETTINGS_KEY_FONT_TYPE);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_FONT_TYPE unset_changed_cb failed.(%d)", ret);

        ret = system_settings_unset_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY unset_changed_cb failed.(%d)", ret);

        ret = system_settings_unset_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE unset_changed_cb failed.(%d)", ret);

        ret = system_settings_unset_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_TIMEFORMAT_24HOUR);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_LOCALE_TIMEFORMAT_24HOUR unset_changed_cb failed.(%d)", ret);

        ret = system_settings_unset_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_TIMEZONE);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_LOCALE_TIMEZONE unset_changed_cb failed.(%d)", ret);

        ret = system_settings_unset_changed_cb(SYSTEM_SETTINGS_KEY_TIME_CHANGED);
        if (ret != SYSTEM_SETTINGS_ERROR_NONE)
            WERROR("SYSTEM_SETTINGS_KEY_TIME_CHANGED unset_changed_cb failed.(%d)", ret);
    } while (0);

    SaDataConsumer::getInstance()->finalize();
    SaCompanyDBManager::getInstance()->finalize();
    feedback_deinitialize();
}

app_assist::WWidgetInstance* SaWidget::onCreateWidgetInstance()
{
    return new SaWidgetInstance();
}

void SaWidget::_onSystemSettingChanged(system_settings_key_e key, void *user_data)
{
    WENTER();
    auto self = (SaWidget *)user_data;

    switch (key)
    {
        case SYSTEM_SETTINGS_KEY_FONT_SIZE:
            break;

        case SYSTEM_SETTINGS_KEY_FONT_TYPE:
            break;

        case SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY:
            break;

        case SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE:
            break;

        case SYSTEM_SETTINGS_KEY_LOCALE_TIMEFORMAT_24HOUR:
            break;

        case SYSTEM_SETTINGS_KEY_LOCALE_TIMEZONE:
            break;

        case SYSTEM_SETTINGS_KEY_TIME_CHANGED:
            break;

        default:
            break;
    }
}
