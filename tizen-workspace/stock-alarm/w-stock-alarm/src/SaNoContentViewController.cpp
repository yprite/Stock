/*
 * SaNoContentViewController.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: devbin
 */

#include "SaNoContentViewController.h"
#include "SaProgressObject.h"
#include "SaDataConsumer.h"

// test for genlist
#include "RotaryManager.h"
#include "SaListObject.h"
#include "SaListStockInfoItem.h"
#include "GLTitleItem.h"
#include "GLPaddingItem.h"
#include "SaDebug.h"

#include <Elementary.h>
#include <app.h>

SaNoContentViewController::SaNoContentViewController()
{
    // TODO Auto-generated constructor stub

}

SaNoContentViewController::~SaNoContentViewController()
{
    // TODO Auto-generated destructor stub
}

Evas_Object* SaNoContentViewController::onCreateView(Evas_Object* parent, void* viewParam)
{
#if 0
    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_theme_set(layout, "layout", "nocontents", "default");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);

    elm_object_part_text_set(layout, "elm.text.title", "title");
    elm_object_part_text_set(layout, "elm.text", "text");
#endif
    //elm_object_part_content_set(layout, "elm.swallow.icon", iconObj);

    Evas_Object *effectBtn = elm_button_add(layout);
    elm_object_style_set(effectBtn, "effect/full");
    evas_object_size_hint_weight_set(effectBtn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(effectBtn);
    evas_object_smart_callback_add(effectBtn, "clicked",
            [](void *data, Evas_Object *obj, void *eventInfo)
            {
                WHIT();
                //SaDataConsumer::getInstance()->requestFinanceQuatesList("APPL");
            }, this);

    elm_object_part_content_set(layout, "elm.swallow.content", effectBtn);

    //elm_object_part_content_set(layout, "elm.swallow.content", content);

    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_theme_set(layout, "layout", "application", "default");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);
    return layout;
}

void SaNoContentViewController::onCreated()
{

    SaProgressObject *progressObj = new SaProgressObject();
    progressObj->create(getEvasObject(), nullptr);
    evas_object_size_hint_weight_set(progressObj->getEvasObject(), 0, 0);
    evas_object_show(progressObj->getEvasObject());
    elm_object_part_content_set(getEvasObject(), "elm.swallow.content", progressObj->getEvasObject());
    //progressObj->run();

    WENTER();
#if 0
    Evas_Object *layout = getEvasObject();
    Evas *e = evas_object_evas_get(layout);

    char targetImgPath[PATH_MAX] = { 0, };
    char clipperImgPath[PATH_MAX] = { 0, };
    char *resPath = app_get_resource_path();

    if (resPath)
    {
        snprintf(targetImgPath, sizeof(targetImgPath), "%s%s", resPath, "images/target.png");
        snprintf(clipperImgPath, sizeof(clipperImgPath), "%s%s", resPath, "images/clipper.png");
        free(resPath);
    }

    Evas_Object *target = evas_object_image_filled_add(e);
    evas_object_image_file_set(target, targetImgPath, nullptr);
    evas_object_size_hint_min_set(target, 360, 360);
    evas_object_size_hint_max_set(target, 360, 360);
    evas_object_resize(target, 360, 360);
    evas_object_show(target);

    Evas_Object *clipper = evas_object_image_filled_add(e);
    evas_object_image_file_set(clipper, clipperImgPath, nullptr);
    evas_object_size_hint_min_set(clipper, 60, 60);
    evas_object_size_hint_max_set(clipper, 60, 60);
    evas_object_resize(clipper, 60, 60);
    evas_object_show(clipper);

    evas_object_clip_set(target, clipper);

    elm_object_part_content_set(layout, "elm.swallow.content", clipper);
#endif


#if 0
    // genlist test.
    Evas_Object *layout = getEvasObject();
    SaListObject *listObj = new SaListObject();
    listObj->create(layout, nullptr);
    elm_object_part_content_set(layout, "elm.swallow.content", listObj->getEvasObject());

    GLTitleItem *titleItem = new GLTitleItem("title");
    listObj->addTitleItem(titleItem);

    for (int i = 0; i < 3; ++i)
    {
        SaListStockInfoItem *item = new SaListStockInfoItem();
        listObj->addItem(item);
    }

    GLPaddingItem *paddingItem = new GLPaddingItem();
    listObj->addPaddingItem(paddingItem);

    Evas_Object *circleObj = eext_circle_object_genlist_add(listObj->getEvasObject(), RotaryManager::getInstance()->getCircleSurface());
    RotaryManager::getInstance()->setOnRotary(circleObj, ROTARY_MANAGER_DEFAULT_ROTARY_HANDLER);
#endif
}

void SaNoContentViewController::onPushed(Elm_Object_Item* naviItem)
{
    elm_naviframe_item_title_enabled_set(naviItem, EINA_FALSE, EINA_FALSE);
}

void SaNoContentViewController::onDestroy()
{

}

bool SaNoContentViewController::onPop()
{
    return true;
}

void SaNoContentViewController::onBecomeTop()
{

}
