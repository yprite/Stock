/*
 * Popup.cpp
 *
 *  Created on: Sep 6, 2017
 *      Author: devbin
 */

#include "Popup.h"
#include "WDebugBase.h"
#include <efl_extension.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "Popup"

/*
Popup::Popup()
{
    // TODO Auto-generated constructor stub
    _dismissCb = nullptr;
    _hideCb = nullptr;
    _timeOutCb = nullptr;
}

Popup::~Popup()
{
    // TODO Auto-generated destructor stub
}
*/

Evas_Object* Popup::createPopup(Evas_Object *parent, Evas_Object *contentLayout)
{
    WASSERT_EX(parent != nullptr, "parent is nullptr!");

    Evas_Object *popup = elm_popup_add(parent);
    elm_object_style_set(popup, "circle");
    eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _onPopupHidden, nullptr);
    evas_object_smart_callback_add(popup, "dismissed", _onPopupDismissed, nullptr);

    if (contentLayout)
        elm_object_content_set(popup, contentLayout);

    evas_object_show(popup);
    return popup;
}

Evas_Object* Popup::createToastPopup(Evas_Object *parent, const char *text, const char *imgPath)
{
    WASSERT_EX(parent != nullptr, "parent is nullptr!");

    Evas_Object *popup = elm_popup_add(parent);
    elm_object_style_set(popup, "toast/circle");
    eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _onPopupHidden, nullptr);
    evas_object_smart_callback_add(popup, "dismissed", _onPopupDismissed, nullptr);
    elm_object_part_text_set(popup, "elm.text", text);

    if (imgPath)
    {
        Evas_Object *image = elm_image_add(popup);
        elm_image_file_set(image, imgPath, nullptr);
        elm_object_part_content_set(popup, "toast,icon", image);
    }

    elm_popup_timeout_set(popup, 2.0);
    evas_object_smart_callback_add(popup, "timeout", _onPopupTimeOut, nullptr);
    evas_object_show(popup);

    return popup;
}

Evas_Object* Popup::createToastCirclePopup(Evas_Object *parent, const char *text)
{
    WASSERT_EX(parent != nullptr, "parent is nullptr!");

    Evas_Object *popup = elm_popup_add(parent);
    elm_object_style_set(popup, "toast/circle/check");
    eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _onPopupHidden, nullptr);
    evas_object_smart_callback_add(popup, "dismissed", _onPopupDismissed, nullptr);
    elm_object_part_text_set(popup, "elm.text", text);

    elm_popup_timeout_set(popup, 2.0);
    evas_object_smart_callback_add(popup, "timeout", _onPopupTimeOut, nullptr);
    evas_object_show(popup);

    return popup;
}

void Popup::setDissmissCb(const std::function<void(void)>& dismissCb)
{
    _dismissCb = dismissCb;
}

void Popup::setHideCb(const std::function<void(void)>& hideCb)
{
    _hideCb = hideCb;
}

void Popup::setTimeOutCb(const std::function<void(void)>& timeOutCb)
{
    _timeOutCb = timeOutCb;
}

void Popup::_onPopupHidden(void *data, Evas_Object *obj, void *eventInfo)
{
    WPRET_M(!obj, "obj is nullptr");
    elm_popup_dismiss(obj);
}

void Popup::_onPopupDismissed(void *data, Evas_Object *obj, void *eventInfo)
{
    WPRET_M(!obj, "obj is nullptr");
    elm_popup_dismiss(obj);
}

void Popup::_onPopupTimeOut(void *data, Evas_Object *obj, void *eventInfo)
{
    WPRET_M(!obj, "obj is nullptr");
    elm_popup_dismiss(obj);
}

