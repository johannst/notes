# rev(1)

```sh
# Reverse each line of file(s), character by character.
rev FILE [FILE]

echo -e '123\nabc' | rev
# 321
# cba
```

## Example: remove the last 2 tokens with unknown length
```sh
# If the number of tokens in a line is unkown but we want to remove the last 2
# tokens we can use rev(1).
echo 'aa bb cc dd' | rev | cut -d ' ' -f3- | rev
# aa bb
```
