/*
 * SaWidgetNoFavoriteViewController.h
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#ifndef SAWIDGETNOFAVORITEVIEWCONTROLLER_H_
#define SAWIDGETNOFAVORITEVIEWCONTROLLER_H_

#include "WViewController.h"
#include <app_control.h>
#include <functional>
#include <string>

class SaWidgetNoFavoriteViewController : public app_assist::WViewController
{
public:
    SaWidgetNoFavoriteViewController();
    virtual ~SaWidgetNoFavoriteViewController();

    void setOnAppControlReplyCb(const std::function<void(const std::string& code, bool result)>& replyCb);
private:
    virtual Evas_Object* onCreateView(Evas_Object *parent, void *viewParam);
    virtual void onCreated();
    virtual void onDestroy();

    static void _onAppControlReply(app_control_h request, app_control_h reply, app_control_result_e result, void *user_data);

private:
    std::function<void(const std::string& code, bool result)> _replyCb;
};

#endif /* SAWIDGETNOFAVORITEVIEWCONTROLLER_H_ */
