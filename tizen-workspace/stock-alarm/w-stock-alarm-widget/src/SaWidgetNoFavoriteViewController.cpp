/*
 * SaWidgetNoFavoriteViewController.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#include "SaWidgetNoFavoriteViewController.h"
#include "SaWidgetDebug.h"

#include <Elementary.h>
#include <app.h>
#include <app_control.h>

SaWidgetNoFavoriteViewController::SaWidgetNoFavoriteViewController()
{
    // TODO Auto-generated constructor stub
    _replyCb = nullptr;
}

SaWidgetNoFavoriteViewController::~SaWidgetNoFavoriteViewController()
{
    // TODO Auto-generated destructor stub
}

void SaWidgetNoFavoriteViewController::setOnAppControlReplyCb(const std::function<void(const std::string& code, bool result)>& replyCb)
{
    _replyCb = replyCb;
}

Evas_Object* SaWidgetNoFavoriteViewController::onCreateView(Evas_Object *parent, void *viewParam)
{
    WENTER();
    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_theme_set(layout, "layout", "nocontents", "default");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);

    elm_object_part_text_set(layout, "elm.text.title", "Gear Stock");
    elm_object_part_text_set(layout, "elm.text", "추가");

    char *resPath = app_get_resource_path();
    char imgPath[PATH_MAX] = {0, };
    if (resPath)
    {
        snprintf(imgPath, sizeof(imgPath), "%s%s", resPath, "images/widget_nocontent.png");
        free(resPath);
    }

    Evas_Object *image = elm_image_add(layout);
    elm_image_file_set(image, imgPath, nullptr);
    elm_object_part_content_set(layout, "elm.swallow.icon", image);

    Evas_Object *effectBtn = elm_button_add(layout);
    elm_object_style_set(effectBtn, "effect/full");
    evas_object_size_hint_weight_set(effectBtn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(effectBtn);
    evas_object_smart_callback_add(effectBtn, "clicked",
            [](void *data, Evas_Object *obj, void *eventInfo)
            {
                WHIT();
                //SaDataConsumer::getInstance()->requestFinanceQuatesList("APPL");
                auto self = (SaWidgetNoFavoriteViewController *)data;

                int ret = APP_CONTROL_ERROR_NONE;
                app_control_h service = nullptr;
                ret = app_control_create(&service);
                if (ret != APP_CONTROL_ERROR_NONE)
                    WERROR("app_control_create failed.(%d)", ret);

                ret = app_control_set_app_id(service, "team.tizenia.w-stock-alarm");
                if (ret != APP_CONTROL_ERROR_NONE)
                    WERROR("app_control_set_app_id failed.(%d)", ret);

                ret = app_control_send_launch_request(service, self->_onAppControlReply, self);
                if (ret != APP_CONTROL_ERROR_NONE)
                    WERROR("app_control_send_launch_request failed.(%d)", ret);

                ret = app_control_destroy(service);
                if (ret != APP_CONTROL_ERROR_NONE)
                    WERROR("app_control_destroy failed.(%d)", ret);
            }, this);

    elm_object_part_content_set(layout, "elm.swallow.content", effectBtn);

    return layout;
}

void SaWidgetNoFavoriteViewController::onCreated()
{

}

void SaWidgetNoFavoriteViewController::onDestroy()
{

}

void SaWidgetNoFavoriteViewController::_onAppControlReply(app_control_h request, app_control_h reply, app_control_result_e result, void *user_data)
{
    WENTER();
    auto self = (SaWidgetNoFavoriteViewController *)user_data;
    if (result == APP_CONTROL_RESULT_SUCCEEDED)
    {
        char *code = nullptr;
        app_control_get_extra_data(reply, "code", &code);
        if (code)
        {
            if (self->_replyCb)
                self->_replyCb(code, true);
            free(code);
        }
    }
    else
    {
        WERROR("result failed.");
        if (self->_replyCb)
            self->_replyCb("", false);
    }
}
