platform='unknown'
unamestr=`uname`
if [[ "$unamestr" == 'Linux' ]]; then
   platform='linux'
elif [[ "$unamestr" == 'Darwin' ]]; then
   platform='freebsd'
fi
if [[ $platform == 'linux' ]]; then
   alias ls='ls --color=auto'
elif [[ $platform == 'freebsd' ]]; then
   alias ls='ls -G'
fi
alias l='ls -lF'
alias ll='ls -alF'
alias lt='ls -altF'
alias lz='ls -alsSF'

alias grep='grep --color=auto'
alias fgrep='fgrep --color=auto'
alias egrep='egrep --color=auto'

alias bc='bc -l'
alias mkdir='mkdir -pv'
alias diff='colordiff'
alias h='history'

alias path='echo -e ${PATH//:/\\n}'
alias now='date +"%Y-%m-%d %T"'

alias ports='netstat -tulanp'

alias reboot='sudo /sbin/reboot'
alias halt='sudo /sbin/halt'
alias shutdown='sudo /sbin/shutdown'

alias ..='cd ..'
alias ...='cd ../../../'
alias ....='cd ../../../../'
alias .....='cd ../../../../'
alias .3='cd ../../../'
alias .4='cd ../../../../'
alias .5='cd ../../../../..'

alias pu='pushd'
alias po='popd'


set -o vi
bind -m vi-insert '\c-l':clear-screen
bind -m vi-insert '\c-e':end-of-line
bind -m vi-insert '\c-a':beginning-of-line
bind -m vi-insert '\c-b':backward-char
bind -m vi-insert '\c-f':forward-char


