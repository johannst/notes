# cut(1)

```sh
# Remove sections from each line of files(s).
cut OPT FILE [FILE]
    -d DELIM     delimiter to tokenize
    -f LIST      field selector
    -c LIST      character selector
```

## Example: only selected characters

```sh
echo 'aa bb cc dd' | cut -c "1-4"
# aa b

# Inverted selection.
echo 'aa bb cc dd' | cut --complement -c "1-4"
# b cc dd
```

## Example: only selected fields
Fields in `cut` are indexed starting from `1` rather than `0`.
```sh
# Fields 2 until 3.
echo 'aa bb cc dd' | cut -d ' ' -f 2-3
# bb cc

# First field until the 2nd.
echo 'aa bb cc dd' | cut -d ' ' -f -2
# aa bb

# Third field until the end.
echo 'aa bb cc dd' | cut -d ' ' -f 3-
# cc dd

# If the number of tokens in a line is unkown but we want to remove the last 2
# tokens we can use rev(1).
echo 'aa bb cc dd' | rev | cut -d ' ' -f3- | rev
# aa bb
```
