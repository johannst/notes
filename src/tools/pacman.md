# pacman(1)

## Remote package repositories

```text
pacman -Sy              refresh package database
pacman -S <pkg>         install pkg
pacman -Ss <regex>      search remote package database
pacman -Si <pkg>        get info for pkg
pacman -Su              upgrade installed packages
pacman -Sc              clean local package cache
```

## Remove packages

```text
pacman -Rsn <pkg>               uninstall package and unneeded deps + config files
```

## Local package database

Local package database of installed packages.

```text
pacman -Q               list all installed packages
pacman -Qs <regex>      search local package database
pacman -Ql <pkg>        list files installed by pkg
pacman -Qo <file>       query package that owns file
pacman -Qe              only list explicitly installed packages
```

## Local file database

Local file database which allows to search packages owning certain files.
Also searches non installed packages, but database must be synced.

```text
pacman -Fy              refresh file database
pacman -Fl <pkg>        list files in pkg (must not be installed)
pacman -Fx <regex>      search 
```

## Hacks

Uninstall all orphaned packages (including config files) that were installed as
dependencies.
```text
pacman -Rsn $(pacman -Qtdq)
```

List explicitly installed packages that are not required as dependency by any
package and sort by size.
```text
pacman -Qetq | xargs pacman -Qi |
    awk '/Name/ { name=$3 }
         /Installed Size/ { printf "%8.2f%s %s\n", $4, $5, name }' |
    sort -h
```
