/*************************************************************   	     										 *                                                    	     *
 * @brief  Source file for the needs of CS-240b project 2018 *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Shopping.h"

#define BUFFER_SIZE 1024  /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

/*	search = traverse shop list
	search_2 = search for the 2nd store in M event
	sorted_p = pointer used to create the sorted list at M event ( O(n+m) )
	search_cust = traverse customers list
	search_shopping_list = traverse customer's shopping list
	search_shop_product = traverse shop's product list
	prev_product = previous node in customer's shopping list (previous product)
	last_node = the last node in Shop List before it returns back to H
*/



struct Shop *tmp_shop, *last_node, *search, *s, *search_2, *shop_type, *K0, *K1, *K2, *K3, *K4;
struct Product *tmp_products, *search_shop_product, *curr_prod, *a, *tmp_node, *search_shop_product_2;
struct Product *p_start, *p_fast, *prev_shop_prod, *sorted_p, *sort_temp, *danger, *S, *S_last_node;
struct Customer *tmp_cust, *search_cust, *x, *b, *current_customer;
struct C_Product *tmp_c_prod, *q, *c_start, *c_fast, *search_shopping_list, *curr_c_prod, *prev_product;



/* -------- FREE POINTERS --------*/
struct C_Product *free_customer_product;
struct Product *free_shop_product;




int temp_pid, temp_quantity, temp_price;
int found_1, found_2;


/* Print Shop List after a registration */
void Display_Shops(void);
void Print_Supply(void);
void Print_Customers(void);
void Print_Cust_ShoppingList(void);
void Print_Types(void);

/* Check if a customer is in the list */
int SearchCustomer(int cid);

/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int Initialize (void)
{
	int i;
	H = tmp_shop = last_node = search = search_2 = s = shop_type = NULL;
	Customers = Sentinel = tmp_cust = search_cust = NULL;
	tmp_products = search_shop_product = curr_prod = NULL;
	tmp_c_prod = NULL;
	free_customer_product = NULL;
	temp_pid = temp_quantity = temp_price = 0;
	found_1 = found_2 = 0;
	sorted_p = NULL;
	K0 = K1 = K2 = K3 = K4 = NULL;
	
	for (i = 0; i < 5; i++) {
		ShopTypes[i] = NULL;
		CheapestProducts[i] = NULL;
	}


	return 1;
}

/**
 * @brief Register shop 
 *
 * @param sid   The shop's id
 * @param type  The shop's type
 *
 * @return 1 on success
 *         0 on failure
 */
int RegisterShop (int sid, int type)
{
	/* For traversing the Shop List */
	search = H;
	if (type < 0 || type > 4)	/* Type is out of [0,1,2,3,4] */
		return 0;


	if (search) {
		/* Search if shop with sid already exists */
		while (search->next != H) {			/* Loop until you reach back to Head */
			if (search->sid == sid) {		/* Already registered, exit */
				return 0;
			}
			search = search->next;	
		}
		/* One node (the head node). Check if sid is the same with head's sid */
		if (search->sid == sid)
			return 0;
	}

	tmp_shop = (struct Shop *)malloc(sizeof(struct Shop));
	if (!tmp_shop)		/* Allocation CHECK */
		return 0;


	tmp_shop->sid = sid;
	tmp_shop->type = type;
	tmp_shop->products = NULL;
	tmp_shop->next = NULL;
	tmp_shop->prev = NULL;
	
	if (!H) {
		H = tmp_shop;
		H->next = H;
		H->prev = H;
		last_node = tmp_shop;
	} else {
		last_node->next = tmp_shop;	/* Link last node with the new allocated one */
		tmp_shop->next = H;			/* New node next points to the first node(head) */
		tmp_shop->prev = last_node;	/* New node prev points to the previous node */
		H->prev = tmp_shop;			/* First(head) node points to new node (tmp_shop) */
		last_node = tmp_shop;		/* Set last node where tmp_shop points */
		last_node->next = H;
	}

	return 1;
}

/**
 * @brief Supply shop 
 *
 * @param sid       The shop's id
 * @param pid       The product's id
 * @param quantity  The product's quantity
 * @param price     The product's price
 *
 * @return 1 on success
 *         0 on failure
 */
int SupplyShop (int sid, int pid, int quantity, int price)
{
	/* For traversing the Shop list */
	search = H;

	if (!H)
		return 0;	/* Empty list. Cannot search */

	while (search->sid != sid) {
		search = search->next;
		if (search == H)		/* Shop did not found in shopping list */
			return 0;
	}

	
	/* Search if product already exists */
	search_shop_product = search->products;
	while (search_shop_product) {
		if (search_shop_product->pid == pid) {
			search_shop_product->quantity += quantity;
			return 1;
		}
		search_shop_product = search_shop_product->next;
	}


	tmp_products = (struct Product *)malloc(sizeof(struct Product));
	if (!tmp_products) {
		return 0;
	}
	tmp_products->pid = pid;
	tmp_products->quantity = quantity;
	tmp_products->price = price;
	tmp_products->next = NULL;

	
	/* Make first product node as head of the product list */
	if (!search->products) {
		search->products = tmp_products;
		return 1;
	} else {
		/* Loop until you reach the end of the list */
		curr_prod = search->products;
		while (curr_prod->next != NULL) {
			curr_prod = curr_prod->next;
		}
		curr_prod->next = tmp_products;		/* Connect last node with the new one */

	}

	/* ..........SORTING.......... */
	for (p_start = search->products; p_start->next != NULL; p_start = p_start->next) {
		for (p_fast = p_start->next; p_fast != NULL; p_fast = p_fast->next) {
			if (p_start->pid > p_fast->pid) {
				temp_pid = p_start->pid;
				temp_quantity = p_start->quantity;
				temp_price = p_start->price;

				p_start->pid = p_fast->pid;
				p_start->quantity = p_fast->quantity;
				p_start->price = p_fast->price;

				p_fast->pid = temp_pid;
				p_fast->quantity = temp_quantity;
				p_fast->price = temp_price;
			}
		}
	}
	
	return 1;
} 

/**
 * @brief Register customer
 *
 * @param cid The customer's id
 *
 * @return 1 on success
 *         0 on failure
 */
int RegisterCustomer (int cid)
{
	search_cust = Customers;

	if (!Sentinel) {
		Sentinel = (struct Customer *)malloc(sizeof(struct Customer));
		Sentinel->shopping_list = NULL;
		Sentinel->next = NULL;
		if (!Sentinel)		/* Error on Sentinel Allocation */
				return 0;
	}



	/* If customer list is not empty, search it */	
	if (search_cust) {
		Sentinel->cid = cid;
		while (search_cust->cid != cid) {
			search_cust = search_cust->next;
		}
		if (search_cust != Sentinel)	/* Found before Sentiner, user exists */
			return 0;
	}

	tmp_cust = (struct Customer *)malloc(sizeof(struct Customer));
	tmp_cust->cid = cid;
	tmp_cust->shopping_list = NULL;
	tmp_cust->next = NULL;


	if (!Customers) {
		Customers = tmp_cust;
		current_customer = tmp_cust;
		tmp_cust->next = Sentinel;
	} else {
		current_customer->next = tmp_cust;
		tmp_cust->next = Sentinel;
		current_customer = current_customer->next;
	}

	Sentinel->cid = 0;
	return 1;
}

/**
 * @brief Add to shopping list
 *
 * @param cid       The customer's id
 * @param pid       The product's id
 * @param quantity  The quantity of the product
 *
 * @return 1 on success
 *         0 on failure
 */
int AddToShoppingList (int cid, int pid, int quantity)
{
	if (SearchCustomer(cid) == 0)
		return 0;

	/* Check if product is already in customer's shopping list */
	search_shopping_list = search_cust->shopping_list;
	while (search_shopping_list) {
		if (search_shopping_list->pid == pid) {
			search_shopping_list->quantity += quantity;
			return 1;
		}
		search_shopping_list = search_shopping_list->next;
	}



	tmp_c_prod = (struct C_Product *)malloc(sizeof(struct C_Product));
	/* Error on Customer Shoppling List allocation */
	if (!tmp_c_prod) {
		return 0;
	}

	tmp_c_prod->pid = pid;
	tmp_c_prod->quantity = quantity;
	tmp_c_prod->next = NULL;

	/* Make first customer product as head of the customer shopping list */
	if (!search_cust->shopping_list) {
		search_cust->shopping_list = tmp_c_prod;
		return 1;
	} else {
		/* Loop until you reach the end of the list */
		curr_c_prod = search_cust->shopping_list;
		while (curr_c_prod->next != NULL) {
			curr_c_prod = curr_c_prod->next;
		}
		curr_c_prod->next = tmp_c_prod;		/* Connect last node with the new one */

	}


	/* ..........SORTING.......... */
	for (c_start = search_cust->shopping_list; c_start->next != NULL; c_start = c_start->next) {
		for (c_fast = c_start->next; c_fast != NULL; c_fast = c_fast->next) {
			if (c_start->pid > c_fast->pid) {
				temp_pid = c_start->pid;
				temp_quantity = c_start->quantity;

				c_start->pid = c_fast->pid;
				c_start->quantity = c_fast->quantity;

				c_fast->pid = temp_pid;
				c_fast->quantity = temp_quantity;
			}
		}
	}

	return 1;
}

/**
 * @brief Delete from shopping list
 *
 * @param cid  The customer's id
 * @param pid  The product's id
 * 
 * @return 1 on success
 *         0 on failure
 */
int DeleteFromShoppingList (int cid, int pid)
{
	if (SearchCustomer(cid) == 0)
		return 0;

	/* Check if product is already in customer's shopping list */
	search_shopping_list = search_cust->shopping_list;
	while (search_shopping_list) {
		if (search_shopping_list->pid == pid) {		/* Product found */
			break;
		}
		prev_product = search_shopping_list;
		search_shopping_list = search_shopping_list->next;	
	}

	/* Product did not found in shopping list (cannot delete it) */
	if (!search_shopping_list) {
		return 0;
	}



	/* Delete the first product in list (where Head points to) */
	if (search_shopping_list->pid == search_cust->shopping_list->pid) {
		free_customer_product = search_shopping_list;
		search_shopping_list = search_shopping_list->next;
		search_cust->shopping_list = search_shopping_list;
		free(free_customer_product);
		/* Product is the last node */
	} else if (search_shopping_list->next == NULL) {
		free(search_shopping_list);
		prev_product->next = NULL;
		search_shopping_list = prev_product;
	} else {
		free_customer_product = search_shopping_list;
		prev_product->next = search_shopping_list->next;
		free(free_customer_product);
	}

	free_customer_product = NULL;
	return 1;
}

/**
 * @brief Go shopping
 *
 * @param cid The customer's id
 *
 * @return 1 on success
 *         0 on failure
 */
int GoShopping (int cid)
{
	/* Checks if customer OR shop list exists */
	if (SearchCustomer(cid) == 0 || H == NULL) {
		return 0;
	}

	
	search = H;
	search_shopping_list = search_cust->shopping_list;

	/* Traverse Shop list until you reach back to Head */
	while (search_shopping_list != NULL) {
		search_shop_product =  prev_shop_prod = search->products;		/* Initialize with shop's product list */
		search_shopping_list = prev_product = search_cust->shopping_list;
		/* Loop while shop's list is not reached to last product */
		while (search_shop_product) {
			if (search_shop_product->pid == search_shopping_list->pid) {
				/* CASE 1: Customer quantity > Shop quantity */
				if (search_shopping_list->quantity > search_shop_product->quantity) {
					search_shopping_list->quantity -= search_shop_product->quantity;
					search_shop_product->quantity = 0;
				/* CASE 2: Customer quantity < Shop quantity */
				} else if (search_shopping_list->quantity < search_shop_product->quantity) {
					search_shop_product->quantity -= search_shopping_list->quantity;
					search_shopping_list->quantity = 0;
				} else {
				/* CASE 3: Customer quantity = Shop quantity */	
					search_shop_product->quantity = 0;
					search_shopping_list->quantity = 0;
				}



				/* Check if Shop Product is going to be removed */
				if (search_shop_product->quantity == 0) {
					/* Remove LAST NODE */
					if (search->products->next == NULL) {
						free(search->products);
						search->products = NULL;
					/* Remove HEAD NODE */
					} else if (search_shop_product->pid == search->products->pid) {
						prev_shop_prod = search_shop_product;
						search_shop_product = search_shop_product->next;
						free(prev_shop_prod);
						prev_shop_prod = search_shop_product;
						search->products = search_shop_product;
					} else if (search_shop_product->next == NULL) {
						free(search_shop_product);
						prev_shop_prod->next = NULL;
						search_shop_product = prev_shop_prod;
					} else {
						/* Remove somewhere else */
						prev_shop_prod->next = search_shop_product->next;
						free(search_shop_product);
						search_shop_product = prev_shop_prod->next;
						prev_shop_prod = search_shop_product;
					}
				}

				/* Check if Customer Product is going to be removed */
				if (search_shopping_list->quantity == 0) {
					/* Remove LAST NODE */
					if (search_cust->shopping_list->next == NULL) {
						free(search_cust->shopping_list);
						search_cust->shopping_list = NULL;
						search_shopping_list = NULL;
						return 1;
					/* Remove HEAD NODE */
					} else if (search_shopping_list->pid == search_cust->shopping_list->pid) {
						prev_product = search_shopping_list;
						search_shopping_list = search_shopping_list->next;
						free(prev_product);
						prev_product = search_shopping_list;
						search_cust->shopping_list = search_shopping_list;
					} else if (search_shopping_list->next == NULL) {
						free(search_shopping_list);
						prev_product->next = NULL;
						search_shopping_list = prev_product;
					/* Remove somewhere else */
					} else {
						prev_product->next = search_shopping_list->next;
						free(search_shopping_list);
						search_shopping_list = prev_product->next;
						prev_product = search_shopping_list;
					}

				}

			} else {
				
				search_shopping_list = search_shopping_list->next;
				if (search_shopping_list == NULL) {
					search_shop_product = search_shop_product->next;
					search_shopping_list = search_cust->shopping_list;
				}
			}

		}
		search = search->next;	/* Go to next shop */
		if (search == H) {
			return 1;
		}
	}
	return 1;
}

/**
 * @brief Store close
 * 
 * @param sid1  The id of the acquisitor shop
 * @param sid2  The id of the closing shop
 *
 * @return 1 on success
 *         0 on failure
 */
int StoreClose (int sid1, int sid2)
{
	/* Empty Shop List OR one shop in shop list OR both sid are the same */
	if ((H == NULL) || (H->next == NULL) || (sid1 == sid2)) {
		return 0;
	}

	search = search_2 = H;

	while (search != last_node && search_2 != last_node) {
		/* Check for sid1 */
		if (search->sid == sid1 && found_1 == 0) {
			found_1 = 1;
		}
		/* If not found, check next shop */
		if (!found_1) {
			search = search->next;
		}


		/* Check for sid2 */
		if (search_2->sid == sid2 && found_2 == 0) {
			found_2 = 1;
		}
		/* If not found, check next shop */
		if (!found_2) {
			search_2 = search_2->next;
		}	


		/* Check last shop before exiting */
		if (search == last_node && found_1 == 0) {
			if (search->sid == sid1) {
				found_1 = 1;
			} else {		/* Last shop and didn't found. EXIT */
				return 0;
			}
		}
		/* Check last shop before exiting */
		if (search_2 == last_node && found_2 == 0) {
			if (search_2->sid == sid2) {
				found_2 = 1;
			} else {		/* Last shop and didn't found. EXIT */
				return 0;
			}
		}


		/* Both shops found. Go merging */
		if (found_1 == 1 && found_2 == 1)
			break;
	}



/* -------------- MERGING ( O(n+m) ) -------------- */

	search_shop_product = search->products;
	search_shop_product_2 = search_2->products;

	/* CASE 1 : SID1 = NULL */
	if (search->products == NULL) {
		search->products = search_2->products;
	/* CASE 2 : SID2 = NULL */
	} else 	if (search_shop_product_2 == NULL) {
		search_shop_product_2 = search_shop_product;
	}

	else {
		if (search_shop_product != NULL && search_shop_product_2 != NULL) {
			if (search_shop_product->pid <= search_shop_product_2->pid) {
				S = search_shop_product;
				S_last_node = S;
				search_shop_product = search_shop_product->next;
			} else {
				S = search_shop_product_2;
				S_last_node = S;
				search_shop_product_2 = search_shop_product_2->next;				
			}

			while (search_shop_product != NULL && search_shop_product_2 != NULL) {
				if (search_shop_product->pid <= search_shop_product_2->pid) {
					S_last_node->next = search_shop_product;
					S_last_node = S_last_node->next;
					search_shop_product = search_shop_product->next;
				} else {
					S_last_node->next = search_shop_product_2;
					S_last_node = S_last_node->next;
					search_shop_product_2 = search_shop_product_2->next;				
				}
			}
		}
		if (search_shop_product_2 == NULL && search_shop_product != NULL) {
			S_last_node->next = search_shop_product;
		} else {
			S_last_node->next = search_shop_product_2;
		}
		search->products = S;
	}
		
	
	


/* Removing Shop with SID2 from Shop List */
	/* CASE 1 : Delete Head node */
	if (search_2 == H) {
		H = search_2->next;
		H->prev = last_node;
		last_node->next = H;
		free(search_2);
	/* CASE 2 : Delete last node */
	} else if (search_2 == last_node) {
		last_node = search_2->prev;
		last_node->next = H;
		H->prev = last_node;
		free(search_2);

	/* CASE 3 : Delete somewhere in the middle */	
	} else {
		search_2->prev->next = search_2->next;
		free(search_2);
	}

	/* If last shop has 0 products, end */
	if (search_2->products == NULL) {
			search_2 = NULL;
			return 1;
	}

	
	/* Remove any duplicates and increase quantity to the first product found */
	search_shop_product = search->products;
	tmp_node = search_shop_product->next;	/* Start one product next to head */
	while (tmp_node) {
		if (search_shop_product->pid == tmp_node->pid) {
			free_shop_product = tmp_node;
			search_shop_product->quantity += tmp_node->quantity;
			tmp_node = tmp_node->next;
			free(free_shop_product);
		/* Different PID. Go to new product and start again */
		} else {
			search_shop_product->next = tmp_node;
			tmp_node = tmp_node->next;
			search_shop_product = search_shop_product->next;
		}
	}

	return 1;

#if 0
		/* ..........MERGING.......... */
	search_shop_product = search->products;
	/* CASE 1 : SID */
	if (search_shop_product) {
		while (search_shop_product->next != NULL) {
			search_shop_product = search_shop_product->next;
		}
		/* Go to previous node and make it's next field to point to next of search_2 */
		search_shop_product->next = search_2->products;
	} else {
		search->products = search_2->products;
	}
	


/* Removing Shop with SID2 from Shop List */
	/* CASE 1 : Delete Head node */
	if (search_2 == H) {
		H = search_2->next;
		H->prev = last_node;
		last_node->next = H;
		free(search_2);
	/* CASE 2 : Delete last node */
	} else if (search_2 == last_node) {
		last_node = search_2->prev;
		last_node->next = H;
		H->prev = last_node;
		free(search_2);

	/* CASE 3 : Delete somewhere in the middle */	
	} else {
		search_2->prev->next = search_2->next;
		free(search_2);
	}

	/* If last shop has 0 products, end */
	if (search_2->products == NULL) {
			search_2 = NULL;
			return 1;
	}
	search_2 = NULL;


	/* ..........SORTING.......... */
	for (p_start = search->products; p_start->next != NULL; p_start = p_start->next) {
		for (p_fast = p_start->next; p_fast != NULL; p_fast = p_fast->next) {
			if (p_start->pid > p_fast->pid) {
				temp_pid = p_start->pid;
				temp_quantity = p_start->quantity;
				temp_price = p_start->price;

				p_start->pid = p_fast->pid;
				p_start->quantity = p_fast->quantity;
				p_start->price = p_fast->price;

				p_fast->pid = temp_pid;
				p_fast->quantity = temp_quantity;
				p_fast->price = temp_price;
			}
		}
	}

#endif
	
}

/**
 * @brief Categorize shops
 *
 * @return 1 on success
 *         0 on failure
 */
int CategorizeShops (void)
{
	int i, j;
	if (!H) {
		return 0;
	}

	search = H;
	last_node->next = NULL;
	while (search != NULL) {
		tmp_shop = (struct Shop *)malloc(sizeof(struct Shop));

		switch (search->type)
		{
			case 0:
				if (ShopTypes[0] == NULL) {
					ShopTypes[0] = search;
					K0 = ShopTypes[0];
					H = H->next;
					K0->next = NULL;
				} else {
					K0->next = search;
					K0 = K0->next;
					H = H->next;
					K0->next = NULL;
					
				}
				break;
			case 1:
				if (ShopTypes[1] == NULL) {
					ShopTypes[1] = search;
					K1 = ShopTypes[1];
					H = H->next;
					K1->next = NULL;
				} else {
					K1->next = search;
					K1 = K1->next;
					H = H->next;
					K1->next = NULL;	
				}
				break;
			case 2:
				if (ShopTypes[2] == NULL) {
					ShopTypes[2] = search;
					K2 = ShopTypes[2];
					H = H->next;
					K2->next = NULL;
				} else {
					K2->next = search;
					K2 = K2->next;
					H = H->next;
					K2->next = NULL;
					
				}
				break;
			case 3:
				if (ShopTypes[3] == NULL) {
					ShopTypes[3] = search;
					K3 = ShopTypes[3];
					H = H->next;
					K3->next = NULL;
				} else {
					K3->next = search;
					K3 = K3->next;
					H = H->next;
					K3->next = NULL;
					
				}
				break;
			default:
				if (ShopTypes[4] == NULL) {
					ShopTypes[4] = search;
					K4 = ShopTypes[4];
					H = H->next;
					K4->next = NULL;
				} else {
					K4->next = search;
					K4 = K4->next;
					H = H->next;
					K4->next = NULL;
					
				}
				break;
		}
		search = H;
	}


	/* Print results */
	for (i = 0; i < 5; i++) {
		j = 1;		/* Counter for shop numeration */
		printf("   Type %d\n", i);
		shop_type = ShopTypes[i];
		/* Run until the shop type shops are finished */
		while (shop_type != NULL) {
			search_shop_product = shop_type->products;
			printf("\tShop %d = ", j);
			/* Run until current's shop product list is finished */
			while (search_shop_product != NULL) {
				printf(" <%d,%d,%d>", search_shop_product->pid,search_shop_product->quantity,search_shop_product->price);
				search_shop_product = search_shop_product->next;
				if (search_shop_product != NULL) {
					printf(",");
				}
			}
			printf("\n");
			shop_type = shop_type->next;
			j++;
		}
		printf("\n");
		
	}
	printf("DONE\n");

	return 1;
}

/**
 * @brief Find cheapest products
 *
 * @return 1 on success
 *         0 on failure
 */
int FindCheapestProducts (void)
{
	/* flag = 0 --> not found */
	int i = 0, j, k, x, flag = 0;
	/* Empty Shop List */
	if (!H) {
		return 0;
	}

	search = H;
	/* Traverse Shop List */
	while (i < 5) {
		search_shop_product = search->products;
		while (search_shop_product != NULL && i < 5) {
			CheapestProducts[i] = search_shop_product;
			search_shop_product = search_shop_product->next;
			i++;		/* 0->1->2->3->4->5 */
		}
		
		/* Found the first 5 cheapest products */
		if (i == 5) {
			break;
		}

		search = search->next;
		if (search == H) {
			break;
		}
	}

/* --------------------------------------------------------------------- */
	/* Products are less than 5, so SORT-->PRINT-->EXIT */
	if (i != 5) {
		/* SORT the array */
		for (j = 0; j < i; j++) {
			for (x = j+1; x < i; x++) {
				if (CheapestProducts[j]->price > CheapestProducts[x]->price) {
					sort_temp = CheapestProducts[j];
					CheapestProducts[j] = CheapestProducts[x];
					CheapestProducts[x] = sort_temp;
				}
			}
		}

		/* Remove duplicates by setting them to NULL */
		for (j = 0; j < i; j++) {
			for (x = j+1; x < i; x++) {
				if (CheapestProducts[j] == NULL) {
					break;
				} else if (CheapestProducts[x] == NULL) {
					continue;
				} else if (CheapestProducts[j]->pid == CheapestProducts[x]->pid) {
					CheapestProducts[x] = NULL;
				} 
			}
		}
		
		printf("\n\tCheapest Products: ");
		/* Print every pointer that is not NULL */
		for (k = 0; k < i; k++){
			if (CheapestProducts[k] != NULL){
				printf("<%d,%d,%d>, ", CheapestProducts[k]->pid, CheapestProducts[k]->quantity, CheapestProducts[k]->price);
				
			}
			
		}
		printf("\n");
		return 1;	
	}
/* --------------------------------------------------------------------- */
		/* SORT */
		for (j = 0; j < 5; j++) {
			for (x = j+1; x < 5; x++) {
				if (CheapestProducts[j]->price > CheapestProducts[x]->price) {
					sort_temp = CheapestProducts[j];
					CheapestProducts[j] = CheapestProducts[x];
					CheapestProducts[x] = sort_temp;
				}
			}
		}


		while (1) {		
			while (search_shop_product) {
				flag = 0;

				/* Check if product is already in the array */
				for (j = 0; j < 5; j++) {
					if (search_shop_product->pid == CheapestProducts[j]->pid) {
						if (search_shop_product->price < CheapestProducts[j]->price) {
							CheapestProducts[j] = search_shop_product;
							flag = 1;
							break;
						}
					}
				}

				/* If product did not found in array, check for price */
				if (flag == 0) {
					for (j = 0; j < 5; j++) {
						if (search_shop_product->price < CheapestProducts[j]->price) {
							tmp_node = CheapestProducts[4];
							for (x = 4; x > j; x--) {
								CheapestProducts[x] = CheapestProducts[x-1];
							}
							CheapestProducts[j] = search_shop_product;
							break;
						}
					}
				}



				/* Sort the array */
				for (j = 0; j < 5; j++) {
					for (x = j+1; x < 5; x++) {
						if (CheapestProducts[j]->price > CheapestProducts[x]->price) {
							sort_temp = CheapestProducts[j];
							CheapestProducts[j] = CheapestProducts[x];
							CheapestProducts[x] = sort_temp;
						}
					}	
				}




				search_shop_product = search_shop_product->next;
			}

			search = search->next;
			if (search == H) {
				break;
			}
			search_shop_product = search->products;
			flag = 0;
		}
		printf("\n\tCheapest Products: ");
		/* Print every pointer that is not NULL */
		for (k = 0; k < 5; k++){
			if (CheapestProducts[k] != NULL){
				printf("<%d,%d,%d>, ", CheapestProducts[k]->pid, CheapestProducts[k]->quantity, CheapestProducts[k]->price);
				
			}
			
		}
		printf("\n");
		return 1;


}

/**
 * @brief Print shops
 *
 * @return 1 on success
 *         0 on failure
 */
int PrintShops (void)
{
	int i = 1;
	s = H;
	
	while (1) {
		printf("\tShop %d = ", i);
		a = s->products;

		/* Loop till you hit NULL */
		while (a) {
			printf("<%d,%d,%d>", a->pid, a->quantity, a->price);
			a = a->next;
			if (a)		/* Last , won't be printed */
				printf(", ");
		}
		printf("\n");
		s = s->next;
		/* End of list shops. Stop printing */
		if (s == H) {
			break;
		}
		i++;		
	}
	printf("DONE\n");
	return 1;
}

/**
 * @brief Print customers
 *
 * @return 1 on success
 *         0 on failure
 */
int PrintCustomers (void)
{
	
	b = Customers;
	
	while (b != Sentinel) {
		printf("\tCustomer %d = ", b->cid);
		q = b->shopping_list;

		/* Loop till you hit NULL */
		while (q) {
			printf("<%d,%d>", q->pid, q->quantity);
			q = q->next;
			if (q)		/* Last , won't be printed */
				printf(", ");
		}
		printf("\n");
		b = b->next;
		/* Loop until you hit the sentinel */		
	}
	printf("DONE\n");
	return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int FreeAll(void)
{
	return 1;
}


/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char** argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Initializations */
	Initialize();

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

			/* Comment */
			case '#':
				break;

				/* Register shop
				 * R <sid> <type> */
			case 'R':
				{
					int sid, type;

					sscanf(buff, "%c %d %d", &event, &sid, &type);
					DPRINT("%c %d %d\n", event, sid, type);

					if (RegisterShop(sid, type)) {
						DPRINT("%c %d %d succeeded\n", event, sid, type);
						printf("\nR <%d> <%d>\n", sid, type);
						Display_Shops();
						printf("\nDONE\n");						
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, sid, type);
					}

					break;
				}

				/* Supply shop
				 * S <sid> <pid> <quantity> <price> */
			case 'S':
				{
					int sid, pid, quantity, price;

					sscanf(buff, "%c %d %d %d %d", &event, &sid, &pid, &quantity, &price);
					DPRINT("%c %d %d %d %d\n", event, sid, pid, quantity, price);

					if (SupplyShop(sid, pid, quantity, price)) {
						DPRINT("%c %d %d %d %d succeeded\n", event, sid, pid, quantity, price);
						printf("\nS <%d> <%d> <%d> <%d>\n", sid, pid, quantity, price);
						Print_Supply();
						printf("DONE\n");	
					} else {
						fprintf(stderr, "%c %d %d %d %d failed\n", event, sid, pid, quantity, price);
					}

					break;
				}

				/* Register customer
				 * C <cid> */
			case 'C':
				{
					int cid;

					sscanf(buff, "%c %d", &event, &cid);
					DPRINT("%c %d\n", event, cid);

					if (RegisterCustomer(cid)) {
						DPRINT("%c %d succeeded\n", event, cid);
						printf("\nC <%d>\n", cid);
						Print_Customers();
						printf("\nDONE\n");
					} else {
						fprintf(stderr, "%c %d failed\n", event, cid);
					}

					break;
				}

				/* Add to shopping list
				 * L <cid> <pid> <quantity> */
			case 'L':
				{
					int cid, pid, quantity;

					sscanf(buff, "%c %d %d %d", &event, &cid, &pid, &quantity);
					DPRINT("%c %d %d %d\n", event, cid, pid, quantity);

					if (AddToShoppingList(cid, pid, quantity)) {
						DPRINT("%c %d %d %d succeeded\n", event, cid, pid, quantity);
						printf("\nL <%d> <%d> <%d>\n", cid, pid, quantity);
						Print_Cust_ShoppingList();
						printf("DONE\n");
					} else {
						fprintf(stderr, "%c %d %d %d failed\n", event, cid, pid, quantity);
					}

					break;
				}

				/* Delete from shopping list
				 * D <cid> <pid> */
			case 'D':
				{
					int cid, pid;

					sscanf(buff, "%c %d %d", &event, &cid, &pid);
					DPRINT("%c %d %d\n", event, cid, pid);

					if (DeleteFromShoppingList(cid, pid)) {
						DPRINT("%c %d %d succeeded\n", event, cid, pid);
						printf("\nD <%d> <%d>\n", cid, pid);
						Print_Cust_ShoppingList();
						printf("DONE\n");	
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, cid, pid);
					}

					break;
				}

				/* Go shopping
				 * G <cid> */
			case 'G':
				{
					int cid;

					sscanf(buff, "%c %d", &event, &cid);
					DPRINT("%c %d\n", event, cid);

					if (GoShopping(cid)) {
						DPRINT("%c %d succeeded\n", event, cid);
						printf("\nG <%d>\n", cid);
						Print_Cust_ShoppingList();
						printf("\n");
						Print_Supply();
						printf("DONE\n");

					} else {
						fprintf(stderr, "%c %d failed\n", event, cid);
					}

					break;
				}

				/* Store close
				 * M <sid1> <sid2> */
			case 'M':
				{
					int sid1, sid2;

					sscanf(buff, "%c %d %d", &event, &sid1, &sid2);
					DPRINT("%c %d %d\n", event, sid1, sid2);

					if (StoreClose(sid1, sid2)) {
						DPRINT("%c %d %d succeeded\n", event, sid1, sid2);
						printf("\nM <%d> <%d>\n", sid1, sid2);
						PrintShops();
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, sid1, sid2);
					}

					break;
				}

				/* Categorize shops
				 * T */
			case 'T':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (CategorizeShops()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Find cheapest products
				 * F */
			case 'F':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (FindCheapestProducts()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print shops
				 * X */
			case 'X':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (PrintShops()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print customers
				 * Y */
			case 'Y':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (PrintCustomers()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Empty line */
			case '\n':
				break;

				/* Ignore everything else */
			default:
				DPRINT("Ignoring buff: %s \n", buff);

				break;
		}
	}

	FreeAll();
	return (EXIT_SUCCESS);
}




/* R event : Print Shop List */
void Display_Shops(void)
{
	printf("\t Shops = ");
	s = H;
	while (1) {
		printf("<%d %d>", s->sid, s->type);
		s = s->next;
		if (s == H) {	/* Reached back to head node.Stop printing */
			break;
		}
		printf(", ");
	}
}


/* S event : Print shops and their product lists */
/* a : pointer to access product list of each shop */
void Print_Supply()
{
	int i = 1;
	s = H;
	
	while (1) {
		printf("\tShop %d = ", i);
		a = s->products;

		/* Loop till you hit NULL */
		while (a) {
			printf("<%d,%d,%d>", a->pid, a->quantity, a->price);
			a = a->next;
			if (a)		/* Last , won't be printed */
				printf(", ");
		}
		printf("\n");
		s = s->next;
		/* End of list shops. Stop printing */
		if (s == H) {
			break;
		}
		i++;		
	}
}


/* C event : Print Customer List */
void Print_Customers()
{
	printf("\t Customers = ");
	x = Customers;
	while (1) {
		printf("<%d>", x->cid);
		x = x->next;
		if (x == Sentinel) {	/* Reached back to head node.Stop printing */
			break;
		}
		printf(", ");
	}
}


/* L event : Print each customer's Shopping List */
void Print_Cust_ShoppingList(void)
{	
	b = Customers;
	
	while (b != Sentinel) {
		printf("\tCustomer %d = ", b->cid);
		q = b->shopping_list;

		/* Loop till you hit NULL */
		while (q) {
			printf("<%d,%d>", q->pid, q->quantity);
			q = q->next;
			if (q)		/* Last , won't be printed */
				printf(", ");
		}
		printf("\n");
		b = b->next;
		/* Loop until you hit the sentinel */	
	}
}



/* Check if a customer is in the list */
int SearchCustomer(int cid)
{
	search_cust = Customers;
	if (!Customers) {	/* Customers List empty */
		return 0;
	}


	/* Search for customer in Customers List */
	Sentinel->cid = cid;
	while (search_cust->cid != cid) {
		search_cust = search_cust->next;
	}

	if (search_cust == Sentinel) {	/* Customer found at Sentinel, return 0 */
		return 0;
	}
	else
		return 1;
}
