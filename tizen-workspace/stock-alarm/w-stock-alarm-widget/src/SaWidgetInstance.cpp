/*
 * SaWidgetInstance.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#include "SaWidgetInstance.h"
#include "WWidgetWindowController.h"
#include "SaWidgetMainViewController.h"
#include "SaWidgetDebug.h"

#include <app.h>
#include <Elementary.h>

using namespace app_assist;

SaWidgetInstance::SaWidgetInstance()
{
    // TODO Auto-generated constructor stub
    _baseView = nullptr;
}

SaWidgetInstance::~SaWidgetInstance()
{
    // TODO Auto-generated destructor stub
}

bool SaWidgetInstance::onCreate(widget_context_h context, bundle* content, int width, int height)
{
    WENTER();

    attachWindowController(context, new WWidgetWindowController());

    auto baseView = new SaWidgetMainViewController();
    getWindowController()->attachBaseViewController(baseView);

    _baseView = baseView;

    return true;
}

bool SaWidgetInstance::onDestroy(widget_context_h context, widget_app_destroy_type_e reason, bundle* content)
{
    WENTER();

    return true;
}

bool SaWidgetInstance::onPause(widget_context_h context)
{
    WENTER();
    return true;
}

bool SaWidgetInstance::onResume(widget_context_h context)
{
    WENTER();

    return true;
}

bool SaWidgetInstance::onUpdate(widget_context_h context, bundle* content, bool isForced)
{
    return true;
}

bool SaWidgetInstance::onResize(widget_context_h context, int width, int height)
{
    return true;
}

