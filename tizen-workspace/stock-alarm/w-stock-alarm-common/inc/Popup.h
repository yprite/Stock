/*
 * Popup.h
 *
 *  Created on: Sep 6, 2017
 *      Author: devbin
 */

#ifndef POPUP_H_
#define POPUP_H_

#include "WDefine.h"
//#include "WTimer.h"
#include <string>
#include <functional>
#include <Elementary.h>

class WAPP_ASSIST_EXPORT Popup
{
public:
    //Popup();
    //virtual ~Popup();

    static Evas_Object* createPopup(Evas_Object *parent, Evas_Object *contentLayout);
    static Evas_Object* createToastPopup(Evas_Object *parent, const char *text, const char *imgPath);
    static Evas_Object* createToastCirclePopup(Evas_Object *parent, const char *text);

    void setDissmissCb(const std::function<void(void)>& dismissCb);
    void setHideCb(const std::function<void(void)>& hideCb);
    void setTimeOutCb(const std::function<void(void)>& timeOutCb);

private:
    static void _onPopupHidden(void *data, Evas_Object *obj, void *eventInfo);
    static void _onPopupDismissed(void *data, Evas_Object *obj, void *eventInfo);
    static void _onPopupTimeOut(void *data, Evas_Object *obj, void *eventInfo);

private:
    std::function<void(void)> _dismissCb;
    std::function<void(void)> _hideCb;
    std::function<void(void)> _timeOutCb;
    //app_assist::WTimerWeakPtr _timeOutTimer;
};

#endif /* POPUP_H_ */
