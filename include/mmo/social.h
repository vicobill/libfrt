#pragma once

/// FRIEND ==========================

// try send make friendship message
void friend_try_add(int64_t fromuserid, int64_t touserid,const char* msg);
// add friend to list
void friend_add(int64_t fromuserid, int64_t touserid);
// remove friend to list
void friend_rm(int64_t fromuserid, int64_t target);
// list friends by pattern
void friend_ls(int64_t fromuserid, const char* pat);
// move friend to blacklist
void friend_black(int64_t fromuserid, int64_t target);
// focus friend
void friend_focus(int64_t fromuserid, int64_t target);
// unfocus 
void friend_unfocus(int64_t fromuserid, int64_t target);

/// RANK ==================================

typedef struct {
	int		id;
	char* name;
	struct {
		int64_t	userid;
		int		score;
	}* users;
} rank_t;

// list all ranks
void	rank_list(rank_t* ranks,int count = 10);
// list a given name rank
void	rank_ls(const char* rankname,const char* pat);
// anyone pos in a rank
int		rank_at(const rank_t* rank, int64_t userid);

/// MAIL ====================================

void mail_send(int64_t sender, int64_t to, const void* mail);
void mail_recv(int64_t userid, void** mails);
void mail_fetch(int64_t userid, void** mailsnapshots);
void mail_gc(int64_t userid, const char* pat);

/// mail accelories
void mail_add_acl(int64_t mailid, const void* acl);
void mail_get_acl(int64_t mailid, void* acls);
void mail_has_acl(int64_t mailid, const char* pat);

void mail_ban(int64_t userid, int64_t targetid);
void mail_unban(int64_t userid, int64_t targetid);
void mail_bans(int64_t userid);

void mail_mark(int64_t mailid, int mark);


/// CHAT =================================
void chat_send(int64_t sender, int channel, const char* msg);
void chat_filter(int64_t sender, int channel, const char* pattern);
void chat_open(int64_t* users, const char* options);
void chat_close_by(int64_t userid,const char* pattern);
void chat_close(int64_t chatid);
void chat_invite(int64_t userid, int64_t* users, int64_t chatid);
void chat_kick(int64_t userid, int64_t chatid);
void chat_enter(int64_t userid, int64_t chatid);
void chat_exit(int64_t userid, int64_t chatid);
void chat_mute(int64_t* users, int64_t chatid);
void chat_unmute(int64_t chatid);
void chat_mono(int64_t userid, int64_t chatid);
void chat_unmono(int64_t userid, int64_t chatid);
void chat_get(int64_t chatid);
void chat_find(int64_t userid, const char* pattern);

/// LIVE ======================================


/// SUPPORT ============================
//type UserSupport{ dones, dos, undos }

void support_call(const char* question);
void support_send(const char* msg);
void support_stats(int64_t supportid);
void support_giveup(int64_t supportid);
void support_scoring(int64_t supportid, int8_t score);

/// INTERACTIVE ==============================

void ia_gift_user(int64_t fromuserid, int64_t touserid, int64_t itemid);
void ia_ban(int64_t* users, int iatype);
void ia_unban(int64_t userid, int iatype);

void ia_share_user(int64_t userid, const void* to);
void ia_share_chat(int64_t chatid, const void* to);
void ia_share_app(int64_t gameid, const void* to);
void ia_share(const void* what, const void* to);

void ia_follow(int64_t userid, int64_t targetuserid);
void ia_focus(int64_t userid, int64_t target);
void ia_unfocus(int64_t userid, int64_t target);

void ia_resp(const void* ia);
void ia_req(const void* ia, const void* req);

void ia_tipoff(const void* what, const char* reason);

