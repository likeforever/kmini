if [ -d ~/kbash/bashrc ]; then
  for i in ~/kbash/bashrc/*.sh; do
    if [ -r $i ]; then
      . $i
    fi
  done
  unset i
fi
