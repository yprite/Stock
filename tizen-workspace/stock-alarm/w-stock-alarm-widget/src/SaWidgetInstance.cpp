/*
 * SaWidgetInstance.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#include "SaWidgetInstance.h"
#include "WWidgetWindowController.h"
#include "SaWidgetMainViewController.h"
#include "AppEventManager.h"
#include "SaCompanyDBManager.h"
#include "SaTypes.h"
#include "SaWidgetStockInfoViewController.h"
#include "SaWidgetNoFavoriteViewController.h"
#include "SaUtility.h"
#include "SaDataConsumer.h"
#include "SaWidgetDebug.h"

#include <app.h>
#include <Elementary.h>
#include <vector>
#include <libxml/HTMLparser.h>

using namespace app_assist;

SaWidgetInstance::SaWidgetInstance()
{
    // TODO Auto-generated constructor stub
    _baseView = nullptr;
    _viewController = nullptr;
    _viewType = ViewType::NONE;
    _instanceState = InstanceState::NONE;
    _context = nullptr;
    _content = nullptr;
    _lastResumeTime = 0;
}

SaWidgetInstance::~SaWidgetInstance()
{
    // TODO Auto-generated destructor stub
}

bool SaWidgetInstance::onCreate(widget_context_h context, bundle* content, int width, int height)
{
    WENTER();

    _instanceState = InstanceState::CREATE;
    _context = context;

    if (!content)
    {
        _content = bundle_create();
        widget_app_context_set_content_info(context, _content);
    }
    else
    {
        _content = bundle_dup(content);
        char *code = nullptr;
        bundle_get_str(_content, "code", &code);
        if (code)
            WINFO("code : %s", code);
        else
            WERROR("code is empty!");
        _code = code;
    }

    widget_app_context_set_tag(context, this);

    attachWindowController(context, new WWidgetWindowController());

    auto baseView = new SaWidgetMainViewController();
    getWindowController()->attachBaseViewController(baseView);
    _baseView = baseView;

    if (!_code.empty())
    {
        SaCompanyInfo companyInfo;
        SaCompanyDBManager::getInstance()->getCompanyInfo(_code, companyInfo);
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

        _viewType = ViewType::COMPANY_INFO;
        auto viewController = new SaWidgetStockInfoViewController(companyInfo);
        viewController->create(_baseView->getEvasObject(), nullptr);
        elm_object_part_content_set(_baseView->getEvasObject(), "elm.swallow.content", viewController->getEvasObject());
        viewController->showAllObjects();
        _viewController = viewController;
    }
    else
    {
        WINFO("viewType is no favorite!");

        _viewType = ViewType::NO_FAVORITE;
        auto viewController = new SaWidgetNoFavoriteViewController();
        viewController->setOnAppControlReplyCb(
            [this](const std::string& code, bool result)
            {
                if (result == true)
                {
                    if (_viewController)
                    {
                        _viewController->destroy();
                        _viewController = nullptr;
                    }
                    _code = code;
                    SaCompanyInfo companyInfo;
                    SaCompanyDBManager::getInstance()->getCompanyInfo(_code, companyInfo);
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

                    _viewType = ViewType::COMPANY_INFO;
                    auto viewController = new SaWidgetStockInfoViewController(companyInfo);
                    viewController->create(_baseView->getEvasObject(), nullptr);
                    elm_object_part_content_set(_baseView->getEvasObject(), "elm.swallow.content", viewController->getEvasObject());
                    _viewController = viewController;
                    _updateContentValue();

                    _lastResumeTime -= 100;
                    viewController->hideAllObjects();
                    //_baseView->showProgress();
                    //SaDataConsumer::getInstance()->reqeuestFinanceAllInfo(_code.c_str());
                }
            });
        viewController->create(_baseView->getEvasObject(), nullptr);
        elm_object_part_content_set(_baseView->getEvasObject(), "elm.swallow.content", viewController->getEvasObject());
        _viewController = viewController;
    }

    _lastResumeTime = time(nullptr);
    return true;
}

bool SaWidgetInstance::onDestroy(widget_context_h context, widget_app_destroy_type_e reason, bundle* content)
{
    WENTER();

    if (_content)
    {
        char *value = nullptr;
        bundle_get_str(_content, "code", &value);
        if (value)
            bundle_add_str(content, "code", value);
        bundle_free(_content);
    }
    return true;
}

bool SaWidgetInstance::onPause(widget_context_h context)
{
    WENTER();

    _instanceState = InstanceState::PAUSE;
    _baseView->removeProgress();
    _baseView->removeResumeEffect();
    return true;
}

bool SaWidgetInstance::onResume(widget_context_h context)
{
    WENTER();
    static const int delayTime = 20;
    _instanceState = InstanceState::RESUME;
    if (_viewType == ViewType::COMPANY_INFO)
    {
        time_t t = time(nullptr);
        if (t - _lastResumeTime > delayTime)
        {
            _lastResumeTime = t;
            // show progress view.'
            SaDataConsumer::getInstance()->reqeuestFinanceAllInfo(_code.c_str());
            _baseView->showProgress();
        }
    }
    return true;
}

bool SaWidgetInstance::onUpdate(widget_context_h context, bundle* content, bool isForced)
{
    return true;
}

bool SaWidgetInstance::onResize(widget_context_h context, int width, int height)
{
    WENTER();
    WINFO("widget : %d, height : %d", width, height);
    return true;
}

void SaWidgetInstance::_updateContentValue()
{
    if (_content)
    {
        char *value = nullptr;
        bundle_get_str(_content, "code", &value);
        if (value)
            bundle_del(_content, "code");

        bundle_add_str(_content, "code", _code.c_str());
    }
    widget_app_context_set_content_info(_context, _content);
}

void SaWidgetInstance::onMessageReceived(const std::string& key, const std::string& code, const std::string& result, const std::string& errorMessage)
{
    if (code != _code)
        return;

    if (_viewType == ViewType::NO_FAVORITE)
        return;

    WINFO("key : %s, code : %s, result : %s, error message : %s",
        key.c_str(), code.c_str(), result.c_str(), errorMessage.c_str());

    SaCompanyInfo companyInfo;
    SaCompanyDBManager::getInstance()->getCompanyInfo(code.c_str(), companyInfo);
    //SaCompanyDBManager::getInstance()->getCompanyInfo("005930", companyInfo);
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

    if (_instanceState == InstanceState::PAUSE)
    {
        if (_viewType == ViewType::COMPANY_INFO)
        {
            SaWidgetStockInfoViewController *viewController = (SaWidgetStockInfoViewController *)_viewController;
            viewController->updateInfo(companyInfo);
            viewController->showAllObjects();
        }
    }
    else if (_instanceState == InstanceState::RESUME)
    {
        _baseView->removeProgress();
        _baseView->showResumeEffect();
        _baseView->setOnResumeEffectFullyShown(
            [this, code]()
            {
                WINFO("setOnResumeEffectFullyShown called!");
                SaWidgetStockInfoViewController *viewController = (SaWidgetStockInfoViewController *)_viewController;
                viewController->hideAllObjects();
            });
        _baseView->setOnResumeEffectFinished(
            [this, code]()
            {
                WINFO("setOnResumeEffectFinished called!");
                _lastResumeTime = time(nullptr);
                if (_viewType == ViewType::COMPANY_INFO)
                {
                    // update info.
                    SaCompanyInfo companyInfo;
                    SaCompanyDBManager::getInstance()->getCompanyInfo(code.c_str(), companyInfo);
                    SaWidgetStockInfoViewController *viewController = (SaWidgetStockInfoViewController *)_viewController;
                    viewController->updateInfo(companyInfo);
                    viewController->showAllObjects();
                }
            });
    }

}

