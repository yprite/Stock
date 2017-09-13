/*
 * SaTypes.h
 *
 *  Created on: Sep 12, 2017
 *      Author: yprite
 */

#ifndef SATYPES_H_
#define SATYPES_H_

struct HistoricalData
{
    std::string date;
    std::string closedPrice;
    std::string volume;
};

#define INFO_RESULT_INDEX_SYMBOL 		 		    0
#define INFO_RESULT_INDEX_CURRENT_PRICE 		    1
#define INFO_RESULT_INDEX_CHANGE                    2
#define INFO_RESULT_INDEX_PERCENT                   3
#define INFO_RESULT_INDEX_MAX_PRICE		            4
#define INFO_RESULT_INDEX_MIN_PRICE	 		        5
#define INFO_RESULT_INDEX_PRIVIOUS                  6
#define INFO_RESULT_INDEX_VOLUME                    7


#define HISTORICAL_DATA_INDEX_DATE		0
#define HISTORICAL_DATA_INDEX_OPEN		1
#define HISTORICAL_DATA_INDEX_HIGH		2
#define HISTORICAL_DATA_INDEX_LOW		3
#define HISTORICAL_DATA_INDEX_CLOSE		4
#define HISTORICAL_DATA_INDEX_VOLUME	5


#endif /* SATYPES_H_ */
