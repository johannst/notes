# column(1)

## Examples
```sh
# Show as table (aligned columns), with comma as delimiter from stdin.
echo -e 'a,b,c\n111,22222,33' | column -t -s ','

# Show file as table.
column -t -s ',' test.csv
```
