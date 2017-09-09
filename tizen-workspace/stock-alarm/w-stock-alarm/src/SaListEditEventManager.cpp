/*
 * SaListEditEventManager.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: devbin
 */

#include "SaListEditEventManager.h"
#include <algorithm>

SINGLETON_INSTANCE(SaListEditEventManager);

SaListEditEventManager::SaListEditEventManager()
{
    // TODO Auto-generated constructor stub
    _checkedItemCount = 0;
    _isEditMode = false;
}

SaListEditEventManager::~SaListEditEventManager()
{
    // TODO Auto-generated destructor stub
}

void SaListEditEventManager::addListener(SaListEditEventListener *listener)
{
    _list.push_back(listener);
}

void SaListEditEventManager::removeListener(SaListEditEventListener *listener)
{
    auto item = std::find(_list.begin(), _list.end(), listener);
    if (item != _list.end())
        _list.erase(item);
}

void SaListEditEventManager::startEditMode()
{
    _isEditMode = true;

    for (auto iter = _list.begin(); iter != _list.end(); ++iter)
    {
        (*iter)->onStartEdit();
    }

    updateEditMode();
}

void SaListEditEventManager::stopEditMode()
{
    _isEditMode = false;
    _checkedItemCount = 0;
    for (auto iter = _list.begin(); iter != _list.end(); ++iter)
    {
        (*iter)->onStopEdit();
    }
}

void SaListEditEventManager::updateEditMode()
{
    for (auto iter = _list.begin(); iter != _list.end(); ++iter)
    {
        (*iter)->onUpdateEdit();
    }
}

int SaListEditEventManager::getCheckedItemCount() const
{
    return _checkedItemCount;
}

void SaListEditEventManager::increaseCheckedItem()
{
    ++_checkedItemCount;
    updateEditMode();
}
void SaListEditEventManager::decreaseCheckedItem()
{
    --_checkedItemCount;
    updateEditMode();
}

bool SaListEditEventManager::isEditMode() const
{
    return _isEditMode;
}

