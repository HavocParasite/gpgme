/* assuan-connect.c - Establish a connection (client) 
 *	Copyright (C) 2001, 2002 Free Software Foundation, Inc.
 *
 * This file is part of Assuan.
 *
 * Assuan is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * Assuan is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA 
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "assuan-defs.h"

/* Disconnect and release the context CTX. */
void
assuan_disconnect (ASSUAN_CONTEXT ctx)
{
  if (ctx)
    {
#if 0
      /* This may not work if the pipe is full and the other end is
	 blocked.  */
      assuan_write_line (ctx, "BYE");
#endif
      ctx->finish_handler (ctx);
      ctx->deinit_handler (ctx);
      ctx->deinit_handler = NULL;
      _assuan_release_context (ctx);
    }
}

pid_t
assuan_get_pid (ASSUAN_CONTEXT ctx)
{
  return ctx ? ctx->pid : -1;
}
