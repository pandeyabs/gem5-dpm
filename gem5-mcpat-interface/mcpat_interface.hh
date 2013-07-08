#ifndef MCPAT_INTERFACE_H
#define MCPAT_INTERFACE_H

// forward definition
class mcpat_stats;

double mcpat_interface(bool init, bool destruct, int plevel, mcpat_stats *rt, char *name, int core_id=-1);

void parse_mcpat_stats(mcpat_stats *rt);

class mcpat_stats
{
	public:
		// global stats/params
		int		num_cores;
		double	total_cycles;
		// per-core stats
		// general core stats	
		double	*clockrate;
		double	*total_insts;
		double	*int_insts;
		double	*fp_insts;
		double	*br_insts;
		double	*br_mispredict;
		double	*ld_insts;
		double	*st_insts;
		double	*commit_insts;
		double	*commit_int_insts;
		double	*commit_fp_insts;
		double	*pipeline_duty_cycle;	// runtime_ipc / peak_ipc
		double	*core_total_cycles;
		// ROB
		double	*ROB_reads;
		double	*ROB_writes;
		// RAT
		double	*rename_reads;
		double	*rename_writes;
		double	*fp_rename_reads;
		double	*fp_rename_writes;
		// Inst Window stats
		double	*inst_window_reads;
		double	*inst_window_writes;
		double	*inst_window_wakeup_accesses;
		double	*fp_inst_window_reads;
		double	*fp_inst_window_writes;
		double	*fp_inst_window_wakeup_accesses;	
		// Register File
		double	*int_rf_reads;
		double	*fp_rf_reads;
		double	*int_rf_writes;
		double	*fp_rf_writes;
		// Accesses to Working Reg
		double	*func_calls;
		double	*context_switches;
		// Functional Unit
		double	*ialu_accesses;
		double	*fpu_accesses;
		double	*mul_accesses;
		// I-TLB
		double	*itlb_accesses;
		double	*itlb_misses;
		// I-Cache
		double	*icache_accesses;
		double	*icache_misses;
		// D-TLB
		double	*dtlb_accesses;
		double	*dtlb_misses;
		// D-Cache
		double	*dcache_accesses;
		double	*dcache_misses;
		// BTB
		double	*BTB_reads;
		double	*BTB_writes;
		// end of per-core stats
		// L1Directory
		double	l1dir_reads;
		double	l1dir_writes;
		double	l1dir_read_misses;
		double	l1dir_write_misses;
		// L2Directory
		double	l2dir_reads;
		double	l2dir_writes;
		double	l2dir_read_misses;
		double	l2dir_write_misses;
		// L2 Cahce
		double	l2_reads;
		double	l2_writes;
		double	l2_read_misses;
		double	l2_write_misses;
		double	l2_duty_cycle;
	        int  cpu_idle_cycles;
	        int  fuBusy;

		// constructor
		mcpat_stats(int num_thread_contexts);
		// destructor
		~mcpat_stats();
};

#endif
