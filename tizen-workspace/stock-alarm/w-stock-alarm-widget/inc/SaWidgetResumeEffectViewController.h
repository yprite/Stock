/*
 * SaWidgetResumeEffectViewController.h
 *
 *  Created on: Sep 11, 2017
 *      Author: devbin
 */

#ifndef SAWIDGETRESUMEEFFECTVIEWCONTROLLER_H_
#define SAWIDGETRESUMEEFFECTVIEWCONTROLLER_H_

#include "WViewController.h"

class SaWidgetResumeEffectViewController : public app_assist::WViewController
{
public:
    SaWidgetResumeEffectViewController();
    virtual ~SaWidgetResumeEffectViewController();

    void removeIcon();

private:
    virtual Evas_Object* onCreateView(Evas_Object *parent, void *viewParam) override;
    virtual void onCreated() override;
    virtual void onDestroy() override;

private:
    Evas_Object *_bgImg;
    Evas_Object *_icon;
};

#endif /* SAWIDGETRESUMEEFFECTVIEWCONTROLLER_H_ */
