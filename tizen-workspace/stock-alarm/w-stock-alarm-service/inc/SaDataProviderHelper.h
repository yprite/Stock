/*
 * SaDataProviderHelper.h
 *
 *  Created on: Aug 12, 2017
 *      Author: devbin
 */

#ifndef SADATAPROVIDERHELPER_H_
#define SADATAPROVIDERHELPER_H_

#include <bundle.h>
#include <string>

// https://github.com/yql/yql-tables/tree/master/yahoo/finance

class SaDataProviderHelper
{
public:
    SaDataProviderHelper();
    virtual ~SaDataProviderHelper();

	static bundle* getNetworkError(const char *encodedBundle);
    static bundle* getFinanceAnalystEstimate(const char *encodedBundle);
    static bundle* getFinanceBalanceSheet(const char *encodedBundle);
    static bundle* getFinanceCashFlow(const char *encodedBundle);
    static bundle* getFinanceComInfo(const char *encodedBundle);
    static bundle* getFinanceDividenHistory(const char *encodedBundle);
    static bundle* getFinanceHistoricalData(const char *encodedBundle);
    static bundle* getFinanceIncomeHistoricalData(const char *encodedBundle);
    static bundle* getFinanceIndustry(const char *encodedBundle);
    static bundle* getFinanceISIN(const char *encodedBundle);
    static bundle* getFinanceKeyStats(const char *encodedBundle);
    static bundle* getFinanceOnVista(const char *encodedBundle);
    static bundle* getFinanceOptionContracts(const char *encodedBundle);
    static bundle* getFinanceOptions(const char *encodedBundle);
    static bundle* getFinanceQuant(const char *encodedBundle);
    static bundle* getFinanceQuant2(const char *encodedBundle);

    static bundle* getFinanceQuates(const char *encodedBundle);
    static bundle* getFinanceQuatesList(const char *encodedBundle);

    static bundle* getFinanceSectors(const char *encodedBundle);
    static bundle* getFinanceStocks(const char *encodedBundle);
    static bundle* getFinanceXChange(const char *encodedBundle);
    static bundle* getFinanceAllInfo(const char *encodedBundle);

private:
    static bool _execute(const std::string &query, std::string &output, std::string &errorMessage);
    static bool _executeForHistorycalData(const std::string &query, std::string &output, std::string &errorMessage);
    static size_t _onDataReceived(void* contents, size_t size, size_t nmemb, void* userData);
};

#endif /* SADATAPROVIDERHELPER_H_ */
