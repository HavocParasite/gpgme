/* ops.h - Internal operation support.
   Copyright (C) 2000 Werner Koch (dd9jn)
   Copyright (C) 2001, 2002, 2003 g10 Code GmbH
 
   This file is part of GPGME.
 
   GPGME is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 
   GPGME is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with GPGME; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef OPS_H
#define OPS_H

#include "gpgme.h"
#include "context.h"


/* From gpgme.c.  */
void _gpgme_release_result (GpgmeCtx ctx);


/* From wait.c.  */
GpgmeError _gpgme_wait_one (GpgmeCtx ctx);
GpgmeError _gpgme_wait_on_condition (GpgmeCtx ctx, volatile int *cond);

/* From recipient.c.  */
int _gpgme_recipients_all_valid ( const GpgmeRecipients rset );


/* From data.c.  */
GpgmeError _gpgme_data_inbound_handler (void *opaque, int fd);
GpgmeError _gpgme_data_outbound_handler (void *opaque, int fd);


/* From op-support.c.  */

/* Find or create the op data object of type TYPE.  */
GpgmeError _gpgme_op_data_lookup (GpgmeCtx ctx, ctx_op_data_type type,
				  void **hook, int size,
				  void (*cleanup) (void *));

/* Prepare a new operation on CTX.  */
GpgmeError _gpgme_op_reset (GpgmeCtx ctx, int synchronous);

/* Parse the invalid user ID status line in ARGS and return the result
   in USERID.  */
GpgmeError _gpgme_parse_inv_userid (char *args, GpgmeInvalidUserID *userid);


/* From verify.c.  */
GpgmeError _gpgme_op_verify_init_result (GpgmeCtx ctx);
GpgmeError _gpgme_verify_status_handler (void *priv, GpgmeStatusCode code,
					 char *args);


/* From decrypt.c.  */
GpgmeError _gpgme_op_decrypt_init_result (GpgmeCtx ctx);
GpgmeError _gpgme_decrypt_status_handler (void *priv, GpgmeStatusCode code,
					  char *args);


/* From sign.c.  */

/* Create an initial op data object for signing.  Needs to be called
   once before calling _gpgme_sign_status_handler.  */
GpgmeError _gpgme_op_sign_init_result (GpgmeCtx ctx);

/* Process a status line for signing operations.  */
GpgmeError _gpgme_sign_status_handler (void *priv, GpgmeStatusCode code,
				       char *args);


/* From encrypt.c.  */

/* Create an initial op data object for encrypt.  Needs to be called
   once before calling _gpgme_encrypt_status_handler.  */
GpgmeError _gpgme_op_encrypt_init_result (GpgmeCtx ctx);

/* Process a status line for encryption operations.  */
GpgmeError _gpgme_encrypt_status_handler (void *priv, GpgmeStatusCode code,
					  char *args);


/* From passphrase.c.  */
GpgmeError _gpgme_passphrase_status_handler (void *priv, GpgmeStatusCode code,
					     char *args);
GpgmeError _gpgme_passphrase_command_handler (void *opaque,
					      GpgmeStatusCode code,
					      const char *key, const char **result);


/* From progress.c.  */
GpgmeError _gpgme_progress_status_handler (void *priv, GpgmeStatusCode code,
					   char *args);


/* From key.c.  */
GpgmeError _gpgme_key_new (GpgmeKey *r_key);
GpgmeError _gpgme_key_add_subkey (GpgmeKey key, GpgmeSubkey *r_subkey);
GpgmeError _gpgme_key_append_name (GpgmeKey key, char *src);
GpgmeKeySig _gpgme_key_add_sig (GpgmeKey key, char *src);


/* From keylist.c.  */
void _gpgme_op_keylist_event_cb (void *data, GpgmeEventIO type, void *type_data);


/* From trust-item.c.  */

/* Create a new trust item.  */
GpgmeError _gpgme_trust_item_new (GpgmeTrustItem *r_item);


/* From trustlist.c.  */
void _gpgme_op_trustlist_event_cb (void *data, GpgmeEventIO type, void *type_data);


/*-- version.c --*/
const char *_gpgme_compare_versions (const char *my_version,
				     const char *req_version);
char *_gpgme_get_program_version (const char *const path);


#endif /* OPS_H */
