/*
 * SaCompanyDBManager.h
 *
 *  Created on: Sep 5, 2017
 *      Author: devbin
 */

#ifndef SACOMPANYDBMANAGER_H_
#define SACOMPANYDBMANAGER_H_

#include "Singleton.h"
#include "SaCompanyInfo.h"
#include "WDefine.h"
#include <sqlite3.h>
#include <vector>

class WAPP_ASSIST_EXPORT SaCompanyDBManager
{
    SINGLETON_IDIOM(SaCompanyDBManager);

public:
    bool initialize();
    bool finalize();

    bool search(const std::string& s);

    bool save(const SaCompanyInfo& info);
    bool remove(const SaCompanyInfo& info);

    const std::vector<SaCompanyInfo>& getSavedList() const;
    const std::vector<SaCompanyInfo>& getSearchResultList() const;
    const std::vector<SaCompanyInfo>& getAllComanyList() const;

protected:
    SaCompanyDBManager();
    virtual ~SaCompanyDBManager();

private:
    bool _loadAllCompanyList();

private:
    sqlite3 *_dbHandler;
    bool _isInit;
    std::vector<SaCompanyInfo> _searchResultList;
    std::vector<SaCompanyInfo> _allCompanyList;
};

#endif /* SACOMPANYDBMANAGER_H_ */
