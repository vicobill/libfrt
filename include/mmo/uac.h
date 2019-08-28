#pragma once

typedef struct {
	int64_t id;
	char*	name;
	char*	locale;
	char*	lang;
	int		ctime;
	int		tz;	// time zone
	int		coins;
	int		bitcoins;
	struct {
		char* name;
		int	coins;
	}* excoins;

}AccInfo;

void uac_add(void* uacinfo);
void uac_rm(int64_t uacid);
void uac_freeze(int64_t uacid);
void uac_unfreeze(int64_t uacid);
void uac_auth(const void* auth);
void uac_invalid(int64_t uacid);
void uac_stats(int64_t uacid);
void uac_alt(int64_t uacid, const void* uacinfo);
void uac_users(int64_t uacid, int64_t* users);
int  uac_userof(int64_t uacid, int64_t userid);
void uac_link(int64_t uacid, int64_t userid);
void uac_unlink(int64_t uacid, int64_t userid);

void uac_auth_lis(int64_t uacid, const char* pattern);


