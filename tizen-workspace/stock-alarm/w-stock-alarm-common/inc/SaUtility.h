/*
 * SaUtility.h
 *
 *  Created on: Aug 4, 2017
 *      Author: devbin
 */

#ifndef SAUTILITY_H_
#define SAUTILITY_H_

#include "WDefine.h"
#include "SaTypes.h"

#include <bundle.h>
#include <vector>
#include <string>

class WAPP_ASSIST_EXPORT SaUtility
{
public:
    SaUtility();
    virtual ~SaUtility();

public:
    static void convertHexColorToRGBA(char const *hexCode, int *r, int *g, int *b);
    static void split( std::vector<std::string> &theStringVector, const  std::string  &theString, const  std::string  &theDelimiter);
    static char* strToMonth(int idx);

    static bundle* convertHistoricalData2Bundle(const HistoricalData& historicalData);
    static void convertBundle2HistoricalDataVec(bundle *b, std::vector<HistoricalData>& vec);
    static void convertBundle2HistoricalData(bundle *b, HistoricalData& historicalData);
    static std::string& trim(std::string &s);
    static std::string& ltrim(std::string &s);
    static std::string& rtrim(std::string &s);
};

#endif /* SAUTILITY_H_ */
