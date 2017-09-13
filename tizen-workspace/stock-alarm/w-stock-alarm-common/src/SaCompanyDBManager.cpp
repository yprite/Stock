/*
 * SaCompanyDBManager.cpp
 *
 *  Created on: Sep 5, 2017
 *      Author: devbin
 */

#include "SaCompanyDBManager.h"
#include "SaCommonDebug.h"

#include <Ecore_File.h>
#include <app.h>
#include <algorithm>
#include <linux/limits.h>
#include <app_preference.h>

#define INFO_TABLE "info"
#define SAVED_LIST_TABLE "savedList"

SINGLETON_INSTANCE(SaCompanyDBManager);

SaCompanyDBManager::SaCompanyDBManager()
{
    // TODO Auto-generated constructor stub
    _isInit = false;

    bool isExist = false;
    preference_is_existing(SA_COMPANY_DB_SAVED_LIST_CHANDED, &isExist);
    if (isExist == false)
        preference_set_boolean(SA_COMPANY_DB_SAVED_LIST_CHANDED, false);
}

SaCompanyDBManager::~SaCompanyDBManager()
{
    // TODO Auto-generated destructor stub
}

bool SaCompanyDBManager::initialize()
{
    WENTER();
    WPRET_VM(_isInit, false, "already init.");
    char dbPath[PATH_MAX] = {0, };
    char originDbPath[PATH_MAX] = {0, };
    char *resPath = app_get_resource_path();
    char *sharedDataPath = app_get_shared_data_path();

    int ret = SQLITE_OK;
    char *errMsg = nullptr;
    sqlite3 *dbHandler = NULL;

    if (resPath)
    {
        snprintf(originDbPath, sizeof(originDbPath), "%s%s", resPath, "db/companyinfo.db");
        free(resPath);
    }

    if (sharedDataPath)
    {
        snprintf(dbPath, sizeof(dbPath), "%s%s", sharedDataPath, "companyinfo.db");
        free(sharedDataPath);
    }
    WDEBUG("dbPath=[%s]", dbPath);

    if (!ecore_file_exists(dbPath))
    {
        WINFO("dbPath[%s] is not existed", dbPath);
        WPRET_VM(ecore_file_cp(originDbPath, dbPath) == false, false, "ecore_file_cp() is failed, Copy : [%s] -> [%s]", originDbPath, dbPath);
    }
    WPRET_VM(!ecore_file_exists(dbPath), false, "db is not exist - path : %s", dbPath);

    ret = sqlite3_open_v2(dbPath, &dbHandler, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_open_v2 failed.(%s)", sqlite3_errstr(ret));

    if (!_loadAllCompanyList())
        return false;

    {
        std::string query = "CREATE TABLE IF NOT EXISTS " + std::string(SAVED_LIST_TABLE) + "("
                + "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                + "Code TEXT NOT NULL, "
                + "Name TEXT NOT NULL, "
                + "Market TEXT NOT NULL);";
        WINFO("query : %s", query.c_str());
        ret = sqlite3_exec(dbHandler, query.c_str(), nullptr, nullptr, &errMsg);
        if (ret != SQLITE_OK)
            WERROR("sqlite3_exec failed.(%s)", errMsg);
    }

    if (!_loadSavedCompanyList())
        return false;

    if (dbHandler)
    {
        ret = sqlite3_close_v2(dbHandler);
        WPRET_VM(ret != SQLITE_OK, false, "sqlite3_close_v2_failed.(%d)", ret);
        dbHandler = nullptr;
    }

    _isInit = true;
    return true;
}

bool SaCompanyDBManager::finalize()
{
    WENTER();

    return true;
}
bool SaCompanyDBManager::getCompanyInfo(const std::string& code, SaCompanyInfo &info)
{
    sqlite3_stmt *stmt = nullptr;
    char *query = nullptr;
    int ret = SQLITE_OK;
    SaCompanyInfo companyInfo;

    sqlite3 *dbHandler = NULL;
    char dbPath[PATH_MAX] = {0, };
    char *sharedDataPath = app_get_shared_data_path();

    if (sharedDataPath)
    {
        snprintf(dbPath, sizeof(dbPath), "%s%s", sharedDataPath, "companyinfo.db");
        free(sharedDataPath);
    }
    WDEBUG("dbPath=[%s]", dbPath);

    ret = sqlite3_open_v2(dbPath, &dbHandler, SQLITE_OPEN_READWRITE, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_open_v2() failed.(%d)", ret);

    // reset search result.
    if (!_searchResultList.empty())
        _searchResultList.clear();

    query = sqlite3_mprintf("SELECT * FROM %q WHERE Code = \'%q\';", INFO_TABLE, code.c_str());
    //query = sqlite3_mprintf("select Code, Name from info where Name LIKE '%%%q%%'", s.c_str());
    WINFO("query : %s", query);

    ret = sqlite3_prepare_v2(dbHandler, query, strlen(query), &stmt, nullptr);
    WINFO("ret : %d", ret);

    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_prepare_v2 failed.(%s)", sqlite3_errstr(ret));
    sqlite3_free(query);

    while (true)
    {
        ret = sqlite3_step(stmt);
        if (ret != SQLITE_DONE)
        {
            WHIT();
            switch (ret)
            {
                case SQLITE_ROW:
                {
                    int n = sqlite3_column_count(stmt);

                    for (int i = 0; i < n; ++i)
                    {
                        const char *colName = sqlite3_column_name(stmt, i);
                        const char *value = (const char *)sqlite3_column_text(stmt, i);
                        if (colName && !strcmp(colName, "Code"))
                            companyInfo.code = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Name"))
                            companyInfo.name = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Market"))
                            companyInfo.market = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Price"))
                            companyInfo.price = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Percent"))
                            companyInfo.percent = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Change"))
                            companyInfo.change = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Max"))
                            companyInfo.max = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Min"))
                            companyInfo.min = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Volume"))
                            companyInfo.volume = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Previous"))
                            companyInfo.previous = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "HistoricalData"))
                            companyInfo.histroicaldata = value ? std::string(value) : std::string("");
                        else
                            WERROR("unknown column name : %s", colName);
                    }

                    if (companyInfo.code.empty() || companyInfo.name.empty() || companyInfo.market.empty())
                        WERROR("companyinfo is not proper.");
                    else
                        _searchResultList.push_back(companyInfo);

                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else
        {
            WINFO("FINISH!");
            break;
        }
    }
    if (stmt)
        sqlite3_finalize(stmt);

    if (dbHandler)
    {
        ret = sqlite3_close_v2(dbHandler);
        WPRET_VM(ret != SQLITE_OK, false, "sqlite3_close_v2_failed.(%d)", ret);
        dbHandler = nullptr;
    }

    info = companyInfo;
    return true;
}
bool SaCompanyDBManager::add(const SaCompanyInfo& info)
{
    if (_savedCompanyList.size() != 0)
    {
        auto ptr = std::find_if(_savedCompanyList.begin(), _savedCompanyList.end(),
            [info](const SaCompanyInfo& companyInfo)->bool
            {
                return companyInfo.code == info.code;
            });

        if (ptr != _savedCompanyList.end())
        {
            WERROR("already exist!");
            return false;
        }
    }

    int ret = SQLITE_OK;
    sqlite3 *dbHandler = NULL;
    char dbPath[PATH_MAX] = {0, };
    char *sharedDataPath = app_get_shared_data_path();

    if (sharedDataPath)
    {
        snprintf(dbPath, sizeof(dbPath), "%s%s", sharedDataPath, "companyinfo.db");
        free(sharedDataPath);
    }
    WDEBUG("dbPath=[%s]", dbPath);

    ret = sqlite3_open_v2(dbPath, &dbHandler, SQLITE_OPEN_READWRITE, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_open_v2() failed.(%d)", ret);

    char *errMsg = nullptr;
    std::string query = "INSERT INTO " + std::string(SAVED_LIST_TABLE) + "(Code, Name, Market) " + "VALUES("
            + "\"" + info.code + "\"" + ", "
            + "\"" + info.name + "\"" + ", "
            + "\"" + info.market + "\"" + ");";
    WINFO("query : %s", query.c_str());


    ret = sqlite3_exec(dbHandler, query.c_str(), nullptr, nullptr, &errMsg);
    if (ret != SQLITE_OK)
        WERROR("sqlite3_exec failed.(%s)", errMsg);

    _savedCompanyList.clear();
    _loadSavedCompanyList();

    bool savingValue = false;
    preference_get_boolean(SA_COMPANY_DB_SAVED_LIST_CHANDED, &savingValue);
    preference_set_boolean(SA_COMPANY_DB_SAVED_LIST_CHANDED, !savingValue);

    if (dbHandler)
    {
        ret = sqlite3_close_v2(dbHandler);
        WPRET_VM(ret != SQLITE_OK, false, "sqlite3_close_v2_failed.(%d)", ret);
        dbHandler = nullptr;
    }

    return true;
}

bool SaCompanyDBManager::remove(const SaCompanyInfo& info)
{
    return false;
}

bool SaCompanyDBManager::search(const std::string& s)
{
    WENTER();

    WPRET_VM(!_isInit, false, "not initialized");

    static const char *tableName = "info";
    sqlite3_stmt *stmt = nullptr;
    char *query = nullptr;
    int ret = SQLITE_OK;

    sqlite3 *dbHandler = NULL;
    char dbPath[PATH_MAX] = {0, };
    char *sharedDataPath = app_get_shared_data_path();

    if (sharedDataPath)
    {
        snprintf(dbPath, sizeof(dbPath), "%s%s", sharedDataPath, "companyinfo.db");
        free(sharedDataPath);
    }
    WDEBUG("dbPath=[%s]", dbPath);

    ret = sqlite3_open_v2(dbPath, &dbHandler, SQLITE_OPEN_READWRITE, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_open_v2() failed.(%d)", ret);

    // reset search result.
    if (!_searchResultList.empty())
        _searchResultList.clear();

    query = sqlite3_mprintf("SELECT * FROM %q WHERE Name LIKE '%%%q%%';", tableName, s.c_str());
    //query = sqlite3_mprintf("select Code, Name from info where Name LIKE '%%%q%%'", s.c_str());
    WINFO("query : %s", query);

    ret = sqlite3_prepare_v2(dbHandler, query, strlen(query), &stmt, nullptr);
    WINFO("ret : %d", ret);

    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_prepare_v2 failed.(%s)", sqlite3_errstr(ret));
    sqlite3_free(query);

    while (true)
    {
        ret = sqlite3_step(stmt);
        if (ret != SQLITE_DONE)
        {
            WHIT();
            switch (ret)
            {
                case SQLITE_ROW:
                {
                    int n = sqlite3_column_count(stmt);
                    SaCompanyInfo companyInfo = { };

                    for (int i = 0; i < n; ++i)
                    {
                        const char *colName = sqlite3_column_name(stmt, i);
                        const char *value = (const char *)sqlite3_column_text(stmt, i);
                        if (colName && !strcmp(colName, "Code"))
                            companyInfo.code = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Name"))
                            companyInfo.name = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Market"))
                            companyInfo.market = value ? std::string(value) : std::string("");
                        else
                            WERROR("unknown column name : %s", colName);
                    }

                    if (companyInfo.code.empty() || companyInfo.name.empty() || companyInfo.market.empty())
                        WERROR("companyinfo is not proper.");
                    else
                        _searchResultList.push_back(companyInfo);

                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else
        {
            WINFO("FINISH!");
            break;
        }
    }
    if (stmt)
        sqlite3_finalize(stmt);

    if (dbHandler)
   	{
    	ret = sqlite3_close_v2(dbHandler);
    	WPRET_VM(ret != SQLITE_OK, false, "sqlite3_close_v2_failed.(%d)", ret);
    	dbHandler = nullptr;
   	}

    return true;
}

bool SaCompanyDBManager::searchByCode(const std::string& s, std::string &market)
{
    int ret = SQLITE_OK;
    char *query = nullptr;
    sqlite3 *dbHandler = NULL;
    sqlite3_stmt *stmt = nullptr;
    char dbPath[PATH_MAX] = {0, };
    char *sharedDataPath = app_get_shared_data_path();

    if (sharedDataPath)
    {
        snprintf(dbPath, sizeof(dbPath), "%s%s", sharedDataPath, "companyinfo.db");
        free(sharedDataPath);
    }
    WDEBUG("dbPath=[%s]", dbPath);

    ret = sqlite3_open_v2(dbPath, &dbHandler, SQLITE_OPEN_READWRITE, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_open_v2() failed.(%d)", ret);

    query = sqlite3_mprintf("SELECT Market FROM %q WHERE Code='%q';", INFO_TABLE, s.c_str());
    WINFO("query : %s", query);

    ret = sqlite3_prepare_v2(dbHandler, query, strlen(query), &stmt, nullptr);
    WINFO("ret : %d", ret);

    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_prepare_v2 failed.(%s)", sqlite3_errstr(ret));
    sqlite3_free(query);

    while (true)
    {
        ret = sqlite3_step(stmt);
        if (ret != SQLITE_DONE)
        {
            switch (ret)
            {
                case SQLITE_ROW:
                {
                    int n = sqlite3_column_count(stmt);
                    for (int i = 0; i < n; ++i)
                    {
                        const char *value = (const char *)sqlite3_column_text(stmt, i);
                        WINFO("Code : %s", value);
                        market = std::string(value);
                    }
                    break;
                }
                default:
                {
                  break;
                }
            }
        }
        else
        {
           WINFO("FINISH!");
           break;
        }
    }

    if (dbHandler)
    {
      ret = sqlite3_close_v2(dbHandler);
      WPRET_VM(ret != SQLITE_OK, false, "sqlite3_close_v2_failed.(%d)", ret);
      dbHandler = nullptr;
    }
    WLEAVE();
    return true;
}

bool SaCompanyDBManager::saveInfo(const std::string& symbol, const std::string& change, const std::string& percent, const std::string& price, const std::string& max, const std::string& min, const std::string& previous, const std::string& volume)
{
    WENTER();

    int ret = SQLITE_OK;
    char *errMsg = nullptr;

    sqlite3 *dbHandler = NULL;
    char dbPath[PATH_MAX] = {0, };
    char *sharedDataPath = app_get_shared_data_path();

    if (sharedDataPath)
    {
        snprintf(dbPath, sizeof(dbPath), "%s%s", sharedDataPath, "companyinfo.db");
        free(sharedDataPath);
    }
    WDEBUG("dbPath=[%s]", dbPath);

    ret = sqlite3_open_v2(dbPath, &dbHandler, SQLITE_OPEN_READWRITE, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_open_v2() failed.(%d)", ret);



    std::string query = "UPDATE " + std::string(INFO_TABLE) + " SET Price='"+price.substr(0, price.find('.'))+
            "', Max='"+ max.substr(0, max.find('.')) +
            "', Change='"+ change.substr(0, change.find('.')) +
            "', Percent='"+ percent.substr(1, percent.size() - 1 - 1) +
            "', Min='"+ min.substr(0, min.find('.')) +
            "', Volume='"+ volume+
            "', Previous='"+ previous.substr(0, previous.find('.'))+
            "' WHERE Code='"+symbol+"'";
    WINFO("query [%s]", query.c_str());
    ret = sqlite3_exec(dbHandler, query.c_str(), nullptr, nullptr, &errMsg);
    if (ret != SQLITE_OK)
    {
        WERROR("sqlite3_exec failed. err=[%d], errMsg=[%s]", ret, errMsg);
        return false;
    }

    if (dbHandler)
    {
        ret = sqlite3_close_v2(dbHandler);
        WPRET_VM(ret != SQLITE_OK, false, "sqlite3_close_v2_failed.(%d)", ret);
        dbHandler = nullptr;
    }
    WINFO("Done");
    return true;
}

bool SaCompanyDBManager::saveHistoricalData(const std::string& symbol, const std::string& data)
    {
    WENTER();

    int ret = SQLITE_OK;
    char *errMsg = nullptr;

    sqlite3 *dbHandler = NULL;
    char dbPath[PATH_MAX] = {0, };
    char *sharedDataPath = app_get_shared_data_path();

    if (sharedDataPath)
    {
        snprintf(dbPath, sizeof(dbPath), "%s%s", sharedDataPath, "companyinfo.db");
        free(sharedDataPath);
    }
    WDEBUG("dbPath=[%s]", dbPath);

    ret = sqlite3_open_v2(dbPath, &dbHandler, SQLITE_OPEN_READWRITE, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_open_v2() failed.(%d)", ret);

    std::string query = "UPDATE " + std::string(INFO_TABLE) + " SET HistoricalData='"+data+"' WHERE Code='"+symbol+"'";
    WINFO("query [%s]", query.c_str());
    ret = sqlite3_exec(dbHandler, query.c_str(), nullptr, nullptr, &errMsg);
    if (ret != SQLITE_OK)
    {
        WERROR("sqlite3_exec failed. err=[%d], errMsg=[%s]", ret, errMsg);
        return false;
    }

    if (dbHandler)
    {
        ret = sqlite3_close_v2(dbHandler);
        WPRET_VM(ret != SQLITE_OK, false, "sqlite3_close_v2_failed.(%d)", ret);
        dbHandler = nullptr;
    }
    WINFO("Done");
    return true;

    }
const std::vector<SaCompanyInfo>& SaCompanyDBManager::getSavedList() const
{
    return _savedCompanyList;
}

const std::vector<SaCompanyInfo>& SaCompanyDBManager::getSearchResultList() const
{
    return _searchResultList;
}

const std::vector<SaCompanyInfo>& SaCompanyDBManager::getAllComanyList() const
{
    return _allCompanyList;
}

bool SaCompanyDBManager::_loadAllCompanyList()
{
    WENTER();

    static const char *tableName = "info";
    sqlite3_stmt *stmt = nullptr;
    char *query = nullptr;
    int ret = SQLITE_OK;

    sqlite3 *dbHandler = NULL;
    char dbPath[PATH_MAX] = {0, };
    char *sharedDataPath = app_get_shared_data_path();

    if (sharedDataPath)
    {
    	snprintf(dbPath, sizeof(dbPath), "%s%s", sharedDataPath, "companyinfo.db");
    	free(sharedDataPath);
    }
    WDEBUG("dbPath=[%s]", dbPath);

    ret = sqlite3_open_v2(dbPath, &dbHandler, SQLITE_OPEN_READWRITE, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_open_v2() failed.(%d)", ret);

    query = sqlite3_mprintf("SELECT * FROM %q;", tableName);

    WINFO("query : %s", query);
    ret = sqlite3_prepare_v2(dbHandler, query, strlen(query), &stmt, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_prepare_v2 failed.(%s)", sqlite3_errstr(ret));
    sqlite3_free(query);

    while (true)
    {
        ret = sqlite3_step(stmt);
        if (ret != SQLITE_DONE)
        {
            switch (ret)
            {
                case SQLITE_ROW:
                {
                    int n = sqlite3_column_count(stmt);
                    SaCompanyInfo companyInfo = { };

                    for (int i = 0; i < n; ++i)
                    {
                        const char *colName = sqlite3_column_name(stmt, i);
                        const char *value = (const char *)sqlite3_column_text(stmt, i);
                        if (colName && !strcmp(colName, "Code"))
                            companyInfo.code = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Name"))
                            companyInfo.name = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Market"))
                            companyInfo.market = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Price"))
                        	companyInfo.price = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Change"))
                                companyInfo.change = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Percent"))
                                companyInfo.percent = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Max"))
                        	companyInfo.max = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Min"))
                        	companyInfo.min = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Volume"))
                        	companyInfo.volume = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "Previous"))
                        	companyInfo.previous = value ? std::string(value) : std::string("");
                        else if (colName && !strcmp(colName, "HistoricalData"))
                        	companyInfo.histroicaldata = value ? std::string(value) : std::string("");
                        else
                            WERROR("unknown column name : %s", colName);

                    }

                    if (companyInfo.code.empty() || companyInfo.name.empty() || companyInfo.market.empty())
                        WERROR("companyinfo is not proper.");
                    else
                        _allCompanyList.push_back(companyInfo);

                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else
        {
            WINFO("FINISH!");
            break;
        }
    }
    if (stmt)
        sqlite3_finalize(stmt);

    if (dbHandler)
    {
    	ret = sqlite3_close_v2(dbHandler);
    	WPRET_VM(ret != SQLITE_OK, false, "sqlite3_close_v2_failed.(%d)", ret);
    	dbHandler = nullptr;
    }

    return true;
}

bool SaCompanyDBManager::_loadSavedCompanyList()
{
    WENTER();

    sqlite3_stmt *stmt = nullptr;
    char *query = nullptr;
    int ret = SQLITE_OK;

    sqlite3 *dbHandler = NULL;
    char dbPath[PATH_MAX] = {0, };
    char *sharedDataPath = app_get_shared_data_path();

    if (sharedDataPath)
    {
        snprintf(dbPath, sizeof(dbPath), "%s%s", sharedDataPath, "companyinfo.db");
        free(sharedDataPath);
    }
    WDEBUG("dbPath=[%s]", dbPath);

    ret = sqlite3_open_v2(dbPath, &dbHandler, SQLITE_OPEN_READWRITE, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_open_v2() failed.(%d)", ret);

    query = sqlite3_mprintf("SELECT * FROM %q;", SAVED_LIST_TABLE);
    //query = sqlite3_mprintf("select Code, Name from info where Name LIKE '%%%q%%'", s.c_str());
    WINFO("query : %s", query);
    ret = sqlite3_prepare_v2(dbHandler, query, strlen(query), &stmt, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_prepare_v2 failed.(%s)", sqlite3_errstr(ret));
    sqlite3_free(query);

    while (true)
    {
        ret = sqlite3_step(stmt);
        if (ret != SQLITE_DONE)
        {
            switch (ret)
            {
                case SQLITE_ROW:
                {
                    int n = sqlite3_column_count(stmt);
                    SaCompanyInfo companyInfo = { };

                    for (int i = 0; i < n; ++i)
                    {
                        const char *colName = sqlite3_column_name(stmt, i);
                        const char *value = (const char *)sqlite3_column_text(stmt, i);

                        if (colName && !strcmp(colName, "Code"))
                        {
                            companyInfo.code = value ? std::string(value) : std::string("");
                        }
                        else if (colName && !strcmp(colName, "Name"))
                        {
                            companyInfo.name = value ? std::string(value) : std::string("");
                        }
                        else if (colName && !strcmp(colName, "Market"))
                        {
                            companyInfo.market = value ? std::string(value) : std::string("");
                        }
                        else if (colName && !strcmp(colName, "id"))
                        {
                            // skip.
                        }
                        else
                        {
                            WERROR("unknown column name : %s", colName);
                        }

                    }

                    if (companyInfo.code.empty() || companyInfo.name.empty() || companyInfo.market.empty())
                        WERROR("companyinfo is not proper.");
                    else
                        _savedCompanyList.push_back(companyInfo);

                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else
        {
            WINFO("FINISH!");
            break;
        }
    }
    if (stmt)
        sqlite3_finalize(stmt);

    if (dbHandler)
    {
        ret = sqlite3_close_v2(dbHandler);
        WPRET_VM(ret != SQLITE_OK, false, "sqlite3_close_v2_failed.(%d)", ret);
        dbHandler = nullptr;
    }
    return true;
}
