/*
 * SaListEditEventManager.h
 *
 *  Created on: Sep 8, 2017
 *      Author: devbin
 */

#ifndef SALISTEDITEVENTMANAGER_H_
#define SALISTEDITEVENTMANAGER_H_

#include "SaListEditEventListener.h"
#include <list>
#include <functional>
#include "Singleton.h"

class SaListEditEventManager
{
    SINGLETON_IDIOM(SaListEditEventManager);
public:
    void addListener(SaListEditEventListener *listener);
    void removeListener(SaListEditEventListener *listener);

    void startEditMode();
    void stopEditMode();
    void updateEditMode();

    int getCheckedItemCount() const;
    void increaseCheckedItem();
    void decreaseCheckedItem();

    bool isEditMode() const;

private:
    SaListEditEventManager();
    virtual ~SaListEditEventManager();

private:
    int _checkedItemCount;
    bool _isEditMode;
    std::list<SaListEditEventListener *> _list;
};

#endif /* SALISTEDITEVENTMANAGER_H_ */
