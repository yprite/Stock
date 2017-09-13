/*
 * SaListViewController.h
 *
 *  Created on: Jul 24, 2017
 *      Author: devbin
 */

#ifndef SALISTVIEWCONTROLLER_H_
#define SALISTVIEWCONTROLLER_H_

#include "WViewController.h"
#include "SaListEditEventListener.h"
#include "AppEventListener.h"
#include "SaListObject.h"
#include "WEvasObject.h"

class SaListViewController : public app_assist::WViewController
                           , public SaListEditEventListener
                           , AppEventListener
{
public:
    SaListViewController();
    virtual ~SaListViewController();

public:
    bool isCtxPopupExist() const;
    void removeCtxPopup();
    void updateList();

private:
    virtual Evas_Object* onCreateView(Evas_Object *parent, void *viewParam);
    virtual void onCreated();
    virtual void onDestroy();

    void onEventOccured(AppEventListener::EventType eventType);

    virtual void onStartEdit();
    virtual void onStopEdit();
    virtual void onUpdateEdit();

    void _pushAllCompanyListView();
private:
    static void _onFloatingButtonClicked(void *data, Evas_Object *obj, void *eventInfo);
    static void _onCtxPopupDismissed(void *data, Evas_Object *obj, void *eventInfo);
    static void _onCtxPopupPopBacked(void *data, Evas_Object *obj, void *eventInfo);
    static void _onDeleteButtonClicked(void *data, Evas_Object *obj, void *eventInfo);

    static void _onItemClicked(void *data, Evas_Object *obj, void *eventInfo);

private:
    Evas_Object *_floatingButton;
    Evas_Object *_delButton;
    SaListObject *_listObj;

    app_assist::WEvasObjectPtr _ctxPopup;
};

#endif /* SALISTVIEWCONTROLLER_H_ */
