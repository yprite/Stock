/*
 * SaWidgetMainViewController.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#include "SaWidgetMainViewController.h"
#include "SaWidgetStockInfoViewController.h"
#include "SaWidgetDebug.h"
#include "SaCompanyDBManager.h"
#include "SaTypes.h"
#include "SaUtility.h"
#include <app.h>
#include <Elementary.h>
#include <vector>
#include <bundle.h>

using namespace app_assist;


SaWidgetMainViewController::SaWidgetMainViewController()
{
    // TODO Auto-generated constructor stub
    _resumeEffectViewController = nullptr;
    _progressObject = nullptr;
    _effectFinishedCb = nullptr;
    _effectFullyShownCb = nullptr;
}

SaWidgetMainViewController::~SaWidgetMainViewController()
{
    // TODO Auto-generated destructor stub
}

Evas_Object* SaWidgetMainViewController::onCreateView(Evas_Object *parent, void *viewParam)
{
    WENTER();

    char *resPath = app_get_resource_path();
    char edjPath[PATH_MAX] = {0, };

    if (resPath)
    {
        snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, "edje/SaWidgetMainView.edj");
        free(resPath);
    }

    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, edjPath, "SaWidgetMainView");
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(layout);

    return layout;
}

void SaWidgetMainViewController::onCreated()
{
    WENTER();
}

void SaWidgetMainViewController::onDestroy()
{

}

void SaWidgetMainViewController::showProgress()
{
    SaProgressObject *progressObj = new SaProgressObject();
    progressObj->create(getEvasObject(), nullptr);
    evas_object_size_hint_weight_set(progressObj->getEvasObject(), 0, 0);
    evas_object_show(progressObj->getEvasObject());
    elm_object_part_content_set(getEvasObject(), "sw.progress.effect", progressObj->getEvasObject());
    progressObj->run();
    _progressObject = progressObj;
}

void SaWidgetMainViewController::removeProgress()
{
    Evas_Object *layout = getEvasObject();
    if (_progressObject)
    {
        elm_object_part_content_unset(layout, "sw.progress.effect");
        _progressObject->destroy();
        _progressObject = nullptr;
    }
}


void SaWidgetMainViewController::showResumeEffect(SaWidgetResumeEffectViewController::ViewType viewType)
{
    WENTER();

    Evas_Object *layout = getEvasObject();

    if (_resumeEffectViewController)
    {
        _resumeEffectViewController->destroy();
        _resumeEffectViewController = nullptr;
    }
    SaWidgetResumeEffectViewController *viewController = new SaWidgetResumeEffectViewController(viewType);
    viewController->create(layout, nullptr);
    elm_object_part_content_set(layout, "sw.resume.effect", viewController->getEvasObject());
    _resumeEffectViewController = viewController;

    edje_object_signal_callback_add(elm_layout_edje_get(_resumeEffectViewController->getEvasObject()), "finished", "*",
        [](void *data, Evas_Object *obj, const char *emission, const char *source)
        {
            WHIT();
            auto self = (SaWidgetMainViewController *)data;
            if (self->_resumeEffectViewController)
            {
                elm_object_part_content_unset(self->getEvasObject(), "sw.resume.effect");
                self->_resumeEffectViewController->destroy();
                self->_resumeEffectViewController = nullptr;
            }

            if (self->_effectFinishedCb)
            {
                self->_effectFinishedCb();
                self->_effectFinishedCb = nullptr;
            }
        }, this);

    _resumeTimer = WTimer::addTimer(2.5, [](void *data)->bool
        {
            WINFO("destroy timer!");
            auto self = (SaWidgetMainViewController *)data;

            if (self->_effectFullyShownCb)
            {
                self->_effectFullyShownCb();
                self->_effectFullyShownCb = nullptr;
            }

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

void SaWidgetMainViewController::removeResumeEffect()
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

void SaWidgetMainViewController::setOnResumeEffectFullyShown(const std::function<void(void)>& effectFullyShownCb)
{
    _effectFullyShownCb = effectFullyShownCb;
}

void SaWidgetMainViewController::setOnResumeEffectFinished(const std::function<void(void)>& effectFinishedCb)
{
    _effectFinishedCb = effectFinishedCb;
}


/*
void SaWidgetMainViewController::onMessageReceived(const std::string& key, const std::string& code, const std::string& result, const std::string& errorMessage)
{
    WINFO("key : %s, code : %s, result : %s, error message : %s",
        key.c_str(), code.c_str(), result.c_str(), errorMessage.c_str());

    SaCompanyInfo companyInfo;
    SaCompanyDBManager::getInstance()->getCompanyInfo("005930", companyInfo);
    WINFO("companyinfo : %s %s %s %s %s %s %s %s %s %s %s",
        companyInfo.code.c_str(),
        companyInfo.name.c_str(),
        companyInfo.market.c_str(),
        companyInfo.price.c_str(),
        companyInfo.percent.c_str(),
        companyInfo.change.c_str(),
        companyInfo.max.c_str(),
        companyInfo.min.c_str(),
        companyInfo.volume.c_str(),
        companyInfo.previous.c_str(),
        companyInfo.histroicaldata.c_str());

    bundle *b = bundle_decode((const bundle_raw *)companyInfo.histroicaldata.c_str(), companyInfo.histroicaldata.size());
    std::vector<HistoricalData> historicalVec;
    SaUtility::convertBundle2HistoricalDataVec(b, historicalVec);

    for (int i = 0; i < historicalVec.size(); ++i)
    {
        WDEBUG("[%3d] : %s %s %s", i, historicalVec[i].date.c_str(), historicalVec[i].closedPrice.c_str(), historicalVec[i].volume.c_str());
    }
}
*/



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

