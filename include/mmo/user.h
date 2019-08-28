#pragma once

typedef int64_t		uid_t;

typedef struct {
	char*	nick;
	char*	url; // home url
	char*	icon;
	char*	bio;
	char*	brief;
	char*	tags;
	int		gender;
	int		locale;
	int		lang;
	int		state;
	int		level;
	int		viplv;
	char*	meta;
}UserProf; // profile

void	user_surl(uid_t userid,char* surl); // short url of user

void	user_tag_add(uid_t userid, const char* tag);
void	user_tag_rm(uid_t userid, const char* tag);
void	user_tag_clr(uid_t userid);

void	user_state_set(uid_t userid, int state);
void	user_state_get(uid_t userid, int* state);

typedef struct {
	char* names;
	char* adrs;
	char* phones;
	char* emails;
	char* idcards;
}UserPrivProf; // private profile



typedef struct {
	appid_t	appid;
	time_t	time;
	int		os;
	int		vendor;
	int		locale;
	uid_t	inviter;
	char*	ip;
	char*	gps;
} UserFrom; // where user from

typedef struct {
	appid_t ingame;
	char*	inroom; // split by `/`, example in scene 3 seat 4 anywhere: 3/4/-1
}UserMot; // user current motion

typedef struct {
	time_t	time;
	char*	op;
	char*	ex;
}UserOp;

typedef struct {
	uid_t		id;
	uid_t		uacid;
	char*		paswd;
	char*		name;
	char**		aprs;  // appraises
	char*		binds;
	UserProf	prof;
	UserPrivProf*	privprof;
	UserFrom*	reg;
	UserFrom*	login;
	UserOp*		oplis;
} UserInfo;

void	user_add(const void* userinfo);
void	user_rm(int64_t userid);
void	user_freeze(int64_t userid, const void* reason);
void	user_unfreeze(int64_t userid, const void* reason);
int		user_invalid(int64_t userid);
void	user_get(int64_t userid, void* userinfo);
void	user_find(int64_t userid, void* userinfo);
void	user_alt(int64_t userid, const void* userinfo);

void	user_auth(const void* auth);
void	user_unauth(const void* auth);

void	user_bind(int64_t userid, const void* user3);
void	user_unbind(int64_t userid, const void* user3);
void	user_binds(int64_t userid, void* binds);

void	user_alt_priv(int64_t userid, const void* privinfo);
int		user_get_priv(int64_t userid, const char* k, char* v);
int		user_set_priv(int64_t userid, const char* k, const char* v);

void	user_pub(int64_t userid, const char** properties);

void	user_set_meta(int64_t userid, const char* k, const char* v);
void	user_get_meta(int64_t userid, const char* k, char* v);

void	user_is_guest(int64_t userid);

// op history

void	user_play_lis(int64_t userid,void* lis);
void	user_auth_lis(int64_t userid,void* lis);
void	user_priv_lis(int64_t userid,void* lis);
void	user_frz_lis(int64_t userid, void* lis);