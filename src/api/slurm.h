/* 
 * slurmlib.h - descriptions of slurm APIs
 * see slurm.h for documentation on external functions and data structures
 *
 * author: moe jette, jette@llnl.gov
 */

#define MAX_ID_LEN	32
#define MAX_NAME_LEN	1024 	/* gethostname in linux returns a FQ DNS name */

#include <src/common/slurm_protocol_defs.h>
#include <stdio.h>

/*
 * slurm_allocate - allocate nodes for a job with supplied contraints. 
 * input: spec - specification of the job's constraints
 *        job_id - place into which a job_id can be stored
 * output: job_id - the job's id
 *         node_list - list of allocated nodes
 *         returns 0 if no error, EINVAL if the request is invalid, 
 *			EAGAIN if the request can not be satisfied at present
 * NOTE: required specifications include: User=<uid>
 *	optional specifications include: Contiguous=<YES|NO> 
 *	Distribution=<BLOCK|CYCLE> Features=<features> Groups=<groups>
 *	JobId=<id> JobName=<name> Key=<credential> MinProcs=<count>
 *	MinRealMemory=<MB> MinTmpDisk=<MB> Partition=<part_name>
 *	Priority=<integer> ProcsPerTask=<count> ReqNodes=<node_list>
 *	Shared=<YES|NO> TimeLimit=<minutes> TotalNodes=<count>
 *	TotalProcs=<count>
 * NOTE: the calling function must free the allocated storage at node_list[0]
 */
extern int slurm_allocate_resources (job_desc_msg_t * job_desc_msg , resource_allocation_response_msg_t ** job_alloc_resp_msg, int immediate ) ;

/*
 * slurm_cancel - cancel the specified job 
 * input: job_id - the job_id to be cancelled
 * output: returns 0 if no error, EINVAL if the request is invalid, 
 *			EAGAIN if the request can not be satisfied at present
 */
extern int slurm_cancel_job (uint32_t job_id);


/***************************
 * slurm_ctl_conf.c
 ***************************/

/*
 * slurm_free_slurm_ctl_conf - free the build information buffer (if allocated)
 * NOTE: buffer is loaded by slurm_load_ctl_conf.
 */
extern void slurm_free_ctl_conf ( slurm_ctl_conf_t* slurm_ctl_conf_ptr);
/*
 * slurm_print_slurm_ctl_conf - prints the build information buffer (if allocated)
 * NOTE: buffer is loaded by slurm_load_slurm_ctl_conf.
 */
extern void slurm_print_ctl_conf ( FILE * out, slurm_ctl_conf_t* slurm_ctl_conf ) ;

/*
 * slurm_free_job_info - free the job information buffer (if allocated)
 * NOTE: buffer is loaded by slurm_load_job.
 */
extern void slurm_free_job_info (job_info_msg_t * job_buffer_ptr);

/*
 * slurm_free_node_info - free the node information buffer (if allocated)
 * NOTE: buffer is loaded by slurm_load_node.
 */
extern void slurm_free_node_info (node_info_msg_t * node_buffer_ptr);

/* 
 * slurm_print_job_info_msg - prints the job information buffer (if allocated)
 * NOTE: buffer is loaded by slurm_load_job_info .
 */
extern void slurm_print_job_info_msg ( FILE* , job_info_msg_t * job_info_msg_ptr ) ;

/* slurm_print_job_table - prints the job table object (if allocated) */
extern void slurm_print_job_table ( FILE*, job_table_t * job_ptr );

/* 
 * slurm_print_node_info_msg - prints the node information buffer (if allocated)
 * NOTE: buffer is loaded by slurm_load_node_info .
 */
extern void slurm_print_node_info_msg ( FILE*, node_info_msg_t * node_info_msg_ptr ) ;

/* slurm_print_node_table - prints the node table object (if allocated) */
extern void slurm_print_node_table ( FILE*, node_table_t * node_ptr );

/*
 * slurm_free_part_info - free the partition information buffer (if allocated)
 * NOTE: buffer is loaded by load_part.
 */
extern void slurm_free_partition_info ( partition_info_msg_t * part_info_ptr);
extern void slurm_print_partition_info ( FILE*, partition_info_msg_t * part_info_ptr ) ;
extern void slurm_print_partition_table ( FILE*, partition_table_t * part_ptr ) ;

/*
 * slurm_load_ctl_conf - load the slurm build information buffer for use by info 
 *	gathering APIs if build info has changed since the time specified. 
 * input: update_time - time of last update
 *	build_buffer_ptr - place to park build_buffer pointer
 * output: build_buffer_ptr - pointer to allocated build_buffer
 *	returns -1 if no update since update_time, 
 *		0 if update with no error, 
 *		EINVAL if the buffer (version or otherwise) is invalid, 
 *		ENOMEM if malloc failure
 * NOTE: the allocated memory at build_buffer_ptr freed by slurm_free_node_info.
 */
extern int slurm_load_ctl_conf (time_t update_time, 
	slurm_ctl_conf_t  **slurm_ctl_conf_ptr);


/* slurm_load_job - load the supplied job information buffer if changed */
extern int slurm_load_jobs (time_t update_time, job_info_msg_t **job_info_msg_pptr);

/* slurm_load_node - load the supplied node information buffer if changed */
extern int slurm_load_node (time_t update_time, node_info_msg_t **node_info_msg_pptr);

/*
 * slurm_load_part - load the supplied partition information buffer for use by info 
 *	gathering APIs if partition records have changed since the time specified. 
 * input: update_time - time of last update
 *	part_buffer_ptr - place to park part_buffer pointer
 * output: part_buffer_ptr - pointer to allocated part_buffer
 *	returns -1 if no update since update_time, 
 *		0 if update with no error, 
 *		EINVAL if the buffer (version or otherwise) is invalid, 
 *		ENOMEM if malloc failure
 * NOTE: the allocated memory at part_buffer_ptr freed by slurm_free_part_info.
 */
extern int slurm_load_partitions (time_t update_time, partition_info_msg_t **part_buffer_ptr);

/*
 * slurm_submit - submit/queue a job with supplied contraints. 
 * input: spec - specification of the job's constraints
 *	job_id - place to store id of submitted job
 * output: job_id - the job's id
 *	returns 0 if no error, EINVAL if the request is invalid
 * NOTE: required specification include: Script=<script_path_name>
 *	User=<uid>
 * NOTE: optional specifications include: Contiguous=<YES|NO> 
 *	Distribution=<BLOCK|CYCLE> Features=<features> Groups=<groups>
 *	JobId=<id> JobName=<name> Key=<key> MinProcs=<count> 
 *	MinRealMemory=<MB> MinTmpDisk=<MB> Partition=<part_name>
 *	Priority=<integer> ProcsPerTask=<count> ReqNodes=<node_list>
 *	Shared=<YES|NO> TimeLimit=<minutes> TotalNodes=<count>
 *	TotalProcs=<count> Immediate=<YES|NO>
 */
extern int slurm_submit_batch_job (job_desc_msg_t * job_desc_msg );

/*
 * slurm_will_run - determine if a job would execute immediately 
 *	if submitted. 
 * input: spec - specification of the job's constraints
 * output: returns 0 if job would run now, EINVAL if the request 
 *		would never run, EAGAIN if job would run later
 * NOTE: required specification include: User=<uid>
 * NOTE: optional specifications include: Contiguous=<YES|NO> 
 *	Features=<features> Groups=<groups>
 *	Key=<key> MinProcs=<count> 
 *	MinRealMemory=<MB> MinTmpDisk=<MB> Partition=<part_name>
 *	Priority=<integer> ReqNodes=<node_list>
 *	Shared=<YES|NO> TimeLimit=<minutes> TotalNodes=<count>
 *	TotalProcs=<count>
 */
extern int slurm_job_will_run (job_desc_msg_t * job_desc_msg , resource_allocation_response_msg_t ** job_alloc_resp_msg );

/* 
 * parse_node_name - parse the node name for regular expressions and return a sprintf format 
 * generate multiple node names as needed.
 * input: node_name - node name to parse
 * output: format - sprintf format for generating names
 *         start_inx - first index to used
 *         end_inx - last index value to use
 *         count_inx - number of index values to use (will be zero if none)
 *         return 0 if no error, error code otherwise
 * NOTE: the calling program must execute free(format) when the storage location is no longer needed
 */
extern int parse_node_name (char *node_name, char **format, int *start_inx,
			    int *end_inx, int *count_inx);

/* 
 * reconfigure - _ request that slurmctld re-read the configuration files
 * output: returns 0 on success, errno otherwise
 */
extern int slurm_reconfigure ();

/*
 * slurm_update_node - updates the node state
 */
int slurm_update_node ( update_node_msg_t * node_msg )  ;
int slurm_update_partition ( update_part_msg_t * desc_msg ) ;
