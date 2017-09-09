/*
 * SaListViewController.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: devbin
 */

#include "SaListViewController.h"
#include "SaListEditEventManager.h"
#include "RotaryManager.h"
#include <app.h>
#include <Elementary.h>

using namespace app_assist;

SaListViewController::SaListViewController()
{
    // TODO Auto-generated constructor stub
    _floatingButton = nullptr;
    _delButton = nullptr;
    _listObj = nullptr;

}

SaListViewController::~SaListViewController()
{
    // TODO Auto-generated destructor stub
}

bool SaListViewController::isCtxPopupExist() const
{
    if (auto safeObj = _ctxPopup.lock())
        return true;
    return false;
}

void SaListViewController::removeCtxPopup()
{
    if (auto safeObj = _ctxPopup.lock())
    {
        Evas_Object *obj = safeObj->getEvasObject();
        elm_ctxpopup_dismiss(obj);
        _ctxPopup.reset();
    }
}

void SaListViewController::updateList()
{

}


Evas_Object* SaListViewController::onCreateView(Evas_Object *parent, void *viewParam)
{
    char edjPath[PATH_MAX] = {0, };
    char *resPath = app_get_resource_path();

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaListView.edj");
        free(resPath);
    }

    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, edjPath, "SaListView");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);

    return layout;
}

void SaListViewController::onCreated()
{
    Evas_Object *layout = getEvasObject();

    SaListObject *listObj = new SaListObject();
    listObj->create(layout, nullptr);
    elm_object_part_content_set(layout, "sw.list.obj", listObj->getEvasObject());

    _listObj = listObj;
}
void SaListViewController::onDestroy()
{

}

void SaListViewController::onEventOccured(AppEventListener::EventType eventType)
{

}

void SaListViewController::onStartEdit()
{
    Evas_Object *layout = getEvasObject();

    {
        if (_delButton)
        {
            evas_object_del(_delButton);
            _delButton = nullptr;
        }

        Evas_Object *button = elm_button_add(layout);
        elm_object_style_set(button, "bottom");
        elm_object_text_set(button, "DELETE");

        evas_object_smart_callback_add(button, "clicked", _onDeleteButtonClicked, this);
        elm_object_part_content_set(layout, "sw.del.button", button);
        evas_object_show(button);

        _delButton = button;
    }

    {
        if (_floatingButton)
        {
            evas_object_del(_floatingButton);
            _floatingButton= nullptr;
        }

        Evas_Object *button = elm_button_add(layout);
        elm_object_style_set(button, "select_mode");
        elm_object_text_set(button, "0");
        evas_object_smart_callback_add(button, "clicked", _onFloatingButtonClicked, this);
        elm_object_part_content_set(layout, "sw.floating.button", button);
        evas_object_show(button);

        _floatingButton = button;
    }
}
void SaListViewController::onStopEdit()
{
    if (_delButton)
    {
        evas_object_del(_delButton);
        _delButton = nullptr;
    }
    if (_floatingButton)
    {
        evas_object_del(_floatingButton);
        _floatingButton= nullptr;
    }
}
void SaListViewController::onUpdateEdit()
{
    WENTER();

    if (SaListEditEventManager::getInstance()->getCheckedItemCount() == 0)
    {
        if (_delButton)
            elm_object_disabled_set(_delButton, EINA_TRUE);
    }
    else
    {
        if (_delButton)
            elm_object_disabled_set(_delButton, EINA_FALSE);
    }

    char number[16] = {0, };
    snprintf(number, sizeof(number), "%d", SaListEditEventManager::getInstance()->getCheckedItemCount());
    elm_object_text_set(_floatingButton, number);
}

void SaListViewController::_onFloatingButtonClicked(void *data, Evas_Object *obj, void *eventInfo)
{
    auto self = (SaListViewController *) data;

    Evas_Object *layout = self->getEvasObject();
    Elm_Object_Item *it1 = nullptr;
    Elm_Object_Item *it2 = nullptr;
    Evas_Object *ctxpopup = elm_ctxpopup_add(layout);
    self->_ctxPopup = wevasobject_get_weak_ptr(ctxpopup);
    elm_object_style_set(ctxpopup, "select_mode");

    elm_ctxpopup_direction_priority_set(ctxpopup, ELM_CTXPOPUP_DIRECTION_DOWN, ELM_CTXPOPUP_DIRECTION_DOWN, ELM_CTXPOPUP_DIRECTION_DOWN, ELM_CTXPOPUP_DIRECTION_DOWN);
    eext_object_event_callback_add(ctxpopup, EEXT_CALLBACK_BACK, self->_onCtxPopupPopBacked, self);
    evas_object_smart_callback_add(ctxpopup, "dismissed", self->_onCtxPopupDismissed, self);

    int citylistSize = 10;

    if (SaListEditEventManager::getInstance()->getCheckedItemCount() < citylistSize)
    {
        it1 = elm_ctxpopup_item_append(ctxpopup, "SELECT ALL" , nullptr,
            [](void *data, Evas_Object *obj, void *event_info)
            {
                auto self = (SaListViewController *)data;

                if (auto safeObj = self->_ctxPopup.lock())
                {
                    Evas_Object *obj = safeObj->getEvasObject();
                    elm_ctxpopup_dismiss(obj);
                    self->_ctxPopup.reset();
                }
                //self->_list->selectAllItems();
            }, self);
    }

    if (SaListEditEventManager::getInstance()->getCheckedItemCount() != 0)
    {
        it2 = elm_ctxpopup_item_append(ctxpopup, "DESELECT ALL", nullptr,
            [](void *data, Evas_Object *obj, void *event_info)
            {
                auto self = (SaListViewController *)data;
                if (auto safeObj = self->_ctxPopup.lock())
                {
                    Evas_Object *obj = safeObj->getEvasObject();
                    elm_ctxpopup_dismiss(obj);
                    self->_ctxPopup.reset();
                }
                //self->_list->deselectAllItems();
            }, self);
    }

    evas_object_show(ctxpopup);

    if (it1 && it2)
    {
        elm_object_item_style_set(it1, "select_mode/top");
        elm_object_item_style_set(it2, "select_mode/bottom");

        WERROR("it1 style : %s", elm_object_item_style_get(it1));
        WERROR("it2 style : %s", elm_object_item_style_get(it2));
    }

    Evas_Coord x, y, w, h;
    evas_object_geometry_get(obj, &x, &y, &w, &h);
    evas_object_move(ctxpopup, x + (w / 2), 70);

}

void SaListViewController::_onCtxPopupDismissed(void *data, Evas_Object *obj, void *eventInfo)
{
    if (!obj)
        return;

    evas_object_smart_callback_del(obj, "dismissed", _onCtxPopupDismissed);
    evas_object_del(obj);
}
void SaListViewController::_onCtxPopupPopBacked(void *data, Evas_Object *obj, void *eventInfo)
{
    if (!obj)
        return;

    elm_ctxpopup_dismiss(obj);
}
void SaListViewController::_onDeleteButtonClicked(void *data, Evas_Object *obj, void *eventInfo)
{
    auto self = (SaListViewController *)data;

#if 0
    self->_list->removeCheckedItems();

    const std::vector<Cityinfo>& citylist = WclDBManager::getInstance()->getAllCityList();
    int citylistSize = citylist.size();
    if (citylistSize != 0)
    {
        WclPopup::showCityDeletedPopup(self->getWindowController()->getEvasObject());
    }
#endif
    SaListEditEventManager::getInstance()->stopEditMode();
}


