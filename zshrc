# Lines configured by zsh-newuser-install
HISTFILE=~/.histfile
HISTSIZE=1000
SAVEHIST=100000
bindkey -e
# End of lines configured by zsh-newuser-install

### COLORS & PROMPT ###
RED=$'%{\033[31m%}'
GREEN=$'%{\033[32m%}'
BLUE=$'%{\033[36m%}'
RESET=$'%{\033[0m%}'

PROMPT="${GREEN}%n@%m ${BLUE}%2~ %(?..${RED}[%?] )${RESET}%# "
setopt promptsubst
case "$(tty)" in
	/dev/tty*)
		bpath='/sys/class/power_supply/BAT0'
		[ -e "$bpath" ] && PROMPT='$(<'"$bpath"'/capacity)%% '"$PROMPT"
		;;
esac

### ENV VARIABLES ###
export EDITOR=vim
export DC_ENV_ARGS='-e 6k'

### SOURCINGS ###
eval "$(zoxide init zsh)"

### ALIASES & ETC ###
alias ls='ls --color -F --group-directories-first'

### GIT ###
alias ga='git add'
alias gst='git status'
alias gl='git pull'
alias glo='git log --oneline'
alias gp='git push'
alias gc='git commit'

### BINDINGS ###
bindkey -s '^z' "fg\n"
