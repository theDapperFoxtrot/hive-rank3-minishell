#include "../../include/minishell.h"

char    *handle_expansions_quotes(t_ms *shell, const char *str)
{
    shell->exp.closing_quote = find_closing_quote(shell, str, str[shell->exp.i], shell->exp.i);
    if (shell->exp.closing_quote == -1)
    {
        free(shell->exp.result);
        return (ft_strdup(str));
    }
    shell->exp.quote_type = str[shell->exp.i];
    shell->exp.i++;
    while (shell->exp.i < shell->exp.closing_quote)
    {
        if (str[shell->exp.i] == '$' && shell->exp.quote_type == '\"')
        {
            if (str[shell->exp.i + 1] == '{')
            {
                shell->exp.closing_brace = find_closing_brace(str, shell->exp.i + 1);
                if (shell->exp.closing_brace != -1)
                {
                    shell->exp.var_name = ft_substr(str, shell->exp.i, shell->exp.closing_brace - shell->exp.i + 1);
                    shell->exp.value = expand_env_var(shell, shell->exp.var_name, 1);
                    ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value, EXP_BUFFER_SIZE - shell->exp.j);
                    shell->exp.j += ft_strlen(shell->exp.value);
                    free(shell->exp.var_name);
                    free(shell->exp.value);
                    shell->exp.i = shell->exp.closing_brace + 1;
                    continue;
                }
            }
            else
            {
                shell->exp.var_start = shell->exp.i + 1;
                shell->exp.var_len = 0;
                while (str[shell->exp.var_start + shell->exp.var_len] && \
                (ft_isalnum(str[shell->exp.var_start + shell->exp.var_len]) || \
                str[shell->exp.var_start + shell->exp.var_len] == '_' || \
                (shell->exp.var_len == 0 && str[shell->exp.var_start] == '?')))
                    shell->exp.var_len++;
                if (shell->exp.var_len > 0)
                {
                    shell->exp.var_name = ft_substr(str, shell->exp.var_start, shell->exp.var_len);
                    shell->exp.value = expand_env_var(shell, shell->exp.var_name, 0);
                    ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value, EXP_BUFFER_SIZE - shell->exp.j);
                    shell->exp.j += ft_strlen(shell->exp.value);
                    free(shell->exp.var_name);
                    free(shell->exp.value);
                    shell->exp.i = shell->exp.var_start + shell->exp.var_len;
                    continue;
                }
            }
        }
        shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
    }
    shell->exp.i = shell->exp.closing_quote + 1;
    shell->exp.result[shell->exp.j] = '\0';
    return (shell->exp.result);
}
