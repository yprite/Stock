/*
 * SaDataConsumerHelper.cpp
 *
 *  Created on: Aug 12, 2017
 *      Author: devbin
 */

#include "SaDataConsumerHelper.h"
#include "SaCommonDebug.h"

SaDataConsumerHelper::SaDataConsumerHelper()
{
    // TODO Auto-generated constructor stub

}

SaDataConsumerHelper::~SaDataConsumerHelper()
{
    // TODO Auto-generated destructor stub
}

void SaDataConsumerHelper::parseNetworkError(bundle *b)
{
    WENTER();

    char *key = nullptr;
    char *result = nullptr;
    char *jsonData = nullptr;

    bundle_get_str(b, "key", &key);
    bundle_get_str(b, "result", &result);

    WINFO("key : %s, result : %s", key, result);
}

void SaDataConsumerHelper::parseFinanceAnalysEstimate(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceBalanceSheet(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceCashFlow(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceComInfo(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceDividenHistory(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceHistorycalData(bundle *b)
{
	WENTER();
	char *key = nullptr;
	char *result = nullptr;
	char *errorMessage = nullptr;
	char *jsonData = nullptr;

	bundle_get_str(b, "key", &key);
	bundle_get_str(b, "result", &result);
	bundle_get_str(b, "errorMessage", &errorMessage);
	bundle_get_str(b, "jsonData", &jsonData);
	WINFO("key : %s, result : %s, errorMessage : %s, jsonData : %s", key, result, errorMessage, jsonData);
}

void SaDataConsumerHelper::parseFinanceIncomeHistoricalData(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceIndustry(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceISIN(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceKeyStats(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceOnVista(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceOptionContracts(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceOptions(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceQuant(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceQuant2(bundle *b)
{
    WENTER();
}


void SaDataConsumerHelper::parseFinanceQuates(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceQuatesList(bundle *b)
{
    WENTER();

    char *key = nullptr;
    char *result = nullptr;
    char *errorMessage = nullptr;
    char *jsonData = nullptr;

    bundle_get_str(b, "key", &key);
    bundle_get_str(b, "result", &result);
    bundle_get_str(b, "errorMessage", &errorMessage);
    bundle_get_str(b, "jsonData", &jsonData);

    WINFO("key : %s, result : %s, errorMessage : %s, jsonData : %s", key, result, errorMessage, jsonData);

    // do parse!
}

void SaDataConsumerHelper::parseFinanceSectors(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceStocks(bundle *b)
{
    WENTER();
}

void SaDataConsumerHelper::parseFinanceXChange(bundle *b)
{
    WENTER();
}
void SaDataConsumerHelper::parseFinanceAllInfo(bundle *b)
{
    WENTER();

    char *key = nullptr;
    char *result = nullptr;
    char *errorMessage = nullptr;

    bundle_get_str(b, "key", &key);
    bundle_get_str(b, "result", &result);
    bundle_get_str(b, "errorMessage", &errorMessage);

    WINFO("key : %s, result : %s, errorMessage: %s", key, result, errorMessage);
}
