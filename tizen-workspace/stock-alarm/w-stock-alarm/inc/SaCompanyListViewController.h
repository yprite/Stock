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
#include <functional>

class SaCompanyListViewController : public app_assist::WContentViewController
{
public:
    SaCompanyListViewController();
    virtual ~SaCompanyListViewController();

    void setOnItemClicked(const std::function<void(void)>& selectedCb);

private:
    virtual Evas_Object* onCreateView(Evas_Object* parent, void* viewParam) override;
    virtual void onCreated() override;
    virtual void onPushed(Elm_Object_Item* naviItem) override;
    virtual void onDestroy() override;
    virtual bool onPop() override;
    virtual void onBecomeTop() override;

private:
    void _onTextSearched(const std::string& s);
    std::string _getHighlightedText(const std::string& originStr, const std::string& searchingText);

private:
    GenlistObject* _listObj;
    std::function<void(void)> _selectedCb;
};

#endif /* SACOMPANYLISTVIEWCONTROLLER_H_ */
