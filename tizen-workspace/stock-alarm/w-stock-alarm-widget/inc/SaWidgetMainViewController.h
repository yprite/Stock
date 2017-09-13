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
#include "SaWidgetStockInfoViewController.h"
#include "SaMessageEventListener.h"
#include "SaWidgetResumeEffectViewController.h"
#include "SaProgressObject.h"
#include <functional>

class SaWidgetMainViewController : public app_assist::WViewController

{
public:
    SaWidgetMainViewController();
    virtual ~SaWidgetMainViewController();

    void showProgress();
    void removeProgress();
    void showResumeEffect();
    void removeResumeEffect();

    void setOnResumeEffectFullyShown(const std::function<void(void)>& effectFullyShownCb);
    void setOnResumeEffectFinished(const std::function<void(void)>& effectFinishedCb);
private:
    virtual Evas_Object* onCreateView(Evas_Object *parent, void *viewParam) override;
    virtual void onCreated() override;
    virtual void onDestroy() override;

private:
    SaWidgetResumeEffectViewController *_resumeEffectViewController;
    SaProgressObject *_progressObject;
    app_assist::WTimerWeakPtr _resumeTimer;

    std::function<void(void)> _effectFinishedCb;
    std::function<void(void)> _effectFullyShownCb;
};

#endif /* SAWIDGETMAINVIEWCONTROLLER_H_ */
