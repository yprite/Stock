/*
 * SaWidgetMainViewController.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#include "SaWidgetMainViewController.h"
#include "SaWidgetStockInfoViewController.h"
#include "SaWidgetDebug.h"

#include <app.h>
#include <Elementary.h>

SaWidgetMainViewController::SaWidgetMainViewController()
{
    // TODO Auto-generated constructor stub
    _isAnimatorRunning = false;
    _animationStartTime = 0.0f;
}

SaWidgetMainViewController::~SaWidgetMainViewController()
{
    // TODO Auto-generated destructor stub
}

Evas_Object* SaWidgetMainViewController::onCreateView(Evas_Object *parent, void *viewParam)
{
    WENTER();

    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = {0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaWidgetMainView.edj");
        free(resPath);
    }

    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, edjPath, "SaWidgetMainView");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);

    return layout;
}

void SaWidgetMainViewController::onCreated()
{
    Evas_Object *layout = getEvasObject();

    auto viewController = new SaWidgetStockInfoViewController();
    viewController->create(layout, nullptr);

    elm_object_part_content_set(layout, "sw.content", viewController->getEvasObject());
}

void SaWidgetMainViewController::onDestroy()
{

}




