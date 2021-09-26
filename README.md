# HASH IMPLEMENTATION

Some hash implementations in pure C.

## Features and Todos

- [x] Feature: Makefile
- [x] Hash: SHA1
- [x] Hash: MD4
- [ ] Hash: NTLM
- [ ] Feature: Rainbow Table generation

## Build

Just run:

```sh
$ make
$ ./build/main
```

## Usage

Initialize your hash context and allocate memory for digest.

```c
SHA1Context sha;
uint8_t Message_Digest[SHA1HashSize];

err = SHA1Reset(&sha);
if (err) {
    fprintf(stderr, "SHA1Reset Error %d.\n", err);
}
```

Then update your context with message.

```c
err = SHA1Update(&sha, (const unsigned char *) "abc", 3);
if (err) {
    fprintf(stderr, "SHA1Update Error %d.\n", err);
}
```

And finaly call result function.

```c
err = SHA1Result(&sha, Message_Digest);
if (err) {
    fprintf(stderr, "SHA1Result Error %d, could not compute message digest.\n", err);
} else {
    for (i = 0; i < SHA1HashSize; ++i) {
        printf("%02X ", Message_Digest[i]);
    }
    printf("\n");
}
```

It should print:

```
A9 99 3E 36 47 06 81 6A BA 3E 25 71 78 50 C2 6C 9C D0 D8 9D
```