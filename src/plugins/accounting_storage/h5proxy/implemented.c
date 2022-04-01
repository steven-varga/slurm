/*
  accounting_storage_h5proxy.c - account interface for H5CLUSTER.
  Copyright (C) 2022 vargaconsulting, Toronto, ON, Canada
*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result" 

#include "src/common/slurm_xlator.h"
#include "src/common/slurm_accounting_storage.h"
#include "src/common/slurm_protocol_defs.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>

const char plugin_name[] = "Accounting storage H5PROXY plugin";
const char plugin_type[] = "accounting_storage/h5proxy";
const uint32_t plugin_version = SLURM_VERSION_NUMBER;

/*
acct_storage_p_flush_jobs_on_cluster
clusteracct_storage_p_node_down
acct_storage_p_get_connection
clusteracct_storage_p_register_ctld
acct_storage_p_get_qos
acct_storage_p_get_users
acct_storage_p_get_res
clusteracct_storage_p_node_up
clusteracct_storage_p_cluster_tres
acct_storage_p_get_connection: 
shutting down Accounting storage H5PROXY plugin

on submit:

slurmctld: error: You can not have AccountingStorageEnforce set for AccountingStorageType='accounting_storage/h5proxy'
slurmctld: accounting_storage/h5proxy: acct_storage_p_flush_jobs_on_cluster: acct_storage_p_flush_jobs_on_cluster
slurmctld: accounting_storage/h5proxy: clusteracct_storage_p_node_down: clusteracct_storage_p_node_down
slurmctld: accounting_storage/h5proxy: acct_storage_p_get_connection: !!! acct_storage_p_get_connection !!!
slurmctld: accounting_storage/h5proxy: clusteracct_storage_p_register_ctld: clusteracct_storage_p_register_ctld
slurmctld: accounting_storage/h5proxy: acct_storage_p_get_qos: acct_storage_p_get_qos
slurmctld: accounting_storage/h5proxy: acct_storage_p_get_users: acct_storage_p_get_users
slurmctld: accounting_storage/h5proxy: acct_storage_p_get_res: acct_storage_p_get_res
slurmctld: accounting_storage/h5proxy: clusteracct_storage_p_cluster_tres: clusteracct_storage_p_cluster_tres
slurmctld: accounting_storage/h5proxy: clusteracct_storage_p_node_up: clusteracct_storage_p_node_up
slurmctld: accounting_storage/h5proxy: acct_storage_p_get_users: acct_storage_p_get_users
slurmctld: accounting_storage/h5proxy: acct_storage_p_get_users: acct_storage_p_get_users
slurmctld: debug3:    work_dir=/home/steven/projects/slurm/src/plugins/accounting_storage alloc_node:sid=io:648134
slurmctld: accounting_storage/h5proxy: jobacct_storage_p_job_start: !!!>> jobacct_storage_p_job_start <<!!!
slurmctld: accounting_storage/h5proxy: jobacct_storage_p_job_start: !!!>> jobacct_storage_p_job_start <<!!!
slurmctld: accounting_storage/h5proxy: jobacct_storage_p_job_start: !!!>> jobacct_storage_p_job_start <<!!!
slurmctld: accounting_storage/h5proxy: jobacct_storage_p_job_start: !!!>> jobacct_storage_p_job_start <<!!!
slurmctld: accounting_storage/h5proxy: jobacct_storage_p_step_start: jobacct_storage_p_step_start
slurmctld: accounting_storage/h5proxy: jobacct_storage_p_step_complete: jobacct_storage_p_step_complete
slurmctld: accounting_storage/h5proxy: jobacct_storage_p_job_start: !!!>> jobacct_storage_p_job_start <<!!!
slurmctld: accounting_storage/h5proxy: jobacct_storage_p_job_complete: jobacct_storage_p_job_complete
slurmctld: accounting_storage/h5proxy: acct_storage_p_close_connection: acct_storage_p_close_connection
slurmctld: accounting_storage/h5proxy: fini: shutting down Accounting storage H5PROXY plugin
*/


//must be less than PIPE_BUF or atomicity is not guaranteed: 
#define h5proxy_buffer_len  1024
//static_assert(h5proxy_buffer_len < PIPE_BUF,"");
const char * h5proxy_socket_path = "/tmp/h5proxy.sock";
typedef struct sockaddr_un socket_t;
typedef struct sockaddr sockaddr_t;

/*
enum job_states {
	JOB_PENDING,		 queued waiting for initiation
	JOB_RUNNING,		 allocated resources and executing
	JOB_SUSPENDED,		 allocated resources, execution suspended
	JOB_COMPLETE,		 completed execution successfully
	JOB_CANCELLED,		 cancelled by user
	JOB_FAILED,		     completed execution unsuccessfully
	JOB_TIMEOUT,		 terminated on reaching time limit
	JOB_NODE_FAIL,		 terminated on node failure
	JOB_PREEMPTED,		 terminated due to preemption
	JOB_BOOT_FAIL,		 terminated due to node boot failure
	JOB_DEADLINE,		 terminated on deadline
	JOB_OOM,		     experienced out of memory error
	JOB_END			     not a real state, last entry in table
};
*/
const char * job_state_map[] = {
	"pending", "running", "suspended","complete", "cancelled","failed","timeout","node-fail","preempted","boot-fail","deadline","OOM", 0
};
const char* get_state(job_record_t * job){
	// have to filter the state
	return job_state_map[job->job_state & JOB_STATE_BASE];
} 

typedef struct {
	char *cluster_name;
	char *buffer;
	int length;
	int fd;
	int id;
	socket_t local;
	socket_t remote;
	pthread_mutex_t lock;
	bool rollback;
} h5proxy_conn_t;


ssize_t send_message(h5proxy_conn_t *conn){
	return sendto(
		conn->fd, conn->buffer, strlen(conn->buffer) + 1, 0, (struct sockaddr *) &conn->remote,  sizeof(struct sockaddr_un));
}

int proxy_reconnect(h5proxy_conn_t *conn){
	if(conn->fd <= 0)
		close(conn->fd);

	return SLURM_SUCCESS;
}

int proxy_sendf(h5proxy_conn_t *conn, const char *fmt, ...){
	slurm_mutex_lock(&conn->lock);
	bzero(conn->buffer, conn->length);

	va_list va; // start: unpack variable arguements into buffer
	va_start(va, fmt);
	vsnprintf(conn->buffer, conn->length, fmt, va);
	va_end(va); // end: unpacking, we have formatted string in buffer
	// actual data xchange
	int try_n =0;
retry:
	printf("sending: %s", conn->buffer);
	if( send(conn->fd, conn->buffer, strlen(conn->buffer) + 1, 0) < 0){
		if( !connect(conn->fd, (sockaddr_t*) &conn->remote, sizeof(socket_t)) )
			goto retry;
		usleep(1000);
		if (++try_n > 100){
			slurm_mutex_unlock(&conn->lock);
			return SLURM_ERROR;
		} 
	}
	slurm_mutex_unlock(&conn->lock);
	return SLURM_SUCCESS;
}

/*
 * init() is called when the plugin is loaded, before any other functions
 * are called.  Put global initialization here.
 */
extern int init ( void ){
	verbose(" %s loaded", plugin_name);
	return SLURM_SUCCESS;
}

extern int fini ( void ){
	verbose("shutting down %s", plugin_name);
	return SLURM_SUCCESS;
}

extern void * acct_storage_p_get_connection(int conn_num, uint16_t *persist_conn_flags,	bool rollback, char *cluster_name) {
	h5proxy_conn_t* conn = xmalloc(sizeof(h5proxy_conn_t));
	
	conn->cluster_name = xstrdup("cluster_name");
	conn->buffer = xmalloc(h5proxy_buffer_len);
	conn->length = h5proxy_buffer_len;
	conn->rollback = rollback;
	conn->id = conn_num;
	slurm_mutex_init(&conn->lock);
	errno = SLURM_SUCCESS;
	conn->fd = socket (AF_UNIX, SOCK_DGRAM, 0);
	memset(&conn->local, 0, sizeof(socket_t));
	memset(&conn->remote, 0, sizeof(socket_t));
	
	conn->local.sun_family = AF_UNIX;
    bind(conn->fd, (struct sockaddr *) &conn->local, sizeof(sa_family_t)); // auto get name
    conn->remote.sun_family = AF_UNIX;
    strncpy(conn->remote.sun_path, h5proxy_socket_path, sizeof(conn->remote.sun_path) - 1);
	int try_n = 0;
retry:
	if( connect(conn->fd, (sockaddr_t*) &conn->remote, sizeof(socket_t)) ){
		if( ++try_n  > 100) goto skip;
		usleep(1000); 
		goto retry;
	}
skip:
	proxy_sendf(conn, "slurm_db_init:%s:%i", cluster_name, conn_num);
	return conn;
}

extern int acct_storage_p_close_connection(h5proxy_conn_t **conn_){
	if (!conn_ || !(*conn_))
		return SLURM_SUCCESS;
	h5proxy_conn_t* conn = *conn_;
	proxy_sendf(conn, "slurm_db_close");
	slurm_mutex_destroy(&conn->lock);
	xfree(conn->buffer); 
	xfree(conn->cluster_name);
	close(conn->fd);
	xfree(conn);
	conn = NULL;
	verbose("closing domain socket %s", h5proxy_socket_path);
	return SLURM_SUCCESS;
}

// job->partition, job->name, job->node_addr, job->end_time, job->start_time, job->job_state, job->job_id, job->nodes, job->comment
// job->group_id, job->user_id, job->user_name
/* load into the storage the start of a job */
extern int jobacct_storage_p_job_start(h5proxy_conn_t *conn, job_record_t *job){
	if(!job->nodes)
		return SLURM_ERROR;
	return proxy_sendf(conn, 
		"slurm_job_start:%d:%s:%s:%d:%s:%s:%s", job->job_id, job->name, job->comment, job->user_id, job->partition, job->nodes, get_state(job));
}

/* load into the storage the end of a job */
extern int jobacct_storage_p_job_complete(h5proxy_conn_t *conn, job_record_t *job){
	return proxy_sendf(conn, 
		"slurm_job_complete:%d:%s", job->job_id, get_state(job));
}

/* load into the storage a suspension of a job */
extern int jobacct_storage_p_suspend(h5proxy_conn_t *conn, job_record_t *job){
	return proxy_sendf(conn, 
		"slurm_job_suspend:%d:%s", job->job_id, get_state(job));
}

extern int clusteracct_storage_p_node_up(h5proxy_conn_t *conn, node_record_t *node, time_t event_time){
	return proxy_sendf(conn, 
		"slurm_node_up:%s:%d:%d:%d:%ld:%ld:%d", node->node_hostname, node->cores, node->threads, node->cpu_load, node->real_memory, node->free_mem,  node->up_time);
}

extern int clusteracct_storage_p_node_down(h5proxy_conn_t *conn, node_record_t *node, time_t event_time, char *reason, uint32_t reason_uid){
	if( IS_NODE_DOWN(node) )
		return proxy_sendf(conn,"slurm_node_down:%s:%s", node->node_hostname, reason);
	else if(IS_NODE_DRAIN(node))
		return proxy_sendf(conn,"slurm_node_drain:%s:%s", node->node_hostname, reason);
	else if(IS_NODE_DRAINED(node))
		return proxy_sendf(conn,"slurm_node_drained:%s:%s", node->node_hostname, reason);
	else if(IS_NODE_DRAINING(node))
		return proxy_sendf(conn,"slurm_node_draining:%s:%s", node->node_hostname, reason);
	else if(IS_NODE_FAIL(node))
		return proxy_sendf(conn,"slurm_node_fail:%s:%s", node->node_hostname, reason);		
	return 0;
}

#pragma GCC diagnostic pop 