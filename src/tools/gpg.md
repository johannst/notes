# gpg(1)

```
gpg
  -o|--output                 Specify output file
  -a|--armor                  Create ascii output
  -u|--local-user <name>      Specify key for signing
  -r|--recipient              Encrypt for user
```

## Generate new keypair
```bash
gpg --full-generate-key
```

## List keys
```
gpg -k / --list-key               # public keys
gpg -K / --list-secret-keys       # secret keys
```

## Edit keys
```bash
gpg --edit-key <KEY ID>
```
Gives prompt to modify `KEY ID`, common commands:
```bash
help         show help
save         save & quit

list         list keys and user IDs
key <N>      select subkey <N>
uid <N>      select user ID <N>

expire       change expiration of selected key

adduid       add user ID
deluid       delete selected user ID

addkey       add subkey
delkey       delete selected subkey
```

## Export & Import Keys
```bash
gpg --export --armor --output <KEY.PUB> <KEY ID>
gpg --import <FILE>
```

## Search & Send keys
```bash
gpg --keyserver <SERVER> --send-keys <KEY ID>
gpg --keyserver <SERVER> --search-keys <KEY ID>
```

## Encrypt (passphrase)
Encrypt file using `passphrase` and write encrypted data to `<file>.gpg`.
```bash
gpg --symmetric <file>

# Decrypt using passphrase
gpg -o <file> --decrypt <file>.gpg
```

## Encrypt (public key)
Encrypt file with `public key` of specified `recipient` and write encrypted
data to `<file>.gpg`.
```bash
gpg --encrypt -r foo@bar.de <file>

# Decrypt at foos side (private key required)
gpg -o <file> --decrypt <file>.gpg
```

## Signing
Generate a signed file and write to `<file>.gpg`.
```bash
gpg --sign -u foor@bar.de <file>

# Verify
gpg --verify <file>

# Extract content from signed file
gpg -o <file> --decrypt <file>.gpg
```
> Without `-u` use first private key in list `gpg -K` for signing.

Files can also be `signed` and `encrypted` at once, gpg will first sign the
file and then encrypt it.
```bash
gpg --sign --encrypt <file>
```

## Signing (detached)
Generate a `detached` signature and write to `<file>.asc`.
Send `<file>.asc` along with `<file>` when distributing.
```bash
gpg --detach-sign --armor -u foor@bar.de <file>

# Verify
gpg --verify <file>.asc <file>
```
> Without `-u` use first private key in list `gpg -K` for signing.

## Abbreviations
- `sec` secret key
- `ssb` secret subkey
- `pub` public key
- `sub` public subkey

## Keyservers
- http://pgp.mit.edu
- http://keyserver.ubuntu.com
- hkps://pgp.mailbox.org
