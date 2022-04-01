/*
  accounting_storage_h5proxy.c - account interface for H5CLUSTER.
  Copyright (C) 2022 vargaconsulting, Toronto, ON, Canada
*/

#include "src/common/slurm_xlator.h"
#include "src/common/slurm_accounting_storage.h"
#include <sys/socket.h>
#include <sys/un.h>


extern int acct_storage_p_commit(void *db_conn, bool commit){
	verbose("acct_storage_p_commit");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_add_users(void *db_conn, uint32_t uid, List user_list){
	verbose("acct_storage_p_add_users");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_add_coord(void *db_conn, uint32_t uid, List acct_list, slurmdb_user_cond_t *user_q){
	verbose("acct_storage_p_add_coord");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_add_accts(void *db_conn, uint32_t uid, List acct_list){
	verbose("acct_storage_p_add_accts");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_add_clusters(void *db_conn, uint32_t uid, List cluster_list){
	verbose("acct_storage_p_add_clusters");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_add_federations(void *db_conn, uint32_t uid, List federation_list){
	verbose("acct_storage_p_add_federations");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_add_tres(void *db_conn,uint32_t uid, List tres_list){
	verbose("acct_storage_p_add_tres");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_add_assocs(void *db_conn, uint32_t uid,List assoc_list){
	verbose("acct_storage_p_add_assocs");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_add_qos(void *db_conn, uint32_t uid, List qos_list){
	verbose("acct_storage_p_add_qos");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_add_res(void *db_conn, uint32_t uid,List res_list){
	verbose("acct_storage_p_add_res");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_add_wckeys(void *db_conn, uint32_t uid, List wckey_list){
	verbose("acct_storage_p_add_wckeys");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_add_reservation(void *db_conn, slurmdb_reservation_rec_t *resv){
	verbose("acct_storage_p_add_reservation");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_modify_users(void *db_conn, uint32_t uid, slurmdb_user_cond_t *user_q, slurmdb_user_rec_t *user){
	verbose("acct_storage_p_modify_users");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_modify_accts(void *db_conn, uint32_t uid, slurmdb_account_cond_t *acct_q, slurmdb_account_rec_t *acct){
	verbose("acct_storage_p_modify_accts");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_modify_clusters(void *db_conn, uint32_t uid,	slurmdb_cluster_cond_t *cluster_q, slurmdb_cluster_rec_t *cluster){
	verbose("acct_storage_p_modify_clusters");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_modify_assocs(void *db_conn, uint32_t uid, slurmdb_assoc_cond_t *assoc_q,slurmdb_assoc_rec_t *assoc){
	verbose("acct_storage_p_modify_assocs");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_modify_federations(void *db_conn, uint32_t uid, slurmdb_federation_cond_t *fed_cond, slurmdb_federation_rec_t *fed){
	verbose("acct_storage_p_modify_federations");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_modify_job(void *db_conn, uint32_t uid, slurmdb_job_cond_t *job_cond, slurmdb_job_rec_t *job){
	verbose("acct_storage_p_modify_job");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_modify_qos(void *db_conn, uint32_t uid,	slurmdb_qos_cond_t *qos_cond, slurmdb_qos_rec_t *qos){
	verbose("acct_storage_p_modify_qos");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_modify_res(void *db_conn, uint32_t uid,	slurmdb_res_cond_t *res_cond, slurmdb_res_rec_t *res){
	verbose("acct_storage_p_modify_res");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_modify_wckeys(void *db_conn, uint32_t uid, slurmdb_wckey_cond_t *wckey_cond, slurmdb_wckey_rec_t *wckey){
	verbose("acct_storage_p_modify_wckeys");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_modify_reservation(void *db_conn, slurmdb_reservation_rec_t *resv){
	verbose("acct_storage_p_modify_reservation");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_remove_users(void *db_conn, uint32_t uid, slurmdb_user_cond_t *user_q){
	verbose("acct_storage_p_remove_users");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_remove_coord(void *db_conn, uint32_t uid, List acct_list, slurmdb_user_cond_t *user_q){
	verbose("acct_storage_p_remove_coord");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_remove_accts(void *db_conn, uint32_t uid, slurmdb_account_cond_t *acct_q){
	verbose("acct_storage_p_remove_accts");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_remove_clusters(void *db_conn, uint32_t uid, slurmdb_account_cond_t *cluster_q){
	verbose("acct_storage_p_remove_clusters");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_remove_assocs(void *db_conn, uint32_t uid, slurmdb_assoc_cond_t *assoc_q){
	verbose("acct_storage_p_remove_assocs");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_remove_federations(void *db_conn, uint32_t uid, slurmdb_federation_cond_t *fed_cond){
	verbose("acct_storage_p_remove_federations");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_remove_qos(void *db_conn, uint32_t uid, slurmdb_qos_cond_t *qos_cond){
	verbose("acct_storage_p_remove_qos");
	return NULL;
}

extern List acct_storage_p_remove_res(void *db_conn, uint32_t uid, slurmdb_res_cond_t *res_cond){
	verbose("acct_storage_p_remove_res");
	return NULL;
}

extern List acct_storage_p_remove_wckeys(void *db_conn, uint32_t uid, slurmdb_wckey_cond_t *wckey_cond){
	verbose("acct_storage_p_remove_wckeys");
	return NULL;
}

extern int acct_storage_p_remove_reservation(void *db_conn, slurmdb_reservation_rec_t *resv){
	verbose("acct_storage_p_remove_reservation");
	return SLURM_SUCCESS;
}

extern List acct_storage_p_get_users(void *db_conn, uid_t uid, slurmdb_user_cond_t *user_q){
	verbose("acct_storage_p_get_users");
	return NULL;
}

extern List acct_storage_p_get_accts(void *db_conn, uid_t uid, slurmdb_account_cond_t *acct_q){
	verbose("acct_storage_p_get_accts");
	return NULL;
}

extern List acct_storage_p_get_clusters(void *db_conn, uid_t uid, slurmdb_cluster_cond_t *cluster_cond){
	verbose("acct_storage_p_get_clusters");
	return NULL;
}

extern List acct_storage_p_get_federations(void *db_conn, uid_t uid, slurmdb_federation_cond_t *fed_cond){
	verbose("acct_storage_p_get_federations");
	return NULL;
}

extern List acct_storage_p_get_config(void *db_conn, char *config_name){
	verbose("acct_storage_p_get_config");
	return NULL;
}

extern List acct_storage_p_get_tres(void *db_conn, uid_t uid, slurmdb_tres_cond_t *tres_cond){
	verbose("acct_storage_p_get_tres");
	return NULL;
}

extern List acct_storage_p_get_assocs(void *db_conn, uid_t uid, slurmdb_assoc_cond_t *assoc_q){
	verbose("acct_storage_p_get_assocs");
	return NULL;
}

extern List acct_storage_p_get_events(void *db_conn, uint32_t uid, slurmdb_event_cond_t *event_cond){
	verbose("acct_storage_p_get_events");
	return NULL;
}

extern List acct_storage_p_get_problems(void *db_conn, uid_t uid, slurmdb_assoc_cond_t *assoc_q){
	verbose("acct_storage_p_get_problems");
	return NULL;
}

extern List acct_storage_p_get_qos(void *db_conn, uid_t uid, slurmdb_qos_cond_t *qos_cond){
	verbose("acct_storage_p_get_qos");
	return NULL;
}

extern List acct_storage_p_get_res(void *db_conn, uid_t uid, slurmdb_res_cond_t *res_cond){
	verbose("acct_storage_p_get_res");
	return NULL;
}

extern List acct_storage_p_get_wckeys(void *db_conn, uid_t uid, slurmdb_wckey_cond_t *wckey_cond){
	verbose("acct_storage_p_get_wckeys");
	return NULL;
}

extern List acct_storage_p_get_reservations(void *db_conn, uid_t uid, slurmdb_reservation_cond_t *resv_cond){
	verbose("acct_storage_p_get_reservations");
	return NULL;
}

extern List acct_storage_p_get_txn(void *db_conn, uid_t uid, slurmdb_txn_cond_t *txn_cond){
	verbose("acct_storage_p_get_txn");
	return NULL;
}

extern int acct_storage_p_get_usage(void *db_conn, uid_t uid, void *in, int type,time_t start, time_t end){
	int rc = SLURM_SUCCESS;
	verbose("acct_storage_p_get_usage");

	return rc;
}

extern int acct_storage_p_roll_usage(void *db_conn, time_t sent_start, time_t sent_end, uint16_t archive_data, List *rollup_stats_list_in){
	int rc = SLURM_SUCCESS;
	verbose("acct_storage_p_roll_usage");

	return rc;
}

extern int acct_storage_p_fix_runaway_jobs(void *db_conn, uint32_t uid, List jobs){
	verbose("acct_storage_p_fix_runaway_jobs");
	return SLURM_SUCCESS;
}

extern int clusteracct_storage_p_register_ctld(void *db_conn, uint16_t port){
	verbose("clusteracct_storage_p_register_ctld");
	return SLURM_SUCCESS;
}

extern int clusteracct_storage_p_register_disconn_ctld(	void *db_conn, char *control_host){
	verbose("clusteracct_storage_p_register_disconn_ctld");
	return SLURM_SUCCESS;
}

extern int clusteracct_storage_p_fini_ctld(void *db_conn, char *ip, uint16_t port, char *cluster_nodes){
	verbose("clusteracct_storage_p_fini_ctld");
	return SLURM_SUCCESS;
}

extern int clusteracct_storage_p_cluster_tres(void *db_conn, char *cluster_nodes, List tres, time_t event_time, uint16_t rpc_version){
	verbose("clusteracct_storage_p_cluster_tres");
	return SLURM_SUCCESS;
}

/* load into the storage the start of a job step */
extern int jobacct_storage_p_step_start(void *db_conn, step_record_t *step_ptr){
	verbose("jobacct_storage_p_step_start");
	return SLURM_SUCCESS;
}

/* load into the storage the end of a job step */
extern int jobacct_storage_p_step_complete(void *db_conn, step_record_t *step_ptr){
	verbose("jobacct_storage_p_step_complete");
	return SLURM_SUCCESS;
}

/* get info from the storage, returns List of slurmdb_job_rec_t  note List needs to be freed when called */
extern List jobacct_storage_p_get_jobs_cond(void *db_conn, uid_t uid, void *job_cond) {
	verbose("jobacct_storage_p_get_jobs_cond");
	return NULL;
}

/* expire old info from the storage */
extern int jobacct_storage_p_archive(void *db_conn, slurmdb_archive_cond_t *arch_cond){
	verbose("jobacct_storage_p_archive");
	return SLURM_SUCCESS;
}

/* load old info into the storage */
extern int jobacct_storage_p_archive_load(void *db_conn, slurmdb_archive_rec_t *arch_rec){
	verbose("jobacct_storage_p_archive_load");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_update_shares_used(void *db_conn, List shares_used){
	verbose("acct_storage_p_update_shares_used");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_flush_jobs_on_cluster(void *db_conn, time_t event_time){
	verbose("acct_storage_p_flush_jobs_on_cluster");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_reconfig(void *db_conn){
	verbose("acct_storage_p_reconfig");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_reset_lft_rgt(void *db_conn, uid_t uid, List cluster_list){
	verbose("acct_storage_p_reset_lft_rgt");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_get_stats(void *db_conn, bool dbd){
	verbose("acct_storage_p_get_stats");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_clear_stats(void *db_conn, bool dbd){
	verbose("acct_storage_p_clear_stats");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_get_data(void *db_conn, acct_storage_info_t dinfo, void *data){
	verbose("acct_storage_p_get_data");
	return SLURM_SUCCESS;
}

extern int acct_storage_p_shutdown(void *db_conn, bool dbd){
	verbose("acct_storage_p_shutdown");
	return SLURM_SUCCESS;
}
