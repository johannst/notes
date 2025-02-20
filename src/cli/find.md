# find(1)

```
find <start> [opts]
    -maxdepth <n>        maximally search n dirs deep
    -type <t>            match on file type
                           f    regular file
                           d    directory
    -user <name>         list files owned by username
    -name <glob>         list files matching glob (only filename)
    -iname <glob>        list files matching glob case-insensitive

    -exec <cmd> {} ;     run cmd on each file
    -exec <cmd> {} +     run cmd with all files as argument
```
> Depending on the shell the `<glob>` must be quoted or escaped. The
> exec modifier characters `;` and `+` also may need to be escaped.

### Example `-exec` option
```sh
> find . -maxdepth 1 -type d -exec echo x {} \;
# x .
# x ./.github
# x ./book
# x ./src
# x ./.git
# x ./docs

> find . -maxdepth 1 -type d -exec echo x {} +
# x . ./.github ./book ./src ./.git ./docs
```
