/*
 * SaWidgetResumeEffectViewController.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: devbin
 */

#include "SaWidgetResumeEffectViewController.h"
#include "SaWidgetDebug.h"

#include "WTimer.h"

#include <app.h>
#include <Elementary.h>

using namespace app_assist;

SaWidgetResumeEffectViewController::SaWidgetResumeEffectViewController(SaWidgetResumeEffectViewController::ViewType viewType)
{
    // TODO Auto-generated constructor stub
    _bgImg = nullptr;
    _icon = nullptr;
    _viewType = viewType;
}

SaWidgetResumeEffectViewController::~SaWidgetResumeEffectViewController()
{
    // TODO Auto-generated destructor stub
}

// 초록색 : rgba(169, 224, 84, 255)
// 빨간색 : rgba(215, 54, 122, 255)
Evas_Object* SaWidgetResumeEffectViewController::onCreateView(Evas_Object *parent, void *viewParam)
{
    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = {0, };
    char bgImgPath[PATH_MAX] = {0, };
    char iconPath[PATH_MAX] = {0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaWidgetResumeEffectView.edj");
        snprintf(bgImgPath, sizeof(bgImgPath), "%s%s", resPath, "images/resume_effect_bg.png");
        snprintf(iconPath, sizeof(iconPath), "%s%s", resPath, "edje/SaWidgetIcon.edj");
        free(resPath);
    }
    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, edjPath, "SaWidgetResumeEffectView");
    evas_object_show(layout);

    Evas_Object *bgImg = elm_image_add(layout);
    elm_image_file_set(bgImg, bgImgPath, nullptr);
    elm_object_part_content_set(layout, "sw.img.bg", bgImg);
    evas_object_show(bgImg);
    if (_viewType == ViewType::PLUS)
        evas_object_color_set(bgImg, 169, 224, 84, 255);
    else
        evas_object_color_set(bgImg, 215, 54, 122, 255);
    _bgImg = bgImg;

    Evas_Object *icon = elm_layout_add(layout);
    if (_viewType == ViewType::PLUS)
        elm_layout_file_set(icon, iconPath, "SaWidgetIcon/Up_Big");
    else
        elm_layout_file_set(icon, iconPath, "SaWidgetIcon/Down_Big");
    elm_object_part_content_set(layout, "sw.icon", icon);
    evas_object_show(icon);
    _icon = icon;
    return layout;
}

void SaWidgetResumeEffectViewController::onCreated()
{
    Evas_Object *layout = getEvasObject();

    edje_object_signal_callback_add(elm_layout_edje_get(layout), "loaded", "*",
            [](void *data, Evas_Object *obj, const char *emission, const char *source)
            {
                WHIT();
                auto self = (SaWidgetResumeEffectViewController *)data;
                elm_layout_signal_emit(self->_icon, "show.anim", "*");
            }, this);

    //elm_object_part_content_set(layout, "sw.icon", )
}

void SaWidgetResumeEffectViewController::onDestroy()
{

}

void SaWidgetResumeEffectViewController::removeIcon()
{
    Evas_Object *layout = getEvasObject();
    if (_icon)
    {
        elm_object_part_content_unset(layout, "sw.icon");
        evas_object_del(_icon);
        _icon = nullptr;
    }
}
