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
#include "WViewController.h"
#include "SaMessageEventListener.h"

class SaWidgetInstance : public app_assist::WWidgetInstance
                       , public SaMessageEventListener
{
    enum class ViewType
    {
        NO_FAVORITE,
        COMPANY_INFO,
        NONE
    };
    enum class InstanceState
    {
        CREATE,
        RESUME,
        PAUSE,
        CHANGE_NO_FAVORITE_TO_COMPANY_INFO,
        NONE
    };
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

    virtual void onMessageReceived(const std::string& key, const std::string& code, const std::string& result, const std::string& errorMessage) override;

    void _updateContentValue();

private:
    SaWidgetMainViewController *_baseView;
    app_assist::WViewController *_viewController;
    ViewType _viewType;
    InstanceState _instanceState;

    widget_context_h _context;
    bundle *_content;
    std::string _code;
    time_t _lastResumeTime;
};

#endif /* SAWIDGETINSTANCE_H_ */
