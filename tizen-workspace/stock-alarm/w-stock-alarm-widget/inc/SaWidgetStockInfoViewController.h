/*
 * SaWidgetStockInfoViewController.h
 *
 *  Created on: Aug 19, 2017
 *      Author: devbin
 */

#ifndef SAWIDGETSTOCKINFOVIEWCONTROLLER_H_
#define SAWIDGETSTOCKINFOVIEWCONTROLLER_H_

#include "WViewController.h"
#include "AppEventListener.h"
#include "WTimer.h"
#include "SaWidgetResumeEffectViewController.h"
#include "SaWidgetGraphObject.h"
#include "SaWidgetDetailInfoViewController.h"
#include "SaCompanyInfo.h"

class SaWidgetStockInfoViewController : public app_assist::WViewController
                                      , public AppEventListener
{
public:
    SaWidgetStockInfoViewController(const SaCompanyInfo& companyInfo);
    virtual ~SaWidgetStockInfoViewController();

    void updateInfo(const SaCompanyInfo& companyInfo);
    void hideAllObjects();
    void showAllObjects();

private:
    virtual Evas_Object* onCreateView(Evas_Object *parent, void *viewParam) override;
    virtual void onCreated() override;
    virtual void onDestroy() override;
    virtual void onEventOccured(AppEventListener::EventType eventType) override;

    void _createTitlePriceInfo();
    void _createSubPriceInfo();
    void _createDetailPriceInfo();

    void _updateText();

    void _hideGraphAnimation();
    void _showGraphAnimation();
    void _showDetailInfoAnimation();
    void _hideDetailInfoAnimation();

private:
    app_assist::WTimerWeakPtr _animator;
    bool _isAnimatorRunning;
    double _animationStartTime;
    Evas_Coord_Rectangle _subPriceInfoStartPos;
    Evas_Coord_Rectangle _subPriceInfoEndPos;

    Evas_Object *_titlePriceInfoBox;
    Evas_Object *_titlePriceInfoIcon;
    Evas_Object *_titlePriceInfoText;

    Evas_Object *_priceIcon;
    Evas_Object *_priceInfoText;

    Evas_Object *_upDownIconView;
    Evas_Object *_upDownIcon;
    Evas_Object *_upDownText;

    Evas_Object *_plusMinusIconView;
    Evas_Object *_plusMinusIcon;
    Evas_Object *_plueMinusText;

    SaWidgetGraphObject *_graphObject;
    SaWidgetDetailInfoViewController *_detailObject;
    SaCompanyInfo _companyInfo;
    bool _isDetailMode;
};

#endif /* SAWIDGETSTOCKINFOVIEWCONTROLLER_H_ */
