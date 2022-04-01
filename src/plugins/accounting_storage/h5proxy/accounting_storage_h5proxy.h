/*
  accounting_storage_h5proxy.c - account interface for H5CLUSTER.
  Copyright (C) 2022 vargaconsulting, Toronto, ON, Canada
*/

#include "src/common/slurm_xlator.h"
#include "src/common/slurm_accounting_storage.h"

const char plugin_name[] = "Accounting storage H5PROXY plugin";
const char plugin_type[] = "accounting_storage/h5proxy";
const uint32_t plugin_version = SLURM_VERSION_NUMBER;

extern "C" {
 int init ( void );
 int fini ( void );
 void * acct_storage_p_get_connection(int conn_num, uint16_t *persist_conn_flags,	bool rollback, char *cluster_name);
 int acct_storage_p_close_connection(void **db_conn);
 int acct_storage_p_commit(void *db_conn, bool commit);
 int acct_storage_p_add_users(void *db_conn, uint32_t uid, List user_list);
 int acct_storage_p_add_coord(void *db_conn, uint32_t uid, List acct_list, slurmdb_user_cond_t *user_q);
 int acct_storage_p_add_accts(void *db_conn, uint32_t uid, List acct_list);
 int acct_storage_p_add_clusters(void *db_conn, uint32_t uid, List cluster_list);
 int acct_storage_p_add_federations(void *db_conn, uint32_t uid, List federation_list);
 int acct_storage_p_add_tres(void *db_conn,uint32_t uid, List tres_list);
 int acct_storage_p_add_assocs(void *db_conn, uint32_t uid,List assoc_list);
 int acct_storage_p_add_qos(void *db_conn, uint32_t uid, List qos_list);
 int acct_storage_p_add_res(void *db_conn, uint32_t uid,List res_list);
 int acct_storage_p_add_wckeys(void *db_conn, uint32_t uid, List wckey_list);
 int acct_storage_p_add_reservation(void *db_conn, slurmdb_reservation_rec_t *resv);
 List acct_storage_p_modify_users(void *db_conn, uint32_t uid, slurmdb_user_cond_t *user_q, slurmdb_user_rec_t *user);
 List acct_storage_p_modify_accts(void *db_conn, uint32_t uid, slurmdb_account_cond_t *acct_q, slurmdb_account_rec_t *acct);
 List acct_storage_p_modify_clusters(void *db_conn, uint32_t uid,	slurmdb_cluster_cond_t *cluster_q, slurmdb_cluster_rec_t *cluster);
 List acct_storage_p_modify_assocs(void *db_conn, uint32_t uid, slurmdb_assoc_cond_t *assoc_q,slurmdb_assoc_rec_t *assoc);
 List acct_storage_p_modify_federations(void *db_conn, uint32_t uid, slurmdb_federation_cond_t *fed_cond, slurmdb_federation_rec_t *fed);
 List acct_storage_p_modify_job(void *db_conn, uint32_t uid, slurmdb_job_cond_t *job_cond, slurmdb_job_rec_t *job);
 List acct_storage_p_modify_qos(void *db_conn, uint32_t uid,	slurmdb_qos_cond_t *qos_cond, slurmdb_qos_rec_t *qos);
 List acct_storage_p_modify_res(void *db_conn, uint32_t uid,	slurmdb_res_cond_t *res_cond, slurmdb_res_rec_t *res);
 List acct_storage_p_modify_wckeys(void *db_conn, uint32_t uid, slurmdb_wckey_cond_t *wckey_cond, slurmdb_wckey_rec_t *wckey);
 int acct_storage_p_modify_reservation(void *db_conn, slurmdb_reservation_rec_t *resv);
 List acct_storage_p_remove_users(void *db_conn, uint32_t uid, slurmdb_user_cond_t *user_q);
 List acct_storage_p_remove_coord(void *db_conn, uint32_t uid, List acct_list, slurmdb_user_cond_t *user_q);
 List acct_storage_p_remove_accts(void *db_conn, uint32_t uid, slurmdb_account_cond_t *acct_q);
 List acct_storage_p_remove_clusters(void *db_conn, uint32_t uid, slurmdb_account_cond_t *cluster_q);
 List acct_storage_p_remove_assocs(void *db_conn, uint32_t uid, slurmdb_assoc_cond_t *assoc_q);
 List acct_storage_p_remove_federations(void *db_conn, uint32_t uid, slurmdb_federation_cond_t *fed_cond);
 List acct_storage_p_remove_qos(void *db_conn, uint32_t uid, slurmdb_qos_cond_t *qos_cond);
 List acct_storage_p_remove_res(void *db_conn, uint32_t uid, slurmdb_res_cond_t *res_cond);
 List acct_storage_p_remove_wckeys(void *db_conn, uint32_t uid, slurmdb_wckey_cond_t *wckey_cond);
 int acct_storage_p_remove_reservation(void *db_conn, slurmdb_reservation_rec_t *resv);
 List acct_storage_p_get_users(void *db_conn, uid_t uid, slurmdb_user_cond_t *user_q);
 List acct_storage_p_get_accts(void *db_conn, uid_t uid, slurmdb_account_cond_t *acct_q);
 List acct_storage_p_get_clusters(void *db_conn, uid_t uid, slurmdb_cluster_cond_t *cluster_cond);
 List acct_storage_p_get_federations(void *db_conn, uid_t uid, slurmdb_federation_cond_t *fed_cond);
 List acct_storage_p_get_config(void *db_conn, char *config_name);
 List acct_storage_p_get_tres(void *db_conn, uid_t uid, slurmdb_tres_cond_t *tres_cond);
 List acct_storage_p_get_assocs(void *db_conn, uid_t uid, slurmdb_assoc_cond_t *assoc_q);
 List acct_storage_p_get_events(void *db_conn, uint32_t uid, slurmdb_event_cond_t *event_cond);
 List acct_storage_p_get_problems(void *db_conn, uid_t uid, slurmdb_assoc_cond_t *assoc_q);
 List acct_storage_p_get_qos(void *db_conn, uid_t uid, slurmdb_qos_cond_t *qos_cond);
 List acct_storage_p_get_res(void *db_conn, uid_t uid, slurmdb_res_cond_t *res_cond);
 List acct_storage_p_get_wckeys(void *db_conn, uid_t uid, slurmdb_wckey_cond_t *wckey_cond);
 List acct_storage_p_get_reservations(void *db_conn, uid_t uid, slurmdb_reservation_cond_t *resv_cond);
 List acct_storage_p_get_txn(void *db_conn, uid_t uid, slurmdb_txn_cond_t *txn_cond);
 int acct_storage_p_get_usage(void *db_conn, uid_t uid, void *in, int type,time_t start, time_t end);
 int acct_storage_p_roll_usage(void *db_conn, time_t sent_start, time_t sent_end, uint16_t archive_data, List *rollup_stats_list_in);
 int acct_storage_p_fix_runaway_jobs(void *db_conn, uint32_t uid, List jobs);
 int clusteracct_storage_p_node_down(void *db_conn, node_record_t *node_ptr, time_t event_time, char *reason, uint32_t reason_uid);
 int clusteracct_storage_p_node_up(void *db_conn, node_record_t *node_ptr, time_t event_time);
 int clusteracct_storage_p_register_ctld(void *db_conn, uint16_t port);
 int clusteracct_storage_p_register_disconn_ctld(	void *db_conn, char *control_host);
 int clusteracct_storage_p_fini_ctld(void *db_conn, char *ip, uint16_t port, char *cluster_nodes);
 int clusteracct_storage_p_cluster_tres(void *db_conn, char *cluster_nodes, List tres, time_t event_time, uint16_t rpc_version);
 int jobacct_storage_p_job_start(void *db_conn, job_record_t *job);
 int jobacct_storage_p_job_complete(void *db_conn, job_record_t *job_ptr);
 int jobacct_storage_p_step_start(void *db_conn, step_record_t *step_ptr);
 int jobacct_storage_p_step_complete(void *db_conn, step_record_t *step_ptr);
 int jobacct_storage_p_suspend(void *db_conn, job_record_t *job_ptr);
 List jobacct_storage_p_get_jobs_cond(void *db_conn, uid_t uid, void *job_cond);
 int jobacct_storage_p_archive(void *db_conn, slurmdb_archive_cond_t *arch_cond);
 int jobacct_storage_p_archive_load(void *db_conn, slurmdb_archive_rec_t *arch_rec);
 int acct_storage_p_update_shares_used(void *db_conn, List shares_used);
 int acct_storage_p_flush_jobs_on_cluster(void *db_conn, time_t event_time);
 int acct_storage_p_reconfig(void *db_conn);
 int acct_storage_p_reset_lft_rgt(void *db_conn, uid_t uid, List cluster_list);
 int acct_storage_p_get_stats(void *db_conn, bool dbd);
 int acct_storage_p_clear_stats(void *db_conn, bool dbd);
 int acct_storage_p_get_data(void *db_conn, acct_storage_info_t dinfo, void *data);
 int acct_storage_p_shutdown(void *db_conn, bool dbd);
}