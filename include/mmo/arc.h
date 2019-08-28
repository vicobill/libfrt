#pragma once
/// ARTICLE & COMMENT ====================
typedef struct {

} Arc;

void arc_add(const void* article);
void arc_rm(strid_t arcid);
void arc_stats(strid_t arcid, void* stats);

void arc_comment(strid_t arcid, const void* comment);
void arc_uncomment(strid_t commentid);
void arc_comments(strid_t arcid);
void arc_comment_stats(strid_t comment);
