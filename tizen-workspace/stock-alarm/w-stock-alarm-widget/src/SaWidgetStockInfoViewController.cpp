/*
 * SaWidgetStockInfoViewController.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#include "SaWidgetStockInfoViewController.h"
#include "SaWidgetGraphObject.h"
#include "SaDataConsumer.h"
#include "SaWidgetResumeEffectViewController.h"
#include "SaTypes.h"
#include "SaUtility.h"

#include "SaWidgetDebug.h"
#include "WTimer.h"


#include <app.h>
#include <Elementary.h>
#include <vector>
#include <bundle.h>

using namespace app_assist;

static const int gSubPriceInfoYDistance = 115;

SaWidgetStockInfoViewController::SaWidgetStockInfoViewController(const SaCompanyInfo& companyInfo)
{
    // TODO Auto-generated constructor stub
    _isAnimatorRunning = false;
    _animationStartTime = 0.0f;

	_titlePriceInfoBox = nullptr;
    _titlePriceInfoIcon = nullptr;
    _titlePriceInfoText = nullptr;

    _priceIcon = nullptr;
    _priceInfoText = nullptr;
    _upDownIcon = nullptr;
    _upDownText = nullptr;
    _plusMinusIcon = nullptr;
    _plueMinusText = nullptr;

    _graphObject = nullptr;
    _detailObject = nullptr;
    _companyInfo = companyInfo;
}

SaWidgetStockInfoViewController::~SaWidgetStockInfoViewController()
{
    // TODO Auto-generated destructor stub
}

Evas_Object* SaWidgetStockInfoViewController::onCreateView(Evas_Object *parent, void *viewParam)
{
    WENTER();

    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = {0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaWidgetStockInfoView.edj");
        free(resPath);
    }

    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, edjPath, "SaWidgetStockInfoView");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);

    return layout;
}

void SaWidgetStockInfoViewController::onCreated()
{
    WENTER();

    Evas_Object *layout = getEvasObject();
    _createTitlePriceInfo();
    _createSubPriceInfo();
    _createDetailPriceInfo();

    /*
    SaWidgetGraphObject *graphObj = new SaWidgetGraphObject(360, 150);
    graphObj->create(layout, nullptr);
    graphObj->addPoint(30);
    graphObj->addPoint(40);
    graphObj->addPoint(10);
    graphObj->addPoint(100);
    graphObj->addPoint(100);
    graphObj->addPoint(300);
    graphObj->addPoint(30);
    graphObj->addPoint(10);
    graphObj->addPoint(2);

    graphObj->drawGraph();
    elm_object_part_content_set(layout, "sw.graph", graphObj->getEvasObject());
    _graphObject = graphObj;
    */
    Evas_Object *effectBtn = elm_button_add(layout);
    elm_object_style_set(effectBtn, "effect/full");
    evas_object_size_hint_weight_set(effectBtn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(effectBtn);

    //elm_layout_signal_emit(_priceIcon, "show.anim", "*");
    //elm_layout_signal_emit(_upDownIcon, "show.anim", "*");
    //elm_layout_signal_emit(_plusMinusIcon, "show.anim", "*");

    //elm_layout_signal_emit(_priceInfoText, "show.anim", "*");
    //elm_layout_signal_emit(_upDownText, "show.anim", "*");
    //elm_layout_signal_emit(_plueMinusText, "show.anim", "*");

    //elm_layout_signal_emit(layout, "show.anim", "*");

    evas_object_smart_callback_add(effectBtn, "clicked",
            [](void *data, Evas_Object *obj, void *eventInfo)
            {
                WHIT();
                auto self = (SaWidgetStockInfoViewController *)data;
                if (self->_isAnimatorRunning)
                    return;
                //Evas_Object *layout = self->getEvasObject();

                //elm_layout_signal_emit(self->_priceIcon, "show.anim", "*");
                //elm_layout_signal_emit(self->_upDownIcon, "show.anim", "*");
                //elm_layout_signal_emit(self->_plusMinusIcon, "show.anim", "*");

                //elm_layout_signal_emit(self->_priceInfoText, "show.anim", "*");
                //elm_layout_signal_emit(self->_upDownText, "show.anim", "*");
                //elm_layout_signal_emit(self->_plueMinusText, "show.anim", "*");
                self->_isAnimatorRunning = true;
                self->_hideGraphAnimation();

            }, this);

    elm_object_part_content_set(layout, "sw.touch.refresh.area", effectBtn);
    //SaDataConsumer::getInstance()->reqeuestFinanceAllInfo(_companyInfo.code.c_str());
}

void SaWidgetStockInfoViewController::hideAllObjects()
{
    Evas_Object *layout = getEvasObject();
    elm_object_signal_emit(layout, "hide", "*");
    evas_object_color_set(_detailObject->getEvasObject(), 0, 0, 0, 0);
    _isAnimatorRunning = false;
}

void SaWidgetStockInfoViewController::showAllObjects()
{
    Evas_Object *layout = getEvasObject();

    elm_layout_signal_emit(_priceIcon, "default", "*");
    elm_layout_signal_emit(_upDownIcon, "default", "*");
    elm_layout_signal_emit(_plusMinusIcon, "default", "*");

    elm_layout_signal_emit(_priceInfoText, "default", "*");
    elm_layout_signal_emit(_upDownText, "default", "*");
    elm_layout_signal_emit(_plueMinusText, "default", "*");

    elm_object_signal_emit(layout, "show", "*");

    WTimer::addTimer(0.2,
        [](void *data)->bool
        {
            auto self = (SaWidgetStockInfoViewController *)data;
            elm_layout_signal_emit(self->_priceIcon, "show.anim", "*");
            elm_layout_signal_emit(self->_upDownIcon, "show.anim", "*");
            elm_layout_signal_emit(self->_plusMinusIcon, "show.anim", "*");

            elm_layout_signal_emit(self->_priceInfoText, "show.anim", "*");
            elm_layout_signal_emit(self->_upDownText, "show.anim", "*");
            elm_layout_signal_emit(self->_plueMinusText, "show.anim", "*");

            return false;
        }, this);
}


void SaWidgetStockInfoViewController::_hideGraphAnimation()
{
    //elm_layout_signal_emit(getEvasObject(), "show.anim", "*");
    _animationStartTime = ecore_loop_time_get();
    Evas_Object *subPriceInfoObj = elm_object_part_content_get(getEvasObject(), "sw.sub.price.info");
    evas_object_geometry_get(subPriceInfoObj, &_subPriceInfoStartPos.x, &_subPriceInfoStartPos.y, &_subPriceInfoStartPos.w, &_subPriceInfoStartPos.h);
    _animator = WTimer::addAnimator(
        [](void *data)->bool
        {
            auto self = (SaWidgetStockInfoViewController *)data;
            double time = ecore_loop_time_get();
            double timeDiff = (time - self->_animationStartTime);

            double ratio = ecore_animator_pos_map(timeDiff, ECORE_POS_MAP_SINUSOIDAL, 0.0, 1.0);
            Evas_Object *subPriceInfoObj = elm_object_part_content_get(self->getEvasObject(), "sw.sub.price.info");

            if (ratio >= 1.0f)
            {
                evas_object_color_set(self->_graphObject->getEvasObject(), 75,75,75,75);
                evas_object_move(subPriceInfoObj, self->_subPriceInfoStartPos.x, self->_subPriceInfoStartPos.y - gSubPriceInfoYDistance);
                self->_subPriceInfoEndPos = self->_subPriceInfoStartPos;
                self->_subPriceInfoEndPos.y = self->_subPriceInfoStartPos.y - gSubPriceInfoYDistance;
                if (!self->_animator.expired())
                    WTimer::destroy(self->_animator);

                self->_showDetailInfoAnimation();
                //self->_showGraphAnimation();
                //self->_moveUpSubPriceInfoAndShowDetailAnimation();
                return false;
            }
            int colorRatio = 255 - ratio * 180;

            evas_object_color_set(self->_graphObject->getEvasObject(), colorRatio, colorRatio, colorRatio, colorRatio);

            int deltaY = self->_subPriceInfoStartPos.y - ratio * gSubPriceInfoYDistance;
            evas_object_move(subPriceInfoObj, self->_subPriceInfoStartPos.x, deltaY);

            return true;
        }, this);
}

void SaWidgetStockInfoViewController::_showGraphAnimation()
{
    _animationStartTime = ecore_loop_time_get();
    Evas_Object *subPriceInfoObj = elm_object_part_content_get(getEvasObject(), "sw.sub.price.info");
    evas_object_geometry_get(subPriceInfoObj, &_subPriceInfoStartPos.x, &_subPriceInfoStartPos.y, &_subPriceInfoStartPos.w, &_subPriceInfoStartPos.h);
    _animator = WTimer::addAnimator(
        [](void *data)->bool
        {
            auto self = (SaWidgetStockInfoViewController *)data;
            double time = ecore_loop_time_get();
            double timeDiff = (time - self->_animationStartTime);

            double ratio = ecore_animator_pos_map(timeDiff, ECORE_POS_MAP_SINUSOIDAL, 0.0, 1.0);
            Evas_Object *subPriceInfoObj = elm_object_part_content_get(self->getEvasObject(), "sw.sub.price.info");

            if (ratio >= 1.0f)
            {
                evas_object_color_set(self->_graphObject->getEvasObject(), 255, 255, 255, 255);
                evas_object_move(subPriceInfoObj, self->_subPriceInfoEndPos.x, self->_subPriceInfoEndPos.y + gSubPriceInfoYDistance);
                self->_isAnimatorRunning = false;
                if (!self->_animator.expired())
                    WTimer::destroy(self->_animator);

                //self->_moveUpSubPriceInfoAndShowDetailAnimation();
                return false;
            }
            int colorRatio = 75 + ratio * 180;
            evas_object_color_set(self->_graphObject->getEvasObject(), colorRatio, colorRatio, colorRatio, colorRatio);

            int deltaY = self->_subPriceInfoEndPos.y + ratio * gSubPriceInfoYDistance;
            evas_object_move(subPriceInfoObj, self->_subPriceInfoStartPos.x, deltaY);

            return true;
        }, this);
}

void SaWidgetStockInfoViewController::_showDetailInfoAnimation()
{
    _animationStartTime = ecore_loop_time_get();
    _animator = WTimer::addAnimator(
        [](void *data)->bool
        {
            auto self = (SaWidgetStockInfoViewController *)data;
            double time = ecore_loop_time_get();
            double timeDiff = (time - self->_animationStartTime);

            double ratio = ecore_animator_pos_map(timeDiff, ECORE_POS_MAP_SINUSOIDAL, 0.0, 1.0);

            if (ratio >= 1.0f)
            {
                evas_object_color_set(self->_detailObject->getEvasObject(), 255, 255, 255, 255);
                if (!self->_animator.expired())
                    WTimer::destroy(self->_animator);

                //self->_hideDetailInfoAnimation();
                return false;
            }

            int colorRatio = 255 * ratio;
            evas_object_color_set(self->_detailObject->getEvasObject(), colorRatio, colorRatio, colorRatio, colorRatio);
            return true;
        }, this);
}

void SaWidgetStockInfoViewController::_hideDetailInfoAnimation()
{
    _animationStartTime = ecore_loop_time_get();
    _animator = WTimer::addAnimator(
        [](void *data)->bool
        {
            auto self = (SaWidgetStockInfoViewController *)data;
            double time = ecore_loop_time_get();
            double timeDiff = (time - self->_animationStartTime);

            double ratio = ecore_animator_pos_map(timeDiff, ECORE_POS_MAP_SINUSOIDAL, 0.0, 1.0);

            if (ratio >= 1.0f)
            {
                evas_object_color_set(self->_detailObject->getEvasObject(), 0, 0, 0, 0);
                if (!self->_animator.expired())
                    WTimer::destroy(self->_animator);

                self->_showGraphAnimation();
                return false;
            }

            int colorRatio = 255 - 255 * ratio;
            evas_object_color_set(self->_detailObject->getEvasObject(), colorRatio, colorRatio, colorRatio, colorRatio);
            return true;
        }, this);
}

void SaWidgetStockInfoViewController::onDestroy()
{

}

void SaWidgetStockInfoViewController::onEventOccured(AppEventListener::EventType eventType)
{
    WENTER();

    if (eventType == AppEventListener::EventType::FONT_CHANGED ||
        eventType == AppEventListener::EventType::COLOR_CHANGED ||
        eventType == AppEventListener::EventType::LANGUAGE_CHANGED ||
        eventType == AppEventListener::EventType::REGION_FORMAT_CHANGED)
    {
        _updateText();
    }

    else if (eventType == AppEventListener::EventType::APP_PAUSE)
    {

    }
    else if (eventType == AppEventListener::EventType::APP_RESUME)
    {

    }
}

#if 0
void SaWidgetStockInfoViewController::pause()
{
    WENTER();
    Evas_Object *layout = getEvasObject();
    if (_resumeEffectViewController)
    {
        elm_object_part_content_unset(layout, "sw.resume.effect");
        _resumeEffectViewController->destroy();
        _resumeEffectViewController = nullptr;
    }
    if (!_resumeTimer.expired())
    {
        WTimer::destroy(_resumeTimer);
    }
}

void SaWidgetStockInfoViewController::resume()
{
    WENTER();
    time_t t = time(nullptr);
    //if (t - _resumeTime < 60)
    //    return;

    _resumeTime = t;

    Evas_Object *layout = getEvasObject();
    SaDataConsumer::getInstance()->reqeuestFinanceAllInfo(_companyInfo.code.c_str());
    if (_resumeEffectViewController)
    {
        _resumeEffectViewController->destroy();
        _resumeEffectViewController = nullptr;
    }
    SaWidgetResumeEffectViewController *viewController = new SaWidgetResumeEffectViewController();
    viewController->create(layout, nullptr);
    elm_object_part_content_set(layout, "sw.resume.effect", viewController->getEvasObject());
    _resumeEffectViewController = viewController;

    edje_object_signal_callback_add(elm_layout_edje_get(_resumeEffectViewController->getEvasObject()), "finished", "*",
        [](void *data, Evas_Object *obj, const char *emission, const char *source)
        {
            WHIT();
            auto self = (SaWidgetStockInfoViewController *)data;
            if (self->_resumeEffectViewController)
            {
                elm_object_part_content_unset(self->getEvasObject(), "sw.resume.effect");
                self->_resumeEffectViewController->destroy();
                self->_resumeEffectViewController = nullptr;
            }
        }, this);

    _resumeTimer = WTimer::addTimer(2.5, [](void *data)->bool
        {
            WINFO("destroy timer!");
            auto self = (SaWidgetStockInfoViewController *)data;
            if (self->_resumeEffectViewController)
            {
                self->_resumeEffectViewController->removeIcon();
                elm_layout_signal_emit(self->_resumeEffectViewController->getEvasObject(), "show.hide.anim", "*");
            }

            if (!self->_resumeTimer.expired())
            {
                WTimer::destroy(self->_resumeTimer);
            }
            return false;
        }, this);
}
#endif
void SaWidgetStockInfoViewController::_createTitlePriceInfo()
{
    WENTER();

    static const int TITLE_PRICE_ICON_SIZE = 50;
    static const int TITLE_PRICE_INFO_PADDING = 30;
    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = { 0, };
    char iconEdjPath[PATH_MAX] = { 0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaWidgetStockInfoView.edj");
        snprintf(iconEdjPath, sizeof(iconEdjPath), "%s%s", resPath, "edje/SaWidgetIcon.edj");
        free(resPath);
    }

    Evas_Object *layout = getEvasObject();
    Evas_Object *box = elm_box_add(layout);
    elm_box_horizontal_set(box, EINA_TRUE);

    Evas_Object *priceInfoIcon = elm_layout_add(box);
    elm_layout_file_set(priceInfoIcon, iconEdjPath, "SaWidgetIcon/W");
    evas_object_size_hint_min_set(priceInfoIcon, TITLE_PRICE_ICON_SIZE, TITLE_PRICE_ICON_SIZE);
    evas_object_size_hint_max_set(priceInfoIcon, TITLE_PRICE_ICON_SIZE, TITLE_PRICE_ICON_SIZE);
    evas_object_resize(priceInfoIcon, TITLE_PRICE_ICON_SIZE, TITLE_PRICE_ICON_SIZE);
    evas_object_show(priceInfoIcon);
    _priceIcon = priceInfoIcon;

    Evas_Object *paddingRect = evas_object_rectangle_add(evas_object_evas_get(box));
    evas_object_size_hint_min_set(paddingRect, TITLE_PRICE_INFO_PADDING, TITLE_PRICE_ICON_SIZE);
    evas_object_size_hint_max_set(paddingRect, TITLE_PRICE_INFO_PADDING, TITLE_PRICE_ICON_SIZE);
    evas_object_resize(paddingRect, TITLE_PRICE_INFO_PADDING, TITLE_PRICE_ICON_SIZE);
    evas_object_color_set(paddingRect, 0, 0, 0, 0);
    evas_object_show(paddingRect);

    Evas_Object *priceInfoText = elm_layout_add(box);
    elm_layout_file_set(priceInfoText, edjPath, "SaWidgetStockInfoView/TitlePriceInfoText");
    evas_object_show(priceInfoText);

    _priceInfoText = priceInfoText;

    elm_box_pack_end(box, priceInfoIcon);
    elm_box_pack_end(box, paddingRect);
    elm_box_pack_end(box, priceInfoText);

    elm_box_recalculate(box);
    evas_object_show(box);

    elm_object_part_content_set(layout, "sw.price.info", box);
}

void SaWidgetStockInfoViewController::_createSubPriceInfo()
{
    //SaWidgetStockInfoView/SubPriceInfoIcon
    //SaWidgetStockInfoView/SubPriceInfoText
    static const int SUB_PRICE_ICON_SIZE = 34;
    static const int CENTER_PADDING_RECT_WIDTH = 30;
    static const int PRICE_INFO_PADDING = 20;

    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = { 0, };
    char iconEdjPath[PATH_MAX] = {0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaWidgetStockInfoView.edj");
        snprintf(iconEdjPath, sizeof(iconEdjPath), "%s%s", resPath, "edje/SaWidgetIcon.edj");
        free(resPath);
    }
    Evas_Object *layout = getEvasObject();
    Evas_Object *wrapperBox = elm_box_add(layout);
    elm_box_horizontal_set(wrapperBox, EINA_TRUE);

    { // create left box sub price info
        Evas_Object *box = elm_box_add(wrapperBox);
        elm_box_horizontal_set(box, EINA_TRUE);

        Evas_Object *iconLayout = elm_layout_add(box);
        elm_layout_file_set(iconLayout, edjPath, "SaWidgetStockInfoView/SubPriceInfoIcon");
        evas_object_show(iconLayout);

//        Evas_Object *icon = evas_object_rectangle_add(evas_object_evas_get(box));
//        evas_object_size_hint_min_set(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
//        evas_object_size_hint_max_set(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
//        evas_object_resize(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
//        evas_object_color_set(icon, 0, 0, 255, 255);
//        evas_object_show(icon);
//        elm_object_part_content_set(iconLayout, "elm.swallow.content", icon);

        Evas_Object *icon = elm_layout_add(box);
        elm_layout_file_set(icon, iconEdjPath, "SaWidgetIcon/Up");
        evas_object_size_hint_min_set(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_size_hint_max_set(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_resize(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_show(icon);
        elm_object_part_content_set(iconLayout, "elm.swallow.content", icon);
        _upDownIcon = icon;

        Evas_Object *paddingRect = evas_object_rectangle_add(evas_object_evas_get(box));
        evas_object_size_hint_min_set(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_size_hint_max_set(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_resize(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_color_set(paddingRect, 0, 0, 0, 0);
        evas_object_show(paddingRect);

        Evas_Object *priceInfoText = elm_layout_add(box);
        elm_layout_file_set(priceInfoText, edjPath, "SaWidgetStockInfoView/SubPriceInfoText");
        //elm_object_part_text_set(priceInfoText, "elm.text", "39,000");
        evas_object_show(priceInfoText);
        _upDownText = priceInfoText;

        elm_box_pack_end(box, iconLayout);
        elm_box_pack_end(box, paddingRect);
        elm_box_pack_end(box, priceInfoText);
        elm_box_recalculate(box);
        evas_object_show(box);

        elm_box_pack_end(wrapperBox, box);
    }

    { // create center padding rect
        Evas_Object *paddingRect = evas_object_rectangle_add(evas_object_evas_get(wrapperBox));
        evas_object_size_hint_min_set(paddingRect, CENTER_PADDING_RECT_WIDTH, SUB_PRICE_ICON_SIZE);
        evas_object_size_hint_max_set(paddingRect, CENTER_PADDING_RECT_WIDTH, SUB_PRICE_ICON_SIZE);
        evas_object_resize(paddingRect, CENTER_PADDING_RECT_WIDTH, SUB_PRICE_ICON_SIZE);
        evas_object_color_set(paddingRect, 0, 0, 0, 0);
        evas_object_show(paddingRect);

        elm_box_pack_end(wrapperBox, paddingRect);
    }

    { // create right box sub price info
        Evas_Object *box = elm_box_add(wrapperBox);
        elm_box_horizontal_set(box, EINA_TRUE);

        Evas_Object *iconLayout = elm_layout_add(box);
        elm_layout_file_set(iconLayout, edjPath, "SaWidgetStockInfoView/SubPriceInfoIcon");
        evas_object_show(iconLayout);

        Evas_Object *icon = elm_layout_add(box);
        elm_layout_file_set(icon, iconEdjPath, "SaWidgetIcon/Plus");
        evas_object_size_hint_min_set(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_size_hint_max_set(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_resize(icon, SUB_PRICE_ICON_SIZE, SUB_PRICE_ICON_SIZE);
        evas_object_show(icon);
        elm_object_part_content_set(iconLayout, "elm.swallow.content", icon);
        _plusMinusIcon = icon;

        Evas_Object *paddingRect = evas_object_rectangle_add(evas_object_evas_get(box));
        evas_object_size_hint_min_set(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_size_hint_max_set(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_resize(paddingRect, PRICE_INFO_PADDING, SUB_PRICE_ICON_SIZE);
        evas_object_color_set(paddingRect, 0, 0, 0, 0);
        evas_object_show(paddingRect);

        Evas_Object *priceInfoText = elm_layout_add(box);
        elm_layout_file_set(priceInfoText, edjPath, "SaWidgetStockInfoView/SubPriceInfoText");
        //elm_object_part_text_set(priceInfoText, "elm.text", "1.78%");
        evas_object_show(priceInfoText);
        _plueMinusText = priceInfoText;

        elm_box_pack_end(box, iconLayout);
        elm_box_pack_end(box, paddingRect);
        elm_box_pack_end(box, priceInfoText);
        elm_box_recalculate(box);
        evas_object_show(box);

        elm_box_pack_end(wrapperBox, box);
    }

    elm_box_recalculate(wrapperBox);
    evas_object_show(wrapperBox);

    elm_object_part_content_set(layout, "sw.sub.price.info", wrapperBox);
}

void SaWidgetStockInfoViewController::_createDetailPriceInfo()
{
    Evas_Object *layout = getEvasObject();
    auto viewController = new SaWidgetDetailInfoViewController(_companyInfo);
    viewController->create(layout, nullptr);
    evas_object_color_set(viewController->getEvasObject(), 0, 0, 0, 0);
    elm_object_part_content_set(layout, "sw.detail.price.info", viewController->getEvasObject());
    _detailObject = viewController;
}

void SaWidgetStockInfoViewController::updateInfo(const SaCompanyInfo& companyInfo)
{
    Evas_Object *layout = getEvasObject();
    _companyInfo = companyInfo;

    elm_object_part_text_set(layout, "txt.company.name", _companyInfo.name.c_str());
    elm_object_part_text_set(_priceInfoText, "elm.text", _companyInfo.price.c_str());
    elm_object_part_text_set(_upDownText, "elm.text", _companyInfo.change.c_str());
    elm_object_part_text_set(_plueMinusText, "elm.text", _companyInfo.percent.c_str());
    _detailObject->updateInfo(_companyInfo);

    // update graph.
    bundle *b = bundle_decode((const bundle_raw *)_companyInfo.histroicaldata.c_str(), _companyInfo.histroicaldata.size());
    std::vector<HistoricalData> vec;
    SaUtility::convertBundle2HistoricalDataVec(b, vec);

    if (_graphObject)
    {
        elm_object_part_content_unset(layout, "sw.graph");
        _graphObject->destroy();
    }
    SaWidgetGraphObject *graphObj = new SaWidgetGraphObject(360, 150);
    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::string number;
        for (int j = 0; j < vec[i].closedPrice.size(); ++j)
        {
            if (vec[i].closedPrice[j] == '.')
                break;
            if (vec[i].closedPrice[j] != ',')
                number.push_back(vec[i].closedPrice[j]);
        }
        graphObj->addPoint(atoi(number.c_str()));
    }
    graphObj->create(layout, nullptr);

    graphObj->drawGraph();
    elm_object_part_content_set(layout, "sw.graph", graphObj->getEvasObject());
    _graphObject = graphObj;
}


void SaWidgetStockInfoViewController::_updateText()
{

    // _detailPriceInfo
}

