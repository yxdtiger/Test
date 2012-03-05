#ifndef SECURITY_UTILS_H
#define SECURITY_UTILS_H

#include "../include/wmdf_structs.h"
#ifdef __linux__
#include <cstddef>
#endif


class SecurityUtils
{
public:
	static void initialize_l1_market(L1MarketSnapshot *marketsnapshot, size_t n);
	static void initialize_l2_market(L2MarketSnapshot *marketsnapshot, size_t n);
	static void initialize_transaction(L2Transaction *transaction, size_t n);
	static void initialize_orderqueue(L2OrderQueue *orderqueue, size_t n);
	static void initialize_indexdata(IndexSnapshot *indexdata, size_t n);
	static void initialize_orders(SZSEL2Order* orders, size_t n);
};





#endif
