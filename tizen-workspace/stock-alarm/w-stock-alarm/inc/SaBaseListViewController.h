/*
 * SaBaseListViewController.h
 *
 *  Created on: Jul 24, 2017
 *      Author: devbin
 */

#ifndef SABASELISTVIEWCONTROLLER_H_
#define SABASELISTVIEWCONTROLLER_H_

#include "SaNoContentViewController.h"
#include "SaListViewController.h"
#include "AppEventListener.h"
#include "SaListEditEventListener.h"
#include "WContentViewController.h"
#include "WViewController.h"
#include "SaCompanyInfo.h"
#include <efl_extension.h>
#include <vector>

class SaBaseListViewController : public app_assist::WContentViewController
                               , public AppEventListener
                               , public SaListEditEventListener
{
public:
    enum class ViewType
    {
        NOCONTENT,
        LIST,
        NONE
    };

public:
    SaBaseListViewController();
    virtual ~SaBaseListViewController();

private:
    virtual Evas_Object *onCreateView(Evas_Object *parent, void *viewParam);
    virtual void onCreated();
    virtual void onPushed(Elm_Object_Item *naviItem);
    virtual void onDestroy();
    virtual bool onPop();

    virtual void onEventOccured(AppEventListener::EventType eventType);

    virtual void onStartEdit();
    virtual void onStopEdit();
    virtual void onUpdateEdit();

private:
    void _createPanel();

private:
    static void _onMoreOptionOpened(void *data, Evas_Object *obj, void *eventInfo);
    static void _onMoreOptionClosed(void *data, Evas_Object *obj, void *eventInfo);
    static void _onMoreOptionClicked(void *data, Evas_Object *obj, void *eventInfo);

    static void _onDbDataChanged(const char *key, void *user_data);
private:
    app_assist::WViewController *_viewController;
    Eext_Object_Item *_addMoreOptionItem;
    Eext_Object_Item *_removeMoreOptionItem;
    Eext_Object_Item *_settingMoreOptionItem;
    ViewType _viewType;
};

#endif /* SABASELISTVIEWCONTROLLER_H_ */
