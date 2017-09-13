/*
 * SaListTitleItem.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: devbin
 */

#include "SaListTitleItem.h"
#include "SaDebug.h"
#include <app.h>
#include <Elementary.h>

SaListTitleItem::SaListTitleItem()
{
    // TODO Auto-generated constructor stub
    _refreshButtonClickedCb = nullptr;
    _addButtonClickedCb = nullptr;
}

SaListTitleItem::~SaListTitleItem()
{
    // TODO Auto-generated destructor stub
}

Elm_Genlist_Item_Class* SaListTitleItem::getItemClassStatic() const
{
    static Elm_Genlist_Item_Class itcNormal =
    { };
    itcNormal.item_style = "stock-alarm/title.2icon";
    itcNormal.func.content_get = _glContentCb;
    itcNormal.func.del = WGENLIST_ITEM_DEL();

    return &itcNormal;
}

WGenlistItemEx::ItemType SaListTitleItem::getItemType() const
{
    return WGenlistItemEx::ItemType::TITLE;
}

void SaListTitleItem::setOnRefreshButtonClicked(const std::function<void(void)>& refreshButtonClickedCb)
{
    _refreshButtonClickedCb = refreshButtonClickedCb;
}

void SaListTitleItem::setOnAddButtonClicked(const std::function<void(void)>& addButtonClickedCb)
{
    _addButtonClickedCb = addButtonClickedCb;
}

Evas_Object* SaListTitleItem::_glContentCb(void *data, Evas_Object *obj, const char *part)
{
    auto self = (SaListTitleItem *) data;
    if (!strcmp(part, "elm.icon.1"))
    {
        char *resPath = app_get_resource_path();
        char imgPath[PATH_MAX] = { 0, };
        if (resPath)
        {
            snprintf(imgPath, sizeof(imgPath), "%s%s", resPath, "images/gl_title_refresh.png");
            free(resPath);
        }
        Evas_Object *image = elm_image_add(obj);
        elm_image_file_set(image, imgPath, nullptr);
        evas_object_size_hint_min_set(image, 76, 76);
        evas_object_size_hint_max_set(image, 76, 76);
        evas_object_show(image);

        return image;
    }
    else if (!strcmp(part, "elm.icon.2"))
    {
        char *resPath = app_get_resource_path();
        char imgPath[PATH_MAX] = { 0, };
        if (resPath)
        {
            snprintf(imgPath, sizeof(imgPath), "%s%s", resPath, "images/gl_title_add.png");
            free(resPath);
        }
        Evas_Object *image = elm_image_add(obj);
        elm_image_file_set(image, imgPath, nullptr);
        evas_object_size_hint_min_set(image, 76, 76);
        evas_object_size_hint_max_set(image, 76, 76);
        evas_object_show(image);

        return image;
    }
    else if (!strcmp(part, "sw.icon.1.effect"))
    {
        WDEBUG("part : %s", part);
        Evas_Object *effectBtn = elm_button_add(obj);
        elm_object_style_set(effectBtn, "effect/full");
        evas_object_size_hint_weight_set(effectBtn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_show(effectBtn);
        evas_object_smart_callback_add(effectBtn, "clicked",
                [](void *data, Evas_Object *obj, void *eventInfo)
                {
                    WHIT();
                    auto self = (SaListTitleItem *) data;
                    elm_genlist_item_bring_in(self->getElmObjectItem(), ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
                    if (self->_refreshButtonClickedCb)
                        self->_refreshButtonClickedCb();
                }, data);
        return effectBtn;
    }
    else if (!strcmp(part, "sw.icon.2.effect"))
    {
        WDEBUG("part : %s", part);
        Evas_Object *effectBtn = elm_button_add(obj);
        elm_object_style_set(effectBtn, "effect/full");
        evas_object_size_hint_weight_set(effectBtn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_show(effectBtn);
        evas_object_smart_callback_add(effectBtn, "clicked",
                [](void *data, Evas_Object *obj, void *eventInfo)
                {
                    WHIT();
                    auto self = (SaListTitleItem *) data;
                    elm_genlist_item_bring_in(self->getElmObjectItem(), ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
                    if (self->_addButtonClickedCb)
                        self->_addButtonClickedCb();
                }, data);
        return effectBtn;
    }
    return nullptr;
}

