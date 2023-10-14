#include "shell.h"
#include "string.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

static size_t str_list_len(const char **str_list)
{
    size_t len = 0;
    if (str_list == NULL)
        return (len);
    while (str_list[len] != NULL)
        len++;
    return (len);
}
sh_session_t *create_sesssion(const char *argv0, const char **envp)
{
    sh_session_t *session = malloc(sizeof(sh_session_t));
    size_t i = 0, len = 0;
    if (session != NULL)
    {
        session->prompt = copy_str("cisfun$");
        session->sh_name = copy_str(argv0);
        len = str_list_len(envp);
        session->env_var_lst = malloc(sizeof(char *) * (len + 1));
        session->env_var_lst[len] = NULL;
        for (i = 0; i < len; ++i)
            session->env_var_lst[i] = copy_str(envp[i]);
    }
    return (session);
}
void free_session(sh_session_t **session)
{
    size_t i;
    if (session == NULL)
    {
        errno = EINVAL;
        return;
    }
    if (*session != NULL)
    {
        if ((*session)->sh_name != NULL)
        {
            free((*session)->sh_name);
            (*session)->sh_name = NULL;
        }
        if ((*session)->env_var_lst != NULL)
        {
            while ((*session)->env_var_lst[i] != NULL)
            {
                free((*session)->env_var_lst[i]);
            }
            free((*session)->env_var_lst);
            (*session)->env_var_lst = NULL;
            free(*session);
            *session = NULL;
        }
    }
}
bool_t is_regular_file(const char *d, const char *f)
{
    char *path = NULL;
    char *tmp = NULL;
    struct stat st;
    bool_t ret = FALSE;
    if (IS_NULL_OR_EMPTY(d) || IS_NULL_OR_EMPTY(f))
    return (ret);
    tmp = concat_str(d, "/");
    path = concat_str(tmp, f);
    if (tmp != NULL)
    {
        free(tmp);
        tmp = NULL;
    }
    if (stat(path, &st) == 0)
    {
        if ((st.st_mode & S_IFMT) == S_IFREG)
            ret = TRUE;
        else
            ret = FALSE;
    }
    if (path != NULL)
    {
        free(path);
        path = NULL;
    }
    return (ret);
}
bool_t file_exists(const char *d, const char *f)
{
    DIR *pDir;
    struct dirent *dir = NULL;
    if (!(IS_NULL_OR_EMPTY(d)) && !(IS_NULL_OR_EMPTY(f)))
    {
        pDir = opendir(d);
        if (pDir == NULL && (errno == EACCES || errno == EBADF))
        {
            perror("open Dir");
            return (FALSE);
        }
        dir = readdir(pDir);
        while (dir != NULL)
        {
            if (str_cmp(dir->d_name, f) == 0)
            {
                if (is_regular_file(d, f) == TRUE)
                {
                    if (closedir(pDir) == -1)
                    {
                        perror("closedir");
                    }
                    return (TRUE);
                }
            }
            dir = readdir(pDir);
        }
        if (dir == NULL && errno == EBADF)
        {
            perror("readdir");
        }
    }
    return (FALSE);
}
char *find_full_path(const char *cmd, const char **paths)
{
    size_t i;
    char *full_path = NULL;
    char *tmp = NULL;
    if (IS_NULL_OR_EMPTY(cmd))
    return (NULL);
    if (paths == NULL || *paths == NULL)
    return (NULL);
    while (paths[i] != NULL)
    {
        if (file_exists(paths[i], cmd))
        {
            tmp = concat_str(paths[i], "/");
            full_path = concat_str(tmp, cmd);
            if (tmp != NULL)
            {
            free(tmp);
            tmp = NULL;
            }
            return (full_path);
        }
        i++;
    }
    return (full_path);
}