/* wait.c 
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

#if HAVE_CONFIG_H
#include <config.h>
#endif
#include <assert.h>

#include "gpgme.h"
#include "context.h"
#include "io.h"
#include "wait.h"


/* The user event loops are used for all asynchronous operations for
   which a user callback is defined.  */


/* Internal I/O Callbacks.  */

GpgmeError
_gpgme_user_io_cb_handler (void *data, int fd)
{
  GpgmeError err;
  struct tag *tag = (struct tag *) data;
  GpgmeCtx ctx;
  struct wait_item_s *item;

  assert (data);
  ctx = tag->ctx;
  assert (ctx);
  item = (struct wait_item_s *) ctx->fdt.fds[tag->idx].opaque;
  assert (item);

  err = (*item->handler) (item->handler_value, fd);
  if (err)
    {
      int idx;

      for (idx = 0; idx < ctx->fdt.size; idx++)
	if (ctx->fdt.fds[idx].fd != -1)
	  _gpgme_io_close (ctx->fdt.fds[idx].fd);
      _gpgme_engine_io_event (ctx->engine, GPGME_EVENT_DONE, &err);
    }
  else
    {
      int i;

      for (i = 0; i < ctx->fdt.size; i++)
	if (ctx->fdt.fds[i].fd != -1)
	  break;
      if (i == ctx->fdt.size)
	_gpgme_engine_io_event (ctx->engine, GPGME_EVENT_DONE, &err);
    }
  return 0;
}


/* Register the file descriptor FD with the handler FNC (which gets
   FNC_DATA as its first argument) for the direction DIR.  DATA should
   be the context for which the fd is added.  R_TAG will hold the tag
   that can be used to remove the fd.  */
GpgmeError
_gpgme_wait_user_add_io_cb (void *data, int fd, int dir, GpgmeIOCb fnc,
			    void *fnc_data, void **r_tag)
{
  GpgmeCtx ctx = (GpgmeCtx) data;
  struct tag *tag;
  GpgmeError err;

  assert (ctx);
  err = _gpgme_add_io_cb (data, fd, dir, fnc, fnc_data, r_tag);
  if (err)
    return err;
  tag = *r_tag;
  assert (tag);
  err = (*ctx->io_cbs.add) (ctx->io_cbs.add_priv, fd, dir,
			    _gpgme_user_io_cb_handler, *r_tag,
			    &tag->user_tag);
  if (err)
    _gpgme_remove_io_cb (*r_tag);
  return err;
}


void
_gpgme_wait_user_remove_io_cb (void *data)
{
  struct tag *tag = (struct tag *) data;
  GpgmeCtx ctx;

  assert (tag);
  ctx = tag->ctx;

  (*ctx->io_cbs.remove) (tag->user_tag);
  _gpgme_remove_io_cb (data);
}


void
_gpgme_wait_user_event_cb (void *data, GpgmeEventIO type, void *type_data)
{
  GpgmeCtx ctx = data;

  if (ctx->io_cbs.event)
    (*ctx->io_cbs.event) (ctx->io_cbs.event_priv, type, type_data);
}
