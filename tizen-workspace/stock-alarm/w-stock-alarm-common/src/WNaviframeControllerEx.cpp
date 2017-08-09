/*
 * WNaviframeControllerEx.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: gabin
 */

#include "WNaviframeControllerEx.h"

#include "WWindowController.h"
#include "WViewController.h"

using namespace app_assist;

WNaviframeControllerEx::WNaviframeControllerEx()
    : WNaviframeController()
{
}

WNaviframeControllerEx::~WNaviframeControllerEx()
{
}

WViewController* WNaviframeControllerEx::getTopView()
{
    Elm_Object_Item* item = elm_naviframe_top_item_get(getEvasObject());
    Evas_Object* obj = elm_object_item_part_content_get(item, "elm.swallow.content");
    if (obj)
        return wviewcontroller_get_instance_from_evasobj(obj);
    return nullptr;
}

WViewController* WNaviframeControllerEx::getBottomView()
{
    Elm_Object_Item* item = elm_naviframe_bottom_item_get(getEvasObject());
    Evas_Object* obj = elm_object_item_part_content_get(item, "elm.swallow.content");
    if (obj)
        return wviewcontroller_get_instance_from_evasobj(obj);
    return nullptr;
}

void WNaviframeControllerEx::deleteTop()
{
    Elm_Object_Item* topItem = elm_naviframe_top_item_get(getEvasObject());
    if (topItem)
        elm_object_item_del(topItem);
}

