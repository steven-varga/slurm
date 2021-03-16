/*****************************************************************************\
 *  pmixp_conf.c - parse pmix.conf configuration file.
 *****************************************************************************
 *  Copyright (C) 2019-2021 Regents of the University of California
 *  Produced at Lawrence Berkeley National Laboratory
 *  Written by Aditi Gaur <agaur@lbl.gov>
 *  All rights reserved.
 *
 *  This file is part of SLURM, a resource management program.
 *  For details, see <https://slurm.schedmd.com/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  SLURM is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  SLURM is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with SLURM; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/
#include <unistd.h>
#include <sys/stat.h>

#include "slurm/slurm_errno.h"

#include "pmixp_common.h"
#include "pmixp_conf.h"

#include "src/common/xstring.h"
#include "src/common/log.h"
#include "src/common/parse_config.h"
#include "src/common/read_config.h"
#include "src/common/xmalloc.h"

static slurm_pmix_conf_t slurm_pmix_conf;
static bool slurm_pmix_conf_inited = false;

static s_p_options_t pmix_options[] = {
	{"PMIxTimeout", S_P_UINT32},
	{"PMIxDebug", S_P_UINT32},
	{"PMIxDirectSameArch", S_P_BOOLEAN},
	{"PMIxDirectConn", S_P_BOOLEAN},
	{"PMIxDirectConnEarly", S_P_BOOLEAN},
	{"PMIxDirectConnUCX", S_P_BOOLEAN},
	{"PMIxFenceBarrier", S_P_BOOLEAN},
	{"PMIxCliTmpDirBase", S_P_STRING},
	{"PMIxCollFence", S_P_STRING},
	{"PMIxNetDevicesUCX", S_P_STRING},
	{"PMIxTlsUCX", S_P_STRING},
	{"PMIxEnv", S_P_STRING},
	{NULL}
};

static void _setup_config(s_p_hashtbl_t *tbl)
{
	if (tbl) {
		if (!s_p_get_uint32(&slurm_pmix_conf.timeout,
				    "PMIxTimeout", tbl))
			slurm_pmix_conf.timeout = PMIXP_TIMEOUT_DEFAULT;

		s_p_get_uint32(&slurm_pmix_conf.debug, "PMIxDebug", tbl);
		s_p_get_boolean(&slurm_pmix_conf.direct_samearch,
				"PMIxDirectSameArch", tbl);
		s_p_get_boolean(&slurm_pmix_conf.direct_conn,
				"PMIxDirectConn", tbl);
		s_p_get_boolean(&slurm_pmix_conf.direct_conn_early,
				"PMIxDirectConnEarly", tbl);
		s_p_get_boolean(&slurm_pmix_conf.direct_conn_ucx,
				"PMIxDirectConnUCX", tbl);
		s_p_get_boolean(&slurm_pmix_conf.fence_barrier,
				"PMIxFenceBarrier", tbl);
		s_p_get_string(&slurm_pmix_conf.cli_tmpdir_base,
			       "PMIxCliTmpDirBase", tbl);
		s_p_get_string(&slurm_pmix_conf.coll_fence,
			       "PMIxCollFence", tbl);
		s_p_get_string(&slurm_pmix_conf.ucx_netdevices,
			       "PMIxNetDevicesUCX", tbl);
		s_p_get_string(&slurm_pmix_conf.ucx_tls, "PMIxTlsUCX", tbl);
		s_p_get_string(&slurm_pmix_conf.env, "PMIxEnv", tbl);
	}
}

static void _print_pmix_config()
{
	debug2("PMIx Configuration file pmix.conf");
	debug2("Timeout = %d", slurm_pmix_conf.timeout);
	debug2("DebugLevel = %d", slurm_pmix_conf.debug);
	debug2("DirectSameArch = %s",
	       slurm_pmix_conf.direct_samearch ? "true" : "false");
	debug2("DirectConn = %s",
	       slurm_pmix_conf.direct_conn ? "true" : "false");
	debug2("DirectConnEarly = %s",
	       slurm_pmix_conf.direct_conn_early ? "true" : "false");
	debug2("DirectConnUCX = %s",
	       slurm_pmix_conf.direct_conn_ucx ? "true" : "false");
	debug2("FenceBarrier = %s",
	       slurm_pmix_conf.fence_barrier ? "true" : "false");
	debug2("CliTmpDirBase = %s", slurm_pmix_conf.cli_tmpdir_base);
	debug2("CollFence = %s", slurm_pmix_conf.coll_fence);
	debug2("NetDevicesUCX = %s", slurm_pmix_conf.ucx_netdevices);
	debug2("TlsUCX = %s", slurm_pmix_conf.ucx_tls);
	debug2("PMIxEnv = %s", slurm_pmix_conf.env);
}

static int _read_slurm_pmix_conf(void)
{
	char *conf_path = NULL;
	s_p_hashtbl_t *tbl = NULL;
	struct stat buf;
	int rc = SLURM_SUCCESS;

	conf_path = get_extra_conf_path("pmix.conf");

	if ((!conf_path) || (stat(conf_path, &buf) == -1)) {
		debug2("No pmix.conf file (%s)", conf_path);
		goto end_it;
	}

	debug2("Reading pmix.conf file %s", conf_path);
	tbl = s_p_hashtbl_create(pmix_options);
	if (s_p_parse_file(tbl, NULL, conf_path, false) == SLURM_ERROR)
	{
		fatal("Could not open/read/parse pmix.conf file %s",
		      conf_path);
		goto end_it;
	}

	_setup_config(tbl);
	_print_pmix_config();

end_it:

	s_p_hashtbl_destroy(tbl);
	xfree(conf_path);

	return rc;
}

extern slurm_pmix_conf_t *get_slurm_pmix_conf(void)
{
	int rc;
	if (!slurm_pmix_conf_inited) {
		memset(&slurm_pmix_conf, 0, sizeof(slurm_pmix_conf_t));
		rc = _read_slurm_pmix_conf();
		if (rc == SLURM_ERROR)
			return NULL;
		slurm_pmix_conf_inited = true;
	}

	return &slurm_pmix_conf;
}

extern void free_pmix_conf(void)
{
	if (slurm_pmix_conf_inited) {
		xfree(slurm_pmix_conf.cli_tmpdir_base);
		xfree(slurm_pmix_conf.coll_fence);
		xfree(slurm_pmix_conf.ucx_netdevices);
		xfree(slurm_pmix_conf.ucx_tls);
		xfree(slurm_pmix_conf.env);
	}
	return;
}
