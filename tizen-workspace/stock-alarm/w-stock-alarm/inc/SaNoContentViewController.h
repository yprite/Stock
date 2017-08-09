/*
 * SaNoContentViewController.h
 *
 *  Created on: Jul 24, 2017
 *      Author: devbin
 */

#ifndef _SA_NO_CONTENT_VIEW_CONTROLLER_H_
#define _SA_NO_CONTENT_VIEW_CONTROLLER_H_

#include "WContentViewController.h"

class SaNoContentViewController: public app_assist::WContentViewController
{
    public:
        SaNoContentViewController();
        virtual ~SaNoContentViewController();

    private:
        virtual Evas_Object* onCreateView(Evas_Object* parent, void* viewParam) override;
        virtual void onCreated() override;
        virtual void onPushed(Elm_Object_Item* naviItem) override;
        virtual void onDestroy() override;
        virtual bool onPop() override;
        virtual void onBecomeTop() override;
};

#endif /* _SA_NO_CONTENT_VIEW_CONTROLLER_H_ */
