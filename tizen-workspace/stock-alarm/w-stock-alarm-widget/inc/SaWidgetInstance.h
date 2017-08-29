/*
 * SaWidgetInstance.h
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#ifndef SAWIDGETINSTANCE_H_
#define SAWIDGETINSTANCE_H_

#include "WWidgetInstance.h"
#include "SaWidgetMainViewController.h"

class SaWidgetInstance : public app_assist::WWidgetInstance
{
public:
    SaWidgetInstance();
    virtual ~SaWidgetInstance();

private:
    virtual bool onCreate(widget_context_h context, bundle* content, int width, int height) override;
    virtual bool onDestroy(widget_context_h context, widget_app_destroy_type_e reason, bundle* content) override;
    virtual bool onPause(widget_context_h context) override;
    virtual bool onResume(widget_context_h context) override;
    virtual bool onUpdate(widget_context_h context, bundle* content, bool isForced) override;
    virtual bool onResize(widget_context_h context, int width, int height) override;

private:
    SaWidgetMainViewController *_baseView;
};

#endif /* SAWIDGETINSTANCE_H_ */
