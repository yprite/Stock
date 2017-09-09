/*
 * SaListObject.cpp
 *
 *  Created on: Sep 3, 2017
 *      Author: devbin
 */

#include "SaListObject.h"
#include "RotaryManager.h"
#include "SaListEditEventManager.h"

#include "SaListStockInfoItem.h"

#include "SaDebug.h"

#include <efl_extension.h>
#include <feedback.h>

SaListObject::SaListObject()
{
    // TODO Auto-generated constructor stub
    _focusItem = nullptr;
}

SaListObject::~SaListObject()
{
    // TODO Auto-generated destructor stub
}

void SaListObject::update()
{
    clear();
}

void SaListObject::selectAllItems()
{

}

void SaListObject::deselectAllItems()
{

}

void SaListObject::removeCheckedItems()
{

}

void SaListObject::onCreated()
{
    WENTER();
}

void SaListObject::onDestroy()
{
    WENTER();
}

void SaListObject::onLongpressed(Elm_Object_Item *item)
{
    WENTER();
    if (!item)
        return;

    if (SaListEditEventManager::getInstance()->isEditMode())
        return;

    auto baseItem = (WGenlistItemEx *)(elm_object_item_data_get(item));

    if (baseItem->getItemType() == WGenlistItemEx::ItemType::NORMAL)
    {
        feedback_play(FEEDBACK_PATTERN_HOLD);

        SaListStockInfoItem *stockInfoItem = (SaListStockInfoItem *)elm_object_item_data_get(item);
        //stockInfoItem->setCheck();
        SaListEditEventManager::getInstance()->startEditMode();
    }
}

void SaListObject::onEventOccured(AppEventListener::EventType eventType)
{
    //updateRealizedItems()
}

void SaListObject::onStartEdit()
{
    updateRealizedItems();
/*
    WGenlistItemEx *item = getFirstItem();
    item = getNextItem(item);
    elm_object_item_signal_emit(item->getElmObjectItem(), "disable", "*");
*/
}

void SaListObject::onStopEdit()
{
    updateRealizedItems();
/*
    WGenlistItemEx *item = getFirstItem();
    item = getNextItem(item);
    elm_object_item_signal_emit(item->getElmObjectItem(), "enable", "*");
*/
}

void SaListObject::onUpdateEdit()
{

}

