/*
 * SaBaseListViewController.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: devbin
 */

#include "SaBaseListViewController.h"
#include "WNaviframeController.h"
#include "SaListEditEventManager.h"
#include "SaNoContentViewController.h"
#include "SaListViewController.h"
#include "SaCompanyDBManager.h"

#include "RotaryManager.h"

#include <app_preference.h>
//

#include <app.h>
#include <Elementary.h>

using namespace app_assist;

#define MAX_REACHED_COUNT   (10)

#define ADD_TEXT        "Add"
#define REMOVE_TEXT     "Remove"
#define SETTING_TEXT    "Setting"

SaBaseListViewController::SaBaseListViewController()
{
    // TODO Auto-generated constructor stub
    _viewController = nullptr;
    _addMoreOptionItem = nullptr;
    _removeMoreOptionItem= nullptr;
    _settingMoreOptionItem = nullptr;
    _viewType = ViewType::NOCONTENT;
}

SaBaseListViewController::~SaBaseListViewController()
{
    // TODO Auto-generated destructor stub
}

Evas_Object* SaBaseListViewController::onCreateView(Evas_Object *parent, void *viewParam)
{
    WENTER();
    Evas_Object *moreOptionLayout = eext_more_option_add(parent);
    evas_object_smart_callback_add(moreOptionLayout, "more,option,opened", _onMoreOptionOpened, this);
    evas_object_smart_callback_add(moreOptionLayout, "more,option,closed", _onMoreOptionClosed, this);
    evas_object_smart_callback_add(moreOptionLayout, "item,clicked", _onMoreOptionClicked, this);
    elm_layout_signal_emit(moreOptionLayout, "cue,show", "elm");

    evas_object_show(moreOptionLayout);

    return moreOptionLayout;
}

void SaBaseListViewController::onCreated()
{
    WENTER();

    Evas_Object *layout = getEvasObject();
    _createPanel();

    const std::vector<SaCompanyInfo>& savedList = SaCompanyDBManager::getInstance()->getSavedList();

    if (savedList.empty())
    {
        _viewType = ViewType::NOCONTENT;
        auto viewController = new SaNoContentViewController();
        viewController->create(layout, nullptr);
        elm_object_part_content_set(layout, "elm.swallow.content", viewController->getEvasObject());
        _viewController = viewController;

        elm_layout_signal_emit(layout, "cue,hide", "elm");
        eext_more_option_opened_set(layout, EINA_FALSE);
    }
    else
    {
        _viewType = ViewType::LIST;
        auto viewController = new SaListViewController();
        viewController->create(layout, nullptr);
        elm_object_part_content_set(layout, "elm.swallow.content", viewController->getEvasObject());
        _viewController = viewController;

        elm_layout_signal_emit(layout, "cue,show", "elm");
    }

    int preferenceRet = preference_set_changed_cb(SA_COMPANY_DB_SAVED_LIST_CHANDED, _onDbDataChanged, this);

    WPRET_M(preferenceRet != PREFERENCE_ERROR_NONE, "preference_set_changed_cb failed.(%d)", preferenceRet);
}

void SaBaseListViewController::onPushed(Elm_Object_Item *naviItem)
{
    elm_naviframe_item_title_enabled_set(naviItem, EINA_FALSE, EINA_FALSE);
}

void SaBaseListViewController::onDestroy()
{
    preference_unset_changed_cb(SA_COMPANY_DB_SAVED_LIST_CHANDED);
}

bool SaBaseListViewController::onPop()
{
    if (SaListEditEventManager::getInstance()->isEditMode())
    {
        SaListEditEventManager::getInstance()->stopEditMode();
        return false;
    }
    return true;
}

void SaBaseListViewController::onEventOccured(AppEventListener::EventType eventType)
{
    WHIT();
}

void SaBaseListViewController::onStartEdit()
{
    Evas_Object *layout = getEvasObject();
    elm_layout_signal_emit(layout, "cue,hide", "elm");
}

void SaBaseListViewController::onStopEdit()
{
    Evas_Object *layout = getEvasObject();
    elm_layout_signal_emit(layout, "cue,show", "elm");
    eext_more_option_opened_set(layout, EINA_FALSE);
}

void SaBaseListViewController::onUpdateEdit()
{

}

void SaBaseListViewController::_createPanel()
{
    WENTER();
    {
        Eext_Object_Item *item = eext_more_option_item_append(getEvasObject());
        eext_more_option_item_part_text_set(item, "selector,main_text", ADD_TEXT);
        char *resPath = app_get_resource_path();
        char imgPath[PATH_MAX] = { 0, };

        if (resPath)
        {
            snprintf(imgPath, PATH_MAX, "%s%s", resPath, "images/ .png");
            free(resPath);
        }
        Evas_Object *img = elm_image_add(getEvasObject());
        elm_image_file_set(img, imgPath, nullptr);
        eext_more_option_item_part_content_set(item, "item,icon", img);
        //elm_object_style_set(item->obj, "prod/more_option_item");

        _addMoreOptionItem = item;
    }
    {
        Eext_Object_Item *item = eext_more_option_item_append(getEvasObject());
        eext_more_option_item_part_text_set(item, "selector,main_text", REMOVE_TEXT);
        char *resPath = app_get_resource_path();
        char imgPath[PATH_MAX] = { 0, };

        if (resPath)
        {
            snprintf(imgPath, PATH_MAX, "%s%s", resPath, "images/world_clock_more_opt_delete.png");
            free(resPath);
        }
        Evas_Object *img = elm_image_add(getEvasObject());
        elm_image_file_set(img, imgPath, nullptr);
        eext_more_option_item_part_content_set(item, "item,icon", img);
        //elm_object_style_set(item->obj, "prod/more_option_item");

        _removeMoreOptionItem = item;
    }
    {
        Eext_Object_Item *item = eext_more_option_item_append(getEvasObject());
        eext_more_option_item_part_text_set(item, "selector,main_text", SETTING_TEXT);
        char *resPath = app_get_resource_path();
        char imgPath[PATH_MAX] = { 0, };

        if (resPath)
        {
            snprintf(imgPath, PATH_MAX, "%s%s", resPath, "images/world_clock_more_opt_delete.png");
            free(resPath);
        }
        Evas_Object *img = elm_image_add(getEvasObject());
        elm_image_file_set(img, imgPath, nullptr);
        eext_more_option_item_part_content_set(item, "item,icon", img);
        //elm_object_style_set(item->obj, "prod/more_option_item");

        _settingMoreOptionItem = item;
    }

}

void SaBaseListViewController::_onMoreOptionOpened(void *data, Evas_Object *obj, void *eventInfo)
{
    RotaryManager::getInstance()->inactive();
}

void SaBaseListViewController::_onMoreOptionClosed(void *data, Evas_Object *obj, void *eventInfo)
{
    RotaryManager::getInstance()->active();
}

void SaBaseListViewController::_onMoreOptionClicked(void *data, Evas_Object *obj, void *eventInfo)
{
    auto self = (SaBaseListViewController *) data;
    Eext_Object_Item *item = (Eext_Object_Item *)eventInfo;
    if (!item)
        return;

    const char *text = eext_more_option_item_part_text_get(item, "selector,main_text");

    if (!strcmp(text, ADD_TEXT))
    {
        eext_more_option_opened_set(self->getEvasObject(), EINA_FALSE);

        // if maximum size reached, show popup.
        // else, push companyview and search.
    }
    else if (!strcmp(text, REMOVE_TEXT))
    {
        eext_more_option_opened_set(self->getEvasObject(), EINA_FALSE);

        SaListEditEventManager::getInstance()->startEditMode();
        // show select item popup.
    }
    else if (!strcmp(text, SETTING_TEXT))
    {
        eext_more_option_opened_set(self->getEvasObject(), EINA_FALSE);

        // TODO
    }
    else
    {
        eext_more_option_opened_set(self->getEvasObject(), EINA_FALSE);
    }
}

void SaBaseListViewController::_onDbDataChanged(const char *key, void *data)
{
    WENTER();
    auto self = (SaBaseListViewController *)data;
    Evas_Object *layout = self->getEvasObject();
    const std::vector<SaCompanyInfo>& savedList = SaCompanyDBManager::getInstance()->getSavedList();

    if (savedList.empty())
    {
        // show nocontent.
        if (self->_viewType == ViewType::NOCONTENT)
        {
            // to do nothing.
        }
        else
        {
            if (self->_viewController)
            {
                elm_object_part_content_unset(layout, "elm.swallow.content");
                self->_viewController->destroy();
                self->_viewController = nullptr;
            }

            self->_viewType = ViewType::NOCONTENT;
            auto viewController = new SaNoContentViewController();
            viewController->create(layout, nullptr);
            elm_object_part_content_set(layout, "elm.swallow.content", viewController->getEvasObject());
            self->_viewController = viewController;

            elm_layout_signal_emit(layout, "cue,hide", "elm");
            eext_more_option_opened_set(layout, EINA_FALSE);
        }
    }
    else
    {
        if (self->_viewType == ViewType::LIST)
        {
            // to do update.
        }
        else
        {
            if (self->_viewController)
            {
                elm_object_part_content_unset(layout, "elm.swallow.content");
                self->_viewController->destroy();
                self->_viewController = nullptr;
            }

            self->_viewType = ViewType::LIST;
            auto viewController = new SaListViewController();
            viewController->create(layout, nullptr);
            elm_object_part_content_set(layout, "elm.swallow.content", viewController->getEvasObject());
            self->_viewController = viewController;

            elm_layout_signal_emit(layout, "cue,show", "elm");
        }
    }
#if 0
    if (savedList.empty())
    {

        _viewType = ViewType::NOCONTENT;
        auto viewController = new SaNoContentViewController();
        viewController->create(layout, nullptr);
        elm_object_part_content_set(layout, "elm.swallow.content", viewController->getEvasObject());
        _viewController = viewController;

        elm_layout_signal_emit(layout, "cue,hide", "elm");
        eext_more_option_opened_set(layout, EINA_FALSE);
    }
    else
    {
        _viewType = ViewType::LIST;
        auto viewController = new SaListViewController();
        viewController->create(layout, nullptr);
        elm_object_part_content_set(layout, "elm.swallow.content", viewController->getEvasObject());
        _viewController = viewController;

        elm_layout_signal_emit(layout, "cue,show", "elm");
    }
#endif
}
