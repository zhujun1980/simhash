/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/sha1.h"
#include "php_simhash.h"

#define HASH_LENGTH 20

static int powers[8] = {1, 2, 4, 8, 16, 32, 64, 128};

/* If you declare any globals in php_simhash.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(simhash)
*/

/* True global resources - no need for thread safety here */
static int le_simhash;

/* {{{ simhash_functions[]
 *
 * Every user visible function must have an entry in simhash_functions[].
 */
const zend_function_entry simhash_functions[] = {
    PHP_FE(genhash,        NULL)
    PHP_FE(cmphash,        NULL)
	PHP_FE_END	/* Must be the last line in simhash_functions[] */
};
/* }}} */

/* {{{ simhash_module_entry
 */
zend_module_entry simhash_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"simhash",
	simhash_functions,
	PHP_MINIT(simhash),
	PHP_MSHUTDOWN(simhash),
	PHP_RINIT(simhash),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(simhash),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(simhash),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SIMHASH
ZEND_GET_MODULE(simhash)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("simhash.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_simhash_globals, simhash_globals)
    STD_PHP_INI_ENTRY("simhash.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_simhash_globals, simhash_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_simhash_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_simhash_init_globals(zend_simhash_globals *simhash_globals)
{
	simhash_globals->global_value = 0;
	simhash_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(simhash)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(simhash)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(simhash)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(simhash)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(simhash)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "simhash support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

PHP_FUNCTION(genhash) {
    zval *dataset, *weights;
    PHP_SHA1_CTX sha1_context;

    PHP_SHA1Init(&sha1_context);
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "az", &dataset, &weights) == FAILURE) {
        RETURN_NULL();
    }
    if(Z_TYPE_P(weights) == IS_ARRAY) {
        if(Z_ARRVAL_P(weights)->nNumOfElements != Z_ARRVAL_P(dataset)->nNumOfElements) {
            zend_error(E_WARNING, "Weights must be NULL or has same size with dataset.");
            RETURN_NULL();
        }
        zend_hash_internal_pointer_reset(Z_ARRVAL_P(weights));
    }

    unsigned char *digest;
    int i;
    double simhash[HASH_LENGTH * 8];
    memset(simhash, 0, sizeof(double) * HASH_LENGTH * 8);
    digest = emalloc(HASH_LENGTH + 1);

    HashTable *htDS = Z_ARRVAL_P(dataset);
    for(zend_hash_internal_pointer_reset(htDS);
        zend_hash_has_more_elements(htDS) == SUCCESS;
        zend_hash_move_forward(htDS)) {

        zval *pval, *pwval, titem, tweight;
        //get current weight, default 1
        if(Z_TYPE_P(weights) == IS_ARRAY) {
            pwval = zend_hash_get_current_data(Z_ARRVAL_P(weights));
            tweight = *pwval;
            zval_copy_ctor(&tweight);
            //INIT_PZVAL(&tweight);
            convert_to_double(&tweight);
            zend_hash_move_forward(Z_ARRVAL_P(weights));
        }
        else
            ZVAL_DOUBLE(&tweight, 1.0);

        //php_printf("%f\n", Z_DVAL(tweight));

        pval = zend_hash_get_current_data(htDS);
        titem = *pval;
        zval_copy_ctor(&titem);
        //INIT_PZVAL(&titem);
        convert_to_string(&titem);
        PHP_SHA1Update(&sha1_context, (unsigned char *)Z_STRVAL(titem), Z_STRLEN(titem));
        PHP_SHA1Final(digest, &sha1_context);

        for(i = 0; i < HASH_LENGTH * 8; i++) {
            int bitpos = powers[7 - (i % 8)];
            int d = digest[i / 8] & bitpos;
            if(d == bitpos)
                simhash[i] += Z_DVAL(tweight);
            else
                simhash[i] -= Z_DVAL(tweight);
        }

        zval_dtor(&titem);
        zval_dtor(&tweight);
    }
    efree(digest);

    char final[HASH_LENGTH + 1];
    for(i = 0; i < HASH_LENGTH * 8; i++) {
        unsigned int bitpos = powers[7 - (i % 8)];
        if(simhash[i] > 0)
            final[i / 8] = final[i / 8] | bitpos;
        else
            final[i / 8] = final[i / 8] & ~bitpos;
    }

    RETVAL_STRINGL(final, HASH_LENGTH);
}

PHP_FUNCTION(cmphash) {
    char *fp1, *fp2;
    zend_string *s1, *s2;
    int len1, len2;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "SS", &s1, &s2) == FAILURE) {
        RETVAL_DOUBLE(0);
    }
	len1 = ZSTR_LEN(s1);
	len2 = ZSTR_LEN(s2);
	fp1 = ZSTR_VAL(s1);
	fp2 = ZSTR_VAL(s2);

	//printf("%d - %d\n", len1, len2);
	//printf("%s - %s\n", fp1, fp2);
    if(len1 != len2) {
        zend_error(E_WARNING, "fp1's length is not equal fp2");
        RETVAL_BOOL(0);
    }

    int i, j;
    int cnt = 0;
    char simhash;
    for(i = 0; i < len1; i++) {
        simhash = fp1[i] ^ fp2[i];
        for(j = 0; j < 8; j++) {
            int bit = powers[j];
            if(bit == (simhash & bit))
                cnt++;
        }
    }

    double differ = cnt / (len1 * 8.0);
    RETVAL_DOUBLE(1 - differ);
}

/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
