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

#define SA_COMPANY_DB_SAVED_LIST_CHANDED    ".savedListChanged"

class WAPP_ASSIST_EXPORT SaCompanyDBManager
{
    SINGLETON_IDIOM(SaCompanyDBManager);

public:
    bool initialize();
    bool finalize();

    bool getCompanyInfo(const std::string& code, SaCompanyInfo &info);
    bool search(const std::string& s);
    bool searchByCode(const std::string& s, std::string &market);

    bool add(const SaCompanyInfo& info);
    bool remove(const SaCompanyInfo& info);

    bool saveInfo(const std::string& symbol, const std::string& change, const std::string& percent, const std::string& price, const std::string& max, const std::string& min, const std::string& previous, const std::string& volume);
    bool saveHistoricalData(const std::string& symbol, const std::string& data);

    const std::vector<SaCompanyInfo>& getSavedList() const;
    const std::vector<SaCompanyInfo>& getSearchResultList() const;
    const std::vector<SaCompanyInfo>& getAllComanyList() const;

protected:
    SaCompanyDBManager();
    virtual ~SaCompanyDBManager();

private:
    bool _loadAllCompanyList();
    bool _loadSavedCompanyList();

private:

    bool _isInit;
    std::vector<SaCompanyInfo> _searchResultList;
    std::vector<SaCompanyInfo> _allCompanyList;
    std::vector<SaCompanyInfo> _savedCompanyList;
};

#endif /* SACOMPANYDBMANAGER_H_ */
