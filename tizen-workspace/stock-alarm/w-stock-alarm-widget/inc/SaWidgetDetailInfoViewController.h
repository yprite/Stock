/*
 * SaWidgetDetailInfoViewController.h
 *
 *  Created on: Sep 12, 2017
 *      Author: devbin
 */

#ifndef SAWIDGETDETAILINFOVIEWCONTROLLER_H_
#define SAWIDGETDETAILINFOVIEWCONTROLLER_H_

#include "WViewController.h"
#include "SaCompanyInfo.h"

class SaWidgetDetailInfoViewController : public app_assist::WViewController
{
public:
    SaWidgetDetailInfoViewController(const SaCompanyInfo& companyInfo);
    virtual ~SaWidgetDetailInfoViewController();

    void updateInfo(const SaCompanyInfo& companyInfo);

private:
    virtual Evas_Object* onCreateView(Evas_Object *parent, void *viewParam) override;
    virtual void onCreated() override;
    virtual void onDestroy() override;

private:
    SaCompanyInfo _companyInfo;

    Evas_Object *_maxTextObj;
    Evas_Object *_minTextObj;
    Evas_Object *_volumeTextObj;
    Evas_Object *_previousTextObj;
};

#endif /* SAWIDGETDETAILINFOVIEWCONTROLLER_H_ */
