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

SaWidgetResumeEffectViewController::SaWidgetResumeEffectViewController()
{
    // TODO Auto-generated constructor stub
    _bgImg = nullptr;
}

SaWidgetResumeEffectViewController::~SaWidgetResumeEffectViewController()
{
    // TODO Auto-generated destructor stub
}

Evas_Object* SaWidgetResumeEffectViewController::onCreateView(Evas_Object *parent, void *viewParam)
{
    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = {0, };
    char bgImgPath[PATH_MAX] = {0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaWidgetResumeEffectView.edj");
        snprintf(bgImgPath, sizeof(bgImgPath), "%s%s", resPath, "images/resume_effect_bg.png");
        free(resPath);
    }
    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, edjPath, "SaWidgetResumeEffectView");
    evas_object_show(layout);

    Evas_Object *bgImg = elm_image_add(layout);
    elm_image_file_set(bgImg, bgImgPath, nullptr);
    elm_object_part_content_set(layout, "sw.img.bg", bgImg);
    evas_object_show(bgImg);
    evas_object_color_set(bgImg, 255, 0, 0, 255);
    _bgImg = bgImg;

    return layout;
}

void SaWidgetResumeEffectViewController::onCreated()
{
    Evas_Object *layout = getEvasObject();

    edje_object_signal_callback_add(elm_layout_edje_get(layout), "loaded", "*",
            [](void *data, Evas_Object *obj, const char *emission, const char *source)
            {
                WHIT();
            }, this);
}

void SaWidgetResumeEffectViewController::onDestroy()
{

}
