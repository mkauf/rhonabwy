/**
 * 
 * @file rhonabwy.h
 * @brief Rhonabwy JSON Web Key (JWK) library
 * 
 * rhonabwy.h: structures and functions declarations
 * 
 * Copyright 2020-2022 Nicolas Mora <mail@babelouest.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation;
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU GENERAL PUBLIC LICENSE for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef __RHONABWY_H_
#define __RHONABWY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rhonabwy-cfg.h"

#include <stdint.h>
#include <jansson.h>
#include <gnutls/gnutls.h>
#include <nettle/version.h>

/**
 * @defgroup const Constants and properties
 * Constant values used as input or output
 * @{
 */

#define RHN_OK                 0
#define RHN_ERROR              1
#define RHN_ERROR_MEMORY       2
#define RHN_ERROR_PARAM        3
#define RHN_ERROR_UNSUPPORTED  4
#define RHN_ERROR_INVALID      5

#define R_X509_TYPE_UNSPECIFIED 0
#define R_X509_TYPE_PUBKEY      1
#define R_X509_TYPE_PRIVKEY     2
#define R_X509_TYPE_CERTIFICATE 3

#define R_FORMAT_PEM 0
#define R_FORMAT_DER 1

#define R_KEY_TYPE_NONE      0x00000000
#define R_KEY_TYPE_PUBLIC    0x00000001
#define R_KEY_TYPE_PRIVATE   0x00000010
#define R_KEY_TYPE_SYMMETRIC 0x00000100
#define R_KEY_TYPE_RSA       0x00001000
#define R_KEY_TYPE_EC        0x00010000
#define R_KEY_TYPE_HMAC      0x00100000
#define R_KEY_TYPE_EDDSA     0x01000000
#define R_KEY_TYPE_ECDH      0x10000000

#define R_FLAG_IGNORE_SERVER_CERTIFICATE 0x00000001
#define R_FLAG_FOLLOW_REDIRECT           0x00000010
#define R_FLAG_IGNORE_REMOTE             0x00000100

#define R_JWT_TYPE_NONE                     0
#define R_JWT_TYPE_SIGN                     1
#define R_JWT_TYPE_ENCRYPT                  2
#define R_JWT_TYPE_NESTED_SIGN_THEN_ENCRYPT 3
#define R_JWT_TYPE_NESTED_ENCRYPT_THEN_SIGN 4

#define R_JWT_CLAIM_NOW     -1
#define R_JWT_CLAIM_PRESENT -2

#define R_JWK_THUMB_SHA256 0
#define R_JWK_THUMB_SHA384 1
#define R_JWK_THUMB_SHA512 2

#define R_JSON_MODE_COMPACT   0
#define R_JSON_MODE_GENERAL   1
#define R_JSON_MODE_FLATTENED 2

#define R_PARSE_NONE           0
#define R_PARSE_HEADER_JWK     1
#define R_PARSE_HEADER_JKU     2
#define R_PARSE_HEADER_X5C     4
#define R_PARSE_HEADER_X5U     8
#define R_PARSE_HEADER_ALL    (R_PARSE_HEADER_JWK|R_PARSE_HEADER_JKU|R_PARSE_HEADER_X5C|R_PARSE_HEADER_X5U)
#define R_PARSE_UNSIGNED       16
#define R_PARSE_ALL           (R_PARSE_HEADER_ALL|R_PARSE_UNSIGNED)

/**
 * @}
 */

/**
 * @defgroup type JWK, JWKS, JWS, JWE type
 * Definition of the types jwk_t, jwks_t, jws_t and jwe_t
 * @{
 */

typedef json_t jwk_t;
typedef json_t jwks_t;
typedef json_int_t rhn_int_t;

#define RHONABWY_INTEGER_FORMAT JSON_INTEGER_FORMAT

typedef enum {
  R_JWA_ALG_UNKNOWN        = 0,
  R_JWA_ALG_NONE           = 1,
  R_JWA_ALG_HS256          = 2,
  R_JWA_ALG_HS384          = 3,
  R_JWA_ALG_HS512          = 4,
  R_JWA_ALG_RS256          = 5,
  R_JWA_ALG_RS384          = 6,
  R_JWA_ALG_RS512          = 7,
  R_JWA_ALG_ES256          = 8,
  R_JWA_ALG_ES384          = 9,
  R_JWA_ALG_ES512          = 10,
  R_JWA_ALG_EDDSA          = 11,
  R_JWA_ALG_PS256          = 12,
  R_JWA_ALG_PS384          = 13,
  R_JWA_ALG_PS512          = 14,
  R_JWA_ALG_RSA1_5         = 15,
  R_JWA_ALG_RSA_OAEP       = 16,
  R_JWA_ALG_RSA_OAEP_256   = 17,
  R_JWA_ALG_A128KW         = 18,
  R_JWA_ALG_A192KW         = 19,
  R_JWA_ALG_A256KW         = 20,
  R_JWA_ALG_DIR            = 21,
  R_JWA_ALG_ECDH_ES        = 22,
  R_JWA_ALG_ECDH_ES_A128KW = 23,
  R_JWA_ALG_ECDH_ES_A192KW = 24,
  R_JWA_ALG_ECDH_ES_A256KW = 25,
  R_JWA_ALG_A128GCMKW      = 26,
  R_JWA_ALG_A192GCMKW      = 27,
  R_JWA_ALG_A256GCMKW      = 28,
  R_JWA_ALG_PBES2_H256     = 29,
  R_JWA_ALG_PBES2_H384     = 30,
  R_JWA_ALG_PBES2_H512     = 31,
  R_JWA_ALG_ES256K         = 32
} jwa_alg;

typedef enum {
  R_JWT_CLAIM_NOP = 0,
  R_JWT_CLAIM_ISS = 1,
  R_JWT_CLAIM_SUB = 2,
  R_JWT_CLAIM_AUD = 3,
  R_JWT_CLAIM_EXP = 4,
  R_JWT_CLAIM_NBF = 5,
  R_JWT_CLAIM_IAT = 6,
  R_JWT_CLAIM_JTI = 7,
  R_JWT_CLAIM_STR = 8,
  R_JWT_CLAIM_INT = 9,
  R_JWT_CLAIM_JSN = 10,
  R_JWT_CLAIM_TYP = 11,
  R_JWT_CLAIM_CTY = 12,
} rhn_claim_opt;

typedef enum {
  R_JWA_ENC_UNKNOWN = 0,
  R_JWA_ENC_A128CBC = 1,
  R_JWA_ENC_A192CBC = 2,
  R_JWA_ENC_A256CBC = 3,
  R_JWA_ENC_A128GCM = 4,
  R_JWA_ENC_A192GCM = 5,
  R_JWA_ENC_A256GCM = 6
} jwa_enc;

typedef enum {
  RHN_OPT_NONE                    = 0, ///< End option list, mandatory at the end of the option list
  RHN_OPT_HEADER_INT_VALUE        = 1, ///< Header Integer value, following parameters must be const char * name, int i_value
  RHN_OPT_HEADER_RHN_INT_VALUE    = 2, ///< Header rhn_int_t value, following parameters must be const char * name, rhn_int_t i_value
  RHN_OPT_HEADER_STR_VALUE        = 3, ///< Header String value, following parameters must be const char * name, const char * str_value
  RHN_OPT_HEADER_JSON_T_VALUE     = 4, ///< Header JSON value, following parameters must be const char * name, json_t * j_value
  RHN_OPT_HEADER_FULL_JSON_T      = 5, ///< JSON value to set the entire header, following parameter must be json_t * j_value
  RHN_OPT_HEADER_FULL_JSON_STR    = 6, ///< Stringified JSON value to set the entire header, following parameter must be const char * str_value
  RHN_OPT_UN_HEADER_FULL_JSON_T   = 7, ///< JSON value to set the entire unprotected header, following parameter must be json_t * j_value
  RHN_OPT_UN_HEADER_FULL_JSON_STR = 8, ///< Stringified JSON value to set the entire unprotected header, following parameter must be const char * str_value
  RHN_OPT_PAYLOAD                 = 9, ///< JSON value to set the entire payload, following parameters must be const unsigned char * value, size_t value_length
  RHN_OPT_CLAIM_INT_VALUE         = 10, ///< Claims Integer value, following parameters must be const char * name, int i_value
  RHN_OPT_CLAIM_RHN_INT_VALUE     = 11, ///< Claims Integer value, following parameters must be const char * name, int i_value
  RHN_OPT_CLAIM_STR_VALUE         = 12, ///< Claims String value, following parameters must be const char * name, const char * str_value
  RHN_OPT_CLAIM_JSON_T_VALUE      = 13, ///< Claims JSON value, following parameters must be const char * name, json_t * j_value
  RHN_OPT_CLAIM_FULL_JSON_T       = 14, ///< JSON value to set the entire claims, following parameter must be json_t * j_value
  RHN_OPT_CLAIM_FULL_JSON_STR     = 15, ///< Stringified JSON value to set the entire claims, following parameter must be const char * str_value
  RHN_OPT_ENC_ALG                 = 16, ///< Key management algorithm, following parameter must be a jwa_alg value
  RHN_OPT_ENC                     = 17, ///< Encryption algorithm, following parameter must be a jwa_enc value
  RHN_OPT_SIG_ALG                 = 18, ///< Signature algorithm, following parameter must be a jwa_alg value
  RHN_OPT_CIPHER_KEY              = 19, ///< Cipher key to encrypt data, following parameters must be const unsigned char * value, size_t value_length
  RHN_OPT_IV                      = 20, ///< Initial Value (IV) for data encryption, following parameters must be const unsigned char * value, size_t value_length
  RHN_OPT_AAD                     = 21, ///< Additional Authenticated Data (AAD) for data encryption, following parameters must be const unsigned char * value, size_t value_length
  RHN_OPT_SIGN_KEY_JWK            = 22, ///< Private key in JWK format to sign the token, following parameter must be a jwk_t * value
  RHN_OPT_SIGN_KEY_JWKS           = 23, ///< Private key set in JWKS format to sign the token, following parameter must be a jwks_t * value
  RHN_OPT_SIGN_KEY_GNUTLS         = 24, ///< Private key in GnuTLS format to sign the token, following parameter must be a gnutls_privkey_t value
  RHN_OPT_SIGN_KEY_JSON_T         = 25, ///< Private key in JSON format to sign the token, following parameter must be a json_t * value
  RHN_OPT_SIGN_KEY_JSON_STR       = 26, ///< Private key in stringified JSON format to sign the token, following parameter must be a const char * value
  RHN_OPT_SIGN_KEY_PEM_DER        = 27, ///< Private key in PEM or DER format to sign the token, following parameter must be R_FORMAT_PEM or R_FORMAT_DER, const unsigned char * value, size_t value_length
  RHN_OPT_VERIFY_KEY_JWK          = 28, ///< Public key in JWK format to verify the token signature, following parameter must be a jwk_t * value
  RHN_OPT_VERIFY_KEY_JWKS         = 29, ///< Public key set in JWKS format to verify the token signature, following parameter must be a jwks_t * value
  RHN_OPT_VERIFY_KEY_GNUTLS       = 30, ///< Public key in GnuTLS format to verify the token signature, following parameter must be a gnutls_pubkey_t value
  RHN_OPT_VERIFY_KEY_JSON_T       = 31, ///< Public key in JSON format to verify the token signature, following parameter must be a json_t * value
  RHN_OPT_VERIFY_KEY_JSON_STR     = 32, ///< Public key in stringified JSON format to verify the token signature, following parameter must be a const char * value
  RHN_OPT_VERIFY_KEY_PEM_DER      = 33, ///< Public key in PEM or DER format to verify the token signature, following parameter must be R_FORMAT_PEM or R_FORMAT_DER, const unsigned char * value, size_t value_length
  RHN_OPT_ENCRYPT_KEY_JWK         = 34, ///< Public key in JWK format to encrypt the token, following parameter must be a jwk_t * value
  RHN_OPT_ENCRYPT_KEY_JWKS        = 35, ///< Public key set in JWKS format to encrypt the token, following parameter must be a jwks_t * value
  RHN_OPT_ENCRYPT_KEY_GNUTLS      = 36, ///< Public key in GnuTLS format to encrypt the token, following parameter must be a gnutls_pubkey_t value
  RHN_OPT_ENCRYPT_KEY_JSON_T      = 37, ///< Public key in JSON format to encrypt the token, following parameter must be a json_t * value
  RHN_OPT_ENCRYPT_KEY_JSON_STR    = 38, ///< Public key in stringified JSON format to encrypt the token, following parameter must be a const char * value
  RHN_OPT_ENCRYPT_KEY_PEM_DER     = 39, ///< Public key in PEM or DER format to encrypt the token, following parameter must be R_FORMAT_PEM or R_FORMAT_DER, const unsigned char * value, size_t value_length
  RHN_OPT_DECRYPT_KEY_JWK         = 40, ///< Private key in JWK format to decrypt the token, following parameter must be a jwk_t * value
  RHN_OPT_DECRYPT_KEY_JWKS        = 41, ///< Private key set in JWKS format to decrypt the token, following parameter must be a jwks_t * value
  RHN_OPT_DECRYPT_KEY_GNUTLS      = 42, ///< Private key in GnuTLS format to decrypt the token, following parameter must be a gnutls_privkey_t value
  RHN_OPT_DECRYPT_KEY_JSON_T      = 43, ///< Private key in JSON format to decrypt the token, following parameter must be a json_t * value
  RHN_OPT_DECRYPT_KEY_JSON_STR    = 44, ///< Private key in stringified JSON format to decrypt the token, following parameter must be a const char * value
  RHN_OPT_DECRYPT_KEY_PEM_DER     = 45  ///< Private key in PEM or DER format to decrypt the token, following parameter must be R_FORMAT_PEM or R_FORMAT_DER, const unsigned char * value, size_t value_length
} rhn_opt;

typedef enum {
  R_IMPORT_NONE      = 0,  ///< End option list, mandatory at the end of the option list when using r_jwks_quick_import
  R_IMPORT_JSON_STR  = 1,  ///< Import from a stringified JSON, following parameter must be a const char * value
  R_IMPORT_JSON_T    = 2,  ///< Import from a json_t *, following parameter must be a const json_t * value
  R_IMPORT_PEM       = 3,  ///< Import from a X509 key in PEM format, following parameters must be type (R_X509_TYPE_PUBKEY, R_X509_TYPE_PRIVKEY or R_X509_TYPE_CERTIFICATE), const unsigned char *, size_t
  R_IMPORT_DER       = 4,  ///< Import from a X509 key in DER format, following parameters must be type (R_X509_TYPE_PUBKEY, R_X509_TYPE_PRIVKEY or R_X509_TYPE_CERTIFICATE), const unsigned char *, size_t
  R_IMPORT_G_PRIVKEY = 5,  ///< Import from a gnutls_privkey_t, following parameters must be gnutls_privkey_t
  R_IMPORT_G_PUBKEY  = 6,  ///< Import from a gnutls_pubkey_t, following parameters must be gnutls_pubkey_t
  R_IMPORT_G_CERT    = 7,  ///< Import from a gnutls_x509_crt_t, following parameters must be gnutls_x509_crt_t
  R_IMPORT_X5U       = 8,  ///< Import from an URL pointing to a x5u, following parameters must be x5u_flags (R_FLAG_IGNORE_SERVER_CERTIFICATE, R_FLAG_FOLLOW_REDIRECT, R_FLAG_IGNORE_REMOTE), const char * value
  R_IMPORT_SYMKEY    = 9,  ///< Import from a symmetric key, following parameters must be const unsigned char *, size_t
  R_IMPORT_PASSWORD  = 10, ///< Import from a password, following parameter must be a const char * value
  R_IMPORT_JKU       = 11  ///< Import from an URL pointing to a jku, available for r_jwks_quick_import only, following parameters must be x5u_flags (R_FLAG_IGNORE_SERVER_CERTIFICATE, R_FLAG_FOLLOW_REDIRECT, R_FLAG_IGNORE_REMOTE), const char * value
} rhn_import;

typedef struct {
  unsigned char * header_b64url;
  unsigned char * payload_b64url;
  unsigned char * signature_b64url;
  json_t        * j_header;
  jwa_alg         alg;
  jwks_t        * jwks_privkey;
  jwks_t        * jwks_pubkey;
  unsigned char * payload;
  size_t          payload_len;
  json_t        * j_json_serialization;
  int             token_mode;
} jws_t;

typedef struct {
  unsigned char * header_b64url;
  unsigned char * encrypted_key_b64url;
  unsigned char * aad_b64url;
  unsigned char * iv_b64url;
  unsigned char * ciphertext_b64url;
  unsigned char * auth_tag_b64url;
  json_t        * j_header;
  json_t        * j_unprotected_header;
  jwa_alg         alg;
  jwa_enc         enc;
  jwks_t        * jwks_privkey;
  jwks_t        * jwks_pubkey;
  unsigned char * aad;
  size_t          aad_len;
  unsigned char * key;
  size_t          key_len;
  unsigned char * iv;
  size_t          iv_len;
  unsigned char * payload;
  size_t          payload_len;
  json_t        * j_json_serialization;
  int             token_mode;
} jwe_t;

typedef struct {
  int             type;
  uint32_t        parse_flags;
  json_t        * j_header;
  json_t        * j_claims;
  jws_t         * jws;
  jwe_t         * jwe;
  jwa_alg         sign_alg;
  jwa_alg         enc_alg;
  jwa_enc         enc;
  unsigned char * key;
  size_t          key_len;
  unsigned char * iv;
  size_t          iv_len;
  jwks_t        * jwks_privkey_sign;
  jwks_t        * jwks_pubkey_sign;
  jwks_t        * jwks_privkey_enc;
  jwks_t        * jwks_pubkey_enc;
} jwt_t;

/**
 * @}
 */

/**
 * @defgroup core Core functions
 * Core functions used to initialize or free jwk_t
 * and check if a jwk is valid and its type
 * @{
 */

/**
 * Initialize rhonabwy global parameters
 * This function isn't thread-safe so it must be called once before any other call to rhonabwy functions
 * The function r_global_close must be called when rhonabwy library is no longer required
 * @return RHN_OK on success, an error value on error
 */
int r_global_init(void);

/**
 * Close rhonabwy global parameters
 */
void r_global_close(void);

/**
 * Get the library information as a json_t * object
 * - library version
 * - supported JWS algorithms
 * - supported JWE algorithms
 * @return the library information
 */
json_t * r_library_info_json_t(void);

/**
 * Get the library information as a JSON object in string format
 * - library version
 * - supported JWS algorithms
 * - supported JWE algorithms
 * @return the library information, must be r_free'd after use
 */
char * r_library_info_json_str(void);

/**
 * Free a heap allocated variable
 * previously returned by a rhonabwy function
 * @param data: the data to free
 */
void r_free(void * data);

/**
 * Initialize a jwk_t
 * @param jwk: a reference to a jwk_t * to initialize
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_init(jwk_t ** jwk);

/**
 * Free a jwk_t
 * @param jwk: the jwk_t * to free
 */
void r_jwk_free(jwk_t * jwk);

/**
 * Initialize a jwks_t
 * @param jwks: a reference to a jwks_t * to initialize
 * @return RHN_OK on success, an error value on error
 */
int r_jwks_init(jwks_t ** jwks);

/**
 * Free a jwks_t
 * @param jwks: the jwks_t * to free
 */
void r_jwks_free(jwks_t * jwks);

/**
 * Initialize a jws_t
 * @param jws: a reference to a jws_t * to initialize
 * @return RHN_OK on success, an error value on error
 */
int r_jws_init(jws_t ** jws);

/**
 * Free a jws_t
 * @param jws: the jws_t * to free
 */
void r_jws_free(jws_t * jws);

/**
 * Initialize a jwe_t
 * @param jwe: a reference to a jwe_t * to initialize
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_init(jwe_t ** jwe);

/**
 * Free a jwe_t
 * @param jwe: the jwe_t * to free
 */
void r_jwe_free(jwe_t * jwe);

/**
 * Initialize a jwt_t
 * @param jwt: a reference to a jwt_t * to initialize
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_init(jwt_t ** jwt);

/**
 * Free a jwt_t
 * @param jwt: the jwt_t * to free
 */
void r_jwt_free(jwt_t * jwt);

/**
 * Get the jwa_alg corresponding to the string algorithm specified
 * @param alg: the algorithm to convert
 * @return the converted jwa_alg, R_JWA_ALG_NONE if alg is unknown
 */
jwa_alg r_str_to_jwa_alg(const char * alg);

/**
 * Get the algorithm string
 * corresponding to the jwa_alg
 * @param alg: the algorithm to convert
 * @return its string name
 */
const char * r_jwa_alg_to_str(jwa_alg alg);

/**
 * Get the jwa_enc corresponding to the string algorithm specified
 * @param enc: the algorithm to convert
 * @return the converted jwa_enc, R_JWA_ENC_NONE if enc is unknown
 */
jwa_enc r_str_to_jwa_enc(const char * enc);

/**
 * Get the encryption string
 * corresponding to the jwa_enc
 * @param enc: the encryption to convert
 * @return its string name
 */
const char * r_jwa_enc_to_str(jwa_enc enc);

/**
 * @}
 */

/**
 * @defgroup jwk_validate Validate a JWK and generate a key pair
 * @{
 */

/**
 * Get the type and algorithm of a jwk_t
 * @param jwk: the jwk_t * to test
 * @param bits: set the key size in bits (may be NULL)
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return an integer containing 
 * - R_KEY_TYPE_NONE if the jwk is invalid
 * * the type:
 * - R_KEY_TYPE_PUBLIC: for a public key
 * - R_KEY_TYPE_PRIVATE: for a private key
 * - R_KEY_TYPE_SYMMETRIC: for a symmetrick key
 * * the algorithm used
 * - R_KEY_TYPE_RSA: for a RSA key
 * - R_KEY_TYPE_EC: for a EC key
 * - R_KEY_TYPE_HMAC: for a HMAC key
 * You must test the result value with bitwise operator
 * Ex: if (r_jwk_key_type(jwk) & R_KEY_TYPE_PUBLIC) {
 *     if (r_jwk_key_type(jwk) & R_KEY_TYPE_RSA) {
 * You can combine type and algorithm values in the bitwise operator
 * Ex: if (r_jwk_key_type(jwk) & (R_KEY_TYPE_RSA|R_KEY_TYPE_PRIVATE)) {
 */
int r_jwk_key_type(jwk_t * jwk, unsigned int * bits, int x5u_flags);

/**
 * Check if the jwk is valid
 * @param jwk: the jwk_t * to test
 * @return RHN_OK on success, an error value on error
 * Logs error message with yder on error
 */
int r_jwk_is_valid(jwk_t * jwk);

/**
 * Check if the x5u property is valid
 * @param jwk: the jwk_t * to test
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 * Logs error message with yder on error
 */
int r_jwk_is_valid_x5u(jwk_t * jwk, int x5u_flags);

/**
 * Generates a pair of private and public key using given parameters
 * @param jwk_privkey: the private key to set, must be initialized
 * @param jwk_pubkey: the public key to set, must be initialized
 * @param type: the type of key, values available are
 * R_KEY_TYPE_RSA or R_KEY_TYPE_EC
 * @param bits: the key size to generate, if the key type is R_KEY_TYPE_EC,
 * the key size is the curve length: 256, 384 or 512
 * @param kid: the key ID to set to the JWKs, if NULL or empty, will be set automatically
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_generate_key_pair(jwk_t * jwk_privkey, jwk_t * jwk_pubkey, int type, unsigned int bits, const char * kid);

/**
 * @}
 */

/**
 * @defgroup jwk_properties JWK Properties
 * read/write/delete jwk properties
 * @{
 */

/**
 * Get a property value from a jwk_t
 * @param jwk: the jwk_t * to get
 * @param key: the key of the property to retrieve
 * @return the property value on success, NULL on error
 */
const char * r_jwk_get_property_str(jwk_t * jwk, const char * key);

/**
 * Get a property value of an array from a jwk_t
 * @param jwk: the jwk_t * to get
 * @param key: the key of the property to retrieve
 * @param index: the index of the value to retrieve in the array
 * @return the property value on success, NULL on error
 */
const char * r_jwk_get_property_array(jwk_t * jwk, const char * key, size_t index);

/**
 * Get the array size of a property from a jwt_t
 * @param jwk: the jwk_t * to get
 * @param key: the key of the property to retrieve
 * @return the size of the array, or -1 if the array does not exist
 */
int r_jwk_get_property_array_size(jwk_t * jwk, const char * key);

/**
 * Set a property value into a jwk_t
 * @param jwk: the jwk_t * to update
 * @param key: the key of the property to set
 * @param value: the value of the property to set
 * @return RHN_OK on success, an error value on error
 * Logs error message with yder on error
 */
int r_jwk_set_property_str(jwk_t * jwk, const char * key, const char * value);

/**
 * Set a property value on an array into a jwk_t
 * @param jwk: the jwk_t * to update
 * @param key: the key of the property to set
 * @param index: the index of the value to set in the array
 * @param value: the value of the property to set
 * @return RHN_OK on success, an error value on error
 * Logs error message with yder on error
 */
int r_jwk_set_property_array(jwk_t * jwk, const char * key, size_t index, const char * value);

/**
 * Append a property value on an array into a jwk_t
 * @param jwk: the jwk_t * to update
 * @param key: the key of the property to set
 * @param value: the value of the property to set
 * @return RHN_OK on success, an error value on error
 * Logs error message with yder on error
 */
int r_jwk_append_property_array(jwk_t * jwk, const char * key, const char * value);

/**
 * Delete a property from a jwk_t
 * @param jwk: the jwk_t * to update
 * @param key: the key of the property to delete
 * @return RHN_OK on success, an error value on error
 * Logs error message with yder on error
 */
int r_jwk_delete_property_str(jwk_t * jwk, const char * key);

/**
 * Delete an array property from a jwk_t
 * @param jwk: the jwk_t * to update
 * @param key: the key of the property to delete
 * @param index: the index of the value to set in the array
 * @return RHN_OK on success, an error value on error
 * Logs error message with yder on error
 */
int r_jwk_delete_property_array_at(jwk_t * jwk, const char * key, size_t index);

/**
 * Appends a X509 certificate in the x5c array
 * @param jwk: the jwk_t * to update
 * @param format: the format of the input, values available are R_FORMAT_PEM or R_FORMAT_DER
 * @param input: the certificate input, must contain the certificate in PEM or DER format
 * @param input_len: the length of the data contained in input
 * @return RHN_OK on success, an error value on error
 * Logs error message with yder on error
 */
int r_jwk_append_x5c(jwk_t * jwk, int format, const unsigned char * input, size_t input_len);

/**
 * @}
 */

/**
 * @defgroup import JWK Import functions
 * Import a jwk from JSON data, gnutls inner types or PEM/DER
 * @{
 */

/**
 * Import a JSON in string format into a jwk_t
 * @param jwk: the jwk_t * to import to
 * @param input: a JWK in JSON stringified format
 * If jwk is set, values will be overwritten
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_import_from_json_str(jwk_t * jwk, const char * input);

/**
 * Import a JSON in json_t format into a jwk_t
 * @param jwk: the jwk_t * to import to
 * @param j_input: a JWK in json_t * format
 * If jwk is set, values will be overwritten
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_import_from_json_t(jwk_t * jwk, json_t * j_input);

/**
 * Import a public or private key or a X509 certificate in PEM or DER format into a jwk_t
 * @param jwk: the jwk_t * to import to
 * @param type: the type of the input, values available are R_X509_TYPE_PUBKEY, R_X509_TYPE_PRIVKEY or R_X509_TYPE_CERTIFICATE
 * @param format: the format of the input, values available are R_FORMAT_PEM or R_FORMAT_DER
 * @param input: the input value, must contain the key or the certificate in PEM or DER format
 * @param input_len: the length of the data contained in input
 * If jwk is set, values will be overwritten
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_import_from_pem_der(jwk_t * jwk, int type, int format, const unsigned char * input, size_t input_len);

/**
 * Import a GnuTLS private key in gnutls_privkey_t format into a jwk_t
 * @param jwk: the jwk_t * to import to
 * @param key: the private key to be imported to jwk
 * If jwk is set, values will be overwritten
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_import_from_gnutls_privkey(jwk_t * jwk, gnutls_privkey_t key);

/**
 * Import a GnuTLS public key in gnutls_pubkey_t format into a jwk_t
 * @param jwk: the jwk_t * to import to
 * @param pub: the public key to be imported to jwk
 * If jwk is set, values will be overwritten
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_import_from_gnutls_pubkey(jwk_t * jwk, gnutls_pubkey_t pub);

/**
 * Import a GnuTLS X509 certificate in gnutls_x509_crt_t format into a jwk_t
 * @param jwk: the jwk_t * to import to
 * @param crt: the X509 certificate whose public key will be imported to jwk
 * If jwk is set, values will be overwritten
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_import_from_gnutls_x509_crt(jwk_t * jwk, gnutls_x509_crt_t crt);

/**
 * Import a certificate from an URL
 * @param jwk: the jwk_t * to import to
 * @param x5u_flags: Flags to retrieve x5u certificates
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * @param x5u: the url to retreive the certificate
 * If jwk is set, values will be overwritten
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_import_from_x5u(jwk_t * jwk, int x5u_flags, const char * x5u);

/**
 * Import a key from an x5c value
 * @param jwk: the jwk_t * to import to
 * @param x5c: the x5c value
 * If jwk is set, values will be overwritten
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_import_from_x5c(jwk_t * jwk, const char * x5c);

/**
 * Import a symmetric key into a jwk
 * The key will be converted to base64url format
 * @param jwk: the jwk_t * to import to
 * @param key: the key to import
 * @param key_len: the size of the key
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_import_from_symmetric_key(jwk_t * jwk, const unsigned char * key, size_t key_len);

/**
 * Import a password into a jwk
 * The password will be converted to base64url format
 * @param jwk: the jwk_t * to import to
 * @param password: the password to import
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_import_from_password(jwk_t * jwk, const char * password);

/**
 * Extract the public key from the private key jwk_privkey and set it into jwk_pubkey
 * @param jwk_privkey: the jwt containing a private key
 * @param jwk_pubkey: the jwt that will be set with the public key data
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_extract_pubkey(jwk_t * jwk_privkey, jwk_t * jwk_pubkey, int x5u_flags);

/**
 * Import data into a jwk
 * @param type: type of the data to import
 * Following parameters must be set of values
 * with the mandatory parameters for each rhn_import
 * See rhn_import documentation
 * @return a jwk containing key parsed, or NULL on error
 */
jwk_t * r_jwk_quick_import(rhn_import type, ...);

/**
 * Return a copy of the JWK
 * @param jwk: the jwk to copy
 * @return a copy of the jwk
 */
jwk_t * r_jwk_copy(jwk_t * jwk);

/**
 * Compare 2 jwk
 * @param jwk1: the first JWK to compare
 * @param jwk2: the second JWK to compare
 * @return 1 if both jwk1 and jwk2 are equal, 0 otherwise
 */
int r_jwk_equal(jwk_t * jwk1, jwk_t * jwk2);

/**
 * @}
 */

/**
 * @defgroup export JWK Export functions
 * Export a jwk to JSON data, gnutls inner types or PEM/DER
 * @{
 */

/**
 * Export a jwk_t into a stringified JSON format
 * @param jwk: the jwk_t * to export
 * @param pretty: indent or compact JSON output
 * @return a char * on success, NULL on error, must be r_free'd after use
 */
char * r_jwk_export_to_json_str(jwk_t * jwk, int pretty);

/**
 * Export a jwk_t into a json_t format
 * @param jwk: the jwk_t * to export
 * @return a json_t * on success, NULL on error
 */
json_t * r_jwk_export_to_json_t(jwk_t * jwk);

/**
 * Export a jwk_t into a gnutls_privkey_t format
 * @param jwk: the jwk_t * to export
 * @return a gnutls_privkey_t on success, NULL on error
 */
gnutls_privkey_t r_jwk_export_to_gnutls_privkey(jwk_t * jwk);

/**
 * Export a jwk_t into a gnutls_pubkey_t format
 * @param jwk: the jwk_t * to export
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return NULL
 * @return a gnutls_pubkey_t on success, NULL on error
 */
gnutls_pubkey_t r_jwk_export_to_gnutls_pubkey(jwk_t * jwk, int x5u_flags);

/**
 * Export a jwk_t into a gnutls_x509_crt_t format
 * the jwt_t must contain a x5c or a x5u property
 * pointing to a certificate
 * @param jwk: the jwk_t * to export
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return NULL
 * @return a gnutls_x509_crt_t on success, NULL on error
 */
gnutls_x509_crt_t r_jwk_export_to_gnutls_crt(jwk_t * jwk, int x5u_flags);

/**
 * Export a jwk_t into a DER or PEM format
 * @param jwk: the jwk_t * to export
 * @param format: the format of the output, values available are R_FORMAT_PEM or R_FORMAT_DER
 * @param output: an unsigned char * that will contain the output
 * @param output_len: the size of output and will be set to the data size that has been written to output
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 * @return RHN_ERROR_PARAM if output_len isn't large enough to hold the output, then output_len will be set to the required size
 */
int r_jwk_export_to_pem_der(jwk_t * jwk, int format, unsigned char * output, size_t * output_len, int x5u_flags);

/**
 * Export a jwk_t into a symmetric key in binary format
 * @param jwk: the jwk_t * to export
 * @param key: an unsigned char * that will contain the key
 * @param key_len: the size of key and will be set to the data size that has been written to output
 * @return RHN_OK on success, an error value on error
 * @return RHN_ERROR_PARAM if output_len isn't large enough to hold the output, then output_len will be set to the required size
 */
int r_jwk_export_to_symmetric_key(jwk_t * jwk, unsigned char * key, size_t * key_len);

/**
 * Genrates a thumbprint of a jwk_t based on the RFC 7638
 * @param jwk: the jwk_t * to translate into a thumbprint
 * @param hash: The hash funtion to use for the thumprint
 * Values available for this parameter are
 * - R_JWK_THUMB_SHA256
 * - R_JWK_THUMB_SHA384
 * - R_JWK_THUMB_SHA512
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return the jwk hashed and base64url encoded on success, NULL on error, must be r_free'd after use
 */
char * r_jwk_thumbprint(jwk_t * jwk, int hash, int x5u_flags);

/**
 * Verifies the certificate chain in the x5c array or the x5u
 * The x5c chain must be complete up to the root certificate
 * @param jwk: the jwk_t * to verify
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwk_validate_x5c_chain(jwk_t * jwk, int x5u_flags);

/**
 * Search if a jwk matches the given properties
 * @param jwk: the jwk_t to look into
 * @param j_match: The query to match. Must be a JSON object with key/values
 * that will be compared against all keys in jwk
 * All parameters must match
 * Example, to match a RSA key with alg "RS256", the parameter j_match must contain:
 * {kty: "RSA", alg: "RS256"}
 * To match a RSA key with the kid "1", the parameter j_match must contain:
 * {kty: "RSA", kid: "1"}
 * @return RHN_OK on success
 *         RHN_ERROR_PARAM if input parameters are invalid
 *         RHN_ERROR_INVALID if j_match does not match
 */
int r_jwk_match_json_t(jwk_t * jwk, json_t * j_match);

/**
 * Search if a jwk matches the given properties
 * @param jwk: the jwk_t to look into
 * @param str_match: The query to match. Must be a stringified JSON object with key/values
 * that will be compared against all keys in jwk
 * All parameters must match
 * Example, to match a RSA key with alg "RS256", the parameter str_match must contain:
 * {kty: "RSA", alg: "RS256"}
 * To match a RSA key with the kid "1", the parameter str_match must contain:
 * {kty: "RSA", kid: "1"}
 * @return RHN_OK on success
 *         RHN_ERROR_PARAM if input parameters are invalid
 *         RHN_ERROR_INVALID if str_match does not match
 */
int r_jwk_match_json_str(jwk_t * jwk, const char * str_match);

/**
 * @}
 */

/**
 * @defgroup jwks JWKS functions
 * Manage JWK sets
 * @{
 */

/**
 * Check if the jwks is valid
 * @param jwks: the jwks_t * to test
 * @return RHN_OK on success, an error value on error
 * Stops at the first error in the array
 * Logs error message with yder on error
 */
int r_jwks_is_valid(jwks_t * jwks);

/**
 * Import a JWKS in string format into a jwks_t
 * @param jwks: the jwk_t * to import to
 * @param input: a JWKS in JSON stringified format
 * If jwks is set, JWK will be appended
 * @return RHN_OK on success, an error value on error
 * may return RHN_ERROR_PARAM if at least one JWK 
 * is invalid, but the will import the others
 */
int r_jwks_import_from_json_str(jwks_t * jwks, const char * input);

/**
 * Import a JWKS in json_t format into a jwk_t
 * @param jwks: the jwk_t * to import to
 * @param j_input: a JWK in json_t * format
 * If jwks is set, JWK will be appended
 * @return RHN_OK on success, an error value on error
 * may return RHN_ERROR_PARAM if at least one JWK 
 * is invalid, but the will import the others
 */
int r_jwks_import_from_json_t(jwks_t * jwks, json_t * j_input);

/**
 * Import a JWKS from an uri
 * @param jwks: the jwk_t * to import to
 * @param uri: an uri pointing to a JWKS
 * If jwks is set, JWK will be appended
 * @param x5u_flags: Flags to retrieve x5u certificates
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * @return RHN_OK on success, an error value on error
 * may return RHN_ERROR_PARAM if at least one JWK 
 * is invalid, but the will import the others
 */
int r_jwks_import_from_uri(jwks_t * jwks, const char * uri, int x5u_flags);

/**
 * Import data into a jwks
 * parameters must be set of values
 * with the mandatory parameters for each rhn_import
 * See rhn_import documentation
 * The parameters list MUST end with R_IMPORT_NONE
 * See rhn_import documentation
 * @return a jwks containing the list of keys parsed
 */
jwks_t * r_jwks_quick_import(rhn_import, ...);

/**
 * Return a copy of the JWKS
 * @param jwks: the jwks to copy
 * @return a copy of the jwks
 */
jwks_t * r_jwks_copy(jwks_t * jwks);

/**
 * Get the number of jwk_t in a jwks_t
 * @param jwks: the jwks_t * to evaluate
 * @return the number of jwk_t in a jwks_t
 */
size_t r_jwks_size(jwks_t * jwks);

/**
 * Get the jwk_t at the specified index of the jwks_t *
 * @param jwks: the jwks_t * to evaluate
 * @param index: the index of the array to retrieve
 * @return a jwk_t * on success, NULL on error
 * The returned jwk must be r_jwk_free after use
 */
jwk_t * r_jwks_get_at(jwks_t * jwks, size_t index);

/**
 * Get the jwk_t at the specified index of the jwks_t *
 * @param jwks: the jwks_t * to evaluate
 * @param kid: the key id of the jwk to retreive
 * @return a jwk_t * on success, NULL on error
 * The returned jwk must be r_jwk_free after use
 */
jwk_t * r_jwks_get_by_kid(jwks_t * jwks, const char * kid);

/**
 * Append a jwk_t at the end of the array of jwk_t in the jwks_t
 * @param jwks: the jwks_t * to append the jwk_t
 * @param jwk: the jwk_t * to be appended
 * @return RHN_OK on success, an error value on error
 */
int r_jwks_append_jwk(jwks_t * jwks, jwk_t * jwk);

/**
 * Update a jwk_t at the specified index of the jwks_t *
 * @param jwks: the jwks_t * to evaluate
 * @param jwk: the jwk_t * to set
 * @param index: the index of the array to update
 * @return RHN_OK on success, an error value on error
 */
int r_jwks_set_at(jwks_t * jwks, size_t index, jwk_t * jwk);

/**
 * Remove a jwk_t at the specified index of the jwks_t *
 * @param jwks: the jwks_t * to evaluate
 * @param index: the index of the array to remove
 * @return RHN_OK on success, an error value on error
 */
int r_jwks_remove_at(jwks_t * jwks, size_t index);

/**
 * Empty a JWKS
 * @param jwks: the jwks_t * to update
 * @return RHN_OK on success, an error value on error
 */
int r_jwks_empty(jwks_t * jwks);

/**
 * Compare 2 jwks
 * The key content and order are compared
 * @param jwks1: the first JWKS to compare
 * @param jwks2: the second JWKS to compare
 * @return 1 if both jwks1 and jwks2 are equal, 0 otherwise
 */
int r_jwks_equal(jwks_t * jwks1, jwks_t * jwks2);

/**
 * Export a jwks_t into a stringified JSON format
 * @param jwks: the jwks_t * to export
 * @param pretty: indent or compact JSON output
 * @return a char * on success, NULL on error, must be r_free'd after use
 */
char * r_jwks_export_to_json_str(jwks_t * jwks, int pretty);

/**
 * Export a jwk_t into a json_t format
 * @param jwks: the jwk_t * to export
 * @return a json_t * on success, NULL on error
 */
json_t * r_jwks_export_to_json_t(jwks_t * jwks);

/**
 * Export a jwks_t into a gnutls_privkey_t format
 * @param jwks: the jwks_t * to export
 * @param len: set the length of the output array
 * @return a heap-allocated gnutls_privkey_t * on success, NULL on error
 * an index of the returned array may be NULL if the corresponding jwk isn't a private key
 */
gnutls_privkey_t * r_jwks_export_to_gnutls_privkey(jwks_t * jwks, size_t * len);

/**
 * Export a jwks_t into a gnutls_pubkey_t format
 * @param jwks: the jwks_t * to export
 * @param len: set the length of the output array
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return NULL
 * @return a heap-allocated gnutls_pubkey_t * on success, NULL on error
 */
gnutls_pubkey_t * r_jwks_export_to_gnutls_pubkey(jwks_t * jwks, size_t * len, int x5u_flags);

/**
 * Export a jwks_t into a DER or PEM format
 * @param jwks: the jwks_t * to export
 * @param format: the format of the output, values available are R_FORMAT_PEM or R_FORMAT_DER
 * @param output: an unsigned char * that will contain the output
 * @param output_len: the size of output and will be set to the data size that has been written to output
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 * @return RHN_ERROR_PARAM if output_len isn't large enough to hold the output, then output_len will be set to the required size
 */
int r_jwks_export_to_pem_der(jwks_t * jwks, int format, unsigned char * output, size_t * output_len, int x5u_flags);

/**
 * Search in a jwks_t for a subset matching the given query
 * @param jwks: the jwks_t to look into
 * @param j_match: The query to match. Must be a JSON object with key/values
 * that will be compared against all keys in jwks
 * All parameters must match
 * Example, to look for all RSA keys, the parameter j_match must contain:
 * {kty: "RSA"}
 * To look for all RSA keys with the kid "1", the parameter j_match must contain:
 * {kty: "RSA", kid: "1"}
 * @return a new jwks_t * containing all the matching keys, or an empty jwks_t if no match
 */
jwks_t * r_jwks_search_json_t(jwks_t * jwks, json_t * j_match);

/**
 * Search in a jwks_t for a subset matching the given query
 * @param jwks: the jwks_t to look into
 * @param str_match: The query to match. Must be a stringified JSON object with key/values
 * that will be compared against all keys in jwks
 * All parameters must match
 * Example, to look for all RSA keys, the parameter str_match must contain:
 * {kty: "RSA"}
 * To look for all RSA keys with the kid "1", the parameter str_match must contain:
 * {kty: "RSA", kid: "1"}
 * @return a new jwks_t * containing all the matching keys, or an empty jwks_t if no match
 */
jwks_t * r_jwks_search_json_str(jwks_t * jwks, const char * str_match);

/**
 * @}
 */

/**
 * @defgroup jws JWS functions
 * Manage JSON Web Signatures
 * @{
 */

/**
 * Add multiple properties to the jws_t *
 * @param jws: the jws_t to set values
 * @param ...: set of values using a rhn_opt and following values
 */
int r_jws_set_properties(jws_t * jws, ...);

/**
 * Return a copy of the JWS
 * @param jws: the jws_t to duplicate
 * @return a copy of jws
 */
jws_t * r_jws_copy(jws_t * jws);

/**
 * Set the payload of the jws
 * @param jws: the jws_t to update
 * @param payload: the payload to set
 * @param payload_len: the size of the payload
 * @return RHN_OK on success, an error value on error
 */
int r_jws_set_payload(jws_t * jws, const unsigned char * payload, size_t payload_len);

/**
 * Get the JWS payload
 * @param jws: the jws_t to get the payload from
 * @param payload_len: the length of the JWS payload, may be NULL
 * @return a pointer to the JWS payload
 */
const unsigned char * r_jws_get_payload(jws_t * jws, size_t * payload_len);

/**
 * Set the JWS alg to use for signature
 * @param jws: the jws_t to update
 * @param alg: the algorithm to use
 * @return RHN_OK on success, an error value on error
 */
int r_jws_set_alg(jws_t * jws, jwa_alg alg);

/**
 * Get the JWS alg used for signature
 * @param jws: the jws_t to update
 * @return the algorithm used
 */
jwa_alg r_jws_get_alg(jws_t * jws);

/**
 * Get the KID specified in the header
 * used for signature
 * @param jws: the jws_t to update
 * @return the KID
 */
const char * r_jws_get_kid(jws_t * jws);

/**
 * Adds a string value to the JWS header
 * @param jws: the jws_t to update
 * @param key: the key to set to the JWS header
 * @param str_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jws_set_header_str_value(jws_t * jws, const char * key, const char * str_value);

/**
 * Adds an integer value to the JWS header
 * @param jws: the jws_t to update
 * @param key: the key to set to the JWS header
 * @param i_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jws_set_header_int_value(jws_t * jws, const char * key, rhn_int_t i_value);

/**
 * Adds a JSON value to the JWS header
 * @param jws: the jws_t to update
 * @param key: the key to set to the JWS header
 * @param j_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jws_set_header_json_t_value(jws_t * jws, const char * key, json_t * j_value);

/**
 * Sets the entire header with the JSON value specified
 * @param jws: the jws_t to update
 * @param j_value: the header to set, must be a JSON object
 * @return RHN_OK on success, an error value on error
 */
int r_jws_set_full_header_json_t(jws_t * jws, json_t * j_value);

/**
 * Sets the entire header with the stringified JSON value specified
 * @param jws: the jws_t to update
 * @param str_value: the header to set, must be a stringified JSON object
 * @return RHN_OK on success, an error value on error
 */
int r_jws_set_full_header_json_str(jws_t * jws, const char * str_value);

/**
 * Gets a string value from the JWS header
 * @param jws: the jws_t to get the value
 * @param key: the key to retreive the value
 * @return a string value, NULL if not present
 */
const char * r_jws_get_header_str_value(jws_t * jws, const char * key);

/**
 * Gets an integer value from the JWS header
 * @param jws: the jws_t to get the value
 * @param key: the key to retreive the value
 * @return an rhn_int_t value, 0 if not present
 */
rhn_int_t r_jws_get_header_int_value(jws_t * jws, const char * key);

/**
 * Gets a JSON value from the JWS header
 * @param jws: the jws_t to get the value
 * @param key: the key to retreive the value
 * @return a json_t * value, NULL if not present
 */
json_t * r_jws_get_header_json_t_value(jws_t * jws, const char * key);

/**
 * Return the full JWS header in JSON format
 * @param jws: the jws_t to get the value
 * @return a json_t * value
 */
json_t * r_jws_get_full_header_json_t(jws_t * jws);

/**
 * Return the full JWS header in char * 
 * @param jws: the jws_t to get the value
 * @return a char * value, must be r_free'd after use
 */
char * r_jws_get_full_header_str(jws_t * jws);

/**
 * Sets the private and public keys for the signature and verification
 * @param jws: the jws_t to update
 * @param jwk_privkey: the private key in jwk_t * format, can be NULL
 * @param jwk_pubkey: the public key in jwk_t * format, can be NULL
 * @return RHN_OK on success, an error value on error
 */
int r_jws_add_keys(jws_t * jws, jwk_t * jwk_privkey, jwk_t * jwk_pubkey);

/**
 * Adds private and/or public keys sets for the signature and verification
 * @param jws: the jws_t to update
 * @param jwks_privkey: the private key set in jwk_t * format, can be NULL
 * @param jwks_pubkey: the public key set in jwk_t * format, can be NULL
 * @return RHN_OK on success, an error value on error
 */
int r_jws_add_jwks(jws_t * jws, jwks_t * jwks_privkey, jwks_t * jwks_pubkey);

/**
 * Add keys to perform signature or signature verification
 * keys must be a JWK stringified
 * @param jws: the jws_t to update
 * @param privkey: the private key to sign
 * @param pubkey: the public key to verify the signature
 * @return RHN_OK on success, an error value on error
 */
int r_jws_add_keys_json_str(jws_t * jws, const char * privkey, const char * pubkey);

/**
 * Add keys to perform signature or signature verification
 * keys must be a JWK in json_t * format
 * @param jws: the jws_t to update
 * @param privkey: the private key to sign the
 * @param pubkey: the public key to verify the signature
 * @return RHN_OK on success, an error value on error
 */
int r_jws_add_keys_json_t(jws_t * jws, json_t * privkey, json_t * pubkey);

/**
 * Add keys to perform signature or signature verification
 * keys must be in PEM or DER format
 * @param jws: the jws_t to update
 * @param format: the format of the input, values available are R_FORMAT_PEM or R_FORMAT_DER
 * @param privkey: the private key to sign the
 * @param privkey_len: length of privkey
 * @param pubkey: the public key to verify the signature
 * @param pubkey_len: length of pubkey
 * @return RHN_OK on success, an error value on error
 */
int r_jws_add_keys_pem_der(jws_t * jws, int format, const unsigned char * privkey, size_t privkey_len, const unsigned char * pubkey, size_t pubkey_len);

/**
 * Add keys to perform signature or signature verification
 * keys must be gnutls key structures
 * @param jws: the jws_t to update
 * @param privkey: the private key to sign the
 * @param pubkey: the public key to verify the signature
 * @return RHN_OK on success, an error value on error
 */
int r_jws_add_keys_gnutls(jws_t * jws, gnutls_privkey_t privkey, gnutls_pubkey_t pubkey);

/**
 * Add symmetric key by value to perform signature or signature verification
 * @param jws: the jws_t to update
 * @param key: the raw key value
 * @param key_len: the length of the key
 * @return RHN_OK on success, an error value on error
 */
int r_jws_add_key_symmetric(jws_t * jws, const unsigned char * key, size_t key_len);

/**
 * Get private keys set for the signature
 * @param jws: the jws_t to get the value
 * @return the private key set in jwks_t * format
 */
jwks_t * r_jws_get_jwks_privkey(jws_t * jws);

/**
 * Get public keys set for the verification
 * @param jws: the jws_t to get the value
 * @return the public key set in jwks_t * format
 */
jwks_t * r_jws_get_jwks_pubkey(jws_t * jws);

/**
 * Parses the serialized JWS in all modes (compact, flattened or general)
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse, must end with a NULL string terminator
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_parse(jws_t * jws, const char * jws_str, int x5u_flags);

/**
 * Parses the serialized JWS in all modes (compact, flattened or general)
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse
 * @param jws_str_len: the length of jws_str to parse
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_parsen(jws_t * jws, const char * jws_str, size_t jws_str_len, int x5u_flags);

/**
 * Parses the serialized JWS in all modes (compact, flattened or general)
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse, must end with a NULL string terminator
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_advanced_parse(jws_t * jws, const char * jws_str, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the serialized JWS in all modes (compact, flattened or general)
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse
 * @param jws_str_len: the length of jws_str to parse
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_advanced_parsen(jws_t * jws, const char * jws_str, size_t jws_str_len, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the serialized JWS in all modes (compact, flattened or general)
 * Allows to parse unsigned JWS
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse, must end with a NULL string terminator
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_parse_unsecure(jws_t * jws, const char * jws_str, int x5u_flags);

/**
 * Parses the serialized JWS in all modes (compact, flattened or general)
 * Allows to parse unsigned JWS
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse
 * @param jws_str_len: the length of jws_str to parse
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_parsen_unsecure(jws_t * jws, const char * jws_str, size_t jws_str_len, int x5u_flags);

/**
 * Parses the serialized JWS in compact mode (xxx.yyy.zzz)
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse, must end with a NULL string terminator
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_compact_parse(jws_t * jws, const char * jws_str, int x5u_flags);

/**
 * Parses the serialized JWS in compact mode (xxx.yyy.zzz)
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse
 * @param jws_str_len: the length of jws_str to parse
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_advanced_compact_parsen(jws_t * jws, const char * jws_str, size_t jws_str_len, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the serialized JWS in compact mode (xxx.yyy.zzz)
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse, must end with a NULL string terminator
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_advanced_compact_parse(jws_t * jws, const char * jws_str, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the serialized JWS in compact mode (xxx.yyy.zzz)
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse
 * @param jws_str_len: the length of jws_str to parse
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_compact_parsen(jws_t * jws, const char * jws_str, size_t jws_str_len, int x5u_flags);

/**
 * Parses the serialized JWS in compact mode (xxx.yyy.zzz)
 * Allows to parse unsigned JWS
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse
 * @param jws_str_len: the length of jws_str to parse
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_compact_parsen_unsecure(jws_t * jws, const char * jws_str, size_t jws_str_len, int x5u_flags);

/**
 * Parses the serialized JWS in compact mode (xxx.yyy.zzz)
 * Allows to parse unsigned JWS
 * @param jws: the jws_t to update
 * @param jws_str: the serialized JWS to parse, must end with a NULL string terminator
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_compact_parse_unsecure(jws_t * jws, const char * jws_str, int x5u_flags);

/**
 * Parses the serialized JWS in JSON mode, general or flattened
 * @param jws: the jws_t to update
 * @param jws_json_str: the serialized JWS to parse in char * format
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_parse_json_str(jws_t * jws, const char * jws_json_str, int x5u_flags);

/**
 * Parses the serialized JWS in JSON mode, general or flattened
 * @param jws: the jws_t to update
 * @param jws_json_str: the serialized JWS to parse in char * format
 * @param jws_json_str_len: the length of jws_str to parse
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_parsen_json_str(jws_t * jws, const char * jws_json_str, size_t jws_json_str_len, int x5u_flags);

/**
 * Parses the serialized JWS in JSON mode, general or flattened
 * @param jws: the jws_t to update
 * @param jws_json: the serialized JWS to parse in json_t * format
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_parse_json_t(jws_t * jws, json_t * jws_json, int x5u_flags);

/**
 * Parses the serialized JWS in JSON mode, general or flattened
 * @param jws: the jws_t to update
 * @param jws_json_str: the serialized JWS to parse in char * format
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_advanced_parse_json_str(jws_t * jws, const char * jws_json_str, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the serialized JWS in JSON mode, general or flattened
 * @param jws: the jws_t to update
 * @param jws_json_str: the serialized JWS to parse in char * format
 * @param jws_json_str_len: the length of jws_str to parse
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_advanced_parsen_json_str(jws_t * jws, const char * jws_json_str, size_t jws_json_str_len, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the serialized JWS in JSON mode, general or flattened
 * @param jws: the jws_t to update
 * @param jws_json: the serialized JWS to parse in json_t * format
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_advanced_parse_json_t(jws_t * jws, json_t * jws_json, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the serialized JWS in all modes (compact, flattened or general)
 * @param jws_str: the serialized JWS to parse in char * format
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return a new jwt_t * on success, NULL on error
 */
jws_t * r_jws_quick_parse(const char * jws_str, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the serialized JWS in all modes (compact, flattened or general)
 * @param jws_str: the serialized JWS to parse in char * format
 * @param jws_str_len: the length of jws_str
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return a new jwt_t * on success, NULL on error
 */
jws_t * r_jws_quick_parsen(const char * jws_str, size_t jws_str_len, uint32_t parse_flags, int x5u_flags);

/**
 * Verifies the signature of the JWS
 * The JWS must contain a signature
 * or the JWS must have alg: none
 * If the jws has multiple signatures, it will return RHN_OK if one signature matches
 * the public key
 * @param jws: the jws_t to update
 * @param jwk_pubkey: the public key to check the signature,
 * can be NULL if jws already contains a public key
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jws_verify_signature(jws_t * jws, jwk_t * jwk_pubkey, int x5u_flags);

/**
 * Serialize a JWS in compact mode (xxx.yyy.zzz)
 * @param jws: the JWS to serialize
 * @param jwk_privkey: the private key to use to sign the JWS
 * can be NULL if jws already contains a private key
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return the JWS in serialized format, returned value must be r_free'd after use
 */
char * r_jws_serialize(jws_t * jws, jwk_t * jwk_privkey, int x5u_flags);

/**
 * Serialize a JWS in compact mode (xxx.yyy.zzz)
 * Allows to serialize unsigned JWS
 * @param jws: the JWS to serialize
 * @param jwk_privkey: the private key to use to sign the JWS
 * can be NULL if jws already contains a private key
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return the JWS in serialized format, returned value must be r_free'd after use
 */
char * r_jws_serialize_unsecure(jws_t * jws, jwk_t * jwk_privkey, int x5u_flags);

/**
 * Serialize a JWS into its JSON format (general or flattened)
 * Mode general: Multiple signatures are generated.
 * You can use the jws prikeys or specify the private keys
 * Every jwk used to sign the jws must have a property 'alg' to specify
 * the signing algorithm
 * It is recommended, but not mandatory, to use JWKs with kid property
 * @param jws: the JWS to serialize
 * @param jwks_privkey: the private keys to use to sign the JWS
 * can be NULL if jws already contains a private key set
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @param mode: JSON serialization mode
 * Values available are
 * - R_JSON_MODE_GENERAL: https://tools.ietf.org/html/rfc7515#section-7.2.1
 * - R_JSON_MODE_FLATTENED: https://tools.ietf.org/html/rfc7515#section-7.2.2
 * @return the JWS in json_t * format, returned value must be json_decref'd after use
 */
json_t * r_jws_serialize_json_t(jws_t * jws, jwks_t * jwks_privkey, int x5u_flags, int mode);

/**
 * Serialize a JWS into its JSON format (general or flattened)
 * Mode general: Multiple signatures are generated.
 * You can use the jws prikeys or specify the private keys
 * Every jwk used to sign the jws must have a property 'alg' to specify
 * the signing algorithm
 * It is recommended, but not mandatory, to use JWKs with kid property
 * @param jws: the JWS to serialize
 * @param jwks_privkey: the private keys to use to sign the JWS
 * can be NULL if jws already contains a private key set
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @param mode: JSON serialization mode
 * Values available are
 * - R_JSON_MODE_GENERAL: https://tools.ietf.org/html/rfc7515#section-7.2.1
 * - R_JSON_MODE_FLATTENED: https://tools.ietf.org/html/rfc7515#section-7.2.2
 * @return the JWS in char * format, returned value must be r_free'd after use
 */
char * r_jws_serialize_json_str(jws_t * jws, jwks_t * jwks_privkey, int x5u_flags, int mode);

/**
 * @}
 */

/**
 * @defgroup jwe JWE functions
 * Manage JSON Web Encryption
 * @{
 */

/**
 * Add multiple properties to the jwe_t *
 * @param jwe: the jwe_t to set values
 * @param ...: set of values using a rhn_opt and following values
 */
int r_jwe_set_properties(jwe_t * jwe, ...);

/**
 * Return a copy of the JWE
 * @param jwe: the jwe_t to duplicate
 * @return a copy of jwe
 */
jwe_t * r_jwe_copy(jwe_t * jwe);

/**
 * Set the payload of the jwe
 * @param jwe: the jwe_t to update
 * @param payload: the payload to set
 * @param payload_len: the size of the payload
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_payload(jwe_t * jwe, const unsigned char * payload, size_t payload_len);

/**
 * Get the JWE payload
 * @param jwe: the jwe_t to get the payload from
 * @param payload_len: the length of the JWE payload, may be NULL
 * @return a pointer to the JWE payload
 */
const unsigned char * r_jwe_get_payload(jwe_t * jwe, size_t * payload_len);

/**
 * Set the JWE alg to use for key encryption
 * @param jwe: the jwe_t to update
 * @param alg: the algorithm to use
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_alg(jwe_t * jwe, jwa_alg alg);

/**
 * Get the JWE alg used for key encryption
 * @param jwe: the jwe_t to update
 * @return the algorithm used
 */
jwa_alg r_jwe_get_alg(jwe_t * jwe);

/**
 * Set the JWE enc to use for payload encryption
 * @param jwe: the jwe_t to update
 * @param enc: the encorithm to use
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_enc(jwe_t * jwe, jwa_enc enc);

/**
 * Get the JWE enc used for payload encryption
 * @param jwe: the jwe_t to update
 * @return the encorithm used
 */
jwa_enc r_jwe_get_enc(jwe_t * jwe);

/**
 * Get the KID specified in the header
 * for payload encryption
 * @param jwe: the jwe_t to update
 * @return the KID
 */
const char * r_jwe_get_kid(jwe_t * jwe);

/**
 * Adds a string value to the JWE header
 * @param jwe: the jwe_t to update
 * @param key: the key to set to the JWE header
 * @param str_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_header_str_value(jwe_t * jwe, const char * key, const char * str_value);

/**
 * Adds an integer value to the JWE header
 * @param jwe: the jwe_t to update
 * @param key: the key to set to the JWE header
 * @param i_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_header_int_value(jwe_t * jwe, const char * key, rhn_int_t i_value);

/**
 * Adds a JSON value to the JWE header
 * @param jwe: the jwe_t to update
 * @param key: the key to set to the JWE header
 * @param j_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_header_json_t_value(jwe_t * jwe, const char * key, json_t * j_value);

/**
 * Sets the entire header with the JSON value specified
 * @param jwe: the jwe_t to update
 * @param j_header: the header to set, must be a JSON object
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_full_header_json_t(jwe_t * jwe, json_t * j_header);

/**
 * Sets the entire header with the stringified JSON value specified
 * @param jwe: the jwe_t to update
 * @param str_header: the header to set, must be a stringified JSON object
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_full_header_json_str(jwe_t * jwe, const char * str_header);

/**
 * Sets the entire unprotected header with the JSON value specified
 * The unprotected header is useful when serializing in JSON format
 * @param jwe: the jwe_t to update
 * @param j_unprotected_header: the unprotected header to set, must be a JSON object
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_full_unprotected_header_json_t(jwe_t * jwe, json_t * j_unprotected_header);

/**
 * Sets the entire unprotected header with the stringified JSON value specified
 * The unprotected header is useful when serializing in JSON format
 * @param jwe: the jwe_t to update
 * @param str_unprotected_header: the unprotected header to set, must be a stringified JSON object
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_full_unprotected_header_json_str(jwe_t * jwe, const char * str_unprotected_header);

/**
 * Return the full JWS unprotected header in JSON format
 * @param jwe: the jwe_t to get the value
 * @return a json_t * value
 */
json_t * r_jwe_get_full_unprotected_header_json_t(jwe_t * jwe);

/**
 * Return the full JWS unprotected header in char * 
 * @param jwe: the jwe_t to get the value
 * @return a char * value, must be r_free'd after use
 */
char * r_jwe_get_full_unprotected_header_str(jwe_t * jwe);

/**
 * Gets a string value from the JWE header
 * @param jwe: the jwe_t to get the value
 * @param key: the key to retreive the value
 * @return a string value, NULL if not present
 */
const char * r_jwe_get_header_str_value(jwe_t * jwe, const char * key);

/**
 * Gets an integer value from the JWE header
 * @param jwe: the jwe_t to get the value
 * @param key: the key to retreive the value
 * @return an rhn_int_t value, 0 if not present
 */
rhn_int_t r_jwe_get_header_int_value(jwe_t * jwe, const char * key);

/**
 * Gets a JSON value from the JWE header
 * @param jwe: the jwe_t to get the value
 * @param key: the key to retreive the value
 * @return a json_t * value, NULL if not present
 */
json_t * r_jwe_get_header_json_t_value(jwe_t * jwe, const char * key);

/**
 * Return the full JWE header in JSON format
 * @param jwe: the jwe_t to get the value
 * @return a json_t * value
 */
json_t * r_jwe_get_full_header_json_t(jwe_t * jwe);

/**
 * Return the full JWE header in char * 
 * @param jwe: the jwe_t to get the value
 * @return a char * value, must be r_free'd after use
 */
char * r_jwe_get_full_header_str(jwe_t * jwe);

/**
 * Adds private and/or public keys for the cypher key encryption and decryption
 * @param jwe: the jwe_t to update
 * @param jwk_privkey: the private key in jwk_t * format, can be NULL
 * @param jwk_pubkey: the public key in jwk_t * format, can be NULL
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_add_keys(jwe_t * jwe, jwk_t * jwk_privkey, jwk_t * jwk_pubkey);

/**
 * Adds private and/or public keys sets for the cypher key encryption and decryption
 * @param jwe: the jwe_t to update
 * @param jwks_privkey: the private key set in jwks_t * format, can be NULL
 * @param jwks_pubkey: the public key set in jwks_t * format, can be NULL
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_add_jwks(jwe_t * jwe, jwks_t * jwks_privkey, jwks_t * jwks_pubkey);

/**
 * Add keys to perform encryption ot decryption
 * keys must be a JWK stringified
 * @param jwe: the jwe_t to update
 * @param privkey: the private key to enc the
 * @param pubkey: the public key to verify the encature
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_add_keys_json_str(jwe_t * jwe, const char * privkey, const char * pubkey);

/**
 * Add keys to perform encryption ot decryption
 * keys must be a JWK in json_t * format
 * @param jwe: the jwe_t to update
 * @param privkey: the private key to enc the
 * @param pubkey: the public key to verify the encature
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_add_keys_json_t(jwe_t * jwe, json_t * privkey, json_t * pubkey);

/**
 * Add keys to perform encryption ot decryption
 * keys must be in PEM or DER format
 * @param jwe: the jwe_t to update
 * @param format: the format of the input, values available are R_FORMAT_PEM or R_FORMAT_DER
 * @param privkey: the private key to sign the
 * @param privkey_len: length of privkey
 * @param pubkey: the public key to verify the signature
 * @param pubkey_len: length of pubkey
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_add_keys_pem_der(jwe_t * jwe, int format, const unsigned char * privkey, size_t privkey_len, const unsigned char * pubkey, size_t pubkey_len);

/**
 * Add keys to perform encryption ot decryption
 * keys must be gnutls key structures
 * @param jwe: the jwe_t to update
 * @param privkey: the private key to enc the
 * @param pubkey: the public key to verify the encature
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_add_keys_gnutls(jwe_t * jwe, gnutls_privkey_t privkey, gnutls_pubkey_t pubkey);

/**
 * Add symmetric key by value to perform encryption ot decryption
 * @param jwe: the jwe_t to update
 * @param key: the raw key value
 * @param key_len: the length of the key
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_add_key_symmetric(jwe_t * jwe, const unsigned char * key, size_t key_len);

/**
 * Get private keys set for the cypher key decryption
 * @param jwe: the jwe_t to get the value
 * @return the private key set in jwks_t * format
 */
jwks_t * r_jwe_get_jwks_privkey(jwe_t * jwe);

/**
 * Get public keys set for the cypher key encryption
 * @param jwe: the jwe_t to get the value
 * @return the public key set in jwks_t * format
 */
jwks_t * r_jwe_get_jwks_pubkey(jwe_t * jwe);

/**
 * Sets the cypher key to encrypt or decrypt the payload
 * @param jwe: the jwe_t to update
 * @param key: the key to encrypt or decrypt the payload
 * @param key_len: the size of the key
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_cypher_key(jwe_t * jwe, const unsigned char * key, size_t key_len);

/**
 * Gets the cypher key to encrypt or decrypt the payload
 * @param jwe: the jwe_t to get the value
 * @param key_len: set the size of the key, may be NULL
 * @return the key to encrypt or decrypt the payload
 */
const unsigned char * r_jwe_get_cypher_key(jwe_t * jwe, size_t * key_len);

/**
 * Generates a random cypher key
 * @param jwe: the jwe_t to update
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_generate_cypher_key(jwe_t * jwe);

/**
 * Sets the Initialization Vector (iv)
 * @param jwe: the jwe_t to update
 * @param iv: the iv to set
 * @param iv_len: the size of the iv
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_iv(jwe_t * jwe, const unsigned char * iv, size_t iv_len);

/**
 * Gets the Initialization Vector (iv)
 * @param jwe: the jwe_t to get the value
 * @param iv_len: set the size of the iv, may be NULL
 * @return the iv
 */
const unsigned char * r_jwe_get_iv(jwe_t * jwe, size_t * iv_len);

/**
 * Generates a random Initialization Vector (iv)
 * @param jwe: the jwe_t to update
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_generate_iv(jwe_t * jwe);

/**
 * Sets the Additional Authenticated Data (aad)
 * @param jwe: the jwe_t to update
 * @param aad: the aad to set
 * @param aad_len: the size of the aad
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_set_aad(jwe_t * jwe, const unsigned char * aad, size_t aad_len);

/**
 * Gets the Additional Authenticated Data (aad)
 * @param jwe: the jwe_t to get the value
 * @param aad_len: set the size of the aad, may be NULL
 * @return the aad
 */
const unsigned char * r_jwe_get_aad(jwe_t * jwe, size_t * aad_len);

/**
 * Encrypts the payload using its key and iv
 * @param jwe: the jwe_t to update
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_encrypt_payload(jwe_t * jwe);

/**
 * Decrypts the payload using its key and iv
 * @param jwe: the jwe_t to update
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_decrypt_payload(jwe_t * jwe);

/**
 * Encrypts the key
 * @param jwe: the jwe_t to update
 * @param jwk_pubkey: the jwk to encrypt the key, may be NULL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_encrypt_key(jwe_t * jwe, jwk_t * jwk_pubkey, int x5u_flags);

/**
 * Decrypts the key
 * @param jwe: the jwe_t to update
 * @param jwk_privkey: the jwk to decrypt the key, may be NULL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_decrypt_key(jwe_t * jwe, jwk_t * jwk_privkey, int x5u_flags);

/**
 * Parses the JWE in all modes (compact, flattened or general)
 * @param jwe: the jwe_t to update
 * @param jwe_str: the jwe serialized to parse, must end with a NULL string terminator
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_parse(jwe_t * jwe, const char * jwe_str, int x5u_flags);

/**
 * Parses the JWE in all modes (compact, flattened or general)
 * @param jwe: the jwe_t to update
 * @param jwe_str: the jwe serialized to parse
 * @param jwe_str_len: the length of jwe_str
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_parsen(jwe_t * jwe, const char * jwe_str, size_t jwe_str_len, int x5u_flags);

/**
 * Parses the JWE in all modes (compact, flattened or general)
 * @param jwe: the jwe_t to update
 * @param jwe_str: the jwe serialized to parse, must end with a NULL string terminator
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_advanced_parse(jwe_t * jwe, const char * jwe_str, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the JWE in all modes (compact, flattened or general)
 * @param jwe: the jwe_t to update
 * @param jwe_str: the jwe serialized to parse
 * @param jwe_str_len: the length of jwe_str
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_advanced_parsen(jwe_t * jwe, const char * jwe_str, size_t jwe_str_len, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the JWE in compact mode (xxx.yyy.zzz.aaa.bbb)
 * @param jwe: the jwe_t to update
 * @param jwe_str: the jwe serialized to parse, must end with a NULL string terminator
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_compact_parse(jwe_t * jwe, const char * jwe_str, int x5u_flags);

/**
 * Parses the JWE in compact mode (xxx.yyy.zzz.aaa.bbb)
 * @param jwe: the jwe_t to update
 * @param jwe_str: the jwe serialized to parse
 * @param jwe_str_len: the length of jwe_str
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_compact_parsen(jwe_t * jwe, const char * jwe_str, size_t jwe_str_len, int x5u_flags);

/**
 * Parses the JWE in compact mode (xxx.yyy.zzz.aaa.bbb)
 * @param jwe: the jwe_t to update
 * @param jwe_str: the jwe serialized to parse, must end with a NULL string terminator
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_advanced_compact_parse(jwe_t * jwe, const char * jwe_str, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the JWE in compact mode (xxx.yyy.zzz.aaa.bbb)
 * @param jwe: the jwe_t to update
 * @param jwe_str: the jwe serialized to parse
 * @param jwe_str_len: the length of jwe_str
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_advanced_compact_parsen(jwe_t * jwe, const char * jwe_str, size_t jwe_str_len, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the serialized JWE in JSON mode, general or flattened
 * @param jwe: the jwe_t to update
 * @param jwe_json_str: the serialized JWE to parse in char * format, must end with a NULL string terminator
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_parse_json_str(jwe_t * jwe, const char * jwe_json_str, int x5u_flags);

/**
 * Parses the serialized JWE in JSON mode, general or flattened
 * @param jwe: the jwe_t to update
 * @param jwe_json_str: the serialized JWE to parse in char * format
 * @param jwe_json_str_len: the length of jwe_json_str
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_parsen_json_str(jwe_t * jwe, const char * jwe_json_str, size_t jwe_json_str_len, int x5u_flags);

/**
 * Parses the serialized JWE in JSON mode, general or flattened
 * @param jwe: the jwe_t to update
 * @param jwe_json: the serialized JWE to parse in json_t * format
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_parse_json_t(jwe_t * jwe, json_t * jwe_json, int x5u_flags);

/**
 * Parses the serialized JWE in JSON mode, general or flattened
 * @param jwe: the jwe_t to update
 * @param jwe_json_str: the serialized JWE to parse in char * format, must end with a NULL string terminator
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_advanced_parse_json_str(jwe_t * jwe, const char * jwe_json_str, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the serialized JWE in JSON mode, general or flattened
 * @param jwe: the jwe_t to update
 * @param jwe_json_str: the serialized JWE to parse in char * format
 * @param jwe_json_str_len: the length of jwe_json_str
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_advanced_parsen_json_str(jwe_t * jwe, const char * jwe_json_str, size_t jwe_json_str_len, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the serialized JWE in JSON mode, general or flattened
 * @param jwe: the jwe_t to update
 * @param jwe_json: the serialized JWE to parse in json_t * format
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_advanced_parse_json_t(jwe_t * jwe, json_t * jwe_json, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the JWE in all modes (compact, flattened or general)
 * @param jwe_str: the jwe serialized to parse
 * @param x5u_flags: Flags to retrieve x5u certificates
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return a new jwe_t * on success, NULL on error
 */
jwe_t * r_jwe_quick_parse(const char * jwe_str, uint32_t parse_flags, int x5u_flags);

/**
 * Parses the JWE in all modes (compact, flattened or general)
 * @param jwe_str: the jwe serialized to parse
 * @param jwe_str_len: the length of jwe_str
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return a new jwe_t * on success, NULL on error
 */
jwe_t * r_jwe_quick_parsen(const char * jwe_str, size_t jwe_str_len, uint32_t parse_flags, int x5u_flags);

/**
 * Decrypts the payload of the JWE
 * @param jwe: the jwe_t to update
 * @param jwk_privkey: the private key to decrypt cypher key,
 * can be NULL if jwe already contains a private key
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwe_decrypt(jwe_t * jwe, jwk_t * jwk_privkey, int x5u_flags);

/**
 * Serialize a JWE into its string format (aaa.bbb.ccc.xxx.yyy.zzz)
 * @param jwe: the JWE to serialize
 * @param jwk_pubkey: the public key to encrypt the cypher key,
 * can be NULL if jwe already contains a public key
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return the JWE in serialized format, returned value must be r_free'd after use
 */
char * r_jwe_serialize(jwe_t * jwe, jwk_t * jwk_pubkey, int x5u_flags);

/**
 * Serialize a JWE into its JSON format (general or flattened)
 * Mode general: Multiple encryptions are generated.
 * You can use the jwe pubkeys or specify the public keys
 * Every jwk used to encrypt the key must have a property 'alg' to specify
 * the encryption algorithm
 * It is recommended, but not mandatory, to use JWKs with kid property
 * @param jwe: the JWE to serialize
 * @param jwks_pubkey: the public keys to use to sign the JWE
 * can be NULL if jwe already contains a public key set
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @param mode: JSON serialization mode
 * Values available are
 * - R_JSON_MODE_GENERAL: https://tools.ietf.org/html/rfc7516#section-7.2.1
 * - R_JSON_MODE_FLATTENED: https://tools.ietf.org/html/rfc7516#section-7.2.2
 * @return the JWE in char * format, returned value must be r_free'd after use
 */
char * r_jwe_serialize_json_str(jwe_t * jwe, jwks_t * jwks_pubkey, int x5u_flags, int mode);

/**
 * Serialize a JWE into its JSON format (general or flattened)
 * Mode general: Multiple encryptions are generated.
 * You can use the jwe pubkeys or specify the public keys
 * Every jwk used to encrypt the key must have a property 'alg' to specify
 * the encryption algorithm
 * It is recommended, but not mandatory, to use JWKs with kid property
 * @param jwe: the JWE to serialize
 * @param jwks_pubkey: the public keys to use to sign the JWE
 * can be NULL if jwe already contains a public key set
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @param mode: JSON serialization mode
 * Values available are
 * - R_JSON_MODE_GENERAL: https://tools.ietf.org/html/rfc7516#section-7.2.1
 * - R_JSON_MODE_FLATTENED: https://tools.ietf.org/html/rfc7516#section-7.2.2
 * @return the JWE in json_t * format, returned value must be json_decref'd after use
 */
json_t * r_jwe_serialize_json_t(jwe_t * jwe, jwks_t * jwks_pubkey, int x5u_flags, int mode);

/**
 * @}
 */

/**
 * @defgroup jwt JWT functions
 * Manage JSON Web Token
 * @{
 */

/**
 * Add multiple properties to the jwt_t *
 * @param jwt: the jwt_t to set values
 * @param ...: set of values using a rhn_opt and following values
 */
int r_jwt_set_properties(jwt_t * jwt, ...);

/**
 * Return a copy of the JWT
 * @param jwt: the jwt_t to duplicate
 * @return a copy of jwt
 */
jwt_t * r_jwt_copy(jwt_t * jwt);

/**
 * Adds a string value to the JWT header
 * @param jwt: the jwt_t to update
 * @param key: the key to set to the JWT header
 * @param str_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_header_str_value(jwt_t * jwt, const char * key, const char * str_value);

/**
 * Adds an integer value to the JWT header
 * @param jwt: the jwt_t to update
 * @param key: the key to set to the JWT header
 * @param i_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_header_int_value(jwt_t * jwt, const char * key, rhn_int_t i_value);

/**
 * Adds a JSON value to the JWT header
 * @param jwt: the jwt_t to update
 * @param key: the key to set to the JWT header
 * @param j_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_header_json_t_value(jwt_t * jwt, const char * key, json_t * j_value);

/**
 * Sets the entire header with the JSON value specified
 * @param jwt: the jwt_t to update
 * @param j_header: the header to set, must be a JSON object
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_full_header_json_t(jwt_t * jwt, json_t * j_header);

/**
 * Sets the entire header with the stringified JSON value specified
 * @param jwt: the jwt_t to update
 * @param str_header: the header to set, must be a JSON object
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_full_header_json_str(jwt_t * jwt, const char * str_header);

/**
 * Gets a string value from the JWT header
 * @param jwt: the jwt_t to get the value
 * @param key: the key to retreive the value
 * @return a string value, NULL if not present
 */
const char * r_jwt_get_header_str_value(jwt_t * jwt, const char * key);

/**
 * Gets an integer value from the JWT header
 * @param jwt: the jwt_t to get the value
 * @param key: the key to retreive the value
 * @return an rhn_int_t value, 0 if not present
 */
rhn_int_t r_jwt_get_header_int_value(jwt_t * jwt, const char * key);

/**
 * Gets a JSON value from the JWT header
 * @param jwt: the jwt_t to get the value
 * @param key: the key to retreive the value
 * @return a json_t * value, NULL if not present
 */
json_t * r_jwt_get_header_json_t_value(jwt_t * jwt, const char * key);

/**
 * Return the full JWT header in JSON format
 * @param jwt: the jwt_t to get the value
 * @return a json_t * value
 */
json_t * r_jwt_get_full_header_json_t(jwt_t * jwt);

/**
 * Return the full JWT header in char * 
 * @param jwt: the jwt_t to get the value
 * @return a char * value, must be r_free'd after use
 */
char * r_jwt_get_full_header_str(jwt_t * jwt);

/**
 * Adds a string value to the JWT claim
 * @param jwt: the jwt_t to update
 * @param key: the key to set to the JWT claim
 * @param str_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_claim_str_value(jwt_t * jwt, const char * key, const char * str_value);

/**
 * Adds an integer value to the JWT claim
 * @param jwt: the jwt_t to update
 * @param key: the key to set to the JWT claim
 * @param i_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_claim_int_value(jwt_t * jwt, const char * key, rhn_int_t i_value);

/**
 * Adds a JSON value to the JWT claim
 * @param jwt: the jwt_t to update
 * @param key: the key to set to the JWT claim
 * @param j_value: the value to set
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_claim_json_t_value(jwt_t * jwt, const char * key, json_t * j_value);

/**
 * Gets a string value from the JWT claim
 * @param jwt: the jwt_t to get the value
 * @param key: the key to retreive the value
 * @return a string value, NULL if not present
 */
const char * r_jwt_get_claim_str_value(jwt_t * jwt, const char * key);

/**
 * Gets an integer value from the JWT claim
 * @param jwt: the jwt_t to get the value
 * @param key: the key to retreive the value
 * @return an rhn_int_t value, 0 if not present
 */
rhn_int_t r_jwt_get_claim_int_value(jwt_t * jwt, const char * key);

/**
 * Gets a JSON value from the JWT claim
 * @param jwt: the jwt_t to get the value
 * @param key: the key to retreive the value
 * @return a json_t * value, NULL if not present
 */
json_t * r_jwt_get_claim_json_t_value(jwt_t * jwt, const char * key);

/**
 * Return the full JWT claim in JSON format
 * @param jwt: the jwt_t to get the value
 * @return a json_t * value
 */
json_t * r_jwt_get_full_claims_json_t(jwt_t * jwt);

/**
 * Return the full JWT claims in char *
 * @param jwt: the jwt_t to get the value
 * @return a char * value, must be r_free'd after use
 */
char * r_jwt_get_full_claims_str(jwt_t * jwt);

/**
 * Set the full JWT claim in JSON format
 * delete all existing value
 * @param jwt: the jwt_t to get the value
 * @param j_claim: the claim to set, must be JSON object
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_full_claims_json_t(jwt_t * jwt, json_t * j_claim);

/**
 * Set the full JWT claim in JSON format
 * delete all existing value
 * @param jwt: the jwt_t to get the value
 * @param str_claims: the claim to set, must be JSON object in string format
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_full_claims_json_str(jwt_t * jwt, const char * str_claims);

/**
 * Append the given JSON object in the JWT payload
 * Replace existing claim if already set
 * @param jwt: the jwt_t to get the value
 * @param j_claim: the payload to set, must be JSON object
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_append_claims_json_t(jwt_t * jwt, json_t * j_claim);

/**
 * Add keys to perform signature or signature verification to the JWT
 * @param jwt: the jwt_t to update
 * @param privkey: the private key to sign the JWT
 * @param pubkey: the public key to verify the JWT signature
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_sign_keys(jwt_t * jwt, jwk_t * privkey, jwk_t * pubkey);

/**
 * Adds private and/or public keys sets for the signature and verification
 * @param jwt: the jwt_t to update
 * @param jwks_privkey: the private key set in jwk_t * format, can be NULL
 * @param jwks_pubkey: the public key set in jwk_t * format, can be NULL
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_sign_jwks(jwt_t * jwt, jwks_t * jwks_privkey, jwks_t * jwks_pubkey);

/**
 * Add keys to perform signature or signature verification to the JWT
 * keys must be a JWK stringified
 * @param jwt: the jwt_t to update
 * @param privkey: the private key to sign the JWT
 * @param pubkey: the public key to verify the JWT signature
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_sign_keys_json_str(jwt_t * jwt, const char * privkey, const char * pubkey);

/**
 * Add keys to perform signature or signature verification to the JWT
 * keys must be a JWK in json_t * format
 * @param jwt: the jwt_t to update
 * @param privkey: the private key to sign the JWT
 * @param pubkey: the public key to verify the JWT signature
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_sign_keys_json_t(jwt_t * jwt, json_t * privkey, json_t * pubkey);

/**
 * Add keys to perform signature or signature verification to the JWT
 * keys must be in PEM or DER format
 * @param jwt: the jwt_t to update
 * @param format: the format of the input, values available are R_FORMAT_PEM or R_FORMAT_DER
 * @param privkey: the private key to sign the
 * @param privkey_len: length of privkey
 * @param pubkey: the public key to verify the signature
 * @param pubkey_len: length of pubkey
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_sign_keys_pem_der(jwt_t * jwt, int format, const unsigned char * privkey, size_t privkey_len, const unsigned char * pubkey, size_t pubkey_len);

/**
 * Add keys to perform signature or signature verification to the JWT
 * keys must be gnutls key structures
 * @param jwt: the jwt_t to update
 * @param privkey: the private key to sign the JWT
 * @param pubkey: the public key to verify the JWT signature
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_sign_keys_gnutls(jwt_t * jwt, gnutls_privkey_t privkey, gnutls_pubkey_t pubkey);

/**
 * Add symmetric key by value to perform signature or signature verification to the JWT
 * @param jwt: the jwt_t to update
 * @param key: the raw key value
 * @param key_len: the length of the key
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_sign_key_symmetric(jwt_t * jwt, const unsigned char * key, size_t key_len);

/**
 * Get private keys set for the signature
 * @param jwt: the jwt_t to get the value
 * @return the private key set in jwks_t * format
 */
jwks_t * r_jwt_get_sign_jwks_privkey(jwt_t * jwt);

/**
 * Get public keys set for the verification
 * @param jwt: the jwt_t to get the value
 * @return the public key set in jwks_t * format
 */
jwks_t * r_jwt_get_sign_jwks_pubkey(jwt_t * jwt);

/**
 * Add keys to perform encryption ot decryption to the JWT
 * @param jwt: the jwt_t to update
 * @param privkey: the private key to decrypt the JWT
 * @param pubkey: the public key to encrypt the JWT
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_enc_keys(jwt_t * jwt, jwk_t * privkey, jwk_t * pubkey);

/**
 * Adds private and/or public keys sets for the cypher key encryption and decryption
 * @param jwt: the jwt_t to update
 * @param jwks_privkey: the private key set in jwks_t * format, can be NULL
 * @param jwks_pubkey: the public key set in jwks_t * format, can be NULL
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_enc_jwks(jwt_t * jwt, jwks_t * jwks_privkey, jwks_t * jwks_pubkey);

/**
 * Add keys to perform encryption ot decryption to the JWT
 * keys must be a JWK stringified
 * @param jwt: the jwt_t to update
 * @param privkey: the private key to enc the JWT
 * @param pubkey: the public key to verify the JWT encature
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_enc_keys_json_str(jwt_t * jwt, const char * privkey, const char * pubkey);

/**
 * Add keys to perform encryption ot decryption to the JWT
 * keys must be a JWK in json_t * format
 * @param jwt: the jwt_t to update
 * @param privkey: the private key to enc the JWT
 * @param pubkey: the public key to verify the JWT encature
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_enc_keys_json_t(jwt_t * jwt, json_t * privkey, json_t * pubkey);

/**
 * Add keys to perform encryption ot decryption to the JWT
 * keys must be in PEM or DER format
 * @param jwt: the jwt_t to update
 * @param format: the format of the input, values available are R_FORMAT_PEM or R_FORMAT_DER
 * @param privkey: the private key to sign the
 * @param privkey_len: length of privkey
 * @param pubkey: the public key to verify the signature
 * @param pubkey_len: length of pubkey
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_enc_keys_pem_der(jwt_t * jwt, int format, const unsigned char * privkey, size_t privkey_len, const unsigned char * pubkey, size_t pubkey_len);

/**
 * Add keys to perform encryption ot decryption to the JWT
 * keys must be gnutls key structures
 * @param jwt: the jwt_t to update
 * @param privkey: the private key to enc the JWT
 * @param pubkey: the public key to verify the JWT encature
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_enc_keys_gnutls(jwt_t * jwt, gnutls_privkey_t privkey, gnutls_pubkey_t pubkey);

/**
 * Add symmetric key by value to perform encryption ot decryption to the JWT
 * @param jwt: the jwt_t to update
 * @param key: the raw key value
 * @param key_len: the length of the key
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_add_enc_key_symmetric(jwt_t * jwt, const unsigned char * key, size_t key_len);

/**
 * Get private keys set for the cypher key decryption
 * @param jwt: the jwt_t to get the value
 * @return the private key set in jwks_t * format
 */
jwks_t * r_jwt_get_enc_jwks_privkey(jwt_t * jwt);

/**
 * Get public keys set for the cypher key encryption
 * @param jwt: the jwt_t to get the value
 * @return the public key set in jwks_t * format
 */
jwks_t * r_jwt_get_enc_jwks_pubkey(jwt_t * jwt);

/**
 * Set the JWT alg to use for signature
 * @param jwt: the jwt_t to update
 * @param alg: the algorithm to use for signature
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_sign_alg(jwt_t * jwt, jwa_alg alg);

/**
 * Get the JWT alg used for signature
 * @param jwt: the jwt_t
 * @return the algorithm used for signature
 */
jwa_alg r_jwt_get_sign_alg(jwt_t * jwt);

/**
 * Set the JWT alg to use for key encryption
 * @param jwt: the jwt_t
 * @param alg: the algorithm to use for key encryption
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_enc_alg(jwt_t * jwt, jwa_alg alg);

/**
 * Get the JWT alg used for key encryption
 * @param jwt: the jwt_t
 * @return the algorithm used for key encryption
 */
jwa_alg r_jwt_get_enc_alg(jwt_t * jwt);

/**
 * Set the JWT enc to use for payload encryption
 * @param jwt: the jwt_t
 * @param enc: the encorithm to use for payload encryption
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_enc(jwt_t * jwt, jwa_enc enc);

/**
 * Get the JWT enc used for payload encryption
 * @param jwt: the jwt_t
 * @return the encorithm used for payload encryption
 */
jwa_enc r_jwt_get_enc(jwt_t * jwt);

/**
 * Get the KID specified in the header
 * for payload encryption
 * @param jwt: the jwt_t
 * @return the KID
 */
const char * r_jwt_get_enc_kid(jwt_t * jwt);

/**
 * Get the KID specified in the header
 * for signature
 * @param jwt: the jwt_t
 * @return the KID
 */
const char * r_jwt_get_sig_kid(jwt_t * jwt);

/**
 * Sets the cypher key to encrypt or decrypt the payload
 * @param jwt: the jwt_t to update
 * @param key: the key to encrypt or decrypt the payload
 * @param key_len: the size of the key
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_enc_cypher_key(jwt_t * jwt, const unsigned char * key, size_t key_len);

/**
 * Gets the cypher key to encrypt or decrypt the payload
 * @param jwt: the jwt_t to get the value
 * @param key_len: set the size of the key, may be NULL
 * @return the key to encrypt or decrypt the payload
 */
const unsigned char * r_jwt_get_enc_cypher_key(jwt_t * jwt, size_t * key_len);

/**
 * Generates a random cypher key
 * @param jwt: the jwt_t to update
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_generate_enc_cypher_key(jwt_t * jwt);

/**
 * Sets the Initialization Vector (iv)
 * @param jwt: the jwt_t to update
 * @param iv: the iv to set
 * @param iv_len: the size of the iv
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_set_enc_iv(jwt_t * jwt, const unsigned char * iv, size_t iv_len);

/**
 * Gets the Initialization Vector (iv)
 * @param jwt: the jwt_t to get the value
 * @param iv_len: set the size of the iv, may be NULL
 * @return the iv
 */
const unsigned char * r_jwt_get_enc_iv(jwt_t * jwt, size_t * iv_len);

/**
 * Generates a random Initialization Vector (iv)
 * @param jwt: the jwt_t to update
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_generate_iv(jwt_t * jwt);

/**
 * Return a signed JWT in serialized format (xxx.yyy.zzz)
 * @param jwt: the jwt_t to sign
 * @param privkey: the private key to sign the JWT, may be NULL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return NULL
 * @return RHN_OK on success, an error value on error
 */
char * r_jwt_serialize_signed(jwt_t * jwt, jwk_t * privkey, int x5u_flags);

/**
 * Return a signed JWT in serialized format (xxx.yyy.zzz)
 * Allows to serialize unsigned JWT
 * @param jwt: the jwt_t to sign
 * @param privkey: the private key to sign the JWT, may be NULL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return NULL
 * @return RHN_OK on success, an error value on error
 */
char * r_jwt_serialize_signed_unsecure(jwt_t * jwt, jwk_t * privkey, int x5u_flags);

/**
 * Return an encrypted JWT in serialized format (xxx.yyy.zzz.aaa.bbb)
 * @param jwt: the jwt_t to encrypt
 * @param pubkey: the public key to encrypt the JWT, may be NULL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return NULL
 * @return RHN_OK on success, an error value on error
 */
char * r_jwt_serialize_encrypted(jwt_t * jwt, jwk_t * pubkey, int x5u_flags);

/**
 * Return a nested JWT in serialized format
 * A nested JWT can be signed, then encrypted, or encrypted, then signed
 * @param jwt: the jwt_t to serialize
 * @param type: the nesting type
 * Values available are
 * - R_JWT_TYPE_NESTED_SIGN_THEN_ENCRYPT: the JWT will be signed, then the token will be encrypted in a JWE
 * - R_JWT_TYPE_NESTED_ENCRYPT_THEN_SIGN: The JWT will be encrypted, then the token will be signed in a JWS
 * @param sign_key: the key to sign the JWT, may be NULL
 * @param sign_key_x5u_flags: Flags to retrieve x5u certificates in sign_key
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return NULL
 * @param encrypt_key: the key to encrypt the JWT, may be NULL
 * @param encrypt_key_x5u_flags: Flags to retrieve x5u certificates in encrypt_key
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return NULL
 * @return RHN_OK on success, an error value on error
 */
char * r_jwt_serialize_nested(jwt_t * jwt, unsigned int type, jwk_t * sign_key, int sign_key_x5u_flags, jwk_t * encrypt_key, int encrypt_key_x5u_flags);

/**
 * Parses a serialized JWT
 * If the JWT is signed only, the claims will be available
 * If the JWT is encrypted, the claims will not be accessible until
 * r_jwt_decrypt or r_jwt_decrypt_verify_signature_nested is succesfull
 * @param jwt: the jwt that will contain the parsed token
 * @param token: the token to parse into a JWT, must end with a NULL string terminator
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_parse(jwt_t * jwt, const char * token, int x5u_flags);

/**
 * Parses a serialized JWT
 * If the JWT is signed only, the claims will be available
 * If the JWT is encrypted, the claims will not be accessible until
 * r_jwt_decrypt or r_jwt_decrypt_verify_signature_nested is succesfull
 * @param jwt: the jwt that will contain the parsed token
 * @param token: the token to parse into a JWT
 * @param token_len: token length
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_parsen(jwt_t * jwt, const char * token, size_t token_len, int x5u_flags);

/**
 * Parses a serialized JWT
 * Allows to parse unsigned JWT
 * If the JWT is signed only, the claims will be available
 * If the JWT is encrypted, the claims will not be accessible until
 * r_jwt_decrypt or r_jwt_decrypt_verify_signature_nested is succesfull
 * @param jwt: the jwt that will contain the parsed token
 * @param token: the token to parse into a JWT, must end with a NULL string terminator
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_parse_unsecure(jwt_t * jwt, const char * token, int x5u_flags);

/**
 * Parses a serialized JWT
 * Allows to parse unsigned JWT
 * If the JWT is signed only, the claims will be available
 * If the JWT is encrypted, the claims will not be accessible until
 * r_jwt_decrypt or r_jwt_decrypt_verify_signature_nested is succesfull
 * @param jwt: the jwt that will contain the parsed token
 * @param token: the token to parse into a JWT
 * @param token_len: token length
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_parsen_unsecure(jwt_t * jwt, const char * token, size_t token_len, int x5u_flags);

/**
 * Parses a serialized JWT
 * If the JWT is signed only, the claims will be available
 * If the JWT is encrypted, the claims will not be accessible until
 * r_jwt_decrypt or r_jwt_decrypt_verify_signature_nested is succesfull
 * @param jwt: the jwt that will contain the parsed token
 * @param token: the token to parse into a JWT, must end with a NULL string terminator
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_advanced_parse(jwt_t * jwt, const char * token, uint32_t parse_flags, int x5u_flags);

/**
 * Parses a serialized JWT
 * If the JWT is signed only, the claims will be available
 * If the JWT is encrypted, the claims will not be accessible until
 * r_jwt_decrypt or r_jwt_decrypt_verify_signature_nested is succesfull
 * @param jwt: the jwt that will contain the parsed token
 * @param token: the token to parse into a JWT
 * @param token_len: token length
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_advanced_parsen(jwt_t * jwt, const char * token, size_t token_len, uint32_t parse_flags, int x5u_flags);

/**
 * Parses a serialized JWT
 * If the JWT is signed only, the claims will be available
 * If the JWT is encrypted, the claims will not be accessible until
 * r_jwt_decrypt or r_jwt_decrypt_verify_signature_nested is succesfull
 * @param token: the token to parse into a JWT, must end with a NULL string terminator
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return a new jwt_t * on success, NULL on error
 */
jwt_t * r_jwt_quick_parse(const char * token, uint32_t parse_flags, int x5u_flags);

/**
 * Parses a serialized JWT
 * If the JWT is signed only, the claims will be available
 * If the JWT is encrypted, the claims will not be accessible until
 * r_jwt_decrypt or r_jwt_decrypt_verify_signature_nested is succesfull
 * @param token: the token to parse into a JWT
 * @param token_len: token length
 * @param parse_flags: Flags to set or unset options
 * Flags available are
 * - R_PARSE_NONE
 * - R_PARSE_HEADER_JWK
 * - R_PARSE_HEADER_JKU
 * - R_PARSE_HEADER_X5C
 * - R_PARSE_HEADER_X5U
 * - R_PARSE_HEADER_ALL
 * - R_PARSE_UNSIGNED
 * - R_PARSE_ALL
 * @param x5u_flags: Flags to retrieve x5u certificates
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return a new jwt_t * on success, NULL on error
 */
jwt_t * r_jwt_quick_parsen(const char * token, size_t token_len, uint32_t parse_flags, int x5u_flags);

/**
 * Get the type of JWT after a succesfull r_jwt_parse
 * @param jwt: the jwt_t to check
 * @return the type of JWT, values available are
 * R_JWT_TYPE_NONE: not a JWT
 * R_JWT_TYPE_SIGN: A signed JWT
 * R_JWT_TYPE_ENCRYPT: An encrypted JWT
 * R_JWT_TYPE_NESTED_SIGN_THEN_ENCRYPT: A nested JWT first signed, then encrypted
 * R_JWT_TYPE_NESTED_ENCRYPT_THEN_SIGN: A nested JWT first encrypted, then signed
 */
int r_jwt_get_type(jwt_t * jwt);

/**
 * Guess the type of JWT based on the
 * token format, but without parsing the token
 * @param token: the token to check
 * @return the type of JWT, values available are
 * R_JWT_TYPE_NONE: not a JWT
 * R_JWT_TYPE_SIGN: A signed JWT
 * R_JWT_TYPE_ENCRYPT: An encrypted JWT
 */
int r_jwt_token_type(const char * token);

/**
 * Guess the type of JWT based on the
 * token format, but without parsing the token
 * @param token: the token to check
 * @param token_len: token length
 * @return the type of JWT, values available are
 * R_JWT_TYPE_NONE: not a JWT
 * R_JWT_TYPE_SIGN: A signed JWT
 * R_JWT_TYPE_ENCRYPT: An encrypted JWT
 */
int r_jwt_token_typen(const char * token, size_t token_len);

/**
 * Verifies the signature of the JWT
 * The JWT must contain a signature
 * or the JWT must have alg: none
 * @param jwt: the jwt_t to update
 * @param pubkey: the public key to check the signature,
 * can be NULL if jws already contains a public key
 * @param x5u_flags: Flags to retrieve x5u certificates in pubkey
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_verify_signature(jwt_t * jwt, jwk_t * pubkey, int x5u_flags);

/**
 * Decrypts the payload of the JWT
 * @param jwt: the jwt_t to decrypt
 * @param privkey: the private key to decrypt cypher key,
 * can be NULL if jwt already contains a private key
 * @param x5u_flags: Flags to retrieve x5u certificates in privkey
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_decrypt(jwt_t * jwt, jwk_t * privkey, int x5u_flags);

/**
 * Decrypts and verifies the signature of a nested JWT
 * Fills the claims if the decryption and signature verifiation are succesfull
 * @param jwt: the jwt_t to decrypt and verify signature
 * @param verify_key: the public key to check the signature,
 * can be NULL if jws already contains a public key
 * @param verify_key_x5u_flags: Flags to retrieve x5u certificates in verify_key
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @param decrypt_key: the private key to decrypt cypher key,
 * can be NULL if jwt already contains a private key
 * @param decrypt_key_x5u_flags: Flags to retrieve x5u certificates in decrypt_key
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_decrypt_verify_signature_nested(jwt_t * jwt, jwk_t * verify_key, int verify_key_x5u_flags, jwk_t * decrypt_key, int decrypt_key_x5u_flags);

/**
 * Decrypts a nested JWT, do not verify the signature
 * Fills the claims if the decryption is succesfull
 * @param jwt: the jwt_t to decrypt and verify signature
 * @param decrypt_key: the private key to decrypt cypher key,
 * can be NULL if jwt already contains a private key
 * @param decrypt_key_x5u_flags: Flags to retrieve x5u certificates in decrypt_key
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_decrypt_nested(jwt_t * jwt, jwk_t * decrypt_key, int decrypt_key_x5u_flags);

/**
 * Verifies the signature of a nested JWT
 * @param jwt: the jwt_t to decrypt and verify signature
 * @param verify_key: the public key to check the signature,
 * can be NULL if jws already contains a public key
 * @param verify_key_x5u_flags: Flags to retrieve x5u certificates in verify_key
 * pointed by x5u if necessary, could be 0 if not needed
 * Flags available are 
 * - R_FLAG_IGNORE_SERVER_CERTIFICATE: ignrore if web server certificate is invalid
 * - R_FLAG_FOLLOW_REDIRECT: follow redirections if necessary
 * - R_FLAG_IGNORE_REMOTE: do not download remote key, but the function may return an error
 * @return RHN_OK on success, an error value on error
 */
int r_jwt_verify_signature_nested(jwt_t * jwt, jwk_t * verify_key, int verify_key_x5u_flags);

/**
 * Validates the jwt claims with the list of expected claims given in parameters
 * The list must end with the claim type R_JWT_CLAIM_NOP
 * Claim types available
 * - R_JWT_CLAIM_ISS: claim "iss", values expected a string or NULL to validate the presence of the claim
 * - R_JWT_CLAIM_SUB: claim "sub", values expected a string or NULL to validate the presence of the claim
 * - R_JWT_CLAIM_AUD: claim "aud", values expected a string or NULL to validate the presence of the claim
 * - R_JWT_CLAIM_EXP: claim "exp", value expected R_JWT_CLAIM_NOW or an positive integer value or R_JWT_CLAIM_PRESENT to validate the presence of the claim
 * - R_JWT_CLAIM_NBF: claim "nbf", value expected R_JWT_CLAIM_NOW or an positive integer value or R_JWT_CLAIM_PRESENT to validate the presence of the claim
 * - R_JWT_CLAIM_IAT: claim "iat", value expected R_JWT_CLAIM_NOW or an positive integer value or R_JWT_CLAIM_PRESENT to validate the presence of the claim
 * - R_JWT_CLAIM_JTI: claim "jti", values expected a string or NULL to validate the presence of the claim
 * - R_JWT_CLAIM_STR: the claim name specified must have the string value expected or NULL to validate the presence of the claim
 * - R_JWT_CLAIM_INT: the claim name specified must have the integer value expected
 * - R_JWT_CLAIM_JSN: the claim name specified must have the json_t * value expected or NULL to validate the presence of the claim
 * Example
 * The following code will check the jwt agains the iss value "https://example.com", the sub value "client_1", the presence of the claim aud and that the claim exp is after now and the claim `nbf` is before now:
 * if (r_jwt_validate_claims(jwt, R_JWT_CLAIM_ISS, "https://example.com", 
 *                                R_JWT_CLAIM_SUB, "client_1", 
 *                                R_JWT_CLAIM_AUD, NULL, 
 *                                R_JWT_CLAIM_EXP, R_JWT_CLAIM_NOW, 
 *                                R_JWT_CLAIM_NBF, R_JWT_CLAIM_NOW,
 *                                R_JWT_CLAIM_STR, "scope", "scope1",
 *                                R_JWT_CLAIM_INT, "age", 42,
 *                                R_JWT_CLAIM_JSN, "verified", json_true(),
 *                                R_JWT_CLAIM_NOP) == RHN_OK)
 */
int r_jwt_validate_claims(jwt_t * jwt, ...);

/**
 * Set the jwt claims with the list of claims given in parameters
 * The list must end with the claim type R_JWT_CLAIM_NOP
 * Claim types available
 * - R_JWT_CLAIM_ISS: claim "iss", values expected a string or NULL
 * - R_JWT_CLAIM_SUB: claim "sub", values expected a string or NULL
 * - R_JWT_CLAIM_AUD: claim "aud", values expected a string or NULL
 * - R_JWT_CLAIM_EXP: claim "exp", value expected R_JWT_CLAIM_NOW or an positive integer value
 * - R_JWT_CLAIM_NBF: claim "nbf", value expected R_JWT_CLAIM_NOW or an positive integer value
 * - R_JWT_CLAIM_IAT: claim "iat", value expected R_JWT_CLAIM_NOW or an positive integer value
 * - R_JWT_CLAIM_JTI: claim "jti", values expected a string or NULL
 * - R_JWT_CLAIM_STR: claim name specified, then string value or NULL
 * - R_JWT_CLAIM_INT: claim name specified, then int value
 * - R_JWT_CLAIM_JSN: claim name specified, then json_t * value or NULL
 */
int r_jwt_set_claims(jwt_t * jwt, ...);

/**
 * @}
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Internal functions
 */
int _r_json_set_str_value(json_t * j_json, const char * key, const char * str_value);

int _r_json_set_int_value(json_t * j_json, const char * key, rhn_int_t i_value);

int _r_json_set_json_t_value(json_t * j_json, const char * key, json_t * j_value);

const char * _r_json_get_str_value(json_t * j_json, const char * key);

rhn_int_t _r_json_get_int_value(json_t * j_json, const char * key);

json_t * _r_json_get_json_t_value(json_t * j_json, const char * key);

json_t * _r_json_get_full_json_t(json_t * j_json);

size_t _r_get_key_size(jwa_enc enc);

gnutls_cipher_algorithm_t _r_get_alg_from_enc(jwa_enc enc);

int _r_deflate_payload(const unsigned char * uncompressed, size_t uncompressed_len, unsigned char ** compressed, size_t * compressed_len);

int _r_inflate_payload(const unsigned char * compressed, size_t compressed_len, unsigned char ** uncompressed, size_t * uncompressed_len);

#endif

#ifdef __cplusplus
}
#endif

#endif // __RHONABWY_H_
