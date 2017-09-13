/*
 * SaUtility.cpp
 *
 *  Created on: Aug 4, 2017
 *      Author: devbin
 */

#include "SaUtility.h"
#include <string.h>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>


SaUtility::SaUtility()
{
    // TODO Auto-generated constructor stub

}

SaUtility::~SaUtility()
{
    // TODO Auto-generated destructor stub
}

void SaUtility::convertHexColorToRGBA(char const *hexCode, int *r, int *g, int *b)
{
    if (!hexCode)
        return;

    sscanf(hexCode, "#%02x%02x%02x", r, g, b);
}

void SaUtility::split( std::vector<std::string> & theStringVector, const  std::string  & theString, const  std::string  & theDelimiter)
{
	if (theDelimiter.size() < 0 || theString.size() < 0)
		return;

    size_t  start = 0, end = 0;

    while ( end != std::string::npos)
    {
        end = theString.find( theDelimiter, start);

        // If at end, use length=maxLength.  Else use length=end-start.
        theStringVector.push_back( theString.substr( start, (end == std::string::npos) ? std::string::npos : end - start));

        // If at end, use start=maxSize.  Else use start=end+delimiter.
        start = ( ( end > (std::string::npos - theDelimiter.size()) ) ? std::string::npos : end + theDelimiter.size());
    }
}

char* SaUtility::strToMonth(int idx)
{
    switch (idx)
    {
    case 0:
        return strdup("Jan");
    case 1:
        return strdup("Feb");
    case 2:
        return strdup("Mar");
    case 3:
        return strdup("Apr");
    case 4:
        return strdup("May");
    case 5:
        return strdup("Jun");
    case 6:
        return strdup("Jul");
    case 7:
        return strdup("Aug");
    case 8:
        return strdup("Sep");
    case 9:
        return strdup("Oct");
    case 10:
        return strdup("Nov");
    case 11:
        return strdup("Dec");
    }
    return nullptr;
}


bundle* SaUtility::convertHistoricalData2Bundle(const HistoricalData& historicalData)
{
    bundle *b = bundle_create();
    bundle_add_str(b, "date", historicalData.date.c_str());
    bundle_add_str(b, "closedPrice", historicalData.closedPrice.c_str());
    bundle_add_str(b, "volume", historicalData.volume.c_str());

    return b;
}

void SaUtility::convertBundle2HistoricalDataVec(bundle *b, std::vector<HistoricalData>& vec)
{
    int len = 0;
    const char **array = bundle_get_str_array(b, "historicalData", &len);

    for (int i = 0; i < len; ++i)
    {
        bundle *sub = bundle_decode((const bundle_raw *)array[i], strlen(array[i]));
        HistoricalData historicalData;
        SaUtility::convertBundle2HistoricalData(sub, historicalData);
        vec.push_back(historicalData);
        bundle_free(sub);
    }

    if (b)
        bundle_free(b);
}

void SaUtility::convertBundle2HistoricalData(bundle *b, HistoricalData& historicalData)
{
    char *tmp = nullptr;

    bundle_get_str(b, "date", &tmp);
    historicalData.date = std::string(tmp);

    bundle_get_str(b, "closedPrice", &tmp);
    historicalData.closedPrice = std::string(tmp);

    bundle_get_str(b, "volume", &tmp);
    historicalData.volume = std::string(tmp);
}

std::string& SaUtility::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string& SaUtility::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
std::string& SaUtility::trim(std::string &s) {
    return ltrim(rtrim(s));
}

