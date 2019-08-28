#pragma once

typedef struct {
	int		coins;	// owner coins
	int		xcoins;	// bit coins
	struct {
		char* name; 
		int coins;
	}* excoins; // external coins
}Curc; // currency

typedef struct {
	int64_t appid;
	int		os;
	int		paypt;
}CurcFrom; // where the currency from

typedef struct {
	char* id;	// string id
	char* from;	// 
	char* to;	// 
	time_t time;

};

void fin_pay(int64_t uacid, const void* order);
void fin_income(int64_t uacid, const void* order);
void fin_charge(int64_t uacid, const void* order);
void fin_cost(int64_t uacid, const void* order);

void fin_pay_lis(int64_t uacid,const char* pattern,void* lis);
void fin_income_lis(int64_t uacid, const char* pattern,void* lis);
void fin_charge_lis(int64_t uacid, const char* pattern,void* lis);
void fin_cost_lis(int64_t uacid,const char* pattern,void* lis);