#pragma once

typedef strid_t		appid_t;

typedef struct {
	appid_t id;
	int		ver;	// version of app
	int		vendor;
	char	*creator;
	char	*credits;
	char	*home_url;	// creator home page url
	char	*surl;	// short url
	char	*name;	// the name of app
	char	*desc;	// description of app
	char	*tags;	// tags of app
	int		ctime;	// create time
	int		atime;	// last alter time
	int		state;	
}AppInfo;

void 	appinfo_add_tag(const char *tag);
void 	appinfo_rm_tag(const char *tag);
bool 	appinfo_has_tag(const char *tag);
int 	appinfo_count_tag(const char *tag = "");

void 	appinfo_add_creator(const char *creator);
void 	appinfo_has_creator(const char *creator);


typedef void(*homeurl_changed_f)(const char *homeurl);
void 	appinfo_set_homeurl(const char *homeurl, homeurl_changed_f);


enum EAppState {
	EAS_None,
	EAS_WaitReview,	// wait for review to publish
	EAS_Review,		// reviewing ,then publish
	EAS_Unpub,		// reviewed ,but not publish
	EAS_Pub,		// published,user can download and play 
	EAS_Frezon,		// frezon, cannot download
	EAS_Invalid,	// invalid, removed
};

void app_add(const AppInfo* app);
void app_rm(appid_t appid);
void app_alt(appid_t appid, const AppInfo* appinfo);
void app_get(appid_t appid,AppInfo* outappinfo);
void app_state(appid_t appid);
void app_find(const char* pattern);
void app_invalid(appid_t appid);


