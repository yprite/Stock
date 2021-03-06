/*
 * SaCompanyListViewController.cpp
 *
 *  Created on: Sep 6, 2017
 *      Author: devbin
 */

#include "SaCompanyListViewController.h"
#include "GLTitleItem.h"
#include "GLPaddingItem.h"
#include "SaCompanyListCompanyItem.h"
#include "SaCompanyListSearchItem.h"

#include "SaCompanyInfo.h"
#include "SaCompanyDBManager.h"
#include "SaSearchCompanyViewController.h"
#include "WNaviframeController.h"
#include "RotaryManager.h"
#include "SaDebug.h"

using namespace app_assist;

SaCompanyListViewController::SaCompanyListViewController()
{
    // TODO Auto-generated constructor stub
    _listObj = nullptr;
}

SaCompanyListViewController::~SaCompanyListViewController()
{
    // TODO Auto-generated destructor stub
}

Evas_Object* SaCompanyListViewController::onCreateView(Evas_Object* parent, void* viewParam)
{
    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_theme_set(layout, "layout", "application", "default");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);

    return layout;
}

void SaCompanyListViewController::onCreated()
{
    WENTER();
    Evas_Object *layout = getEvasObject();

    _listObj = new GenlistObject();
    _listObj->create(layout, nullptr);
    elm_object_part_content_set(layout, "elm.swallow.content", _listObj->getEvasObject());

    Evas_Object *circleGenlist = eext_circle_object_genlist_add(_listObj->getEvasObject(), RotaryManager::getInstance()->getCircleSurface());
    RotaryManager::getInstance()->setOnRotary(circleGenlist, ROTARY_MANAGER_DEFAULT_ROTARY_HANDLER);

    const std::vector<SaCompanyInfo>& allCompanyList = SaCompanyDBManager::getInstance()->getAllComanyList();

    GLPaddingItem *titlePaddingItem = new GLPaddingItem();
    _listObj->addPaddingItem(titlePaddingItem);

    SaCompanyListCompanyItem *item = new SaCompanyListCompanyItem(allCompanyList[0]);
    _listObj->addItem(item,
        [](void *data, Evas_Object *obj, void *eventInfo)
        {
            auto it = (Elm_Object_Item *)eventInfo;
            if (!it)
                return;
            elm_genlist_item_selected_set(it, EINA_FALSE);

            auto self = (SaCompanyListViewController *)data;
            auto navi = (WNaviframeController *)(self->getWindowController()->getBaseViewController());
            auto searchView = new SaSearchCompanyViewController();
            navi->push(searchView);
        }, this);
#if 0
    GLTitleItem *titleItem = new GLTitleItem("title");
    _listObj->addTitleItem(titleItem,
        [](void *data, Evas_Object *obj, void *eventInfo)
        {
            auto it = (Elm_Object_Item *)eventInfo;
            if (!it)
                return;
            elm_genlist_item_selected_set(it, EINA_FALSE);

            auto self = (SaCompanyListViewController *)data;
            auto navi = (WNaviframeController *)(self->getWindowController()->getBaseViewController());
            auto searchView = new SaSearchCompanyViewController();
            navi->push(searchView);
        }, this);
#endif
    for (size_t i = 0; i < allCompanyList.size(); ++i)
    {
        SaCompanyListCompanyItem *item = new SaCompanyListCompanyItem(allCompanyList[i]);
        _listObj->addItem(item);
    }
    GLPaddingItem *bottomPaddingItem = new GLPaddingItem();
    _listObj->addPaddingItem(bottomPaddingItem);
}

void SaCompanyListViewController::onPushed(Elm_Object_Item* naviItem)
{
    elm_naviframe_item_title_enabled_set(naviItem, EINA_FALSE, EINA_FALSE);
}

void SaCompanyListViewController::onDestroy()
{

}

bool SaCompanyListViewController::onPop()
{
    return true;
}

void SaCompanyListViewController::onBecomeTop()
{

}

