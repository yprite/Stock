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
#include <linux/limits.h>

SINGLETON_INSTANCE(SaCompanyDBManager);

SaCompanyDBManager::SaCompanyDBManager()
{
    // TODO Auto-generated constructor stub
    _dbHandler = nullptr;
    _isInit = false;
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
    char *resPath = app_get_resource_path();

    if (resPath)
    {
        snprintf(dbPath, sizeof(dbPath), "%s%s", resPath, "db/companyinfo.db");
        free(resPath);
    }

    WPRET_VM(!ecore_file_exists(dbPath), false, "db is not exist - path : %s", dbPath);

    int ret = SQLITE_OK;
    ret = sqlite3_open_v2(dbPath, &_dbHandler, SQLITE_OPEN_READONLY, nullptr);
    WPRET_VM(ret != SQLITE_OK, false, "sqlite3_open_v2 failed.(%s)", sqlite3_errstr(ret));

    if (!_loadAllCompanyList())
        return false;

    _isInit = true;
    return true;
}

bool SaCompanyDBManager::finalize()
{
    WENTER();
    int ret = SQLITE_OK;
    if (_dbHandler)
    {
        ret = sqlite3_close_v2(_dbHandler);
        WPRET_VM(ret != SQLITE_OK, false, "sqlite3_close_v2_failed.(%d)", ret);
        _dbHandler = nullptr;
    }
    return true;
}

bool SaCompanyDBManager::search(const std::string& s)
{
    WENTER();

    WPRET_VM(!_isInit, false, "not initialized");

    static const char *tableName = "info";
    sqlite3_stmt *stmt = nullptr;
    char *query = nullptr;
    int ret = SQLITE_OK;

    // reset search result.
    if (!_searchResultList.empty())
        _searchResultList.clear();

    query = sqlite3_mprintf("SELECT * FROM %q WHERE Name LIKE '%%%q%%';", tableName, s.c_str());
    //query = sqlite3_mprintf("select Code, Name from info where Name LIKE '%%%q%%'", s.c_str());
    WINFO("query : %s", query);

    ret = sqlite3_prepare_v2(_dbHandler, query, strlen(query), &stmt, nullptr);
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

                        WDEBUG("[%d] colName : %s, value : %s", i, colName, (const char *)sqlite3_column_text(stmt, i));
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
    return true;
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

    query = sqlite3_mprintf("SELECT * FROM %q;", tableName);
    //query = sqlite3_mprintf("select Code, Name from info where Name LIKE '%%%q%%'", s.c_str());
    WINFO("query : %s", query);
    ret = sqlite3_prepare_v2(_dbHandler, query, strlen(query), &stmt, nullptr);
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
                        else
                            WERROR("unknown column name : %s", colName);

                        //WDEBUG("[%d] colName : %s, value : %s", i, colName, (const char *)sqlite3_column_text(stmt, i));
                    }

                    if (companyInfo.code.empty() || companyInfo.name.empty() || companyInfo.market.empty())
                    {
                        //WERROR("companyinfo is not proper.");
                    }
                    else
                    {
                        _allCompanyList.push_back(companyInfo);
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
    if (stmt)
        sqlite3_finalize(stmt);
    return true;
}
