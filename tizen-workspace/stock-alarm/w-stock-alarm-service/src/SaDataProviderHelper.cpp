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
    //http://www.google.com/finance/historical?q=NASDAQ:ADBE&startdate=Jan+01%2C+2009&enddate=Aug+2%2C+2012&output=csv

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

#if YDEBUG
bool SaDataProviderHelper::_executeForHistorycalData(const std::string &code)
{
	WENTER();

	const std::string baseUrl = "http://www.google.com/finance/historical?q=KRX:" + code +"&startdate=Jan+01%2C+2009&enddate=Aug+2%2C+2012&output=csv";

	CURL *curl = nullptr;
	std::string response;

	curl = curl_easy_init();

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, baseUrl.c_str());
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
#endif

size_t SaDataProviderHelper::_onDataReceived(void* contents, size_t size, size_t nmemb, void* userData)
{
    WENTER();
    ((std::string *)userData)->append((char *)contents, size * nmemb);

    return size * nmemb;
}


struct HistoricalData
{
    std::string date;
    std::string closedPrice;
    std::string volume;
};

static const int dummyDataSize = 10;
static HistoricalData dummyData[dummyDataSize] =
{
    {"date 0", "closed price 0", "volume 0"},
    {"date 1", "closed price 1", "volume 1"},
    {"date 2", "closed price 2", "volume 2"},
    {"date 3", "closed price 3", "volume 3"},
    {"date 4", "closed price 4", "volume 4"},
    {"date 5", "closed price 5", "volume 5"},
    {"date 6", "closed price 6", "volume 6"},
    {"date 7", "closed price 7", "volume 7"},
    {"date 8", "closed price 8", "volume 8"},
    {"date 9", "closed price 9", "volume 9"}
};

static bundle* convertHistoricalData2Bundle(const HistoricalData& historicalData)
{
    bundle *b = bundle_create();
    bundle_add_str(b, "date", historicalData.date.c_str());
    bundle_add_str(b, "closedPrice", historicalData.closedPrice.c_str());
    bundle_add_str(b, "volume", historicalData.volume.c_str());

    return b;
}

static void convertBundle2HistoricalData(bundle *b, HistoricalData& historicalData)
{
    char *tmp = nullptr;

    bundle_get_str(b, "date", &tmp);
    historicalData.date = std::string(tmp);

    bundle_get_str(b, "closedPrice", &tmp);
    historicalData.closedPrice = std::string(tmp);

    bundle_get_str(b, "volume", &tmp);
    historicalData.volume = std::string(tmp);
}

static void bundle_test()
{
    std::string savingData;
    // encode bundle
    {
        int totSize = dummyDataSize;
        char **valueArr = (char **)calloc(totSize, sizeof(char *));
        for (int i = 0; i < totSize; ++i)
        {
            bundle *subBundle = convertHistoricalData2Bundle(dummyData[i]);
            bundle_raw *encodedStr = nullptr;
            int len = 0;
            bundle_encode(subBundle, &encodedStr, &len);
            valueArr[i] = strdup((char *)encodedStr);
            bundle_free(subBundle);
            free(encodedStr);
        }

        bundle *b = bundle_create();
        bundle_add_str_array(b, "historicalData", (const char **)valueArr, totSize);
        {
            int len = 0;
            bundle_raw *encodedStr = nullptr;
            bundle_encode(b, &encodedStr, &len);

            savingData = std::string((char *)encodedStr);

            free(encodedStr);
        }
        bundle_free(b);

        for (int i = 0; i < totSize; ++i)
            free(valueArr[i]);
        free(valueArr);
    }
    dlog_print(DLOG_INFO, LOG_TAG, "saving data : %s", savingData.c_str());
    // ======================================================================
    // decode bundle
    {
        bundle *b = bundle_decode((const bundle_raw *)savingData.c_str(), savingData.size());

        int len = 0;
        const char **array = bundle_get_str_array(b, "historicalData", &len);


        for (int i = 0; i < len; ++i)
        {
            bundle *sub = bundle_decode((const bundle_raw *)array[i], strlen(array[i]));
            HistoricalData historicalData;
            convertBundle2HistoricalData(sub, historicalData);
            dlog_print(DLOG_INFO, LOG_TAG, "%s %s %s",
                    historicalData.date.c_str(),
                    historicalData.closedPrice.c_str(),
                    historicalData.volume.c_str());
            bundle_free(sub);
        }

        if (b)
            bundle_free(b);
    }
}

