/*
 * FILENAME
 * 	OrdersData.h
 * 
 * MODULE DESCRIPTION
 * 	Defines constants and structures used for storing uploaded order data, both in
 *  shared memory and on disk. Declares functions that read and write the orders index
 * 	and data files.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __ORDERSDATA_H_INCLUDED
#define __ORDERSDATA_H_INCLUDED

#include "CommonDefines.h"

#define MAX_QUEUED_ORDERS	100

// Orders data in shared memory.
typedef struct
{
	int				numOrders;
	int				index[ MAX_QUEUED_ORDERS ];		// Indices get moved around as we insert/delete orders.
	UploadedOrder_t	orders[ MAX_QUEUED_ORDERS ];	// Orders don't move - only inserted and deleted.
} OrdersList_t;

void	DeleteOrdersFile();
void	ReadOrdersFile( OrdersList_t * ordersList );
void	WriteOrdersFile( OrdersList_t * ordersList );

#endif	// __ORDERSDATA_H_INCLUDED
