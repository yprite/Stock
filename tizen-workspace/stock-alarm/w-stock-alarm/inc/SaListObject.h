/*
 * SaListObject.h
 *
 *  Created on: Sep 3, 2017
 *      Author: devbin
 */

#ifndef SALISTOBJECT_H_
#define SALISTOBJECT_H_

#include "GenlistObject.h"
#include "SaListEditEventListener.h"
#include "AppEventListener.h"

class SaListObject : public GenlistObject
                   , public SaListEditEventListener
                   , public AppEventListener
{
public:
    SaListObject();
    virtual ~SaListObject();

public:
    void update();
    void selectAllItems();
    void deselectAllItems();
    void removeCheckedItems();

private:
    virtual void onCreated();
    virtual void onDestroy();

    virtual void onLongpressed(Elm_Object_Item *item);

    void onEventOccured(AppEventListener::EventType eventType);

    virtual void onStartEdit();
    virtual void onStopEdit();
    virtual void onUpdateEdit();

private:
    Elm_Object_Item *_focusItem;
};

#endif /* SALISTOBJECT_H_ */
