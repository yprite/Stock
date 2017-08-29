/*
 * SaDataProviderHelper.cpp
 *
 *  Created on: Aug 12, 2017
 *      Author: devbin
 */

#include "SaDataProviderHelper.h"
#include "SaDataExchangeKeys.h"
#include "SaServiceDebug.h"

#include <bundle.h>
#include <curl/curl.h>
#include <string>

SaDataProviderHelper::SaDataProviderHelper()
{
    // TODO Auto-generated constructor stub

}

SaDataProviderHelper::~SaDataProviderHelper()
{
    // TODO Auto-generated destructor stub
}

bundle* SaDataProviderHelper::getNetworkError(const char *encodedBundle)
{
    WENTER();

    bundle *resultBundle = bundle_create();
    bundle_add_str(resultBundle, "key", SaDataExchangeKeys::networkError);
    bundle_add_str(resultBundle, "result", "fail");

    return resultBundle;
}


bundle* SaDataProviderHelper::getFinanceAnalystEstimate(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceBalanceSheet(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceCashFlow(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceComInfo(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceDividenHistory(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceHistorycalData(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceIncomeHistoricalData(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceIndustry(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceISIN(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceKeyStats(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceOnVista(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceOptionContracts(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceOptions(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceQuant(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceQuant2(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}


bundle* SaDataProviderHelper::getFinanceQuates(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

#if 0
#include <bundle.h>
bundle *b = bundle_create(); // Create a new bundle object
bundle_add_str(b, "foo_key", "bar_val"); // Add a key-value pair

bundle_raw *encoded_b;
int len;
bundle_encode(b, &encoded_b, &len); // Encode b

bundle *b_dup;
b_dup = bundle_decode(encoded_b, len); // Decoded bundle object

bundle_free(b);
free(encoded_b);
bundle_free(b_dup);
#endif

bundle* SaDataProviderHelper::getFinanceQuatesList(const char *encodedBundle)
{
    WENTER();
    WINFO("data : %s, len : %d", encodedBundle, strlen(encodedBundle));
    bundle *b = bundle_decode((const bundle_raw *)encodedBundle, strlen(encodedBundle));
    char *symbol = nullptr;
    int r = bundle_get_str(b, "symbol", &symbol);
    if (r != BUNDLE_ERROR_NONE)
        WERROR("bundle_get_str failed.(%d)", r);

    WINFO("request symbol : %s", symbol);
    std::string query = "select * from yahoo.finance.quoteslist where symbol='" + std::string(symbol) + "'";
    std::string response;
    std::string errorMessage;
    bundle_free(b);

    bool result = SaDataProviderHelper::_execute(query, response, errorMessage);

    bundle *resultBundle = bundle_create();

    bundle_add_str(resultBundle, "key", SaDataExchangeKeys::quotesList);
    bundle_add_str(resultBundle, "result", result ? "success" : "fail");
    bundle_add_str(resultBundle, "errorMessage", errorMessage.c_str());
    bundle_add_str(resultBundle, "jsonData", response.c_str());

    return resultBundle;
}


bundle* SaDataProviderHelper::getFinanceSectors(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceStocks(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bundle* SaDataProviderHelper::getFinanceXChange(const char *encodedBundle)
{
    WENTER();
    return nullptr;
}

bool SaDataProviderHelper::_execute(const std::string &query, std::string &output, std::string &errorMessage)
{
    WENTER();
    const std::string baseUrl = "http://query.yahooapis.com/v1/public/yql";
    const std::string envCmd = "&env=store://datatables.org/alltableswithkeys";
    std::string strRequest = "q=" + query + envCmd + "&format=json";

    CURL *curl = nullptr;
    std::string response;

    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, baseUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strRequest.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _onDataReceived);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode ret = curl_easy_perform(curl);
        std::string errMsg = curl_easy_strerror(ret);
        WINFO("response : %s", response.c_str());
        WINFO("errMsg : %s", errMsg.c_str());
        output = response;
        curl_easy_cleanup(curl);
        return ret == CURLE_OK;
    }
    return false;
}

size_t SaDataProviderHelper::_onDataReceived(void* contents, size_t size, size_t nmemb, void* userData)
{
    WENTER();
    ((std::string *)userData)->append((char *)contents, size * nmemb);

    return size * nmemb;
}
