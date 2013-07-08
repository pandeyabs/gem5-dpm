#include <iostream>
#include "xmlParser.h"
#include "XML_Parse.h"
#include "processor.h"
#include "globalvar.h"
#include "mcpat_interface.hh"

using namespace std;

static ParseXML *pxml;
static Processor *proc;

double mcpat_interface(bool init, bool destruct, int plevel, mcpat_stats *rt, char *name, int core_id)
{
	//char fb[50] = "sample.xml";
	char fb[100];
        strcpy(fb,(const char *) name);
        
	opt_for_clk	= true;

	if (destruct) {
		if (pxml) delete pxml;
		if (proc) delete proc;
		return 0;
	}

	if (init) {
		//cout << "==================== McPAT Initialization ====================" << endl;
		pxml= new ParseXML();
		pxml->parse(fb);
		proc = new Processor(pxml);
	        //std::cout << "mcpat interface: core size" << proc->cores.size() << "clock rate" << proc->cores[0]->clockRate << endl;
                for(int i=0; i < proc->cores.size();i++) {
		      proc->cores[i]->displayEnergy(4, 4);
                }
		//proc->displayEnergy(4, plevel);
//		proc->displayDynamic(4, plevel);
	}
	else {
	        //cout << "parsing rt-stats file in mcpat interface" << endl;
		parse_mcpat_stats(rt);
                //modified by rsingh - display only core
		//proc->compute();
		//proc->displayDynamic(4, plevel);
                if(core_id == -1) {
                    for(int i=0; i < proc->cores.size();i++) {
		        proc->cores[i]->computeEnergy(false);
		        //proc->cores[i]->displayDynamic(4, plevel);
		        proc->cores[i]->displayEnergy(4, plevel);
                    }
                } else {
		    proc->cores[core_id]->computeEnergy(false);
		    //proc->cores[core_id]->displayDynamic(4, plevel);
		    return proc->cores[core_id]->getDynamicEnergy();
                }
	}
        return 0;
}

void parse_mcpat_stats(mcpat_stats *rt)
{
	// global stats
	pxml->sys.total_cycles = rt->total_cycles;

	// per-core stats
	for (int i = 0; i < rt->num_cores; i++) {
		// general core stats
		pxml->sys.core[i].clock_rate					= rt->clockrate[i];	
	       //std::cout<<"MCPAT clock rate inside " << pxml->sys.core[i].clock_rate <<std::endl;
		pxml->sys.core[i].total_cycles					= rt->core_total_cycles[i];
        //std::cout << "Inside McPAT: total cycles for an interval " <<rt->core_total_cycles[i] <<std::endl;
		pxml->sys.core[i].total_instructions			= rt->total_insts[i];
		pxml->sys.core[i].int_instructions				= rt->int_insts[i];
		pxml->sys.core[i].fp_instructions				= rt->fp_insts[i];
		pxml->sys.core[i].branch_instructions			= rt->br_insts[i];
		pxml->sys.core[i].branch_mispredictions			= rt->br_mispredict[i];
		pxml->sys.core[i].load_instructions				= rt->ld_insts[i];
		pxml->sys.core[i].store_instructions			= rt->st_insts[i];
		pxml->sys.core[i].committed_instructions		= rt->commit_insts[i];
		pxml->sys.core[i].committed_int_instructions	= rt->commit_int_insts[i];
		pxml->sys.core[i].committed_fp_instructions		= rt->commit_fp_insts[i];
		pxml->sys.core[i].pipeline_duty_cycle			= rt->pipeline_duty_cycle[i];
		// ROB
		pxml->sys.core[i].ROB_reads						= rt->ROB_reads[i];
		pxml->sys.core[i].ROB_writes					= rt->ROB_writes[i];
		// RAT
		pxml->sys.core[i].rename_reads					= rt->rename_reads[i];
		pxml->sys.core[i].rename_writes					= rt->rename_writes[i];
		pxml->sys.core[i].fp_rename_reads				= rt->fp_rename_reads[i];
		pxml->sys.core[i].fp_rename_writes				= rt->fp_rename_writes[i];
		// Inst Window stats
		pxml->sys.core[i].inst_window_reads				= rt->inst_window_reads[i];
		pxml->sys.core[i].inst_window_writes			= rt->inst_window_writes[i];
		pxml->sys.core[i].inst_window_wakeup_accesses	= rt->inst_window_wakeup_accesses[i];
		pxml->sys.core[i].fp_inst_window_reads			= rt->fp_inst_window_reads[i];
		pxml->sys.core[i].fp_inst_window_writes			= rt->fp_inst_window_writes[i];
		pxml->sys.core[i].fp_inst_window_wakeup_accesses= rt->fp_inst_window_wakeup_accesses[i];
		// Register File
		pxml->sys.core[i].int_regfile_reads				= rt->int_rf_reads[i];
		pxml->sys.core[i].float_regfile_reads			= rt->fp_rf_reads[i];
		pxml->sys.core[i].int_regfile_writes			= rt->int_rf_writes[i];
		pxml->sys.core[i].float_regfile_writes			= rt->fp_rf_writes[i];
		// Accesses to Working Reg
		pxml->sys.core[i].function_calls				= rt->func_calls[i];
		pxml->sys.core[i].context_switches				= rt->context_switches[i];
		// Functional Unit
		pxml->sys.core[i].ialu_accesses					= rt->ialu_accesses[i];
		pxml->sys.core[i].fpu_accesses					= rt->fpu_accesses[i];
		pxml->sys.core[i].mul_accesses					= rt->mul_accesses[i];
		pxml->sys.core[i].cdb_alu_accesses				= rt->ialu_accesses[i];
		pxml->sys.core[i].cdb_fpu_accesses				= rt->fpu_accesses[i];
		pxml->sys.core[i].cdb_mul_accesses				= rt->mul_accesses[i];
		
		// I-TLB
		//pxml->sys.core[i].itlb.total_accesses			= rt->itlb_accesses[i];
		//pxml->sys.core[i].itlb.total_misses				= rt->itlb_misses[i];
		//// I-Cache
		//pxml->sys.core[i].icache.read_accesses			= rt->icache_accesses[i];
		//pxml->sys.core[i].icache.read_misses			= rt->icache_misses[i];
		//// D-TLB
		//pxml->sys.core[i].dtlb.total_accesses			= rt->dtlb_accesses[i];
		//pxml->sys.core[i].dtlb.total_misses				= rt->dtlb_misses[i];
		//// D-Cache
		//pxml->sys.core[i].dcache.read_accesses			= rt->dcache_accesses[i];
		//pxml->sys.core[i].dcache.read_misses			= rt->dcache_misses[i];
	
		//// BTB
		//pxml->sys.core[i].BTB.read_accesses				= rt->BTB_reads[i];
		//pxml->sys.core[i].BTB.write_accesses			= rt->BTB_writes[i];
	}
	/*
	// L1Dir
	pxml->sys.L1Directory[0].read_accesses				= rt->l1dir_reads;
	pxml->sys.L1Directory[0].write_accesses				= rt->l1dir_writes;
	pxml->sys.L1Directory[0].read_misses				= rt->l1dir_read_misses;
	pxml->sys.L1Directory[0].write_misses				= rt->l1dir_write_misses;
	// L2Dir
	pxml->sys.L2Directory[0].read_accesses				= rt->l2dir_reads;
	pxml->sys.L2Directory[0].write_accesses				= rt->l2dir_writes;
	pxml->sys.L2Directory[0].read_misses				= rt->l2dir_read_misses;
	pxml->sys.L2Directory[0].write_misses				= rt->l2dir_write_misses;
	*/
	// L2 Cache
	/*
	pxml->sys.L2[0].read_accesses						= rt->l2_reads;
	pxml->sys.L2[0].write_accesses						= rt->l2_writes;
	pxml->sys.L2[0].read_misses							= rt->l2_read_misses;
	pxml->sys.L2[0].write_misses						= rt->l2_write_misses;
	pxml->sys.L2[0].duty_cycle							= rt->l2_duty_cycle;
	*/
}

mcpat_stats::mcpat_stats(int num_thread_contexts)
{
	num_cores = num_thread_contexts;
	
	// general core stats	
	clockrate = new double[num_cores];
	total_insts = new double[num_cores];
	int_insts = new double[num_cores];
	fp_insts = new double[num_cores];
	br_insts = new double[num_cores];
	br_mispredict = new double[num_cores];
	ld_insts = new double[num_cores];
	st_insts = new double[num_cores];
	commit_insts = new double[num_cores];
	commit_int_insts = new double[num_cores];
	commit_fp_insts = new double[num_cores];
	pipeline_duty_cycle = new double[num_cores];
	core_total_cycles = new double[num_cores];
	// ROB
	ROB_reads = new double[num_cores];
	ROB_writes = new double[num_cores];
	// RAT
	rename_reads = new double[num_cores];
	rename_writes = new double[num_cores];
	fp_rename_reads = new double[num_cores];
	fp_rename_writes = new double[num_cores];
	// Inst Window stats
	inst_window_reads = new double[num_cores];
	inst_window_writes = new double[num_cores];
	inst_window_wakeup_accesses = new double[num_cores];
	fp_inst_window_reads = new double[num_cores];
	fp_inst_window_writes = new double[num_cores];
	fp_inst_window_wakeup_accesses = new double[num_cores];	
	// Register File
	int_rf_reads = new double[num_cores];
	fp_rf_reads = new double[num_cores];
	int_rf_writes = new double[num_cores];
	fp_rf_writes = new double[num_cores];
	// Accesses to Working Reg
	func_calls = new double[num_cores];
	context_switches = new double[num_cores];
	// Functional Unit
	ialu_accesses = new double[num_cores];
	fpu_accesses = new double[num_cores];
	mul_accesses = new double[num_cores];
	// I-TLB
	itlb_accesses = new double[num_cores];
	itlb_misses = new double[num_cores];
	// I-Cache
	icache_accesses = new double[num_cores];
	icache_misses = new double[num_cores];
	// D-TLB
	dtlb_accesses = new double[num_cores];
	dtlb_misses = new double[num_cores];
	// D-Cache
	dcache_accesses = new double[num_cores];
	dcache_misses = new double[num_cores];
	// BTB
	BTB_reads = new double[num_cores];
	BTB_writes = new double[num_cores];
}

mcpat_stats::~mcpat_stats()
{
	delete total_insts;
	delete int_insts;
	delete fp_insts;
	delete br_insts;
	delete br_mispredict;
	delete ld_insts;
	delete st_insts;
	delete commit_insts;
	delete commit_int_insts;
	delete commit_fp_insts;
	delete pipeline_duty_cycle;
	delete core_total_cycles;
	// ROB
	delete ROB_reads;
	delete ROB_writes;
	// RAT
	delete rename_reads;
	delete rename_writes;
	delete fp_rename_reads;
	delete fp_rename_writes;
	// Inst Window stats
	delete inst_window_reads;
	delete inst_window_writes;
	delete inst_window_wakeup_accesses;
	delete fp_inst_window_reads;
	delete fp_inst_window_writes;
	delete fp_inst_window_wakeup_accesses;	
	// Register File
	delete int_rf_reads;
	delete fp_rf_reads;
	delete int_rf_writes;
	delete fp_rf_writes;
	// Accesses to Working Reg
	delete func_calls;
	delete context_switches;
	// Functional Unit
	delete ialu_accesses;
	delete fpu_accesses;
	delete mul_accesses;
	// I-TLB
	delete itlb_accesses;
	delete itlb_misses;
	// I-Cache
	delete icache_accesses;
	delete icache_misses;
	// D-TLB
	delete dtlb_accesses;
	delete dtlb_misses;
	// D-Cache
	delete dcache_accesses;
	delete dcache_misses;
	// BTB
	delete BTB_reads;
	delete BTB_writes;
}
