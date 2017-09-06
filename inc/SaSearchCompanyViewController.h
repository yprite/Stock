/*
 * SaSearchCompanyViewController.h
 *
 *  Created on: Sep 5, 2017
 *      Author: devbin
 */

#ifndef SASEARCHCOMPANYVIEWCONTROLLER_H_
#define SASEARCHCOMPANYVIEWCONTROLLER_H_

#include "WContentViewController.h"
#include <functional>

class SaSearchCompanyViewController : public app_assist::WContentViewController
{
public:
    SaSearchCompanyViewController();
    virtual ~SaSearchCompanyViewController();

private:
    virtual Evas_Object* onCreateView(Evas_Object *parent, void *viewParam);
    virtual void onCreated();
    virtual void onPushed(Elm_Object_Item *naviItem);
    virtual bool onPop();
    virtual void onBecomeTop();
    virtual void onDestroy();
    //virtual Elm_Object_Item* onPushToNaviframe(Evas_Object* naviFrame);

private:
    void _registerEntryCallback();
    void _unregisterEntryCallback();

private:
    static void _onInputPanelEventShowAndHide(void *data, Ecore_IMF_Context *imfContext, int value);
    static void _onKeypadResized(void *data, Evas_Object *obj, void *eventInfo);
    static void _onEntryMarkupFilterAppended(void *data, Evas_Object *entry, char **text);
    static void _onEntryChanged(void *data, Evas_Object *obj, void *eventInfo);
    static void _onEntryActivated(void *data, Evas_Object *obj, void *eventInfo);

private:
    Evas_Object *_entry;
    bool _isUnregistered;
    bool _isPopupCreated;
};

#endif /* SASEARCHCOMPANYVIEWCONTROLLER_H_ */
