/*
 * SaCompanyListViewController.h
 *
 *  Created on: Sep 6, 2017
 *      Author: devbin
 */

#ifndef SACOMPANYLISTVIEWCONTROLLER_H_
#define SACOMPANYLISTVIEWCONTROLLER_H_

#include "WContentViewController.h"
#include "GenlistObject.h"

class SaCompanyListViewController : public app_assist::WContentViewController
{
public:
    SaCompanyListViewController();
    virtual ~SaCompanyListViewController();

private:
    virtual Evas_Object* onCreateView(Evas_Object* parent, void* viewParam) override;
    virtual void onCreated() override;
    virtual void onPushed(Elm_Object_Item* naviItem) override;
    virtual void onDestroy() override;
    virtual bool onPop() override;
    virtual void onBecomeTop() override;

private:
    GenlistObject* _listObj;
};

#endif /* SACOMPANYLISTVIEWCONTROLLER_H_ */
