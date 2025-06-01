// MIT License
//
// Copyright (c) 2021 Kai Zhu
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


// LINT_C_FILE


#if !defined(SRC_SQLMATH_BASE_H3)
#define SRC_SQLMATH_BASE_H3


#if defined(SRC_SQLMATH_BASE_C2)


#define LIGHTGBM_C_EXPORT typedef
#ifdef _WIN32
#define LGBM_DLSYM(func) \
    func = (func##_t) GetProcAddress((HMODULE) lgbm_library, #func);
#else
#define LGBM_DLSYM(func) \
    func = (func##_t) dlsym(lgbm_library, #func);
#endif                          // _WIN32
// https://github.com/microsoft/LightGBM/blob/v4.6.0/include/LightGBM/arrow.h
typedef struct ArrowSchema {
    // Array type description
    const char *format;
    const char *name;
    const char *metadata;
    int64_t flags;
    int64_t n_children;
    struct ArrowSchema **children;
    struct ArrowSchema *dictionary;
    // Release callback
    void (
        *release
    ) (
        struct ArrowSchema *
    );
    // Opaque producer-specific data
    void *private_data;
} ArrowSchema;
typedef struct ArrowArray {
    // Array data description
    int64_t length;
    int64_t null_count;
    int64_t offset;
    int64_t n_buffers;
    int64_t n_children;
    const void **buffers;
    struct ArrowArray **children;
    struct ArrowArray *dictionary;
    // Release callback
    void (
        *release
    ) (
        struct ArrowArray *
    );
    // Opaque producer-specific data
    void *private_data;
} ArrowArray;
static void *lgbm_library = NULL;


#endif                          // SRC_SQLMATH_BASE_C2


// *INDENT-OFF*
/*jslint-disable*/
/*
shRollupFetch
{
    "fetchList": [
        {
            "comment": true,
            "url": "https://github.com/intel/tinycrypt/blob/v0.2.8/LICENSE"
        },
        {
            "url": "https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/sha256.h"
        },
        {
            "header": "\n#if defined(SRC_SQLMATH_BASE_C2)\n",
            "url": "https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/constants.h"
        },
        {
            "url": "https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/utils.h"
        },
        {
            "url": "https://github.com/intel/tinycrypt/blob/v0.2.8/lib/source/utils.c"
        },
        {
            "footer": "\n#endif // SRC_SQLMATH_BASE_C2\n",
            "replaceList": [
                {
                    "aa": "\\bcompress\\b",
                    "bb": "tc_sha256_compress",
                    "flags": "g",
                    "substr": ""
                }
            ],
            "url": "https://github.com/intel/tinycrypt/blob/v0.2.8/lib/source/sha256.c"
        },
        {
            "header": "\n#if defined(SRC_SQLMATH_BASE_C2)\n",
            "replaceList": [
                {
                    "aa": "\nLIGHTGBM_C_EXPORT ([^\\(]*?) (\\w*?)(\\([\\S\\s]*?\\));\n",
                    "bb": "\nLIGHTGBM_C_EXPORT $1 (*$2_t) $3;\nstatic $2_t $2 = NULL;\n",
                    "flags": "g",
                    "substr": ""
                }
            ],
            "url": "https://github.com/microsoft/LightGBM/blob/v4.6.0/include/LightGBM/c_api.h"
        },
        {
            "footer": "\n}\n#endif // SRC_SQLMATH_BASE_C2\n",
            "header": "static void LGBM_dlsym() {\n",
            "replaceList": [
                {
                    "aa": "\nLIGHTGBM_C_EXPORT ([^\\(]*?) (\\w*?)(\\([\\S\\s]*?\\));\n",
                    "bb": "\n^LGBM_DLSYM($2);\n",
                    "flags": "g",
                    "substr": ""
                },
                {
                    "aa": "^[^^].*?\n|^\n",
                    "bb": "",
                    "flags": "gm",
                    "substr": ""
                },
                {
                    "aa": "^[^z]",
                    "bb": "",
                    "flags": "gm",
                    "substr": ""
                }
            ],
            "url": "https://github.com/microsoft/LightGBM/blob/v4.6.0/include/LightGBM/c_api.h"
        },
        {
            "comment": true,
            "header": "\n#if defined(SRC_SQLMATH_NODEJS_C2)\n",
            "url": "https://github.com/nodejs/node/blob/v10.22.1/LICENSE"
        },
        {
            "url": "https://github.com/nodejs/node/blob/v10.22.1/src/node_api_types.h"
        },
        {
            "footer": "\n#endif // SRC_SQLMATH_NODEJS_C2\n",
            "replaceList": [
                {
                    "aa": "^#include \"node_api_types.h\"",
                    "bb": "// hack-nodejs - inline header\n// $&",
                    "flags": "gm",
                    "substr": ""
                }
            ],
            "url": "https://github.com/nodejs/node/blob/v10.22.1/src/node_api.h"
        }
    ],
    "replaceList": [
        {
            "aa": "^#include .\\b(?:LightGBM|tinycrypt|node_api_types)\\b",
            "bb": "// hack-header - inline header\n// $&",
            "flags": "gm",
            "substr": ""
        }
    ]
}
-#define INLINE_FUNCTION inline
+// hack-lightgbm - fix warning
+#define INLINE_FUNCTION static inline

-void _set(void *to, uint8_t val, unsigned int len)
+void _set(void *to, uint8_t val, size_t len)

-void _set(void *to, uint8_t val, unsigned int len);
+void _set(void *to, uint8_t val, size_t len);
*/


/*
repo https://github.com/intel/tinycrypt/tree/v0.2.8
committed 2017-08-29T21:21:56Z
*/


/*
file https://github.com/intel/tinycrypt/blob/v0.2.8/LICENSE
*/
/*
================================================================================

                     TinyCrypt Cryptographic Library

================================================================================

          Copyright (c) 2017, Intel Corporation. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

  - Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

  - Neither the name of the Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

================================================================================
Copyright (c) 2014, Kenneth MacKay
All rights reserved.

https://github.com/kmackay/micro-ecc

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

================================================================================
*/


/*
file https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/sha256.h
*/
/* sha256.h - TinyCrypt interface to a SHA-256 implementation */

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief Interface to a SHA-256 implementation.
 *
 *  Overview:   SHA-256 is a NIST approved cryptographic hashing algorithm
 *              specified in FIPS 180. A hash algorithm maps data of arbitrary
 *              size to data of fixed length.
 *
 *  Security:   SHA-256 provides 128 bits of security against collision attacks
 *              and 256 bits of security against pre-image attacks. SHA-256 does
 *              NOT behave like a random oracle, but it can be used as one if
 *              the string being hashed is prefix-free encoded before hashing.
 *
 *  Usage:      1) call tc_sha256_init to initialize a struct
 *              tc_sha256_state_struct before hashing a new string.
 *
 *              2) call tc_sha256_update to hash the next string segment;
 *              tc_sha256_update can be called as many times as needed to hash
 *              all of the segments of a string; the order is important.
 *
 *              3) call tc_sha256_final to out put the digest from a hashing
 *              operation.
 */

#ifndef __TC_SHA256_H__
#define __TC_SHA256_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TC_SHA256_BLOCK_SIZE (64)
#define TC_SHA256_DIGEST_SIZE (32)
#define TC_SHA256_STATE_BLOCKS (TC_SHA256_DIGEST_SIZE/4)

struct tc_sha256_state_struct {
	unsigned int iv[TC_SHA256_STATE_BLOCKS];
	uint64_t bits_hashed;
	uint8_t leftover[TC_SHA256_BLOCK_SIZE];
	size_t leftover_offset;
};

typedef struct tc_sha256_state_struct *TCSha256State_t;

/**
 *  @brief SHA256 initialization procedure
 *  Initializes s
 *  @return returns TC_CRYPTO_SUCCESS (1)
 *          returns TC_CRYPTO_FAIL (0) if s == NULL
 *  @param s Sha256 state struct
 */
int tc_sha256_init(TCSha256State_t s);

/**
 *  @brief SHA256 update procedure
 *  Hashes data_length bytes addressed by data into state s
 *  @return returns TC_CRYPTO_SUCCESS (1)
 *          returns TC_CRYPTO_FAIL (0) if:
 *                s == NULL,
 *                s->iv == NULL,
 *                data == NULL
 *  @note Assumes s has been initialized by tc_sha256_init
 *  @warning The state buffer 'leftover' is left in memory after processing
 *           If your application intends to have sensitive data in this
 *           buffer, remind to erase it after the data has been processed
 *  @param s Sha256 state struct
 *  @param data message to hash
 *  @param datalen length of message to hash
 */
int tc_sha256_update (TCSha256State_t s, const uint8_t *data, size_t datalen);

/**
 *  @brief SHA256 final procedure
 *  Inserts the completed hash computation into digest
 *  @return returns TC_CRYPTO_SUCCESS (1)
 *          returns TC_CRYPTO_FAIL (0) if:
 *                s == NULL,
 *                s->iv == NULL,
 *                digest == NULL
 *  @note Assumes: s has been initialized by tc_sha256_init
 *        digest points to at least TC_SHA256_DIGEST_SIZE bytes
 *  @warning The state buffer 'leftover' is left in memory after processing
 *           If your application intends to have sensitive data in this
 *           buffer, remind to erase it after the data has been processed
 *  @param digest unsigned eight bit integer
 *  @param Sha256 state struct
 */
int tc_sha256_final(uint8_t *digest, TCSha256State_t s);

#ifdef __cplusplus
}
#endif

#endif /* __TC_SHA256_H__ */


/*
file https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/constants.h
*/

#if defined(SRC_SQLMATH_BASE_C2)
/* constants.h - TinyCrypt interface to constants */

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief -- Interface to constants.
 *
 */

#ifndef __TC_CONSTANTS_H__
#define __TC_CONSTANTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#define TC_CRYPTO_SUCCESS 1
#define TC_CRYPTO_FAIL 0

#define TC_ZERO_BYTE 0x00

#ifdef __cplusplus
}
#endif

#endif /* __TC_CONSTANTS_H__ */


/*
file https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/utils.h
*/
/* utils.h - TinyCrypt interface to platform-dependent run-time operations */

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief Interface to platform-dependent run-time operations.
 *
 */

#ifndef __TC_UTILS_H__
#define __TC_UTILS_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Copy the the buffer 'from' to the buffer 'to'.
 * @return returns TC_CRYPTO_SUCCESS (1)
 *         returns TC_CRYPTO_FAIL (0) if:
 *                from_len > to_len.
 *
 * @param to OUT -- destination buffer
 * @param to_len IN -- length of destination buffer
 * @param from IN -- origin buffer
 * @param from_len IN -- length of origin buffer
 */
unsigned int _copy(uint8_t *to, unsigned int to_len,
	           const uint8_t *from, unsigned int from_len);

/**
 * @brief Set the value 'val' into the buffer 'to', 'len' times.
 *
 * @param to OUT -- destination buffer
 * @param val IN -- value to be set in 'to'
 * @param len IN -- number of times the value will be copied
 */
void _set(void *to, uint8_t val, size_t len);

/*
 * @brief AES specific doubling function, which utilizes
 * the finite field used by AES.
 * @return Returns a^2
 *
 * @param a IN/OUT -- value to be doubled
 */
uint8_t _double_byte(uint8_t a);

/*
 * @brief Constant-time algorithm to compare if two sequences of bytes are equal
 * @return Returns 0 if equal, and non-zero otherwise
 *
 * @param a IN -- sequence of bytes a
 * @param b IN -- sequence of bytes b
 * @param size IN -- size of sequences a and b
 */
int _compare(const uint8_t *a, const uint8_t *b, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __TC_UTILS_H__ */


/*
file https://github.com/intel/tinycrypt/blob/v0.2.8/lib/source/utils.c
*/
/* utils.c - TinyCrypt platform-dependent run-time operations */

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

// hack-header - inline header
// #include <tinycrypt/utils.h>
// hack-header - inline header
// #include <tinycrypt/constants.h>

#include <string.h>

#define MASK_TWENTY_SEVEN 0x1b

unsigned int _copy(uint8_t *to, unsigned int to_len,
		   const uint8_t *from, unsigned int from_len)
{
	if (from_len <= to_len) {
		(void)memcpy(to, from, from_len);
		return from_len;
	} else {
		return TC_CRYPTO_FAIL;
	}
}

void _set(void *to, uint8_t val, size_t len)
{
	(void)memset(to, val, len);
}

/*
 * Doubles the value of a byte for values up to 127.
 */
uint8_t _double_byte(uint8_t a)
{
	return ((a<<1) ^ ((a>>7) * MASK_TWENTY_SEVEN));
}

int _compare(const uint8_t *a, const uint8_t *b, size_t size)
{
	const uint8_t *tempa = a;
	const uint8_t *tempb = b;
	uint8_t result = 0;

	for (unsigned int i = 0; i < size; i++) {
		result |= tempa[i] ^ tempb[i];
	}
	return result;
}


/*
file https://github.com/intel/tinycrypt/blob/v0.2.8/lib/source/sha256.c
*/
/* sha256.c - TinyCrypt SHA-256 crypto hash algorithm implementation */

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

// hack-header - inline header
// #include <tinycrypt/sha256.h>
// hack-header - inline header
// #include <tinycrypt/constants.h>
// hack-header - inline header
// #include <tinycrypt/utils.h>

static void tc_sha256_compress(unsigned int *iv, const uint8_t *data);

int tc_sha256_init(TCSha256State_t s)
{
	/* input sanity check: */
	if (s == (TCSha256State_t) 0) {
		return TC_CRYPTO_FAIL;
	}

	/*
	 * Setting the initial state values.
	 * These values correspond to the first 32 bits of the fractional parts
	 * of the square roots of the first 8 primes: 2, 3, 5, 7, 11, 13, 17
	 * and 19.
	 */
	_set((uint8_t *) s, 0x00, sizeof(*s));
	s->iv[0] = 0x6a09e667;
	s->iv[1] = 0xbb67ae85;
	s->iv[2] = 0x3c6ef372;
	s->iv[3] = 0xa54ff53a;
	s->iv[4] = 0x510e527f;
	s->iv[5] = 0x9b05688c;
	s->iv[6] = 0x1f83d9ab;
	s->iv[7] = 0x5be0cd19;

	return TC_CRYPTO_SUCCESS;
}

int tc_sha256_update(TCSha256State_t s, const uint8_t *data, size_t datalen)
{
	/* input sanity check: */
	if (s == (TCSha256State_t) 0 ||
	    data == (void *) 0) {
		return TC_CRYPTO_FAIL;
	} else if (datalen == 0) {
		return TC_CRYPTO_SUCCESS;
	}

	while (datalen-- > 0) {
		s->leftover[s->leftover_offset++] = *(data++);
		if (s->leftover_offset >= TC_SHA256_BLOCK_SIZE) {
			tc_sha256_compress(s->iv, s->leftover);
			s->leftover_offset = 0;
			s->bits_hashed += (TC_SHA256_BLOCK_SIZE << 3);
		}
	}

	return TC_CRYPTO_SUCCESS;
}

int tc_sha256_final(uint8_t *digest, TCSha256State_t s)
{
	unsigned int i;

	/* input sanity check: */
	if (digest == (uint8_t *) 0 ||
	    s == (TCSha256State_t) 0) {
		return TC_CRYPTO_FAIL;
	}

	s->bits_hashed += (s->leftover_offset << 3);

	s->leftover[s->leftover_offset++] = 0x80; /* always room for one byte */
	if (s->leftover_offset > (sizeof(s->leftover) - 8)) {
		/* there is not room for all the padding in this block */
		_set(s->leftover + s->leftover_offset, 0x00,
		     sizeof(s->leftover) - s->leftover_offset);
		tc_sha256_compress(s->iv, s->leftover);
		s->leftover_offset = 0;
	}

	/* add the padding and the length in big-Endian format */
	_set(s->leftover + s->leftover_offset, 0x00,
	     sizeof(s->leftover) - 8 - s->leftover_offset);
	s->leftover[sizeof(s->leftover) - 1] = (uint8_t)(s->bits_hashed);
	s->leftover[sizeof(s->leftover) - 2] = (uint8_t)(s->bits_hashed >> 8);
	s->leftover[sizeof(s->leftover) - 3] = (uint8_t)(s->bits_hashed >> 16);
	s->leftover[sizeof(s->leftover) - 4] = (uint8_t)(s->bits_hashed >> 24);
	s->leftover[sizeof(s->leftover) - 5] = (uint8_t)(s->bits_hashed >> 32);
	s->leftover[sizeof(s->leftover) - 6] = (uint8_t)(s->bits_hashed >> 40);
	s->leftover[sizeof(s->leftover) - 7] = (uint8_t)(s->bits_hashed >> 48);
	s->leftover[sizeof(s->leftover) - 8] = (uint8_t)(s->bits_hashed >> 56);

	/* hash the padding and length */
	tc_sha256_compress(s->iv, s->leftover);

	/* copy the iv out to digest */
	for (i = 0; i < TC_SHA256_STATE_BLOCKS; ++i) {
		unsigned int t = *((unsigned int *) &s->iv[i]);
		*digest++ = (uint8_t)(t >> 24);
		*digest++ = (uint8_t)(t >> 16);
		*digest++ = (uint8_t)(t >> 8);
		*digest++ = (uint8_t)(t);
	}

	/* destroy the current state */
	_set(s, 0, sizeof(*s));

	return TC_CRYPTO_SUCCESS;
}

/*
 * Initializing SHA-256 Hash constant words K.
 * These values correspond to the first 32 bits of the fractional parts of the
 * cube roots of the first 64 primes between 2 and 311.
 */
static const unsigned int k256[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
	0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
	0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static inline unsigned int ROTR(unsigned int a, unsigned int n)
{
	return (((a) >> n) | ((a) << (32 - n)));
}

#define Sigma0(a)(ROTR((a), 2) ^ ROTR((a), 13) ^ ROTR((a), 22))
#define Sigma1(a)(ROTR((a), 6) ^ ROTR((a), 11) ^ ROTR((a), 25))
#define sigma0(a)(ROTR((a), 7) ^ ROTR((a), 18) ^ ((a) >> 3))
#define sigma1(a)(ROTR((a), 17) ^ ROTR((a), 19) ^ ((a) >> 10))

#define Ch(a, b, c)(((a) & (b)) ^ ((~(a)) & (c)))
#define Maj(a, b, c)(((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))

static inline unsigned int BigEndian(const uint8_t **c)
{
	unsigned int n = 0;

	n = (((unsigned int)(*((*c)++))) << 24);
	n |= ((unsigned int)(*((*c)++)) << 16);
	n |= ((unsigned int)(*((*c)++)) << 8);
	n |= ((unsigned int)(*((*c)++)));
	return n;
}

static void tc_sha256_compress(unsigned int *iv, const uint8_t *data)
{
	unsigned int a, b, c, d, e, f, g, h;
	unsigned int s0, s1;
	unsigned int t1, t2;
	unsigned int work_space[16];
	unsigned int n;
	unsigned int i;

	a = iv[0]; b = iv[1]; c = iv[2]; d = iv[3];
	e = iv[4]; f = iv[5]; g = iv[6]; h = iv[7];

	for (i = 0; i < 16; ++i) {
		n = BigEndian(&data);
		t1 = work_space[i] = n;
		t1 += h + Sigma1(e) + Ch(e, f, g) + k256[i];
		t2 = Sigma0(a) + Maj(a, b, c);
		h = g; g = f; f = e; e = d + t1;
		d = c; c = b; b = a; a = t1 + t2;
	}

	for ( ; i < 64; ++i) {
		s0 = work_space[(i+1)&0x0f];
		s0 = sigma0(s0);
		s1 = work_space[(i+14)&0x0f];
		s1 = sigma1(s1);

		t1 = work_space[i&0xf] += s0 + s1 + work_space[(i+9)&0xf];
		t1 += h + Sigma1(e) + Ch(e, f, g) + k256[i];
		t2 = Sigma0(a) + Maj(a, b, c);
		h = g; g = f; f = e; e = d + t1;
		d = c; c = b; b = a; a = t1 + t2;
	}

	iv[0] += a; iv[1] += b; iv[2] += c; iv[3] += d;
	iv[4] += e; iv[5] += f; iv[6] += g; iv[7] += h;
}
#endif // SRC_SQLMATH_BASE_C2


/*
repo https://github.com/microsoft/LightGBM/tree/v4.6.0
committed 2025-02-14T20:27:28Z
*/


/*
file https://github.com/microsoft/LightGBM/blob/v4.6.0/include/LightGBM/c_api.h
*/

#if defined(SRC_SQLMATH_BASE_C2)
/*!
 * \file c_api.h
 * \copyright Copyright (c) 2016 Microsoft Corporation. All rights reserved.
 *            Licensed under the MIT License. See LICENSE file in the project root for license information.
 * \note
 * To avoid type conversion on large data, the most of our exposed interface supports both float32 and float64,
 * except the following:
 * 1. gradient and Hessian;
 * 2. current score for training and validation data.
 * .
 * The reason is that they are called frequently, and the type conversion on them may be time-cost.
 */
#ifndef LIGHTGBM_C_API_H_
#define LIGHTGBM_C_API_H_

// hack-header - inline header
// #include <LightGBM/arrow.h>
// hack-header - inline header
// #include <LightGBM/export.h>

#ifdef __cplusplus
#include <cstdint>
#include <cstdio>
#include <cstring>
#else
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#endif


typedef void* DatasetHandle;  /*!< \brief Handle of dataset. */
typedef void* BoosterHandle;  /*!< \brief Handle of booster. */
typedef void* FastConfigHandle; /*!< \brief Handle of FastConfig. */
typedef void* ByteBufferHandle; /*!< \brief Handle of ByteBuffer. */

#define C_API_DTYPE_FLOAT32 (0)  /*!< \brief float32 (single precision float). */
#define C_API_DTYPE_FLOAT64 (1)  /*!< \brief float64 (double precision float). */
#define C_API_DTYPE_INT32   (2)  /*!< \brief int32. */
#define C_API_DTYPE_INT64   (3)  /*!< \brief int64. */

#define C_API_PREDICT_NORMAL     (0)  /*!< \brief Normal prediction, with transform (if needed). */
#define C_API_PREDICT_RAW_SCORE  (1)  /*!< \brief Predict raw score. */
#define C_API_PREDICT_LEAF_INDEX (2)  /*!< \brief Predict leaf index. */
#define C_API_PREDICT_CONTRIB    (3)  /*!< \brief Predict feature contributions (SHAP values). */

#define C_API_MATRIX_TYPE_CSR (0)  /*!< \brief CSR sparse matrix type. */
#define C_API_MATRIX_TYPE_CSC (1)  /*!< \brief CSC sparse matrix type. */

#define C_API_FEATURE_IMPORTANCE_SPLIT (0)  /*!< \brief Split type of feature importance. */
#define C_API_FEATURE_IMPORTANCE_GAIN  (1)  /*!< \brief Gain type of feature importance. */

/*!
 * \brief Get string message of the last error.
 * \return Error information
 */
LIGHTGBM_C_EXPORT const char* (*LGBM_GetLastError_t) ();
static LGBM_GetLastError_t LGBM_GetLastError = NULL;

/*!
 * \brief Dump all parameter names with their aliases to JSON.
 * \param buffer_len String buffer length, if ``buffer_len < out_len``, you should re-allocate buffer
 * \param[out] out_len Actual output length
 * \param[out] out_str JSON format string of parameters, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DumpParamAliases_t) (int64_t buffer_len,
                                            int64_t* out_len,
                                            char* out_str);
static LGBM_DumpParamAliases_t LGBM_DumpParamAliases = NULL;

/*!
 * \brief Register a callback function for log redirecting.
 * \param callback The callback function to register
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_RegisterLogCallback_t) (void (*callback)(const char*));
static LGBM_RegisterLogCallback_t LGBM_RegisterLogCallback = NULL;

/*!
 * \brief Get number of samples based on parameters and total number of rows of data.
 * \param num_total_row Number of total rows
 * \param parameters Additional parameters, namely, ``bin_construct_sample_cnt`` is used to calculate returned value
 * \param[out] out Number of samples. This value is used to pre-allocate memory to hold sample indices when calling ``LGBM_SampleIndices``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_GetSampleCount_t) (int32_t num_total_row,
                                          const char* parameters,
                                          int* out);
static LGBM_GetSampleCount_t LGBM_GetSampleCount = NULL;

/*!
 * \brief Create sample indices for total number of rows.
 * \note
 * You should pre-allocate memory for ``out``, you can get its length by ``LGBM_GetSampleCount``.
 * \param num_total_row Number of total rows
 * \param parameters Additional parameters, namely, ``bin_construct_sample_cnt`` and ``data_random_seed`` are used to produce the output
 * \param[out] out Created indices, type is int32_t
 * \param[out] out_len Number of indices
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_SampleIndices_t) (int32_t num_total_row,
                                         const char* parameters,
                                         void* out,
                                         int32_t* out_len);
static LGBM_SampleIndices_t LGBM_SampleIndices = NULL;

/*!
 * \brief Get a ByteBuffer value at an index.
 * \param handle Handle of byte buffer to be read
 * \param index Index of value to return
 * \param[out] out_val Byte value at index to return
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_ByteBufferGetAt_t) (ByteBufferHandle handle, int32_t index, uint8_t* out_val);
static LGBM_ByteBufferGetAt_t LGBM_ByteBufferGetAt = NULL;

/*!
 * \brief Free space for byte buffer.
 * \param handle Handle of byte buffer to be freed
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_ByteBufferFree_t) (ByteBufferHandle handle);
static LGBM_ByteBufferFree_t LGBM_ByteBufferFree = NULL;

/* --- start Dataset interface */

/*!
 * \brief Load dataset from file (like LightGBM CLI version does).
 * \param filename The name of the file
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out A loaded dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromFile_t) (const char* filename,
                                                 const char* parameters,
                                                 const DatasetHandle reference,
                                                 DatasetHandle* out);
static LGBM_DatasetCreateFromFile_t LGBM_DatasetCreateFromFile = NULL;

/*!
 * \brief Allocate the space for dataset and bucket feature bins according to sampled data.
 * \param sample_data Sampled data, grouped by the column
 * \param sample_indices Indices of sampled data
 * \param ncol Number of columns
 * \param num_per_col Size of each sampling column
 * \param num_sample_row Number of sampled rows
 * \param num_local_row Total number of rows local to machine
 * \param num_dist_row Number of total distributed rows
 * \param parameters Additional parameters
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromSampledColumn_t) (double** sample_data,
                                                          int** sample_indices,
                                                          int32_t ncol,
                                                          const int* num_per_col,
                                                          int32_t num_sample_row,
                                                          int32_t num_local_row,
                                                          int64_t num_dist_row,
                                                          const char* parameters,
                                                          DatasetHandle* out);
static LGBM_DatasetCreateFromSampledColumn_t LGBM_DatasetCreateFromSampledColumn = NULL;

/*!
 * \brief Allocate the space for dataset and bucket feature bins according to reference dataset.
 * \param reference Used to align bin mapper with other dataset
 * \param num_total_row Number of total rows
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateByReference_t) (const DatasetHandle reference,
                                                    int64_t num_total_row,
                                                    DatasetHandle* out);
static LGBM_DatasetCreateByReference_t LGBM_DatasetCreateByReference = NULL;

/*!
 * \brief Initialize the Dataset for streaming.
 * \param dataset Handle of dataset
 * \param has_weights Whether the dataset has Metadata weights
 * \param has_init_scores Whether the dataset has Metadata initial scores
 * \param has_queries Whether the dataset has Metadata queries/groups
 * \param nclasses Number of initial score classes
 * \param nthreads Number of external threads that will use the PushRows APIs
 * \param omp_max_threads Maximum number of OpenMP threads (-1 for default)
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetInitStreaming_t) (DatasetHandle dataset,
                                                int32_t has_weights,
                                                int32_t has_init_scores,
                                                int32_t has_queries,
                                                int32_t nclasses,
                                                int32_t nthreads,
                                                int32_t omp_max_threads);
static LGBM_DatasetInitStreaming_t LGBM_DatasetInitStreaming = NULL;

/*!
 * \brief Allocate the space for dataset and bucket feature bins according to serialized reference dataset.
 * \param ref_buffer A binary representation of the dataset schema (feature groups, bins, etc.)
 * \param ref_buffer_size The size of the reference array in bytes
 * \param num_row Number of total rows the dataset will contain
 * \param num_classes Number of classes (will be used only in case of multiclass and specifying initial scores)
 * \param parameters Additional parameters
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromSerializedReference_t) (const void* ref_buffer,
                                                                int32_t ref_buffer_size,
                                                                int64_t num_row,
                                                                int32_t num_classes,
                                                                const char* parameters,
                                                                DatasetHandle* out);
static LGBM_DatasetCreateFromSerializedReference_t LGBM_DatasetCreateFromSerializedReference = NULL;

/*!
 * \brief Push data to existing dataset, if ``nrow + start_row == num_total_row``, will call ``dataset->FinishLoad``.
 * \param dataset Handle of dataset
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param start_row Row start index
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetPushRows_t) (DatasetHandle dataset,
                                           const void* data,
                                           int data_type,
                                           int32_t nrow,
                                           int32_t ncol,
                                           int32_t start_row);
static LGBM_DatasetPushRows_t LGBM_DatasetPushRows = NULL;

/*!
 * \brief Push data to existing dataset.
 *        The general flow for a streaming scenario is:
 *        1. create Dataset "schema" (e.g. ``LGBM_DatasetCreateFromSampledColumn``)
 *        2. init them for thread-safe streaming (``LGBM_DatasetInitStreaming``)
 *        3. push data (``LGBM_DatasetPushRowsWithMetadata`` or ``LGBM_DatasetPushRowsByCSRWithMetadata``)
 *        4. call ``LGBM_DatasetMarkFinished``
 * \param dataset Handle of dataset
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of feature columns
 * \param start_row Row start index, i.e., the index at which to start inserting data
 * \param label Pointer to array with nrow labels
 * \param weight Optional pointer to array with nrow weights
 * \param init_score Optional pointer to array with nrow*nclasses initial scores, in column format
 * \param query Optional pointer to array with nrow query values
 * \param tid The id of the calling thread, from 0...N-1 threads
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetPushRowsWithMetadata_t) (DatasetHandle dataset,
                                                       const void* data,
                                                       int data_type,
                                                       int32_t nrow,
                                                       int32_t ncol,
                                                       int32_t start_row,
                                                       const float* label,
                                                       const float* weight,
                                                       const double* init_score,
                                                       const int32_t* query,
                                                       int32_t tid);
static LGBM_DatasetPushRowsWithMetadata_t LGBM_DatasetPushRowsWithMetadata = NULL;

/*!
 * \brief Push data to existing dataset, if ``nrow + start_row == num_total_row``, will call ``dataset->FinishLoad``.
 * \param dataset Handle of dataset
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param start_row Row start index
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetPushRowsByCSR_t) (DatasetHandle dataset,
                                                const void* indptr,
                                                int indptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t nindptr,
                                                int64_t nelem,
                                                int64_t num_col,
                                                int64_t start_row);
static LGBM_DatasetPushRowsByCSR_t LGBM_DatasetPushRowsByCSR = NULL;

/*!
 * \brief Push CSR data to existing dataset. (See ``LGBM_DatasetPushRowsWithMetadata`` for more details.)
 * \param dataset Handle of dataset
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param start_row Row start index
 * \param label Pointer to array with nindptr-1 labels
 * \param weight Optional pointer to array with nindptr-1 weights
 * \param init_score Optional pointer to array with (nindptr-1)*nclasses initial scores, in column format
 * \param query Optional pointer to array with nindptr-1 query values
 * \param tid The id of the calling thread, from 0...N-1 threads
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetPushRowsByCSRWithMetadata_t) (DatasetHandle dataset,
                                                            const void* indptr,
                                                            int indptr_type,
                                                            const int32_t* indices,
                                                            const void* data,
                                                            int data_type,
                                                            int64_t nindptr,
                                                            int64_t nelem,
                                                            int64_t start_row,
                                                            const float* label,
                                                            const float* weight,
                                                            const double* init_score,
                                                            const int32_t* query,
                                                            int32_t tid);
static LGBM_DatasetPushRowsByCSRWithMetadata_t LGBM_DatasetPushRowsByCSRWithMetadata = NULL;

/*!
 * \brief Set whether or not the Dataset waits for a manual MarkFinished call or calls FinishLoad on itself automatically.
 *        Set to 1 for streaming scenario, and use ``LGBM_DatasetMarkFinished`` to manually finish the Dataset.
 * \param dataset Handle of dataset
 * \param wait Whether to wait or not (1 or 0)
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSetWaitForManualFinish_t) (DatasetHandle dataset, int wait);
static LGBM_DatasetSetWaitForManualFinish_t LGBM_DatasetSetWaitForManualFinish = NULL;

/*!
 * \brief Mark the Dataset as complete by calling ``dataset->FinishLoad``.
 * \param dataset Handle of dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetMarkFinished_t) (DatasetHandle dataset);
static LGBM_DatasetMarkFinished_t LGBM_DatasetMarkFinished = NULL;

/*!
 * \brief Create a dataset from CSR format.
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromCSR_t) (const void* indptr,
                                                int indptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t nindptr,
                                                int64_t nelem,
                                                int64_t num_col,
                                                const char* parameters,
                                                const DatasetHandle reference,
                                                DatasetHandle* out);
static LGBM_DatasetCreateFromCSR_t LGBM_DatasetCreateFromCSR = NULL;

/*!
 * \brief Create a dataset from CSR format through callbacks.
 * \param get_row_funptr Pointer to ``std::function<void(int idx, std::vector<std::pair<int, double>>& ret)>``
 *                       (called for every row and expected to clear and fill ``ret``)
 * \param num_rows Number of rows
 * \param num_col Number of columns
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromCSRFunc_t) (void* get_row_funptr,
                                                    int num_rows,
                                                    int64_t num_col,
                                                    const char* parameters,
                                                    const DatasetHandle reference,
                                                    DatasetHandle* out);
static LGBM_DatasetCreateFromCSRFunc_t LGBM_DatasetCreateFromCSRFunc = NULL;

/*!
 * \brief Create a dataset from CSC format.
 * \param col_ptr Pointer to column headers
 * \param col_ptr_type Type of ``col_ptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to row indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol_ptr Number of columns in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_row Number of rows
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromCSC_t) (const void* col_ptr,
                                                int col_ptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t ncol_ptr,
                                                int64_t nelem,
                                                int64_t num_row,
                                                const char* parameters,
                                                const DatasetHandle reference,
                                                DatasetHandle* out);
static LGBM_DatasetCreateFromCSC_t LGBM_DatasetCreateFromCSC = NULL;

/*!
 * \brief Create dataset from dense matrix.
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param is_row_major 1 for row-major, 0 for column-major
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromMat_t) (const void* data,
                                                int data_type,
                                                int32_t nrow,
                                                int32_t ncol,
                                                int is_row_major,
                                                const char* parameters,
                                                const DatasetHandle reference,
                                                DatasetHandle* out);
static LGBM_DatasetCreateFromMat_t LGBM_DatasetCreateFromMat = NULL;

/*!
 * \brief Create dataset from array of dense matrices.
 * \param nmat Number of dense matrices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param is_row_major Pointer to the data layouts. 1 for row-major, 0 for column-major
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromMats_t) (int32_t nmat,
                                                 const void** data,
                                                 int data_type,
                                                 int32_t* nrow,
                                                 int32_t ncol,
                                                 int* is_row_major,
                                                 const char* parameters,
                                                 const DatasetHandle reference,
                                                 DatasetHandle* out);
static LGBM_DatasetCreateFromMats_t LGBM_DatasetCreateFromMats = NULL;

/*!
 * \brief Create dataset from Arrow.
 * \param n_chunks The number of Arrow arrays passed to this function
 * \param chunks Pointer to the list of Arrow arrays
 * \param schema Pointer to the schema of all Arrow arrays
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromArrow_t) (int64_t n_chunks,
                                                  const ArrowArray* chunks,
                                                  const ArrowSchema* schema,
                                                  const char* parameters,
                                                  const DatasetHandle reference,
                                                  DatasetHandle *out);
static LGBM_DatasetCreateFromArrow_t LGBM_DatasetCreateFromArrow = NULL;

/*!
 * \brief Create subset of a data.
 * \param handle Handle of full dataset
 * \param used_row_indices Indices used in subset
 * \param num_used_row_indices Length of ``used_row_indices``
 * \param parameters Additional parameters
 * \param[out] out Subset of data
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetSubset_t) (const DatasetHandle handle,
                                            const int32_t* used_row_indices,
                                            int32_t num_used_row_indices,
                                            const char* parameters,
                                            DatasetHandle* out);
static LGBM_DatasetGetSubset_t LGBM_DatasetGetSubset = NULL;

/*!
 * \brief Save feature names to dataset.
 * \param handle Handle of dataset
 * \param feature_names Feature names
 * \param num_feature_names Number of feature names
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSetFeatureNames_t) (DatasetHandle handle,
                                                  const char** feature_names,
                                                  int num_feature_names);
static LGBM_DatasetSetFeatureNames_t LGBM_DatasetSetFeatureNames = NULL;

/*!
 * \brief Get feature names of dataset.
 * \param handle Handle of dataset
 * \param len Number of ``char*`` pointers stored at ``out_strs``.
 *            If smaller than the max size, only this many strings are copied
 * \param[out] num_feature_names Number of feature names
 * \param buffer_len Size of pre-allocated strings.
 *                   Content is copied up to ``buffer_len - 1`` and null-terminated
 * \param[out] out_buffer_len String sizes required to do the full string copies
 * \param[out] feature_names Feature names, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetFeatureNames_t) (DatasetHandle handle,
                                                  const int len,
                                                  int* num_feature_names,
                                                  const size_t buffer_len,
                                                  size_t* out_buffer_len,
                                                  char** feature_names);
static LGBM_DatasetGetFeatureNames_t LGBM_DatasetGetFeatureNames = NULL;

/*!
 * \brief Free space for dataset.
 * \param handle Handle of dataset to be freed
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetFree_t) (DatasetHandle handle);
static LGBM_DatasetFree_t LGBM_DatasetFree = NULL;

/*!
 * \brief Save dataset to binary file.
 * \param handle Handle of dataset
 * \param filename The name of the file
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSaveBinary_t) (DatasetHandle handle,
                                             const char* filename);
static LGBM_DatasetSaveBinary_t LGBM_DatasetSaveBinary = NULL;

/*!
 * \brief Create a dataset schema representation as a binary byte array (excluding data).
 * \param handle Handle of dataset
 * \param[out] out The output byte array
 * \param[out] out_len The length of the output byte array (returned for convenience)
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSerializeReferenceToBinary_t) (DatasetHandle handle,
                                                             ByteBufferHandle* out,
                                                             int32_t* out_len);
static LGBM_DatasetSerializeReferenceToBinary_t LGBM_DatasetSerializeReferenceToBinary = NULL;

/*!
 * \brief Save dataset to text file, intended for debugging use only.
 * \param handle Handle of dataset
 * \param filename The name of the file
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetDumpText_t) (DatasetHandle handle,
                                           const char* filename);
static LGBM_DatasetDumpText_t LGBM_DatasetDumpText = NULL;

/*!
 * \brief Set vector to a content in info.
 * \note
 * - \a group only works for ``C_API_DTYPE_INT32``;
 * - \a label and \a weight only work for ``C_API_DTYPE_FLOAT32``;
 * - \a init_score only works for ``C_API_DTYPE_FLOAT64``.
 * \param handle Handle of dataset
 * \param field_name Field name, can be \a label, \a weight, \a init_score, \a group
 * \param field_data Pointer to data vector
 * \param num_element Number of elements in ``field_data``
 * \param type Type of ``field_data`` pointer, can be ``C_API_DTYPE_INT32``, ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSetField_t) (DatasetHandle handle,
                                           const char* field_name,
                                           const void* field_data,
                                           int num_element,
                                           int type);
static LGBM_DatasetSetField_t LGBM_DatasetSetField = NULL;

/*!
 * \brief Set vector to a content in info.
 * \note
 * - \a group converts input datatype into ``int32``;
 * - \a label and \a weight convert input datatype into ``float32``;
 * - \a init_score converts input datatype into ``float64``.
 * \param handle Handle of dataset
 * \param field_name Field name, can be \a label, \a weight, \a init_score, \a group
 * \param n_chunks The number of Arrow arrays passed to this function
 * \param chunks Pointer to the list of Arrow arrays
 * \param schema Pointer to the schema of all Arrow arrays
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSetFieldFromArrow_t) (DatasetHandle handle,
                                                    const char* field_name,
                                                    int64_t n_chunks,
                                                    const ArrowArray* chunks,
                                                    const ArrowSchema* schema);
static LGBM_DatasetSetFieldFromArrow_t LGBM_DatasetSetFieldFromArrow = NULL;

/*!
 * \brief Get info vector from dataset.
 * \param handle Handle of dataset
 * \param field_name Field name
 * \param[out] out_len Used to set result length
 * \param[out] out_ptr Pointer to the result
 * \param[out] out_type Type of result pointer, can be ``C_API_DTYPE_INT32``, ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetField_t) (DatasetHandle handle,
                                           const char* field_name,
                                           int* out_len,
                                           const void** out_ptr,
                                           int* out_type);
static LGBM_DatasetGetField_t LGBM_DatasetGetField = NULL;

/*!
 * \brief Raise errors for attempts to update dataset parameters.
 * \param old_parameters Current dataset parameters
 * \param new_parameters New dataset parameters
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetUpdateParamChecking_t) (const char* old_parameters,
                                                      const char* new_parameters);
static LGBM_DatasetUpdateParamChecking_t LGBM_DatasetUpdateParamChecking = NULL;

/*!
 * \brief Get number of data points.
 * \param handle Handle of dataset
 * \param[out] out The address to hold number of data points
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetNumData_t) (DatasetHandle handle,
                                             int* out);
static LGBM_DatasetGetNumData_t LGBM_DatasetGetNumData = NULL;

/*!
 * \brief Get number of features.
 * \param handle Handle of dataset
 * \param[out] out The address to hold number of features
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetNumFeature_t) (DatasetHandle handle,
                                                int* out);
static LGBM_DatasetGetNumFeature_t LGBM_DatasetGetNumFeature = NULL;

/*!
 * \brief Get number of bins for feature.
 * \param handle Handle of dataset
 * \param feature Index of the feature
 * \param[out] out The address to hold number of bins
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetFeatureNumBin_t) (DatasetHandle handle,
                                                   int feature,
                                                   int* out);
static LGBM_DatasetGetFeatureNumBin_t LGBM_DatasetGetFeatureNumBin = NULL;

/*!
 * \brief Add features from ``source`` to ``target``.
 * \param target The handle of the dataset to add features to
 * \param source The handle of the dataset to take features from
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetAddFeaturesFrom_t) (DatasetHandle target,
                                                  DatasetHandle source);
static LGBM_DatasetAddFeaturesFrom_t LGBM_DatasetAddFeaturesFrom = NULL;

/* --- start Booster interfaces */

/*!
* \brief Get int representing whether booster is fitting linear trees.
* \param handle Handle of booster
* \param[out] out The address to hold linear trees indicator
* \return 0 when succeed, -1 when failure happens
*/
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetLinear_t) (BoosterHandle handle, int* out);
static LGBM_BoosterGetLinear_t LGBM_BoosterGetLinear = NULL;

/*!
 * \brief Create a new boosting learner.
 * \param train_data Training dataset
 * \param parameters Parameters in format 'key1=value1 key2=value2'
 * \param[out] out Handle of created booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterCreate_t) (const DatasetHandle train_data,
                                         const char* parameters,
                                         BoosterHandle* out);
static LGBM_BoosterCreate_t LGBM_BoosterCreate = NULL;

/*!
 * \brief Load an existing booster from model file.
 * \param filename Filename of model
 * \param[out] out_num_iterations Number of iterations of this booster
 * \param[out] out Handle of created booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterCreateFromModelfile_t) (const char* filename,
                                                      int* out_num_iterations,
                                                      BoosterHandle* out);
static LGBM_BoosterCreateFromModelfile_t LGBM_BoosterCreateFromModelfile = NULL;

/*!
 * \brief Load an existing booster from string.
 * \param model_str Model string
 * \param[out] out_num_iterations Number of iterations of this booster
 * \param[out] out Handle of created booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterLoadModelFromString_t) (const char* model_str,
                                                      int* out_num_iterations,
                                                      BoosterHandle* out);
static LGBM_BoosterLoadModelFromString_t LGBM_BoosterLoadModelFromString = NULL;

/*!
 * \brief Get parameters as JSON string.
 * \param handle Handle of booster
 * \param buffer_len Allocated space for string
 * \param[out] out_len Actual size of string
 * \param[out] out_str JSON string containing parameters
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetLoadedParam_t) (BoosterHandle handle,
                                                 int64_t buffer_len,
                                                 int64_t* out_len,
                                                 char* out_str);
static LGBM_BoosterGetLoadedParam_t LGBM_BoosterGetLoadedParam = NULL;


/*!
 * \brief Free space for booster.
 * \param handle Handle of booster to be freed
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterFree_t) (BoosterHandle handle);
static LGBM_BoosterFree_t LGBM_BoosterFree = NULL;

/*!
 * \brief Shuffle models.
 * \param handle Handle of booster
 * \param start_iter The first iteration that will be shuffled
 * \param end_iter The last iteration that will be shuffled
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterShuffleModels_t) (BoosterHandle handle,
                                                int start_iter,
                                                int end_iter);
static LGBM_BoosterShuffleModels_t LGBM_BoosterShuffleModels = NULL;

/*!
 * \brief Merge model from ``other_handle`` into ``handle``.
 * \param handle Handle of booster, will merge another booster into this one
 * \param other_handle Other handle of booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterMerge_t) (BoosterHandle handle,
                                        BoosterHandle other_handle);
static LGBM_BoosterMerge_t LGBM_BoosterMerge = NULL;

/*!
 * \brief Add new validation data to booster.
 * \param handle Handle of booster
 * \param valid_data Validation dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterAddValidData_t) (BoosterHandle handle,
                                               const DatasetHandle valid_data);
static LGBM_BoosterAddValidData_t LGBM_BoosterAddValidData = NULL;

/*!
 * \brief Reset training data for booster.
 * \param handle Handle of booster
 * \param train_data Training dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterResetTrainingData_t) (BoosterHandle handle,
                                                    const DatasetHandle train_data);
static LGBM_BoosterResetTrainingData_t LGBM_BoosterResetTrainingData = NULL;

/*!
 * \brief Reset config for booster.
 * \param handle Handle of booster
 * \param parameters Parameters in format 'key1=value1 key2=value2'
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterResetParameter_t) (BoosterHandle handle,
                                                 const char* parameters);
static LGBM_BoosterResetParameter_t LGBM_BoosterResetParameter = NULL;

/*!
 * \brief Get number of classes.
 * \param handle Handle of booster
 * \param[out] out_len Number of classes
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetNumClasses_t) (BoosterHandle handle,
                                                int* out_len);
static LGBM_BoosterGetNumClasses_t LGBM_BoosterGetNumClasses = NULL;

/*!
 * \brief Update the model for one iteration.
 * \param handle Handle of booster
 * \param[out] is_finished 1 means the update was successfully finished (cannot split any more), 0 indicates failure
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterUpdateOneIter_t) (BoosterHandle handle,
                                                int* is_finished);
static LGBM_BoosterUpdateOneIter_t LGBM_BoosterUpdateOneIter = NULL;

/*!
 * \brief Refit the tree model using the new data (online learning).
 * \param handle Handle of booster
 * \param leaf_preds Pointer to predicted leaf indices
 * \param nrow Number of rows of ``leaf_preds``
 * \param ncol Number of columns of ``leaf_preds``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterRefit_t) (BoosterHandle handle,
                                        const int32_t* leaf_preds,
                                        int32_t nrow,
                                        int32_t ncol);
static LGBM_BoosterRefit_t LGBM_BoosterRefit = NULL;

/*!
 * \brief Update the model by specifying gradient and Hessian directly
 *        (this can be used to support customized loss functions).
 * \note
 * The length of the arrays referenced by ``grad`` and ``hess`` must be equal to
 * ``num_class * num_train_data``, this is not verified by the library, the caller must ensure this.
 * \param handle Handle of booster
 * \param grad The first order derivative (gradient) statistics
 * \param hess The second order derivative (Hessian) statistics
 * \param[out] is_finished 1 means the update was successfully finished (cannot split any more), 0 indicates failure
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterUpdateOneIterCustom_t) (BoosterHandle handle,
                                                      const float* grad,
                                                      const float* hess,
                                                      int* is_finished);
static LGBM_BoosterUpdateOneIterCustom_t LGBM_BoosterUpdateOneIterCustom = NULL;

/*!
 * \brief Rollback one iteration.
 * \param handle Handle of booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterRollbackOneIter_t) (BoosterHandle handle);
static LGBM_BoosterRollbackOneIter_t LGBM_BoosterRollbackOneIter = NULL;

/*!
 * \brief Get index of the current boosting iteration.
 * \param handle Handle of booster
 * \param[out] out_iteration Index of the current boosting iteration
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetCurrentIteration_t) (BoosterHandle handle,
                                                      int* out_iteration);
static LGBM_BoosterGetCurrentIteration_t LGBM_BoosterGetCurrentIteration = NULL;

/*!
 * \brief Get number of trees per iteration.
 * \param handle Handle of booster
 * \param[out] out_tree_per_iteration Number of trees per iteration
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterNumModelPerIteration_t) (BoosterHandle handle,
                                                       int* out_tree_per_iteration);
static LGBM_BoosterNumModelPerIteration_t LGBM_BoosterNumModelPerIteration = NULL;

/*!
 * \brief Get number of weak sub-models.
 * \param handle Handle of booster
 * \param[out] out_models Number of weak sub-models
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterNumberOfTotalModel_t) (BoosterHandle handle,
                                                     int* out_models);
static LGBM_BoosterNumberOfTotalModel_t LGBM_BoosterNumberOfTotalModel = NULL;

/*!
 * \brief Get number of evaluation metrics.
 * \param handle Handle of booster
 * \param[out] out_len Total number of evaluation metrics
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetEvalCounts_t) (BoosterHandle handle,
                                                int* out_len);
static LGBM_BoosterGetEvalCounts_t LGBM_BoosterGetEvalCounts = NULL;

/*!
 * \brief Get names of evaluation metrics.
 * \param handle Handle of booster
 * \param len Number of ``char*`` pointers stored at ``out_strs``.
 *            If smaller than the max size, only this many strings are copied
 * \param[out] out_len Total number of evaluation metrics
 * \param buffer_len Size of pre-allocated strings.
 *                   Content is copied up to ``buffer_len - 1`` and null-terminated
 * \param[out] out_buffer_len String sizes required to do the full string copies
 * \param[out] out_strs Names of evaluation metrics, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetEvalNames_t) (BoosterHandle handle,
                                               const int len,
                                               int* out_len,
                                               const size_t buffer_len,
                                               size_t* out_buffer_len,
                                               char** out_strs);
static LGBM_BoosterGetEvalNames_t LGBM_BoosterGetEvalNames = NULL;

/*!
 * \brief Get names of features.
 * \param handle Handle of booster
 * \param len Number of ``char*`` pointers stored at ``out_strs``.
 *            If smaller than the max size, only this many strings are copied
 * \param[out] out_len Total number of features
 * \param buffer_len Size of pre-allocated strings.
 *                   Content is copied up to ``buffer_len - 1`` and null-terminated
 * \param[out] out_buffer_len String sizes required to do the full string copies
 * \param[out] out_strs Names of features, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetFeatureNames_t) (BoosterHandle handle,
                                                  const int len,
                                                  int* out_len,
                                                  const size_t buffer_len,
                                                  size_t* out_buffer_len,
                                                  char** out_strs);
static LGBM_BoosterGetFeatureNames_t LGBM_BoosterGetFeatureNames = NULL;

/*!
 * \brief Check that the feature names of the data match the ones used to train the booster.
 * \param handle Handle of booster
 * \param data_names Array with the feature names in the data
 * \param data_num_features Number of features in the data
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterValidateFeatureNames_t) (BoosterHandle handle,
                                                       const char** data_names,
                                                       int data_num_features);
static LGBM_BoosterValidateFeatureNames_t LGBM_BoosterValidateFeatureNames = NULL;

/*!
 * \brief Get number of features.
 * \param handle Handle of booster
 * \param[out] out_len Total number of features
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetNumFeature_t) (BoosterHandle handle,
                                                int* out_len);
static LGBM_BoosterGetNumFeature_t LGBM_BoosterGetNumFeature = NULL;

/*!
 * \brief Get evaluation for training data and validation data.
 * \note
 *   1. You should call ``LGBM_BoosterGetEvalNames`` first to get the names of evaluation metrics.
 *   2. You should pre-allocate memory for ``out_results``, you can get its length by ``LGBM_BoosterGetEvalCounts``.
 * \param handle Handle of booster
 * \param data_idx Index of data, 0: training data, 1: 1st validation data, 2: 2nd validation data and so on
 * \param[out] out_len Length of output result
 * \param[out] out_results Array with evaluation results
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetEval_t) (BoosterHandle handle,
                                          int data_idx,
                                          int* out_len,
                                          double* out_results);
static LGBM_BoosterGetEval_t LGBM_BoosterGetEval = NULL;

/*!
 * \brief Get number of predictions for training data and validation data
 *        (this can be used to support customized evaluation functions).
 * \param handle Handle of booster
 * \param data_idx Index of data, 0: training data, 1: 1st validation data, 2: 2nd validation data and so on
 * \param[out] out_len Number of predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetNumPredict_t) (BoosterHandle handle,
                                                int data_idx,
                                                int64_t* out_len);
static LGBM_BoosterGetNumPredict_t LGBM_BoosterGetNumPredict = NULL;

/*!
 * \brief Get prediction for training data and validation data.
 * \note
 * You should pre-allocate memory for ``out_result``, its length is equal to ``num_class * num_data``.
 * \param handle Handle of booster
 * \param data_idx Index of data, 0: training data, 1: 1st validation data, 2: 2nd validation data and so on
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetPredict_t) (BoosterHandle handle,
                                             int data_idx,
                                             int64_t* out_len,
                                             double* out_result);
static LGBM_BoosterGetPredict_t LGBM_BoosterGetPredict = NULL;

/*!
 * \brief Make prediction for file.
 * \param handle Handle of booster
 * \param data_filename Filename of file with data
 * \param data_has_header Whether file has header or not
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param result_filename Filename of result file in which predictions will be written
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForFile_t) (BoosterHandle handle,
                                                 const char* data_filename,
                                                 int data_has_header,
                                                 int predict_type,
                                                 int start_iteration,
                                                 int num_iteration,
                                                 const char* parameter,
                                                 const char* result_filename);
static LGBM_BoosterPredictForFile_t LGBM_BoosterPredictForFile = NULL;

/*!
 * \brief Get number of predictions.
 * \param handle Handle of booster
 * \param num_row Number of rows
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param[out] out_len Length of prediction
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterCalcNumPredict_t) (BoosterHandle handle,
                                                 int num_row,
                                                 int predict_type,
                                                 int start_iteration,
                                                 int num_iteration,
                                                 int64_t* out_len);
static LGBM_BoosterCalcNumPredict_t LGBM_BoosterCalcNumPredict = NULL;

/*!
 * \brief Release FastConfig object.
 *
 * \param fastConfig Handle to the FastConfig object acquired with a ``*FastInit()`` method.
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_FastConfigFree_t) (FastConfigHandle fastConfig);
static LGBM_FastConfigFree_t LGBM_FastConfigFree = NULL;

/*!
 * \brief Make prediction for a new dataset in CSR format.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSR_t) (BoosterHandle handle,
                                                const void* indptr,
                                                int indptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t nindptr,
                                                int64_t nelem,
                                                int64_t num_col,
                                                int predict_type,
                                                int start_iteration,
                                                int num_iteration,
                                                const char* parameter,
                                                int64_t* out_len,
                                                double* out_result);
static LGBM_BoosterPredictForCSR_t LGBM_BoosterPredictForCSR = NULL;

/*!
 * \brief Make sparse prediction for a new dataset in CSR or CSC format. Currently only used for feature contributions.
 * \note
 * The outputs are pre-allocated, as they can vary for each invocation, but the shape should be the same:
 *   - for feature contributions, the shape of sparse matrix will be ``num_class * num_data * (num_feature + 1)``.
 * The output indptr_type for the sparse matrix will be the same as the given input indptr_type.
 * Call ``LGBM_BoosterFreePredictSparse`` to deallocate resources.
 * \param handle Handle of booster
 * \param indptr Pointer to row headers for CSR or column headers for CSC
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices for CSR or row indices for CSC
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of entries in ``indptr``
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col_or_row Number of columns for CSR or number of rows for CSC
 * \param predict_type What should be predicted, only feature contributions supported currently
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param matrix_type Type of matrix input and output, can be ``C_API_MATRIX_TYPE_CSR`` or ``C_API_MATRIX_TYPE_CSC``
 * \param[out] out_len Length of output data and output indptr (pointer to an array with two entries where to write them)
 * \param[out] out_indptr Pointer to output row headers for CSR or column headers for CSC
 * \param[out] out_indices Pointer to sparse column indices for CSR or row indices for CSC
 * \param[out] out_data Pointer to sparse data space
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictSparseOutput_t) (BoosterHandle handle,
                                                      const void* indptr,
                                                      int indptr_type,
                                                      const int32_t* indices,
                                                      const void* data,
                                                      int data_type,
                                                      int64_t nindptr,
                                                      int64_t nelem,
                                                      int64_t num_col_or_row,
                                                      int predict_type,
                                                      int start_iteration,
                                                      int num_iteration,
                                                      const char* parameter,
                                                      int matrix_type,
                                                      int64_t* out_len,
                                                      void** out_indptr,
                                                      int32_t** out_indices,
                                                      void** out_data);
static LGBM_BoosterPredictSparseOutput_t LGBM_BoosterPredictSparseOutput = NULL;

/*!
 * \brief Method corresponding to ``LGBM_BoosterPredictSparseOutput`` to free the allocated data.
 * \param indptr Pointer to output row headers or column headers to be deallocated
 * \param indices Pointer to sparse indices to be deallocated
 * \param data Pointer to sparse data space to be deallocated
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterFreePredictSparse_t) (void* indptr, int32_t* indices, void* data, int indptr_type, int data_type);
static LGBM_BoosterFreePredictSparse_t LGBM_BoosterFreePredictSparse = NULL;

/*!
 * \brief Make prediction for a new dataset in CSR format. This method re-uses the internal predictor structure
 *        from previous calls and is optimized for single row invocation.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSRSingleRow_t) (BoosterHandle handle,
                                                         const void* indptr,
                                                         int indptr_type,
                                                         const int32_t* indices,
                                                         const void* data,
                                                         int data_type,
                                                         int64_t nindptr,
                                                         int64_t nelem,
                                                         int64_t num_col,
                                                         int predict_type,
                                                         int start_iteration,
                                                         int num_iteration,
                                                         const char* parameter,
                                                         int64_t* out_len,
                                                         double* out_result);
static LGBM_BoosterPredictForCSRSingleRow_t LGBM_BoosterPredictForCSRSingleRow = NULL;

/*!
 * \brief Initialize and return a ``FastConfigHandle`` for use with ``LGBM_BoosterPredictForCSRSingleRowFast``.
 *
 * Release the ``FastConfig`` by passing its handle to ``LGBM_FastConfigFree`` when no longer needed.
 *
 * \param handle Booster handle
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param num_col Number of columns
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_fastConfig FastConfig object with which you can call ``LGBM_BoosterPredictForCSRSingleRowFast``
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSRSingleRowFastInit_t) (BoosterHandle handle,
                                                                 const int predict_type,
                                                                 const int start_iteration,
                                                                 const int num_iteration,
                                                                 const int data_type,
                                                                 const int64_t num_col,
                                                                 const char* parameter,
                                                                 FastConfigHandle *out_fastConfig);
static LGBM_BoosterPredictForCSRSingleRowFastInit_t LGBM_BoosterPredictForCSRSingleRowFastInit = NULL;

/*!
 * \brief Faster variant of ``LGBM_BoosterPredictForCSRSingleRow``.
 *
 * Score single rows after setup with ``LGBM_BoosterPredictForCSRSingleRowFastInit``.
 *
 * By removing the setup steps from this call extra optimizations can be made like
 * initializing the config only once, instead of once per call.
 *
 * \note
 *   Setting up the number of threads is only done once at ``LGBM_BoosterPredictForCSRSingleRowFastInit``
 *   instead of at each prediction.
 *   If you use a different number of threads in other calls, you need to start the setup process over,
 *   or that number of threads will be used for these calls as well.
 *
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 *
 * \param fastConfig_handle FastConfig object handle returned by ``LGBM_BoosterPredictForCSRSingleRowFastInit``
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSRSingleRowFast_t) (FastConfigHandle fastConfig_handle,
                                                             const void* indptr,
                                                             const int indptr_type,
                                                             const int32_t* indices,
                                                             const void* data,
                                                             const int64_t nindptr,
                                                             const int64_t nelem,
                                                             int64_t* out_len,
                                                             double* out_result);
static LGBM_BoosterPredictForCSRSingleRowFast_t LGBM_BoosterPredictForCSRSingleRowFast = NULL;

/*!
 * \brief Make prediction for a new dataset in CSC format.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param col_ptr Pointer to column headers
 * \param col_ptr_type Type of ``col_ptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to row indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol_ptr Number of columns in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_row Number of rows
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSC_t) (BoosterHandle handle,
                                                const void* col_ptr,
                                                int col_ptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t ncol_ptr,
                                                int64_t nelem,
                                                int64_t num_row,
                                                int predict_type,
                                                int start_iteration,
                                                int num_iteration,
                                                const char* parameter,
                                                int64_t* out_len,
                                                double* out_result);
static LGBM_BoosterPredictForCSC_t LGBM_BoosterPredictForCSC = NULL;

/*!
 * \brief Make prediction for a new dataset.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param is_row_major 1 for row-major, 0 for column-major
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMat_t) (BoosterHandle handle,
                                                const void* data,
                                                int data_type,
                                                int32_t nrow,
                                                int32_t ncol,
                                                int is_row_major,
                                                int predict_type,
                                                int start_iteration,
                                                int num_iteration,
                                                const char* parameter,
                                                int64_t* out_len,
                                                double* out_result);
static LGBM_BoosterPredictForMat_t LGBM_BoosterPredictForMat = NULL;

/*!
 * \brief Make prediction for a new dataset. This method re-uses the internal predictor structure
 *        from previous calls and is optimized for single row invocation.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol Number columns
 * \param is_row_major 1 for row-major, 0 for column-major
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMatSingleRow_t) (BoosterHandle handle,
                                                         const void* data,
                                                         int data_type,
                                                         int ncol,
                                                         int is_row_major,
                                                         int predict_type,
                                                         int start_iteration,
                                                         int num_iteration,
                                                         const char* parameter,
                                                         int64_t* out_len,
                                                         double* out_result);
static LGBM_BoosterPredictForMatSingleRow_t LGBM_BoosterPredictForMatSingleRow = NULL;

/*!
 * \brief Initialize and return a ``FastConfigHandle`` for use with ``LGBM_BoosterPredictForMatSingleRowFast``.
 *
 * Release the ``FastConfig`` by passing its handle to ``LGBM_FastConfigFree`` when no longer needed.
 *
 * \param handle Booster handle
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol Number of columns
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_fastConfig FastConfig object with which you can call ``LGBM_BoosterPredictForMatSingleRowFast``
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMatSingleRowFastInit_t) (BoosterHandle handle,
                                                                 const int predict_type,
                                                                 const int start_iteration,
                                                                 const int num_iteration,
                                                                 const int data_type,
                                                                 const int32_t ncol,
                                                                 const char* parameter,
                                                                 FastConfigHandle *out_fastConfig);
static LGBM_BoosterPredictForMatSingleRowFastInit_t LGBM_BoosterPredictForMatSingleRowFastInit = NULL;

/*!
 * \brief Faster variant of ``LGBM_BoosterPredictForMatSingleRow``.
 *
 * Score a single row after setup with ``LGBM_BoosterPredictForMatSingleRowFastInit``.
 *
 * By removing the setup steps from this call extra optimizations can be made like
 * initializing the config only once, instead of once per call.
 *
 * \note
 *   Setting up the number of threads is only done once at ``LGBM_BoosterPredictForMatSingleRowFastInit``
 *   instead of at each prediction.
 *   If you use a different number of threads in other calls, you need to start the setup process over,
 *   or that number of threads will be used for these calls as well.
 *
 * \param fastConfig_handle FastConfig object handle returned by ``LGBM_BoosterPredictForMatSingleRowFastInit``
 * \param data Single-row array data (no other way than row-major form).
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMatSingleRowFast_t) (FastConfigHandle fastConfig_handle,
                                                             const void* data,
                                                             int64_t* out_len,
                                                             double* out_result);
static LGBM_BoosterPredictForMatSingleRowFast_t LGBM_BoosterPredictForMatSingleRowFast = NULL;

/*!
 * \brief Make prediction for a new dataset presented in a form of array of pointers to rows.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number columns
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMats_t) (BoosterHandle handle,
                                                 const void** data,
                                                 int data_type,
                                                 int32_t nrow,
                                                 int32_t ncol,
                                                 int predict_type,
                                                 int start_iteration,
                                                 int num_iteration,
                                                 const char* parameter,
                                                 int64_t* out_len,
                                                 double* out_result);
static LGBM_BoosterPredictForMats_t LGBM_BoosterPredictForMats = NULL;

/*!
 * \brief Make prediction for a new dataset.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param n_chunks The number of Arrow arrays passed to this function
 * \param chunks Pointer to the list of Arrow arrays
 * \param schema Pointer to the schema of all Arrow arrays
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForArrow_t) (BoosterHandle handle,
                                                  int64_t n_chunks,
                                                  const ArrowArray* chunks,
                                                  const ArrowSchema* schema,
                                                  int predict_type,
                                                  int start_iteration,
                                                  int num_iteration,
                                                  const char* parameter,
                                                  int64_t* out_len,
                                                  double* out_result);
static LGBM_BoosterPredictForArrow_t LGBM_BoosterPredictForArrow = NULL;

/*!
 * \brief Save model into file.
 * \param handle Handle of booster
 * \param start_iteration Start index of the iteration that should be saved
 * \param num_iteration Index of the iteration that should be saved, <= 0 means save all
 * \param feature_importance_type Type of feature importance, can be ``C_API_FEATURE_IMPORTANCE_SPLIT`` or ``C_API_FEATURE_IMPORTANCE_GAIN``
 * \param filename The name of the file
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterSaveModel_t) (BoosterHandle handle,
                                            int start_iteration,
                                            int num_iteration,
                                            int feature_importance_type,
                                            const char* filename);
static LGBM_BoosterSaveModel_t LGBM_BoosterSaveModel = NULL;

/*!
 * \brief Save model to string.
 * \param handle Handle of booster
 * \param start_iteration Start index of the iteration that should be saved
 * \param num_iteration Index of the iteration that should be saved, <= 0 means save all
 * \param feature_importance_type Type of feature importance, can be ``C_API_FEATURE_IMPORTANCE_SPLIT`` or ``C_API_FEATURE_IMPORTANCE_GAIN``
 * \param buffer_len String buffer length, if ``buffer_len < out_len``, you should re-allocate buffer
 * \param[out] out_len Actual output length
 * \param[out] out_str String of model, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterSaveModelToString_t) (BoosterHandle handle,
                                                    int start_iteration,
                                                    int num_iteration,
                                                    int feature_importance_type,
                                                    int64_t buffer_len,
                                                    int64_t* out_len,
                                                    char* out_str);
static LGBM_BoosterSaveModelToString_t LGBM_BoosterSaveModelToString = NULL;

/*!
 * \brief Dump model to JSON.
 * \param handle Handle of booster
 * \param start_iteration Start index of the iteration that should be dumped
 * \param num_iteration Index of the iteration that should be dumped, <= 0 means dump all
 * \param feature_importance_type Type of feature importance, can be ``C_API_FEATURE_IMPORTANCE_SPLIT`` or ``C_API_FEATURE_IMPORTANCE_GAIN``
 * \param buffer_len String buffer length, if ``buffer_len < out_len``, you should re-allocate buffer
 * \param[out] out_len Actual output length
 * \param[out] out_str JSON format string of model, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterDumpModel_t) (BoosterHandle handle,
                                            int start_iteration,
                                            int num_iteration,
                                            int feature_importance_type,
                                            int64_t buffer_len,
                                            int64_t* out_len,
                                            char* out_str);
static LGBM_BoosterDumpModel_t LGBM_BoosterDumpModel = NULL;

/*!
 * \brief Get leaf value.
 * \param handle Handle of booster
 * \param tree_idx Index of tree
 * \param leaf_idx Index of leaf
 * \param[out] out_val Output result from the specified leaf
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetLeafValue_t) (BoosterHandle handle,
                                               int tree_idx,
                                               int leaf_idx,
                                               double* out_val);
static LGBM_BoosterGetLeafValue_t LGBM_BoosterGetLeafValue = NULL;

/*!
 * \brief Set leaf value.
 * \param handle Handle of booster
 * \param tree_idx Index of tree
 * \param leaf_idx Index of leaf
 * \param val Leaf value
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterSetLeafValue_t) (BoosterHandle handle,
                                               int tree_idx,
                                               int leaf_idx,
                                               double val);
static LGBM_BoosterSetLeafValue_t LGBM_BoosterSetLeafValue = NULL;

/*!
 * \brief Get model feature importance.
 * \param handle Handle of booster
 * \param num_iteration Number of iterations for which feature importance is calculated, <= 0 means use all
 * \param importance_type Method of importance calculation:
 *   - ``C_API_FEATURE_IMPORTANCE_SPLIT``: result contains numbers of times the feature is used in a model;
 *   - ``C_API_FEATURE_IMPORTANCE_GAIN``: result contains total gains of splits which use the feature
 * \param[out] out_results Result array with feature importance
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterFeatureImportance_t) (BoosterHandle handle,
                                                    int num_iteration,
                                                    int importance_type,
                                                    double* out_results);
static LGBM_BoosterFeatureImportance_t LGBM_BoosterFeatureImportance = NULL;

/*!
 * \brief Get model upper bound value.
 * \param handle Handle of booster
 * \param[out] out_results Result pointing to max value
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetUpperBoundValue_t) (BoosterHandle handle,
                                                     double* out_results);
static LGBM_BoosterGetUpperBoundValue_t LGBM_BoosterGetUpperBoundValue = NULL;

/*!
 * \brief Get model lower bound value.
 * \param handle Handle of booster
 * \param[out] out_results Result pointing to min value
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetLowerBoundValue_t) (BoosterHandle handle,
                                                     double* out_results);
static LGBM_BoosterGetLowerBoundValue_t LGBM_BoosterGetLowerBoundValue = NULL;

/*!
 * \brief Initialize the network.
 * \param machines List of machines in format 'ip1:port1,ip2:port2'
 * \param local_listen_port TCP listen port for local machines
 * \param listen_time_out Socket time-out in minutes
 * \param num_machines Total number of machines
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_NetworkInit_t) (const char* machines,
                                       int local_listen_port,
                                       int listen_time_out,
                                       int num_machines);
static LGBM_NetworkInit_t LGBM_NetworkInit = NULL;

/*!
 * \brief Finalize the network.
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_NetworkFree_t) ();
static LGBM_NetworkFree_t LGBM_NetworkFree = NULL;

/*!
 * \brief Initialize the network with external collective functions.
 * \param num_machines Total number of machines
 * \param rank Rank of local machine
 * \param reduce_scatter_ext_fun The external reduce-scatter function
 * \param allgather_ext_fun The external allgather function
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_NetworkInitWithFunctions_t) (int num_machines,
                                                    int rank,
                                                    void* reduce_scatter_ext_fun,
                                                    void* allgather_ext_fun);
static LGBM_NetworkInitWithFunctions_t LGBM_NetworkInitWithFunctions = NULL;

/*!
 * \brief Set maximum number of threads used by LightGBM routines in this process.
 * \param num_threads maximum number of threads used by LightGBM. -1 means defaulting to omp_get_num_threads().
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_SetMaxThreads_t) (int num_threads);
static LGBM_SetMaxThreads_t LGBM_SetMaxThreads = NULL;

/*!
 * \brief Get current maximum number of threads used by LightGBM routines in this process.
 * \param[out] out current maximum number of threads used by LightGBM. -1 means defaulting to omp_get_num_threads().
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_GetMaxThreads_t) (int* out);
static LGBM_GetMaxThreads_t LGBM_GetMaxThreads = NULL;

#if !defined(__cplusplus) && (!defined(__STDC__) || (__STDC_VERSION__ < 199901L))
/*! \brief Inline specifier no-op in C using standards before C99. */
#define INLINE_FUNCTION
#else
/*! \brief Inline specifier. */
// hack-lightgbm - fix warning
#define INLINE_FUNCTION static inline
#endif

#if !defined(__cplusplus) && (!defined(__STDC__) || (__STDC_VERSION__ < 201112L))
/*! \brief Thread local specifier no-op in C using standards before C11. */
#define THREAD_LOCAL
#elif !defined(__cplusplus)
/*! \brief Thread local specifier. */
#define THREAD_LOCAL _Thread_local
#elif defined(_MSC_VER)
/*! \brief Thread local specifier. */
#define THREAD_LOCAL __declspec(thread)
#else
/*! \brief Thread local specifier. */
#define THREAD_LOCAL thread_local
#endif

/*!
 * \brief Handle of error message.
 * \return Error message
 */
static char* LastErrorMsg() { static THREAD_LOCAL char err_msg[512] = "Everything is fine"; return err_msg; }

#ifdef _MSC_VER
  #pragma warning(disable : 4996)
#endif
/*!
 * \brief Set string message of the last error.
 * \note
 * This will call unsafe ``sprintf`` when compiled using C standards before C99.
 * \param msg Error message
 */
INLINE_FUNCTION void LGBM_SetLastError(const char* msg) {
#if !defined(__cplusplus) && (!defined(__STDC__) || (__STDC_VERSION__ < 199901L))
  sprintf(LastErrorMsg(), "%s", msg);  /* NOLINT(runtime/printf) */
#else
  const int err_buf_len = 512;
  snprintf(LastErrorMsg(), err_buf_len, "%s", msg);
#endif
}

#endif  /* LIGHTGBM_C_API_H_ */


/*
file https://github.com/microsoft/LightGBM/blob/v4.6.0/include/LightGBM/c_api.h
*/
static void LGBM_dlsym() {
LGBM_DLSYM(LGBM_GetLastError);
LGBM_DLSYM(LGBM_DumpParamAliases);
LGBM_DLSYM(LGBM_RegisterLogCallback);
LGBM_DLSYM(LGBM_GetSampleCount);
LGBM_DLSYM(LGBM_SampleIndices);
LGBM_DLSYM(LGBM_ByteBufferGetAt);
LGBM_DLSYM(LGBM_ByteBufferFree);
LGBM_DLSYM(LGBM_DatasetCreateFromFile);
LGBM_DLSYM(LGBM_DatasetCreateFromSampledColumn);
LGBM_DLSYM(LGBM_DatasetCreateByReference);
LGBM_DLSYM(LGBM_DatasetInitStreaming);
LGBM_DLSYM(LGBM_DatasetCreateFromSerializedReference);
LGBM_DLSYM(LGBM_DatasetPushRows);
LGBM_DLSYM(LGBM_DatasetPushRowsWithMetadata);
LGBM_DLSYM(LGBM_DatasetPushRowsByCSR);
LGBM_DLSYM(LGBM_DatasetPushRowsByCSRWithMetadata);
LGBM_DLSYM(LGBM_DatasetSetWaitForManualFinish);
LGBM_DLSYM(LGBM_DatasetMarkFinished);
LGBM_DLSYM(LGBM_DatasetCreateFromCSR);
LGBM_DLSYM(LGBM_DatasetCreateFromCSRFunc);
LGBM_DLSYM(LGBM_DatasetCreateFromCSC);
LGBM_DLSYM(LGBM_DatasetCreateFromMat);
LGBM_DLSYM(LGBM_DatasetCreateFromMats);
LGBM_DLSYM(LGBM_DatasetCreateFromArrow);
LGBM_DLSYM(LGBM_DatasetGetSubset);
LGBM_DLSYM(LGBM_DatasetSetFeatureNames);
LGBM_DLSYM(LGBM_DatasetGetFeatureNames);
LGBM_DLSYM(LGBM_DatasetFree);
LGBM_DLSYM(LGBM_DatasetSaveBinary);
LGBM_DLSYM(LGBM_DatasetSerializeReferenceToBinary);
LGBM_DLSYM(LGBM_DatasetDumpText);
LGBM_DLSYM(LGBM_DatasetSetField);
LGBM_DLSYM(LGBM_DatasetSetFieldFromArrow);
LGBM_DLSYM(LGBM_DatasetGetField);
LGBM_DLSYM(LGBM_DatasetUpdateParamChecking);
LGBM_DLSYM(LGBM_DatasetGetNumData);
LGBM_DLSYM(LGBM_DatasetGetNumFeature);
LGBM_DLSYM(LGBM_DatasetGetFeatureNumBin);
LGBM_DLSYM(LGBM_DatasetAddFeaturesFrom);
LGBM_DLSYM(LGBM_BoosterGetLinear);
LGBM_DLSYM(LGBM_BoosterCreate);
LGBM_DLSYM(LGBM_BoosterCreateFromModelfile);
LGBM_DLSYM(LGBM_BoosterLoadModelFromString);
LGBM_DLSYM(LGBM_BoosterGetLoadedParam);
LGBM_DLSYM(LGBM_BoosterFree);
LGBM_DLSYM(LGBM_BoosterShuffleModels);
LGBM_DLSYM(LGBM_BoosterMerge);
LGBM_DLSYM(LGBM_BoosterAddValidData);
LGBM_DLSYM(LGBM_BoosterResetTrainingData);
LGBM_DLSYM(LGBM_BoosterResetParameter);
LGBM_DLSYM(LGBM_BoosterGetNumClasses);
LGBM_DLSYM(LGBM_BoosterUpdateOneIter);
LGBM_DLSYM(LGBM_BoosterRefit);
LGBM_DLSYM(LGBM_BoosterUpdateOneIterCustom);
LGBM_DLSYM(LGBM_BoosterRollbackOneIter);
LGBM_DLSYM(LGBM_BoosterGetCurrentIteration);
LGBM_DLSYM(LGBM_BoosterNumModelPerIteration);
LGBM_DLSYM(LGBM_BoosterNumberOfTotalModel);
LGBM_DLSYM(LGBM_BoosterGetEvalCounts);
LGBM_DLSYM(LGBM_BoosterGetEvalNames);
LGBM_DLSYM(LGBM_BoosterGetFeatureNames);
LGBM_DLSYM(LGBM_BoosterValidateFeatureNames);
LGBM_DLSYM(LGBM_BoosterGetNumFeature);
LGBM_DLSYM(LGBM_BoosterGetEval);
LGBM_DLSYM(LGBM_BoosterGetNumPredict);
LGBM_DLSYM(LGBM_BoosterGetPredict);
LGBM_DLSYM(LGBM_BoosterPredictForFile);
LGBM_DLSYM(LGBM_BoosterCalcNumPredict);
LGBM_DLSYM(LGBM_FastConfigFree);
LGBM_DLSYM(LGBM_BoosterPredictForCSR);
LGBM_DLSYM(LGBM_BoosterPredictSparseOutput);
LGBM_DLSYM(LGBM_BoosterFreePredictSparse);
LGBM_DLSYM(LGBM_BoosterPredictForCSRSingleRow);
LGBM_DLSYM(LGBM_BoosterPredictForCSRSingleRowFastInit);
LGBM_DLSYM(LGBM_BoosterPredictForCSRSingleRowFast);
LGBM_DLSYM(LGBM_BoosterPredictForCSC);
LGBM_DLSYM(LGBM_BoosterPredictForMat);
LGBM_DLSYM(LGBM_BoosterPredictForMatSingleRow);
LGBM_DLSYM(LGBM_BoosterPredictForMatSingleRowFastInit);
LGBM_DLSYM(LGBM_BoosterPredictForMatSingleRowFast);
LGBM_DLSYM(LGBM_BoosterPredictForMats);
LGBM_DLSYM(LGBM_BoosterPredictForArrow);
LGBM_DLSYM(LGBM_BoosterSaveModel);
LGBM_DLSYM(LGBM_BoosterSaveModelToString);
LGBM_DLSYM(LGBM_BoosterDumpModel);
LGBM_DLSYM(LGBM_BoosterGetLeafValue);
LGBM_DLSYM(LGBM_BoosterSetLeafValue);
LGBM_DLSYM(LGBM_BoosterFeatureImportance);
LGBM_DLSYM(LGBM_BoosterGetUpperBoundValue);
LGBM_DLSYM(LGBM_BoosterGetLowerBoundValue);
LGBM_DLSYM(LGBM_NetworkInit);
LGBM_DLSYM(LGBM_NetworkFree);
LGBM_DLSYM(LGBM_NetworkInitWithFunctions);
LGBM_DLSYM(LGBM_SetMaxThreads);
LGBM_DLSYM(LGBM_GetMaxThreads);
}
#endif // SRC_SQLMATH_BASE_C2


/*
repo https://github.com/nodejs/node/tree/v10.22.1
committed 2020-09-08T23:55:04Z
*/


/*
file https://github.com/nodejs/node/blob/v10.22.1/LICENSE
*/

#if defined(SRC_SQLMATH_NODEJS_C2)
/*
Node.js is licensed for use as follows:

"""
Copyright Node.js contributors. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
"""

This license applies to parts of Node.js originating from the
https://github.com/joyent/node repository:

"""
Copyright Joyent, Inc. and other Node contributors. All rights reserved.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
"""

The Node.js license applies to all parts of Node.js that are not externally
maintained libraries.

The externally maintained libraries used by Node.js are:

- Acorn, located at deps/acorn, is licensed as follows:
  """
    Copyright (C) 2012-2018 by various contributors (see AUTHORS)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
  """

- c-ares, located at deps/cares, is licensed as follows:
  """
    Copyright (c) 2007 - 2018, Daniel Stenberg with many contributors, see AUTHORS
    file.

    Copyright 1998 by the Massachusetts Institute of Technology.

    Permission to use, copy, modify, and distribute this software and its
    documentation for any purpose and without fee is hereby granted, provided that
    the above copyright notice appear in all copies and that both that copyright
    notice and this permission notice appear in supporting documentation, and that
    the name of M.I.T. not be used in advertising or publicity pertaining to
    distribution of the software without specific, written prior permission.
    M.I.T. makes no representations about the suitability of this software for any
    purpose.  It is provided "as is" without express or implied warranty.
  """

- HTTP Parser, located at deps/http_parser, is licensed as follows:
  """
    Copyright Joyent, Inc. and other Node contributors.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
  """

- ICU, located at deps/icu-small, is licensed as follows:
  """
    COPYRIGHT AND PERMISSION NOTICE (ICU 58 and later)

    Copyright © 1991-2019 Unicode, Inc. All rights reserved.
    Distributed under the Terms of Use in https://www.unicode.org/copyright.html.

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of the Unicode data files and any associated documentation
    (the "Data Files") or Unicode software and any associated documentation
    (the "Software") to deal in the Data Files or Software
    without restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, and/or sell copies of
    the Data Files or Software, and to permit persons to whom the Data Files
    or Software are furnished to do so, provided that either
    (a) this copyright and permission notice appear with all copies
    of the Data Files or Software, or
    (b) this copyright and permission notice appear in associated
    Documentation.

    THE DATA FILES AND SOFTWARE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF
    ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT OF THIRD PARTY RIGHTS.
    IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS
    NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL
    DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
    DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
    PERFORMANCE OF THE DATA FILES OR SOFTWARE.

    Except as contained in this notice, the name of a copyright holder
    shall not be used in advertising or otherwise to promote the sale,
    use or other dealings in these Data Files or Software without prior
    written authorization of the copyright holder.

    ---------------------

    Third-Party Software Licenses

    This section contains third-party software notices and/or additional
    terms for licensed third-party software components included within ICU
    libraries.

    1. ICU License - ICU 1.8.1 to ICU 57.1

    COPYRIGHT AND PERMISSION NOTICE

    Copyright (c) 1995-2016 International Business Machines Corporation and others
    All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, and/or sell copies of the Software, and to permit persons
    to whom the Software is furnished to do so, provided that the above
    copyright notice(s) and this permission notice appear in all copies of
    the Software and that both the above copyright notice(s) and this
    permission notice appear in supporting documentation.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
    OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY
    SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER
    RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
    CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
    CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

    Except as contained in this notice, the name of a copyright holder
    shall not be used in advertising or otherwise to promote the sale, use
    or other dealings in this Software without prior written authorization
    of the copyright holder.

    All trademarks and registered trademarks mentioned herein are the
    property of their respective owners.

    2. Chinese/Japanese Word Break Dictionary Data (cjdict.txt)

     #     The Google Chrome software developed by Google is licensed under
     # the BSD license. Other software included in this distribution is
     # provided under other licenses, as set forth below.
     #
     #  The BSD License
     #  http://opensource.org/licenses/bsd-license.php
     #  Copyright (C) 2006-2008, Google Inc.
     #
     #  All rights reserved.
     #
     #  Redistribution and use in source and binary forms, with or without
     # modification, are permitted provided that the following conditions are met:
     #
     #  Redistributions of source code must retain the above copyright notice,
     # this list of conditions and the following disclaimer.
     #  Redistributions in binary form must reproduce the above
     # copyright notice, this list of conditions and the following
     # disclaimer in the documentation and/or other materials provided with
     # the distribution.
     #  Neither the name of  Google Inc. nor the names of its
     # contributors may be used to endorse or promote products derived from
     # this software without specific prior written permission.
     #
     #
     #  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
     # CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
     # INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
     # MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
     # DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
     # LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
     # CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
     # SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
     # BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
     # LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
     # NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
     # SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
     #
     #
     #  The word list in cjdict.txt are generated by combining three word lists
     # listed below with further processing for compound word breaking. The
     # frequency is generated with an iterative training against Google web
     # corpora.
     #
     #  * Libtabe (Chinese)
     #    - https://sourceforge.net/project/?group_id=1519
     #    - Its license terms and conditions are shown below.
     #
     #  * IPADIC (Japanese)
     #    - http://chasen.aist-nara.ac.jp/chasen/distribution.html
     #    - Its license terms and conditions are shown below.
     #
     #  ---------COPYING.libtabe ---- BEGIN--------------------
     #
     #  /\*
     #   * Copyright (c) 1999 TaBE Project.
     #   * Copyright (c) 1999 Pai-Hsiang Hsiao.
     #   * All rights reserved.
     #   *
     #   * Redistribution and use in source and binary forms, with or without
     #   * modification, are permitted provided that the following conditions
     #   * are met:
     #   *
     #   * . Redistributions of source code must retain the above copyright
     #   *   notice, this list of conditions and the following disclaimer.
     #   * . Redistributions in binary form must reproduce the above copyright
     #   *   notice, this list of conditions and the following disclaimer in
     #   *   the documentation and/or other materials provided with the
     #   *   distribution.
     #   * . Neither the name of the TaBE Project nor the names of its
     #   *   contributors may be used to endorse or promote products derived
     #   *   from this software without specific prior written permission.
     #   *
     #   * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     #   * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     #   * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     #   * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
     #   * REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
     #   * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
     #   * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
     #   * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
     #   * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
     #   * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
     #   * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
     #   * OF THE POSSIBILITY OF SUCH DAMAGE.
     #   *\/
     #
     #  /\*
     #   * Copyright (c) 1999 Computer Systems and Communication Lab,
     #   *                    Institute of Information Science, Academia
     #       *                    Sinica. All rights reserved.
     #   *
     #   * Redistribution and use in source and binary forms, with or without
     #   * modification, are permitted provided that the following conditions
     #   * are met:
     #   *
     #   * . Redistributions of source code must retain the above copyright
     #   *   notice, this list of conditions and the following disclaimer.
     #   * . Redistributions in binary form must reproduce the above copyright
     #   *   notice, this list of conditions and the following disclaimer in
     #   *   the documentation and/or other materials provided with the
     #   *   distribution.
     #   * . Neither the name of the Computer Systems and Communication Lab
     #   *   nor the names of its contributors may be used to endorse or
     #   *   promote products derived from this software without specific
     #   *   prior written permission.
     #   *
     #   * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     #   * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     #   * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     #   * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
     #   * REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
     #   * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
     #   * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
     #   * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
     #   * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
     #   * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
     #   * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
     #   * OF THE POSSIBILITY OF SUCH DAMAGE.
     #   *\/
     #
     #  Copyright 1996 Chih-Hao Tsai @ Beckman Institute,
     #      University of Illinois
     #  c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4
     #
     #  ---------------COPYING.libtabe-----END--------------------------------
     #
     #
     #  ---------------COPYING.ipadic-----BEGIN-------------------------------
     #
     #  Copyright 2000, 2001, 2002, 2003 Nara Institute of Science
     #  and Technology.  All Rights Reserved.
     #
     #  Use, reproduction, and distribution of this software is permitted.
     #  Any copy of this software, whether in its original form or modified,
     #  must include both the above copyright notice and the following
     #  paragraphs.
     #
     #  Nara Institute of Science and Technology (NAIST),
     #  the copyright holders, disclaims all warranties with regard to this
     #  software, including all implied warranties of merchantability and
     #  fitness, in no event shall NAIST be liable for
     #  any special, indirect or consequential damages or any damages
     #  whatsoever resulting from loss of use, data or profits, whether in an
     #  action of contract, negligence or other tortuous action, arising out
     #  of or in connection with the use or performance of this software.
     #
     #  A large portion of the dictionary entries
     #  originate from ICOT Free Software.  The following conditions for ICOT
     #  Free Software applies to the current dictionary as well.
     #
     #  Each User may also freely distribute the Program, whether in its
     #  original form or modified, to any third party or parties, PROVIDED
     #  that the provisions of Section 3 ("NO WARRANTY") will ALWAYS appear
     #  on, or be attached to, the Program, which is distributed substantially
     #  in the same form as set out herein and that such intended
     #  distribution, if actually made, will neither violate or otherwise
     #  contravene any of the laws and regulations of the countries having
     #  jurisdiction over the User or the intended distribution itself.
     #
     #  NO WARRANTY
     #
     #  The program was produced on an experimental basis in the course of the
     #  research and development conducted during the project and is provided
     #  to users as so produced on an experimental basis.  Accordingly, the
     #  program is provided without any warranty whatsoever, whether express,
     #  implied, statutory or otherwise.  The term "warranty" used herein
     #  includes, but is not limited to, any warranty of the quality,
     #  performance, merchantability and fitness for a particular purpose of
     #  the program and the nonexistence of any infringement or violation of
     #  any right of any third party.
     #
     #  Each user of the program will agree and understand, and be deemed to
     #  have agreed and understood, that there is no warranty whatsoever for
     #  the program and, accordingly, the entire risk arising from or
     #  otherwise connected with the program is assumed by the user.
     #
     #  Therefore, neither ICOT, the copyright holder, or any other
     #  organization that participated in or was otherwise related to the
     #  development of the program and their respective officials, directors,
     #  officers and other employees shall be held liable for any and all
     #  damages, including, without limitation, general, special, incidental
     #  and consequential damages, arising out of or otherwise in connection
     #  with the use or inability to use the program or any product, material
     #  or result produced or otherwise obtained by using the program,
     #  regardless of whether they have been advised of, or otherwise had
     #  knowledge of, the possibility of such damages at any time during the
     #  project or thereafter.  Each user will be deemed to have agreed to the
     #  foregoing by his or her commencement of use of the program.  The term
     #  "use" as used herein includes, but is not limited to, the use,
     #  modification, copying and distribution of the program and the
     #  production of secondary products from the program.
     #
     #  In the case where the program, whether in its original form or
     #  modified, was distributed or delivered to or received by a user from
     #  any person, organization or entity other than ICOT, unless it makes or
     #  grants independently of ICOT any specific warranty to the user in
     #  writing, such person, organization or entity, will also be exempted
     #  from and not be held liable to the user for any such damages as noted
     #  above as far as the program is concerned.
     #
     #  ---------------COPYING.ipadic-----END----------------------------------

    3. Lao Word Break Dictionary Data (laodict.txt)

     #  Copyright (c) 2013 International Business Machines Corporation
     #  and others. All Rights Reserved.
     #
     # Project: http://code.google.com/p/lao-dictionary/
     # Dictionary: http://lao-dictionary.googlecode.com/git/Lao-Dictionary.txt
     # License: http://lao-dictionary.googlecode.com/git/Lao-Dictionary-LICENSE.txt
     #              (copied below)
     #
     #  This file is derived from the above dictionary, with slight
     #  modifications.
     #  ----------------------------------------------------------------------
     #  Copyright (C) 2013 Brian Eugene Wilson, Robert Martin Campbell.
     #  All rights reserved.
     #
     #  Redistribution and use in source and binary forms, with or without
     #  modification,
     #  are permitted provided that the following conditions are met:
     #
     #
     # Redistributions of source code must retain the above copyright notice, this
     #  list of conditions and the following disclaimer. Redistributions in
     #  binary form must reproduce the above copyright notice, this list of
     #  conditions and the following disclaimer in the documentation and/or
     #  other materials provided with the distribution.
     #
     #
     # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     # "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     # LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     # FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
     # COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
     # INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
     # (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
     # SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
     # HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
     # STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
     # ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
     # OF THE POSSIBILITY OF SUCH DAMAGE.
     #  --------------------------------------------------------------------------

    4. Burmese Word Break Dictionary Data (burmesedict.txt)

     #  Copyright (c) 2014 International Business Machines Corporation
     #  and others. All Rights Reserved.
     #
     #  This list is part of a project hosted at:
     #    github.com/kanyawtech/myanmar-karen-word-lists
     #
     #  --------------------------------------------------------------------------
     #  Copyright (c) 2013, LeRoy Benjamin Sharon
     #  All rights reserved.
     #
     #  Redistribution and use in source and binary forms, with or without
     #  modification, are permitted provided that the following conditions
     #  are met: Redistributions of source code must retain the above
     #  copyright notice, this list of conditions and the following
     #  disclaimer.  Redistributions in binary form must reproduce the
     #  above copyright notice, this list of conditions and the following
     #  disclaimer in the documentation and/or other materials provided
     #  with the distribution.
     #
     #    Neither the name Myanmar Karen Word Lists, nor the names of its
     #    contributors may be used to endorse or promote products derived
     #    from this software without specific prior written permission.
     #
     #  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
     #  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
     #  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
     #  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
     #  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
     #  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
     #  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
     #  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
     #  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
     #  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
     #  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
     #  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
     #  SUCH DAMAGE.
     #  --------------------------------------------------------------------------

    5. Time Zone Database

      ICU uses the public domain data and code derived from Time Zone
    Database for its time zone support. The ownership of the TZ database
    is explained in BCP 175: Procedure for Maintaining the Time Zone
    Database section 7.

     # 7.  Database Ownership
     #
     #    The TZ database itself is not an IETF Contribution or an IETF
     #    document.  Rather it is a pre-existing and regularly updated work
     #    that is in the public domain, and is intended to remain in the
     #    public domain.  Therefore, BCPs 78 [RFC5378] and 79 [RFC3979] do
     #    not apply to the TZ Database or contributions that individuals make
     #    to it.  Should any claims be made and substantiated against the TZ
     #    Database, the organization that is providing the IANA
     #    Considerations defined in this RFC, under the memorandum of
     #    understanding with the IETF, currently ICANN, may act in accordance
     #    with all competent court orders.  No ownership claims will be made
     #    by ICANN or the IETF Trust on the database or the code.  Any person
     #    making a contribution to the database or code waives all rights to
     #    future claims in that contribution or in the TZ Database.

    6. Google double-conversion

    Copyright 2006-2011, the V8 project authors. All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above
          copyright notice, this list of conditions and the following
          disclaimer in the documentation and/or other materials provided
          with the distribution.
        * Neither the name of Google Inc. nor the names of its
          contributors may be used to endorse or promote products derived
          from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- libuv, located at deps/uv, is licensed as follows:
  """
    libuv is licensed for use as follows:

    ====
    Copyright (c) 2015-present libuv project contributors.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
    ====

    This license applies to parts of libuv originating from the
    https://github.com/joyent/libuv repository:

    ====

    Copyright Joyent, Inc. and other Node contributors. All rights reserved.
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.

    ====

    This license applies to all parts of libuv that are not externally
    maintained libraries.

    The externally maintained libraries used by libuv are:

      - tree.h (from FreeBSD), copyright Niels Provos. Two clause BSD license.

      - inet_pton and inet_ntop implementations, contained in src/inet.c, are
        copyright the Internet Systems Consortium, Inc., and licensed under the ISC
        license.

      - stdint-msvc2008.h (from msinttypes), copyright Alexander Chemeris. Three
        clause BSD license.

      - pthread-fixes.c, copyright Google Inc. and Sony Mobile Communications AB.
        Three clause BSD license.

      - android-ifaddrs.h, android-ifaddrs.c, copyright Berkeley Software Design
        Inc, Kenneth MacKay and Emergya (Cloud4all, FP7/2007-2013, grant agreement
        n° 289016). Three clause BSD license.
  """

- OpenSSL, located at deps/openssl, is licensed as follows:
  """
    Copyright (c) 1998-2018 The OpenSSL Project.  All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.

    3. All advertising materials mentioning features or use of this
    software must display the following acknowledgment:
    "This product includes software developed by the OpenSSL Project
    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"

    4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
    endorse or promote products derived from this software without
    prior written permission. For written permission, please contact
    openssl-core@openssl.org.

    5. Products derived from this software may not be called "OpenSSL"
    nor may "OpenSSL" appear in their names without prior written
    permission of the OpenSSL Project.

    6. Redistributions of any form whatsoever must retain the following
    acknowledgment:
    "This product includes software developed by the OpenSSL Project
    for use in the OpenSSL Toolkit (http://www.openssl.org/)"

    THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
    EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
    ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
    OF THE POSSIBILITY OF SUCH DAMAGE.
    ====================================================================

    This product includes cryptographic software written by Eric Young
    (eay@cryptsoft.com).  This product includes software written by Tim
    Hudson (tjh@cryptsoft.com).
  """

- Punycode.js, located at lib/punycode.js, is licensed as follows:
  """
    Copyright Mathias Bynens <https://mathiasbynens.be/>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
    LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  """

- V8, located at deps/v8, is licensed as follows:
  """
    This license applies to all parts of V8 that are not externally
    maintained libraries.  The externally maintained libraries used by V8
    are:

      - PCRE test suite, located in
        test/mjsunit/third_party/regexp-pcre/regexp-pcre.js.  This is based on the
        test suite from PCRE-7.3, which is copyrighted by the University
        of Cambridge and Google, Inc.  The copyright notice and license
        are embedded in regexp-pcre.js.

      - Layout tests, located in test/mjsunit/third_party/object-keys.  These are
        based on layout tests from webkit.org which are copyrighted by
        Apple Computer, Inc. and released under a 3-clause BSD license.

      - Strongtalk assembler, the basis of the files assembler-arm-inl.h,
        assembler-arm.cc, assembler-arm.h, assembler-ia32-inl.h,
        assembler-ia32.cc, assembler-ia32.h, assembler-x64-inl.h,
        assembler-x64.cc, assembler-x64.h, assembler-mips-inl.h,
        assembler-mips.cc, assembler-mips.h, assembler.cc and assembler.h.
        This code is copyrighted by Sun Microsystems Inc. and released
        under a 3-clause BSD license.

      - Valgrind client API header, located at third_party/valgrind/valgrind.h
        This is release under the BSD license.

      - antlr4 parser generator Cpp library located in third_party/antlr4
        This is release under the BSD license.

    These libraries have their own licenses; we recommend you read them,
    as their terms may differ from the terms below.

    Further license information can be found in LICENSE files located in
    sub-directories.

    Copyright 2014, the V8 project authors. All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above
          copyright notice, this list of conditions and the following
          disclaimer in the documentation and/or other materials provided
          with the distribution.
        * Neither the name of Google Inc. nor the names of its
          contributors may be used to endorse or promote products derived
          from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- SipHash, located at deps/v8/src/third_party/siphash, is licensed as follows:
  """
    SipHash reference C implementation

    Copyright (c) 2016 Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>

    To the extent possible under law, the author(s) have dedicated all
    copyright and related and neighboring rights to this software to the public
    domain worldwide. This software is distributed without any warranty.
  """

- zlib, located at deps/zlib, is licensed as follows:
  """
    zlib.h -- interface of the 'zlib' general purpose compression library
    version 1.2.11, January 15th, 2017

    Copyright (C) 1995-2017 Jean-loup Gailly and Mark Adler

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.

    Jean-loup Gailly        Mark Adler
    jloup@gzip.org          madler@alumni.caltech.edu
  """

- npm, located at deps/npm, is licensed as follows:
  """
    The npm application
    Copyright (c) npm, Inc. and Contributors
    Licensed on the terms of The Artistic License 2.0

    Node package dependencies of the npm application
    Copyright (c) their respective copyright owners
    Licensed on their respective license terms

    The npm public registry at https://registry.npmjs.org
    and the npm website at https://www.npmjs.com
    Operated by npm, Inc.
    Use governed by terms published on https://www.npmjs.com

    "Node.js"
    Trademark Joyent, Inc., https://joyent.com
    Neither npm nor npm, Inc. are affiliated with Joyent, Inc.

    The Node.js application
    Project of Node Foundation, https://nodejs.org

    The npm Logo
    Copyright (c) Mathias Pettersson and Brian Hammond

    "Gubblebum Blocky" typeface
    Copyright (c) Tjarda Koster, https://jelloween.deviantart.com
    Used with permission

    --------

    The Artistic License 2.0

    Copyright (c) 2000-2006, The Perl Foundation.

    Everyone is permitted to copy and distribute verbatim copies
    of this license document, but changing it is not allowed.

    Preamble

    This license establishes the terms under which a given free software
    Package may be copied, modified, distributed, and/or redistributed.
    The intent is that the Copyright Holder maintains some artistic
    control over the development of that Package while still keeping the
    Package available as open source and free software.

    You are always permitted to make arrangements wholly outside of this
    license directly with the Copyright Holder of a given Package.  If the
    terms of this license do not permit the full use that you propose to
    make of the Package, you should contact the Copyright Holder and seek
    a different licensing arrangement.

    Definitions

        "Copyright Holder" means the individual(s) or organization(s)
        named in the copyright notice for the entire Package.

        "Contributor" means any party that has contributed code or other
        material to the Package, in accordance with the Copyright Holder's
        procedures.

        "You" and "your" means any person who would like to copy,
        distribute, or modify the Package.

        "Package" means the collection of files distributed by the
        Copyright Holder, and derivatives of that collection and/or of
        those files. A given Package may consist of either the Standard
        Version, or a Modified Version.

        "Distribute" means providing a copy of the Package or making it
        accessible to anyone else, or in the case of a company or
        organization, to others outside of your company or organization.

        "Distributor Fee" means any fee that you charge for Distributing
        this Package or providing support for this Package to another
        party.  It does not mean licensing fees.

        "Standard Version" refers to the Package if it has not been
        modified, or has been modified only in ways explicitly requested
        by the Copyright Holder.

        "Modified Version" means the Package, if it has been changed, and
        such changes were not explicitly requested by the Copyright
        Holder.

        "Original License" means this Artistic License as Distributed with
        the Standard Version of the Package, in its current version or as
        it may be modified by The Perl Foundation in the future.

        "Source" form means the source code, documentation source, and
        configuration files for the Package.

        "Compiled" form means the compiled bytecode, object code, binary,
        or any other form resulting from mechanical transformation or
        translation of the Source form.

    Permission for Use and Modification Without Distribution

    (1)  You are permitted to use the Standard Version and create and use
    Modified Versions for any purpose without restriction, provided that
    you do not Distribute the Modified Version.

    Permissions for Redistribution of the Standard Version

    (2)  You may Distribute verbatim copies of the Source form of the
    Standard Version of this Package in any medium without restriction,
    either gratis or for a Distributor Fee, provided that you duplicate
    all of the original copyright notices and associated disclaimers.  At
    your discretion, such verbatim copies may or may not include a
    Compiled form of the Package.

    (3)  You may apply any bug fixes, portability changes, and other
    modifications made available from the Copyright Holder.  The resulting
    Package will still be considered the Standard Version, and as such
    will be subject to the Original License.

    Distribution of Modified Versions of the Package as Source

    (4)  You may Distribute your Modified Version as Source (either gratis
    or for a Distributor Fee, and with or without a Compiled form of the
    Modified Version) provided that you clearly document how it differs
    from the Standard Version, including, but not limited to, documenting
    any non-standard features, executables, or modules, and provided that
    you do at least ONE of the following:

        (a)  make the Modified Version available to the Copyright Holder
        of the Standard Version, under the Original License, so that the
        Copyright Holder may include your modifications in the Standard
        Version.

        (b)  ensure that installation of your Modified Version does not
        prevent the user installing or running the Standard Version. In
        addition, the Modified Version must bear a name that is different
        from the name of the Standard Version.

        (c)  allow anyone who receives a copy of the Modified Version to
        make the Source form of the Modified Version available to others
        under

            (i)  the Original License or

            (ii)  a license that permits the licensee to freely copy,
            modify and redistribute the Modified Version using the same
            licensing terms that apply to the copy that the licensee
            received, and requires that the Source form of the Modified
            Version, and of any works derived from it, be made freely
            available in that license fees are prohibited but Distributor
            Fees are allowed.

    Distribution of Compiled Forms of the Standard Version
    or Modified Versions without the Source

    (5)  You may Distribute Compiled forms of the Standard Version without
    the Source, provided that you include complete instructions on how to
    get the Source of the Standard Version.  Such instructions must be
    valid at the time of your distribution.  If these instructions, at any
    time while you are carrying out such distribution, become invalid, you
    must provide new instructions on demand or cease further distribution.
    If you provide valid instructions or cease distribution within thirty
    days after you become aware that the instructions are invalid, then
    you do not forfeit any of your rights under this license.

    (6)  You may Distribute a Modified Version in Compiled form without
    the Source, provided that you comply with Section 4 with respect to
    the Source of the Modified Version.

    Aggregating or Linking the Package

    (7)  You may aggregate the Package (either the Standard Version or
    Modified Version) with other packages and Distribute the resulting
    aggregation provided that you do not charge a licensing fee for the
    Package.  Distributor Fees are permitted, and licensing fees for other
    components in the aggregation are permitted. The terms of this license
    apply to the use and Distribution of the Standard or Modified Versions
    as included in the aggregation.

    (8) You are permitted to link Modified and Standard Versions with
    other works, to embed the Package in a larger work of your own, or to
    build stand-alone binary or bytecode versions of applications that
    include the Package, and Distribute the result without restriction,
    provided the result does not expose a direct interface to the Package.

    Items That are Not Considered Part of a Modified Version

    (9) Works (including, but not limited to, modules and scripts) that
    merely extend or make use of the Package, do not, by themselves, cause
    the Package to be a Modified Version.  In addition, such works are not
    considered parts of the Package itself, and are not subject to the
    terms of this license.

    General Provisions

    (10)  Any use, modification, and distribution of the Standard or
    Modified Versions is governed by this Artistic License. By using,
    modifying or distributing the Package, you accept this license. Do not
    use, modify, or distribute the Package, if you do not accept this
    license.

    (11)  If your Modified Version has been derived from a Modified
    Version made by someone other than you, you are nevertheless required
    to ensure that your Modified Version complies with the requirements of
    this license.

    (12)  This license does not grant you the right to use any trademark,
    service mark, tradename, or logo of the Copyright Holder.

    (13)  This license includes the non-exclusive, worldwide,
    free-of-charge patent license to make, have made, use, offer to sell,
    sell, import and otherwise transfer the Package with respect to any
    patent claims licensable by the Copyright Holder that are necessarily
    infringed by the Package. If you institute patent litigation
    (including a cross-claim or counterclaim) against any party alleging
    that the Package constitutes direct or contributory patent
    infringement, then this Artistic License to you shall terminate on the
    date that such litigation is filed.

    (14)  Disclaimer of Warranty:
    THE PACKAGE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS
    IS' AND WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES. THE IMPLIED
    WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
    NON-INFRINGEMENT ARE DISCLAIMED TO THE EXTENT PERMITTED BY YOUR LOCAL
    LAW. UNLESS REQUIRED BY LAW, NO COPYRIGHT HOLDER OR CONTRIBUTOR WILL
    BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL
    DAMAGES ARISING IN ANY WAY OUT OF THE USE OF THE PACKAGE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    --------
  """

- GYP, located at tools/gyp, is licensed as follows:
  """
    Copyright (c) 2009 Google Inc. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

       * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following disclaimer
    in the documentation and/or other materials provided with the
    distribution.
       * Neither the name of Google Inc. nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- inspector_protocol, located at tools/inspector_protocol, is licensed as follows:
  """
    // Copyright 2016 The Chromium Authors. All rights reserved.
    //
    // Redistribution and use in source and binary forms, with or without
    // modification, are permitted provided that the following conditions are
    // met:
    //
    //    * Redistributions of source code must retain the above copyright
    // notice, this list of conditions and the following disclaimer.
    //    * Redistributions in binary form must reproduce the above
    // copyright notice, this list of conditions and the following disclaimer
    // in the documentation and/or other materials provided with the
    // distribution.
    //    * Neither the name of Google Inc. nor the names of its
    // contributors may be used to endorse or promote products derived from
    // this software without specific prior written permission.
    //
    // THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    // "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    // LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    // A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    // OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    // SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    // LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    // DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    // THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    // (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    // OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- jinja2, located at tools/inspector_protocol/jinja2, is licensed as follows:
  """
    Copyright (c) 2009 by the Jinja Team, see AUTHORS for more details.

    Some rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.

        * Redistributions in binary form must reproduce the above
          copyright notice, this list of conditions and the following
          disclaimer in the documentation and/or other materials provided
          with the distribution.

        * The names of the contributors may not be used to endorse or
          promote products derived from this software without specific
          prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- markupsafe, located at tools/inspector_protocol/markupsafe, is licensed as follows:
  """
    Copyright (c) 2010 by Armin Ronacher and contributors.  See AUTHORS
    for more details.

    Some rights reserved.

    Redistribution and use in source and binary forms of the software as well
    as documentation, with or without modification, are permitted provided
    that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * The names of the contributors may not be used to endorse or
      promote products derived from this software without specific
      prior written permission.

    THIS SOFTWARE AND DOCUMENTATION IS PROVIDED BY THE COPYRIGHT HOLDERS AND
    CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
    NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
    OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE AND DOCUMENTATION, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
    DAMAGE.
  """

- cpplint.py, located at tools/cpplint.py, is licensed as follows:
  """
    Copyright (c) 2009 Google Inc. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

       * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following disclaimer
    in the documentation and/or other materials provided with the
    distribution.
       * Neither the name of Google Inc. nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- ESLint, located at tools/node_modules/eslint, is licensed as follows:
  """
    Copyright JS Foundation and other contributors, https://js.foundation

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
  """

- babel-eslint, located at tools/node_modules/babel-eslint, is licensed as follows:
  """
    Copyright (c) 2014-2016 Sebastian McKenzie <sebmck@gmail.com>

    MIT License

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
    LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  """

- gtest, located at test/cctest/gtest, is licensed as follows:
  """
    Copyright 2008, Google Inc.
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following disclaimer
    in the documentation and/or other materials provided with the
    distribution.
        * Neither the name of Google Inc. nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- nghttp2, located at deps/nghttp2, is licensed as follows:
  """
    The MIT License

    Copyright (c) 2012, 2014, 2015, 2016 Tatsuhiro Tsujikawa
    Copyright (c) 2012, 2014, 2015, 2016 nghttp2 contributors

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
    LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  """

- node-inspect, located at deps/node-inspect, is licensed as follows:
  """
    Copyright Node.js contributors. All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
  """

- large_pages, located at src/large_pages, is licensed as follows:
  """
     Copyright (C) 2018 Intel Corporation

     Permission is hereby granted, free of charge, to any person obtaining a copy
     of this software and associated documentation files (the "Software"),
     to deal in the Software without restriction, including without limitation
     the rights to use, copy, modify, merge, publish, distribute, sublicense,
     and/or sell copies of the Software, and to permit persons to whom
     the Software is furnished to do so, subject to the following conditions:

     The above copyright notice and this permission notice shall be included
     in all copies or substantial portions of the Software.

     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
     OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
     THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
     OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
     ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
     OR OTHER DEALINGS IN THE SOFTWARE.
  """

- brotli, located at deps/brotli, is licensed as follows:
  """
    Copyright (c) 2009, 2010, 2013-2016 by the Brotli Authors.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
  """
*/


/*
file https://github.com/nodejs/node/blob/v10.22.1/src/node_api_types.h
*/
#ifndef SRC_NODE_API_TYPES_H_
#define SRC_NODE_API_TYPES_H_

#include <stddef.h>
#include <stdint.h>

#if !defined __cplusplus || (defined(_MSC_VER) && _MSC_VER < 1900)
    typedef uint16_t char16_t;
#endif

// JSVM API types are all opaque pointers for ABI stability
// typedef undefined structs instead of void* for compile time type safety
typedef struct napi_env__* napi_env;
typedef struct napi_value__* napi_value;
typedef struct napi_ref__* napi_ref;
typedef struct napi_handle_scope__* napi_handle_scope;
typedef struct napi_escapable_handle_scope__* napi_escapable_handle_scope;
typedef struct napi_callback_scope__* napi_callback_scope;
typedef struct napi_callback_info__* napi_callback_info;
typedef struct napi_async_context__* napi_async_context;
typedef struct napi_async_work__* napi_async_work;
typedef struct napi_deferred__* napi_deferred;
#if NAPI_VERSION >= 4
typedef struct napi_threadsafe_function__* napi_threadsafe_function;
#endif  // NAPI_VERSION >= 4

typedef enum {
  napi_default = 0,
  napi_writable = 1 << 0,
  napi_enumerable = 1 << 1,
  napi_configurable = 1 << 2,

  // Used with napi_define_class to distinguish static properties
  // from instance properties. Ignored by napi_define_properties.
  napi_static = 1 << 10,
} napi_property_attributes;

typedef enum {
  // ES6 types (corresponds to typeof)
  napi_undefined,
  napi_null,
  napi_boolean,
  napi_number,
  napi_string,
  napi_symbol,
  napi_object,
  napi_function,
  napi_external,
  napi_bigint,
} napi_valuetype;

typedef enum {
  napi_int8_array,
  napi_uint8_array,
  napi_uint8_clamped_array,
  napi_int16_array,
  napi_uint16_array,
  napi_int32_array,
  napi_uint32_array,
  napi_float32_array,
  napi_float64_array,
  napi_bigint64_array,
  napi_biguint64_array,
} napi_typedarray_type;

typedef enum {
  napi_ok,
  napi_invalid_arg,
  napi_object_expected,
  napi_string_expected,
  napi_name_expected,
  napi_function_expected,
  napi_number_expected,
  napi_boolean_expected,
  napi_array_expected,
  napi_generic_failure,
  napi_pending_exception,
  napi_cancelled,
  napi_escape_called_twice,
  napi_handle_scope_mismatch,
  napi_callback_scope_mismatch,
  napi_queue_full,
  napi_closing,
  napi_bigint_expected,
  napi_date_expected,
  napi_arraybuffer_expected,
  napi_detachable_arraybuffer_expected,
} napi_status;
// Note: when adding a new enum value to `napi_status`, please also update
// `const int last_status` in `napi_get_last_error_info()' definition,
// in file js_native_api_v8.cc. Please also update the definition of
// `napi_status` in doc/api/n-api.md to reflect the newly added value(s).

#if NAPI_VERSION >= 4
typedef enum {
  napi_tsfn_release,
  napi_tsfn_abort
} napi_threadsafe_function_release_mode;

typedef enum {
  napi_tsfn_nonblocking,
  napi_tsfn_blocking
} napi_threadsafe_function_call_mode;
#endif  // NAPI_VERSION >= 4

typedef napi_value (*napi_callback)(napi_env env,
                                    napi_callback_info info);
typedef void (*napi_finalize)(napi_env env,
                              void* finalize_data,
                              void* finalize_hint);
typedef void (*napi_async_execute_callback)(napi_env env,
                                            void* data);
typedef void (*napi_async_complete_callback)(napi_env env,
                                             napi_status status,
                                             void* data);

#if NAPI_VERSION >= 4
typedef void (*napi_threadsafe_function_call_js)(napi_env env,
                                                 napi_value js_callback,
                                                 void* context,
                                                 void* data);
#endif  // NAPI_VERSION >= 4

typedef struct {
  // One of utf8name or name should be NULL.
  const char* utf8name;
  napi_value name;

  napi_callback method;
  napi_callback getter;
  napi_callback setter;
  napi_value value;

  napi_property_attributes attributes;
  void* data;
} napi_property_descriptor;

typedef struct {
  const char* error_message;
  void* engine_reserved;
  uint32_t engine_error_code;
  napi_status error_code;
} napi_extended_error_info;

typedef struct {
  uint32_t major;
  uint32_t minor;
  uint32_t patch;
  const char* release;
} napi_node_version;

#if NAPI_VERSION >= 6
typedef enum {
  napi_key_include_prototypes,
  napi_key_own_only
} napi_key_collection_mode;

typedef enum {
  napi_key_all_properties = 0,
  napi_key_writable = 1,
  napi_key_enumerable = 1 << 1,
  napi_key_configurable = 1 << 2,
  napi_key_skip_strings = 1 << 3,
  napi_key_skip_symbols = 1 << 4
} napi_key_filter;

typedef enum {
  napi_key_keep_numbers,
  napi_key_numbers_to_strings
} napi_key_conversion;
#endif  // NAPI_VERSION >= 6

#endif  // SRC_NODE_API_TYPES_H_


/*
file https://github.com/nodejs/node/blob/v10.22.1/src/node_api.h
*/
#ifndef SRC_NODE_API_H_
#define SRC_NODE_API_H_

#include <stddef.h>
#include <stdbool.h>

#ifndef NAPI_VERSION
#ifdef NAPI_EXPERIMENTAL
// Use INT_MAX, this should only be consumed by the pre-processor anyway.
#define NAPI_VERSION 2147483647
#else
// The baseline version for N-API
#define NAPI_VERSION 6
#endif
#endif

// hack-nodejs - inline header
// #include "node_api_types.h"

struct uv_loop_s;  // Forward declaration.

#ifdef _WIN32
  #ifdef BUILDING_NODE_EXTENSION
    #ifdef EXTERNAL_NAPI
      // Building external N-API, or native module against external N-API
      #define NAPI_EXTERN /* nothing */
    #else
      // Building native module against node with built-in N-API
      #define NAPI_EXTERN __declspec(dllimport)
    #endif
  #else
    // Building node with built-in N-API
    #define NAPI_EXTERN __declspec(dllexport)
  #endif
#else
  #define NAPI_EXTERN /* nothing */
#endif

#ifdef _WIN32
# define NAPI_MODULE_EXPORT __declspec(dllexport)
#else
# define NAPI_MODULE_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __GNUC__
#define NAPI_NO_RETURN __attribute__((noreturn))
#else
#define NAPI_NO_RETURN
#endif


typedef napi_value (*napi_addon_register_func)(napi_env env,
                                               napi_value exports);

typedef struct {
  int nm_version;
  unsigned int nm_flags;
  const char* nm_filename;
  napi_addon_register_func nm_register_func;
  const char* nm_modname;
  void* nm_priv;
  void* reserved[4];
} napi_module;

#define NAPI_MODULE_VERSION  1

#if defined(_MSC_VER)
#pragma section(".CRT$XCU", read)
#define NAPI_C_CTOR(fn)                                                     \
  static void __cdecl fn(void);                                             \
  __declspec(dllexport, allocate(".CRT$XCU")) void(__cdecl * fn##_)(void) = \
      fn;                                                                   \
  static void __cdecl fn(void)
#else
#define NAPI_C_CTOR(fn)                              \
  static void fn(void) __attribute__((constructor)); \
  static void fn(void)
#endif

#ifdef __cplusplus
#define EXTERN_C_START extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_START
#define EXTERN_C_END
#endif

#define NAPI_MODULE_X(modname, regfunc, priv, flags)                  \
  EXTERN_C_START                                                      \
    static napi_module _module =                                      \
    {                                                                 \
      NAPI_MODULE_VERSION,                                            \
      flags,                                                          \
      __FILE__,                                                       \
      regfunc,                                                        \
      #modname,                                                       \
      priv,                                                           \
      {0},                                                            \
    };                                                                \
    NAPI_C_CTOR(_register_ ## modname) {                              \
      napi_module_register(&_module);                                 \
    }                                                                 \
  EXTERN_C_END

#define NAPI_MODULE(modname, regfunc)                                 \
  NAPI_MODULE_X(modname, regfunc, NULL, 0)  // NOLINT (readability/null_usage)

#define NAPI_MODULE_INITIALIZER_BASE napi_register_module_v

#define NAPI_MODULE_INITIALIZER_X(base, version)                      \
    NAPI_MODULE_INITIALIZER_X_HELPER(base, version)
#define NAPI_MODULE_INITIALIZER_X_HELPER(base, version) base##version

#define NAPI_MODULE_INITIALIZER                                       \
  NAPI_MODULE_INITIALIZER_X(NAPI_MODULE_INITIALIZER_BASE,             \
      NAPI_MODULE_VERSION)

#define NAPI_MODULE_INIT()                                            \
  EXTERN_C_START                                                      \
  NAPI_MODULE_EXPORT napi_value                                       \
  NAPI_MODULE_INITIALIZER(napi_env env, napi_value exports);          \
  EXTERN_C_END                                                        \
  NAPI_MODULE(NODE_GYP_MODULE_NAME, NAPI_MODULE_INITIALIZER)          \
  napi_value NAPI_MODULE_INITIALIZER(napi_env env,                    \
                                     napi_value exports)

#define NAPI_AUTO_LENGTH SIZE_MAX

EXTERN_C_START

NAPI_EXTERN void napi_module_register(napi_module* mod);

NAPI_EXTERN napi_status
napi_get_last_error_info(napi_env env,
                         const napi_extended_error_info** result);

NAPI_EXTERN NAPI_NO_RETURN void napi_fatal_error(const char* location,
                                                 size_t location_len,
                                                 const char* message,
                                                 size_t message_len);

// Getters for defined singletons
NAPI_EXTERN napi_status napi_get_undefined(napi_env env, napi_value* result);
NAPI_EXTERN napi_status napi_get_null(napi_env env, napi_value* result);
NAPI_EXTERN napi_status napi_get_global(napi_env env, napi_value* result);
NAPI_EXTERN napi_status napi_get_boolean(napi_env env,
                                         bool value,
                                         napi_value* result);

// Methods to create Primitive types/Objects
NAPI_EXTERN napi_status napi_create_object(napi_env env, napi_value* result);
NAPI_EXTERN napi_status napi_create_array(napi_env env, napi_value* result);
NAPI_EXTERN napi_status napi_create_array_with_length(napi_env env,
                                                      size_t length,
                                                      napi_value* result);
NAPI_EXTERN napi_status napi_create_double(napi_env env,
                                           double value,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_create_int32(napi_env env,
                                          int32_t value,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_create_uint32(napi_env env,
                                           uint32_t value,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_create_int64(napi_env env,
                                          int64_t value,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_create_string_latin1(napi_env env,
                                                  const char* str,
                                                  size_t length,
                                                  napi_value* result);
NAPI_EXTERN napi_status napi_create_string_utf8(napi_env env,
                                                const char* str,
                                                size_t length,
                                                napi_value* result);
NAPI_EXTERN napi_status napi_create_string_utf16(napi_env env,
                                                 const char16_t* str,
                                                 size_t length,
                                                 napi_value* result);
NAPI_EXTERN napi_status napi_create_symbol(napi_env env,
                                           napi_value description,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_create_function(napi_env env,
                                             const char* utf8name,
                                             size_t length,
                                             napi_callback cb,
                                             void* data,
                                             napi_value* result);
NAPI_EXTERN napi_status napi_create_error(napi_env env,
                                          napi_value code,
                                          napi_value msg,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_create_type_error(napi_env env,
                                               napi_value code,
                                               napi_value msg,
                                               napi_value* result);
NAPI_EXTERN napi_status napi_create_range_error(napi_env env,
                                                napi_value code,
                                                napi_value msg,
                                                napi_value* result);

// Methods to get the native napi_value from Primitive type
NAPI_EXTERN napi_status napi_typeof(napi_env env,
                                    napi_value value,
                                    napi_valuetype* result);
NAPI_EXTERN napi_status napi_get_value_double(napi_env env,
                                              napi_value value,
                                              double* result);
NAPI_EXTERN napi_status napi_get_value_int32(napi_env env,
                                             napi_value value,
                                             int32_t* result);
NAPI_EXTERN napi_status napi_get_value_uint32(napi_env env,
                                              napi_value value,
                                              uint32_t* result);
NAPI_EXTERN napi_status napi_get_value_int64(napi_env env,
                                             napi_value value,
                                             int64_t* result);
NAPI_EXTERN napi_status napi_get_value_bool(napi_env env,
                                            napi_value value,
                                            bool* result);

// Copies LATIN-1 encoded bytes from a string into a buffer.
NAPI_EXTERN napi_status napi_get_value_string_latin1(napi_env env,
                                                     napi_value value,
                                                     char* buf,
                                                     size_t bufsize,
                                                     size_t* result);

// Copies UTF-8 encoded bytes from a string into a buffer.
NAPI_EXTERN napi_status napi_get_value_string_utf8(napi_env env,
                                                   napi_value value,
                                                   char* buf,
                                                   size_t bufsize,
                                                   size_t* result);

// Copies UTF-16 encoded bytes from a string into a buffer.
NAPI_EXTERN napi_status napi_get_value_string_utf16(napi_env env,
                                                    napi_value value,
                                                    char16_t* buf,
                                                    size_t bufsize,
                                                    size_t* result);

// Methods to coerce values
// These APIs may execute user scripts
NAPI_EXTERN napi_status napi_coerce_to_bool(napi_env env,
                                            napi_value value,
                                            napi_value* result);
NAPI_EXTERN napi_status napi_coerce_to_number(napi_env env,
                                              napi_value value,
                                              napi_value* result);
NAPI_EXTERN napi_status napi_coerce_to_object(napi_env env,
                                              napi_value value,
                                              napi_value* result);
NAPI_EXTERN napi_status napi_coerce_to_string(napi_env env,
                                              napi_value value,
                                              napi_value* result);

// Methods to work with Objects
NAPI_EXTERN napi_status napi_get_prototype(napi_env env,
                                           napi_value object,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_get_property_names(napi_env env,
                                                napi_value object,
                                                napi_value* result);
NAPI_EXTERN napi_status napi_set_property(napi_env env,
                                          napi_value object,
                                          napi_value key,
                                          napi_value value);
NAPI_EXTERN napi_status napi_has_property(napi_env env,
                                          napi_value object,
                                          napi_value key,
                                          bool* result);
NAPI_EXTERN napi_status napi_get_property(napi_env env,
                                          napi_value object,
                                          napi_value key,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_delete_property(napi_env env,
                                             napi_value object,
                                             napi_value key,
                                             bool* result);
NAPI_EXTERN napi_status napi_has_own_property(napi_env env,
                                              napi_value object,
                                              napi_value key,
                                              bool* result);
NAPI_EXTERN napi_status napi_set_named_property(napi_env env,
                                          napi_value object,
                                          const char* utf8name,
                                          napi_value value);
NAPI_EXTERN napi_status napi_has_named_property(napi_env env,
                                          napi_value object,
                                          const char* utf8name,
                                          bool* result);
NAPI_EXTERN napi_status napi_get_named_property(napi_env env,
                                          napi_value object,
                                          const char* utf8name,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_set_element(napi_env env,
                                         napi_value object,
                                         uint32_t index,
                                         napi_value value);
NAPI_EXTERN napi_status napi_has_element(napi_env env,
                                         napi_value object,
                                         uint32_t index,
                                         bool* result);
NAPI_EXTERN napi_status napi_get_element(napi_env env,
                                         napi_value object,
                                         uint32_t index,
                                         napi_value* result);
NAPI_EXTERN napi_status napi_delete_element(napi_env env,
                                            napi_value object,
                                            uint32_t index,
                                            bool* result);
NAPI_EXTERN napi_status
napi_define_properties(napi_env env,
                       napi_value object,
                       size_t property_count,
                       const napi_property_descriptor* properties);

// Methods to work with Arrays
NAPI_EXTERN napi_status napi_is_array(napi_env env,
                                      napi_value value,
                                      bool* result);
NAPI_EXTERN napi_status napi_get_array_length(napi_env env,
                                              napi_value value,
                                              uint32_t* result);

// Methods to compare values
NAPI_EXTERN napi_status napi_strict_equals(napi_env env,
                                           napi_value lhs,
                                           napi_value rhs,
                                           bool* result);

// Methods to work with Functions
NAPI_EXTERN napi_status napi_call_function(napi_env env,
                                           napi_value recv,
                                           napi_value func,
                                           size_t argc,
                                           const napi_value* argv,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_new_instance(napi_env env,
                                          napi_value constructor,
                                          size_t argc,
                                          const napi_value* argv,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_instanceof(napi_env env,
                                        napi_value object,
                                        napi_value constructor,
                                        bool* result);

// Methods to work with napi_callbacks

// Gets all callback info in a single call. (Ugly, but faster.)
NAPI_EXTERN napi_status napi_get_cb_info(
    napi_env env,               // [in] NAPI environment handle
    napi_callback_info cbinfo,  // [in] Opaque callback-info handle
    size_t* argc,      // [in-out] Specifies the size of the provided argv array
                       // and receives the actual count of args.
    napi_value* argv,  // [out] Array of values
    napi_value* this_arg,  // [out] Receives the JS 'this' arg for the call
    void** data);          // [out] Receives the data pointer for the callback.

NAPI_EXTERN napi_status napi_get_new_target(napi_env env,
                                            napi_callback_info cbinfo,
                                            napi_value* result);
NAPI_EXTERN napi_status
napi_define_class(napi_env env,
                  const char* utf8name,
                  size_t length,
                  napi_callback constructor,
                  void* data,
                  size_t property_count,
                  const napi_property_descriptor* properties,
                  napi_value* result);

// Methods to work with external data objects
NAPI_EXTERN napi_status napi_wrap(napi_env env,
                                  napi_value js_object,
                                  void* native_object,
                                  napi_finalize finalize_cb,
                                  void* finalize_hint,
                                  napi_ref* result);
NAPI_EXTERN napi_status napi_unwrap(napi_env env,
                                    napi_value js_object,
                                    void** result);
NAPI_EXTERN napi_status napi_remove_wrap(napi_env env,
                                         napi_value js_object,
                                         void** result);
NAPI_EXTERN napi_status napi_create_external(napi_env env,
                                             void* data,
                                             napi_finalize finalize_cb,
                                             void* finalize_hint,
                                             napi_value* result);
NAPI_EXTERN napi_status napi_get_value_external(napi_env env,
                                                napi_value value,
                                                void** result);

// Methods to control object lifespan

// Set initial_refcount to 0 for a weak reference, >0 for a strong reference.
NAPI_EXTERN napi_status napi_create_reference(napi_env env,
                                              napi_value value,
                                              uint32_t initial_refcount,
                                              napi_ref* result);

// Deletes a reference. The referenced value is released, and may
// be GC'd unless there are other references to it.
NAPI_EXTERN napi_status napi_delete_reference(napi_env env, napi_ref ref);

// Increments the reference count, optionally returning the resulting count.
// After this call the  reference will be a strong reference because its
// refcount is >0, and the referenced object is effectively "pinned".
// Calling this when the refcount is 0 and the object is unavailable
// results in an error.
NAPI_EXTERN napi_status napi_reference_ref(napi_env env,
                                           napi_ref ref,
                                           uint32_t* result);

// Decrements the reference count, optionally returning the resulting count.
// If the result is 0 the reference is now weak and the object may be GC'd
// at any time if there are no other references. Calling this when the
// refcount is already 0 results in an error.
NAPI_EXTERN napi_status napi_reference_unref(napi_env env,
                                             napi_ref ref,
                                             uint32_t* result);

// Attempts to get a referenced value. If the reference is weak,
// the value might no longer be available, in that case the call
// is still successful but the result is NULL.
NAPI_EXTERN napi_status napi_get_reference_value(napi_env env,
                                                 napi_ref ref,
                                                 napi_value* result);

NAPI_EXTERN napi_status napi_open_handle_scope(napi_env env,
                                               napi_handle_scope* result);
NAPI_EXTERN napi_status napi_close_handle_scope(napi_env env,
                                                napi_handle_scope scope);
NAPI_EXTERN napi_status
napi_open_escapable_handle_scope(napi_env env,
                                 napi_escapable_handle_scope* result);
NAPI_EXTERN napi_status
napi_close_escapable_handle_scope(napi_env env,
                                  napi_escapable_handle_scope scope);

NAPI_EXTERN napi_status napi_escape_handle(napi_env env,
                                           napi_escapable_handle_scope scope,
                                           napi_value escapee,
                                           napi_value* result);

// Methods to support error handling
NAPI_EXTERN napi_status napi_throw(napi_env env, napi_value error);
NAPI_EXTERN napi_status napi_throw_error(napi_env env,
                                         const char* code,
                                         const char* msg);
NAPI_EXTERN napi_status napi_throw_type_error(napi_env env,
                                         const char* code,
                                         const char* msg);
NAPI_EXTERN napi_status napi_throw_range_error(napi_env env,
                                         const char* code,
                                         const char* msg);
NAPI_EXTERN napi_status napi_is_error(napi_env env,
                                      napi_value value,
                                      bool* result);

// Methods to support catching exceptions
NAPI_EXTERN napi_status napi_is_exception_pending(napi_env env, bool* result);
NAPI_EXTERN napi_status napi_get_and_clear_last_exception(napi_env env,
                                                          napi_value* result);

// Methods to provide node::Buffer functionality with napi types
NAPI_EXTERN napi_status napi_create_buffer(napi_env env,
                                           size_t length,
                                           void** data,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_create_external_buffer(napi_env env,
                                                    size_t length,
                                                    void* data,
                                                    napi_finalize finalize_cb,
                                                    void* finalize_hint,
                                                    napi_value* result);
NAPI_EXTERN napi_status napi_create_buffer_copy(napi_env env,
                                                size_t length,
                                                const void* data,
                                                void** result_data,
                                                napi_value* result);
NAPI_EXTERN napi_status napi_is_buffer(napi_env env,
                                       napi_value value,
                                       bool* result);
NAPI_EXTERN napi_status napi_get_buffer_info(napi_env env,
                                             napi_value value,
                                             void** data,
                                             size_t* length);

// Methods to work with array buffers and typed arrays
NAPI_EXTERN napi_status napi_is_arraybuffer(napi_env env,
                                            napi_value value,
                                            bool* result);
NAPI_EXTERN napi_status napi_create_arraybuffer(napi_env env,
                                                size_t byte_length,
                                                void** data,
                                                napi_value* result);
NAPI_EXTERN napi_status
napi_create_external_arraybuffer(napi_env env,
                                 void* external_data,
                                 size_t byte_length,
                                 napi_finalize finalize_cb,
                                 void* finalize_hint,
                                 napi_value* result);
NAPI_EXTERN napi_status napi_get_arraybuffer_info(napi_env env,
                                                  napi_value arraybuffer,
                                                  void** data,
                                                  size_t* byte_length);
NAPI_EXTERN napi_status napi_is_typedarray(napi_env env,
                                           napi_value value,
                                           bool* result);
NAPI_EXTERN napi_status napi_create_typedarray(napi_env env,
                                               napi_typedarray_type type,
                                               size_t length,
                                               napi_value arraybuffer,
                                               size_t byte_offset,
                                               napi_value* result);
NAPI_EXTERN napi_status napi_get_typedarray_info(napi_env env,
                                                 napi_value typedarray,
                                                 napi_typedarray_type* type,
                                                 size_t* length,
                                                 void** data,
                                                 napi_value* arraybuffer,
                                                 size_t* byte_offset);

NAPI_EXTERN napi_status napi_create_dataview(napi_env env,
                                             size_t length,
                                             napi_value arraybuffer,
                                             size_t byte_offset,
                                             napi_value* result);
NAPI_EXTERN napi_status napi_is_dataview(napi_env env,
                                         napi_value value,
                                         bool* result);
NAPI_EXTERN napi_status napi_get_dataview_info(napi_env env,
                                               napi_value dataview,
                                               size_t* bytelength,
                                               void** data,
                                               napi_value* arraybuffer,
                                               size_t* byte_offset);

// Methods to manage simple async operations
NAPI_EXTERN
napi_status napi_create_async_work(napi_env env,
                                   napi_value async_resource,
                                   napi_value async_resource_name,
                                   napi_async_execute_callback execute,
                                   napi_async_complete_callback complete,
                                   void* data,
                                   napi_async_work* result);
NAPI_EXTERN napi_status napi_delete_async_work(napi_env env,
                                               napi_async_work work);
NAPI_EXTERN napi_status napi_queue_async_work(napi_env env,
                                              napi_async_work work);
NAPI_EXTERN napi_status napi_cancel_async_work(napi_env env,
                                               napi_async_work work);

// Methods for custom handling of async operations
NAPI_EXTERN napi_status napi_async_init(napi_env env,
                                        napi_value async_resource,
                                        napi_value async_resource_name,
                                        napi_async_context* result);

NAPI_EXTERN napi_status napi_async_destroy(napi_env env,
                                           napi_async_context async_context);

NAPI_EXTERN napi_status napi_make_callback(napi_env env,
                                           napi_async_context async_context,
                                           napi_value recv,
                                           napi_value func,
                                           size_t argc,
                                           const napi_value* argv,
                                           napi_value* result);

// version management
NAPI_EXTERN napi_status napi_get_version(napi_env env, uint32_t* result);

NAPI_EXTERN
napi_status napi_get_node_version(napi_env env,
                                  const napi_node_version** version);

// Promises
NAPI_EXTERN napi_status napi_create_promise(napi_env env,
                                            napi_deferred* deferred,
                                            napi_value* promise);
NAPI_EXTERN napi_status napi_resolve_deferred(napi_env env,
                                              napi_deferred deferred,
                                              napi_value resolution);
NAPI_EXTERN napi_status napi_reject_deferred(napi_env env,
                                             napi_deferred deferred,
                                             napi_value rejection);
NAPI_EXTERN napi_status napi_is_promise(napi_env env,
                                        napi_value promise,
                                        bool* is_promise);

// Memory management
NAPI_EXTERN napi_status napi_adjust_external_memory(napi_env env,
                                                    int64_t change_in_bytes,
                                                    int64_t* adjusted_value);

// Running a script
NAPI_EXTERN napi_status napi_run_script(napi_env env,
                                        napi_value script,
                                        napi_value* result);

#if NAPI_VERSION >= 2

// Return the current libuv event loop for a given environment
NAPI_EXTERN napi_status napi_get_uv_event_loop(napi_env env,
                                               struct uv_loop_s** loop);

#endif  // NAPI_VERSION >= 2

#if NAPI_VERSION >= 3

NAPI_EXTERN napi_status napi_open_callback_scope(napi_env env,
                                                 napi_value resource_object,
                                                 napi_async_context context,
                                                 napi_callback_scope* result);

NAPI_EXTERN napi_status napi_close_callback_scope(napi_env env,
                                                  napi_callback_scope scope);

NAPI_EXTERN napi_status napi_fatal_exception(napi_env env, napi_value err);

NAPI_EXTERN napi_status napi_add_env_cleanup_hook(napi_env env,
                                                  void (*fun)(void* arg),
                                                  void* arg);

NAPI_EXTERN napi_status napi_remove_env_cleanup_hook(napi_env env,
                                                     void (*fun)(void* arg),
                                                     void* arg);

#endif  // NAPI_VERSION >= 3

#if NAPI_VERSION >= 4

// Calling into JS from other threads
NAPI_EXTERN napi_status
napi_create_threadsafe_function(napi_env env,
                                napi_value func,
                                napi_value async_resource,
                                napi_value async_resource_name,
                                size_t max_queue_size,
                                size_t initial_thread_count,
                                void* thread_finalize_data,
                                napi_finalize thread_finalize_cb,
                                void* context,
                                napi_threadsafe_function_call_js call_js_cb,
                                napi_threadsafe_function* result);

NAPI_EXTERN napi_status
napi_get_threadsafe_function_context(napi_threadsafe_function func,
                                     void** result);

NAPI_EXTERN napi_status
napi_call_threadsafe_function(napi_threadsafe_function func,
                              void* data,
                              napi_threadsafe_function_call_mode is_blocking);

NAPI_EXTERN napi_status
napi_acquire_threadsafe_function(napi_threadsafe_function func);

NAPI_EXTERN napi_status
napi_release_threadsafe_function(napi_threadsafe_function func,
                                 napi_threadsafe_function_release_mode mode);

NAPI_EXTERN napi_status
napi_unref_threadsafe_function(napi_env env, napi_threadsafe_function func);

NAPI_EXTERN napi_status
napi_ref_threadsafe_function(napi_env env, napi_threadsafe_function func);

#endif  // NAPI_VERSION >= 4

#if NAPI_VERSION >= 5

// Dates
NAPI_EXTERN napi_status napi_create_date(napi_env env,
                                         double time,
                                         napi_value* result);

NAPI_EXTERN napi_status napi_is_date(napi_env env,
                                     napi_value value,
                                     bool* is_date);

NAPI_EXTERN napi_status napi_get_date_value(napi_env env,
                                            napi_value value,
                                            double* result);

// Add finalizer for pointer
NAPI_EXTERN napi_status napi_add_finalizer(napi_env env,
                                           napi_value js_object,
                                           void* native_object,
                                           napi_finalize finalize_cb,
                                           void* finalize_hint,
                                           napi_ref* result);

#endif  // NAPI_VERSION >= 5

#if NAPI_VERSION >= 6

// BigInt
NAPI_EXTERN napi_status napi_create_bigint_int64(napi_env env,
                                                 int64_t value,
                                                 napi_value* result);
NAPI_EXTERN napi_status napi_create_bigint_uint64(napi_env env,
                                                  uint64_t value,
                                                  napi_value* result);
NAPI_EXTERN napi_status napi_create_bigint_words(napi_env env,
                                                 int sign_bit,
                                                 size_t word_count,
                                                 const uint64_t* words,
                                                 napi_value* result);
NAPI_EXTERN napi_status napi_get_value_bigint_int64(napi_env env,
                                                    napi_value value,
                                                    int64_t* result,
                                                    bool* lossless);
NAPI_EXTERN napi_status napi_get_value_bigint_uint64(napi_env env,
                                                     napi_value value,
                                                     uint64_t* result,
                                                     bool* lossless);
NAPI_EXTERN napi_status napi_get_value_bigint_words(napi_env env,
                                                    napi_value value,
                                                    int* sign_bit,
                                                    size_t* word_count,
                                                    uint64_t* words);

// Instance data
NAPI_EXTERN napi_status napi_set_instance_data(napi_env env,
                                               void* data,
                                               napi_finalize finalize_cb,
                                               void* finalize_hint);

NAPI_EXTERN napi_status napi_get_instance_data(napi_env env,
                                               void** data);

// Object
NAPI_EXTERN napi_status
napi_get_all_property_names(napi_env env,
                            napi_value object,
                            napi_key_collection_mode key_mode,
                            napi_key_filter key_filter,
                            napi_key_conversion key_conversion,
                            napi_value* result);

#endif  // NAPI_VERSION >= 6

#ifdef NAPI_EXPERIMENTAL
// ArrayBuffer detaching
NAPI_EXTERN napi_status
napi_detach_arraybuffer(napi_env env,
                        napi_value arraybuffer);

NAPI_EXTERN napi_status
napi_is_detached_arraybuffer(napi_env env,
                             napi_value value,
                             bool* result);
#endif  // NAPI_EXPERIMENTAL

EXTERN_C_END

#endif  // SRC_NODE_API_H_
#endif // SRC_SQLMATH_NODEJS_C2


/*
file none
*/
/*jslint-enable*/


// *INDENT-ON*
#endif                          // SRC_SQLMATH_BASE_H3
