/*
 * SaNoContentViewController.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: devbin
 */

#include "SaNoContentViewController.h"
#include "SaProgressObject.h"
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
    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_theme_set(layout, "layout", "nocontents", "default");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);

    elm_object_part_text_set(layout, "elm.text.title", "title");
    elm_object_part_text_set(layout, "elm.text", "text");
    //elm_object_part_content_set(layout, "elm.swallow.icon", iconObj);

//    Evas_Object *effectBtn = elm_button_add(layout);
//    elm_object_style_set(effectBtn, "effect/full");
//    evas_object_size_hint_weight_set(effectBtn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//    evas_object_show(effectBtn);
//    evas_object_smart_callback_add(effectBtn, "clicked",
//            [](void *data, Evas_Object *obj, void *eventInfo)
//            {
//                WHIT();
//            }, this);
//
//    elm_object_part_content_set(layout, "elm.swallow.content", effectBtn);

    //elm_object_part_content_set(layout, "elm.swallow.content", content);
    return layout;
}

void SaNoContentViewController::onCreated()
{
    SaProgressObject *progressObj = new SaProgressObject();
    progressObj->create(getEvasObject(), nullptr);
    evas_object_size_hint_weight_set(progressObj->getEvasObject(), EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(progressObj->getEvasObject());
    elm_object_part_content_set(getEvasObject(), "elm.swallow.content", progressObj->getEvasObject());
    progressObj->run();
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
