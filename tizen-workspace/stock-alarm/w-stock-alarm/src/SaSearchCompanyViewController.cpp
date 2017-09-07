/*
 * SaSearchCompanyViewController.cpp
 *
 *  Created on: Sep 5, 2017
 *      Author: devbin
 */

#include "SaSearchCompanyViewController.h"
#include "RotaryManager.h"
#include "SaCompanyInfo.h"
#include "SaCompanyDBManager.h"
#include "Popup.h"
#include "SaDebug.h"
#include <efl_extension.h>
#include <app.h>
#include <Elementary.h>

SaSearchCompanyViewController::SaSearchCompanyViewController()
{
    // TODO Auto-generated constructor stub
    _entry = nullptr;
    _isUnregistered = false;
    _isPopupCreated = false;

    _textSearchedCb = nullptr;
}

SaSearchCompanyViewController::~SaSearchCompanyViewController()
{
    // TODO Auto-generated destructor stub
}

Evas_Object* SaSearchCompanyViewController::onCreateView(Evas_Object *parent, void *viewParam)
{
    char edjPath[PATH_MAX] = {0, };
    char *resPath = app_get_resource_path();

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaSearchCompanyView.edj");
        free(resPath);
    }

    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, edjPath, "SaSearchCompanyView");
    //elm_layout_theme_set(layout, "layout", "application", "default");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);

    return layout;
}

void SaSearchCompanyViewController::onCreated()
{
    Evas_Object *layout = getEvasObject();
    Evas_Object *entry = elm_entry_add(layout);
    evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);

    eext_entry_selection_back_event_allow_set(entry, EINA_TRUE);

    elm_entry_context_menu_disabled_set(entry, EINA_TRUE);
    elm_entry_select_allow_set(entry, EINA_FALSE);
    elm_entry_single_line_set(entry, EINA_TRUE);
    elm_entry_scrollable_set(entry, EINA_TRUE);
    elm_entry_prediction_allow_set(entry, EINA_FALSE);
    elm_entry_input_panel_return_key_type_set(entry, ELM_INPUT_PANEL_RETURN_KEY_TYPE_SEARCH);

    evas_object_show(entry);
    _entry = entry;
    _registerEntryCallback();
    elm_object_part_content_set(layout, "elm.swallow.content", entry);
}

void SaSearchCompanyViewController::onPushed(Elm_Object_Item *naviItem)
{
    RotaryManager::getInstance()->inactive();
    elm_naviframe_item_title_enabled_set(naviItem, EINA_FALSE, EINA_FALSE);
}

bool SaSearchCompanyViewController::onPop()
{
    WENTER();
    _unregisterEntryCallback();
    return true;
}

void SaSearchCompanyViewController::onBecomeTop()
{

}

void SaSearchCompanyViewController::onDestroy()
{
    WENTER();
    _unregisterEntryCallback();
}

void SaSearchCompanyViewController::setOnTextSearched(const std::function<void(const std::string& str)>& textSearchedCb)
{
    _textSearchedCb = textSearchedCb;
}
/*
Elm_Object_Item* SaSearchCompanyViewController::onPushToNaviframe(Evas_Object* naviFrame)
{

}
*/

void SaSearchCompanyViewController::_registerEntryCallback()
{
    evas_object_smart_callback_add(_entry, "changed", _onEntryChanged, this);
    evas_object_smart_callback_add(_entry, "activated", _onEntryActivated, this);

    Ecore_IMF_Context *imfContext = (Ecore_IMF_Context *)elm_entry_imf_context_get(_entry);
    ecore_imf_context_input_panel_event_callback_add(imfContext, ECORE_IMF_INPUT_PANEL_STATE_EVENT,
        _onInputPanelEventShowAndHide, this);
    evas_object_smart_callback_add(getWindowController()->getConformantEvasObject(), "virtualkeypad,size,changed",
        _onKeypadResized, this);
}

void SaSearchCompanyViewController::_unregisterEntryCallback()
{
    WENTER();
    if (!_isUnregistered)
    {
        evas_object_smart_callback_del(getWindowController()->getConformantEvasObject(),
            "virtualkeypad,size,changed", _onKeypadResized);
        evas_object_smart_callback_del(_entry, "changed", _onEntryChanged);
        evas_object_smart_callback_del(_entry, "activated", _onEntryActivated);
        //elm_entry_markup_filter_remove(_entry, _onEntryMarkupFilterAppended, nullptr);
        Ecore_IMF_Context *imfContext = (Ecore_IMF_Context *) elm_entry_imf_context_get(_entry);
        ecore_imf_context_input_panel_event_callback_del(imfContext, ECORE_IMF_INPUT_PANEL_STATE_EVENT, _onInputPanelEventShowAndHide);

        RotaryManager::getInstance()->active();
        _isUnregistered = true;
    }
}

void SaSearchCompanyViewController::_onInputPanelEventShowAndHide(void *data, Ecore_IMF_Context *imfContext, int value)
{
    WENTER();
    WPRET_M(!data, "data is nullptr!");
    WPRET_M(!imfContext, "imfContext is nullptr!");

    auto self = (SaSearchCompanyViewController *)data;
    if (self->_isPopupCreated)
    {
        self->_isPopupCreated = false;
        return;
    }

    switch (value)
    {
        case ECORE_IMF_INPUT_PANEL_STATE_HIDE:

            self->popOut();
            WINFO("ECORE_IMF_INPUT_PANEL_STATE_HIDE");
            break;
        case ECORE_IMF_INPUT_PANEL_STATE_WILL_SHOW:
            WINFO("ECORE_IMF_INPUT_PANEL_WILL_SHOW");
            break;
        case ECORE_IMF_INPUT_PANEL_STATE_SHOW:
            break;
        default:
            break;
    }
}

void SaSearchCompanyViewController::_onKeypadResized(void *data, Evas_Object *obj, void *eventInfo)
{
    WENTER();
}

void SaSearchCompanyViewController::_onEntryMarkupFilterAppended(void *data, Evas_Object *entry, char **text)
{
    WENTER();
    char *str = nullptr;
    str = elm_entry_markup_to_utf8(*text);
    WINFO("str : %s", str);

    if (str)
    {
        free(str);
        str = nullptr;
    }
}

void SaSearchCompanyViewController::_onEntryChanged(void *data, Evas_Object *obj, void *eventInfo)
{
    WENTER();
    WPRET_M(!data, "data is nullptr!");
    WPRET_M(!obj, "obj is nullptr!");
}

void SaSearchCompanyViewController::_onEntryActivated(void *data, Evas_Object *obj, void *eventInfo)
{
    WENTER();
    WPRET_M(!data, "data is nullptr!");
    WPRET_M(!obj, "obj is nullptr!");

    auto self = (SaSearchCompanyViewController *)data;
    char *entryStr = elm_entry_markup_to_utf8(elm_entry_entry_get(self->_entry));
    std::string str;

    if (entryStr)
    {
        str = std::string(entryStr);
        free(entryStr);
    }

    WPRET_M(str.empty(), "entry string is empty!");
    WINFO("str : %s", str.c_str());

    // check str existing.
    SaCompanyDBManager::getInstance()->search(str);
    const std::vector<SaCompanyInfo>& searchedResultList = SaCompanyDBManager::getInstance()->getSearchResultList();
    if (searchedResultList.empty())
    {
        // show result popup.
        WINFO("show popup!");

        Evas_Object *toastPopup = Popup::createToastCirclePopup(self->getWindowController()->getEvasObject(), "Cannot Find Name");
        evas_object_show(toastPopup);
        self->_isPopupCreated = true;
        elm_entry_entry_set(self->_entry, "");
    }
    else
    {
        // update company list.
        WINFO("popout!");

        if (self->_textSearchedCb)
            self->_textSearchedCb(str);
        self->popOut();
    }
}

