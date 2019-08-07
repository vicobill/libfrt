#ifndef FUX_SYS_COM_H_INCLUDED
#define FUX_SYS_COM_H_INCLUDED

/**
 * 文件管理
 */
/// file manage
typedef struct SFileStat {
    int     makeTime;
    int     altTime;
    char*   creatorName;
    char*   altorName;
    usize   size;
}SFileStat;

int     file_size(const char* filename);
void    file_base(const char* filename, char* base);
void    file_ext(const char* filename, char* ext);
int     file_read(const FILE* f,char* readbuf,usize len, isize offset);
int     file_write(const FILE* f, const char* writebuf, usize len, isize offset);
int     file_append(const FILE* f,const char* writebuf,usize len);
int     file_stat(const FILE* f,SFileStat* stat);

int     sys_homedir(char* dir);
int     sys_username(char* username);
int     sys_cpunum();
int     sys_osname(char* );
int     sys_osversion();


/// dll file manage
typedef union UDllHandle{
    void* h;
    int id;
}UDllHandle;

int     dll_load(const char* dllname,UDllHandle* out);
int     dll_unload(UDllHandle* handle);
void*   dll_symbol(UDllHandle* handle,const char* symbol);




/**
 * 完整网络相关部分，包括检索网卡信息、操作Socket、高效底层消息泵等
 */
int     net_init(int version);
int     net_shutdown();


int     net_socket_listen()



#endif