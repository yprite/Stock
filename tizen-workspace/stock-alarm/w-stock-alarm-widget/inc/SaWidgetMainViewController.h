/*
 * SaWidgetMainViewController.h
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#ifndef SAWIDGETMAINVIEWCONTROLLER_H_
#define SAWIDGETMAINVIEWCONTROLLER_H_

#include "WViewController.h"
#include "WTimer.h"
#include <widget_app.h>
#include "SaWidgetStockInfoViewController.h"

class SaWidgetMainViewController : public app_assist::WViewController
{
public:
    SaWidgetMainViewController();
    virtual ~SaWidgetMainViewController();

    void pause();
    void resume();

private:
    virtual Evas_Object* onCreateView(Evas_Object *parent, void *viewParam) override;
    virtual void onCreated() override;
    virtual void onDestroy() override;

private:
    app_assist::WTimerWeakPtr _animator;
    bool _isAnimatorRunning;
    double _animationStartTime;
    widget_context_h _context;

    SaWidgetStockInfoViewController *_stockInfoViewController;
};

#endif /* SAWIDGETMAINVIEWCONTROLLER_H_ */
