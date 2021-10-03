# HASH IMPLEMENTATION

Some hash implementations in pure C.

## Features and Todos

- [x] Feature: Makefile
- [x] Hash: SHA1
- [x] Hash: MD4
- [x] Feature: Rainbow Table - Precompute Phase
- [ ] Feature: Rainbow Table - Attack Phase
- [x] Hash: NTLM

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

## Tool

You can use main.c to test some primitives and implementations.

To hash a custom string, use `-sstring`

```sh
$ make
$ ./build/main -sabc

SHA1 ("abc") = 
        A9 99 3E 36 47 06 81 6A BA 3E 25 71 78 50 C2 6C 9C D0 D8 9D 
MD4 ("abc") = 
        A4 48 01 7A AF 21 D8 52 5F C1 0A E8 7A A6 72 9D 
```

To launch test suite with test vectors, use `-x`

```sh
$ make
$ ./build/main -x

:: SHA1 Test Vectors ::

SHA1 ("") = 
        DA 39 A3 EE 5E 6B 4B 0D 32 55 BF EF 95 60 18 90 AF D8 07 09 
Should match:
        DA 39 A3 EE 5E 6B 4B 0D 32 55 BF EF 95 60 18 90 AF D8 07 09

...
```

And to use rainbow table script use `-r`

```sh
$ make
$ ./build/main -r

:: SHA1 Rainbow Table Test ::

0 -> ... -> ZJ
1 -> ... -> e3
2 -> ... -> VS

...
```