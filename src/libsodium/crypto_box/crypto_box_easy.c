
#include "crypto_box.h"
#include "crypto_secretbox.h"
#include "utils.h"

int
crypto_box_easy_detached(unsigned char *c, unsigned char *mac,
                         const unsigned char *m, unsigned long long mlen,
                         const unsigned char *n, const unsigned char *pk,
                         const unsigned char *sk)
{
    unsigned char k[crypto_box_BEFORENMBYTES];
    int           ret;

    (void) sizeof(int[crypto_box_BEFORENMBYTES >=
                      crypto_secretbox_KEYBYTES ? 1 : -1]);
    crypto_box_beforenm(k, pk, sk);
    ret = crypto_secretbox_easy_detached(c, mac, m, mlen, n, k);
    sodium_memzero(k, sizeof k);

    return ret;
}

int
crypto_box_easy(unsigned char *c, const unsigned char *m,
                unsigned long long mlen, const unsigned char *n,
                const unsigned char *pk, const unsigned char *sk)
{
    return crypto_box_easy_detached(c + crypto_box_MACBYTES, c, m, mlen, n,
                                    pk, sk);
}

int
crypto_box_open_easy_detached(unsigned char *m, const unsigned char *c,
                              const unsigned char *mac,
                              unsigned long long clen, const unsigned char *n,
                              const unsigned char *pk, const unsigned char *sk)
{
    unsigned char k[crypto_box_BEFORENMBYTES];
    int           ret;

    crypto_box_beforenm(k, pk, sk);
    ret = crypto_secretbox_open_easy_detached(m, c, mac, clen, n, k);
    sodium_memzero(k, sizeof k);

    return ret;
}

int
crypto_box_open_easy(unsigned char *m, const unsigned char *c,
                     unsigned long long clen, const unsigned char *n,
                     const unsigned char *pk, const unsigned char *sk)
{
    if (clen < crypto_box_MACBYTES) {
        return -1;
    }
    return crypto_box_open_easy_detached(m, c + crypto_box_MACBYTES, c,
                                         clen - crypto_box_MACBYTES,
                                         n, pk, sk);
}
