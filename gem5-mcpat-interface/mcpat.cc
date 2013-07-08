#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include "sim/mcpat.hh"
//#include "params/BaseMcPAT.hh"
#include "params/System.hh"
#include "debug/BaseMcPAT.hh"
#include "debug/PseudoInst.hh"
#include "sim/sim_events.hh"
#include "sim/eventq.hh"

#include "sim/stat_control.hh"
#include "sim/system.hh"
#include "cpu/thread_context.hh"
#include "cpu/base.hh"
#include "cpu/o3/cpu.hh"
#include "cpu/o3/impl.hh"
#include <stdlib.h> 
#define INTERVAL 100000
BaseMcPAT::TickEvent::TickEvent(BaseMcPAT *g)
	: Event(CPU_Tick_Pri), mcpat(g)
{
}

void BaseMcPAT::TickEvent::process()
{
	mcpat->tick();
}

const char * BaseMcPAT::TickEvent::description() const
{
	return "BaseMcPAT tick";
}

//BaseMcPAT::BaseMcPAT(SystemParams *p, System *sys)
//: SimObject(p), system(sys), clock(p->clock), phase(0), tickEvent(this)
//BaseMcPAT::BaseMcPAT(SystemParams *p, System *sys)
//: system(sys), clock(0), phase(0), tickEvent(this)
BaseMcPAT::BaseMcPAT(SystemParams *p, System *sys)
: SimObject(p), system(sys), clock(p->clock), phase(0), mytickEvent(this), tickEvent(this)
{
	mcpat_activated = p->mcpat_activated;
	mcpat_tick_multiplier = p->mcpat_tick_multiplier;
	mcpat_clock = clock * mcpat_tick_multiplier;
	mcpat_plevel = p->mcpat_plevel;
	using_mcpat_tick = p->using_mcpat_tick;
        strcpy(archFileName,p->mcpat_arch_file_name.c_str());
}

BaseMcPAT::~BaseMcPAT()
{
	if (mcpat_activated)
	{
		// free mcpat side memory
		bool init		= false;
		bool destruct	= true;
		mcpat_interface(init, destruct, mcpat_plevel, rt_stats, archFileName);
		delete rt_stats;
	}
	// de-schedule tick event
	//if (tickEvent.scheduled())
	//	deschedule(tickEvent);
}

void BaseMcPAT::init()
{
}

void BaseMcPAT::mcpatEvent()
{
    tick(); 
} 
//pandey
void BaseMcPAT::startup()
{
	if (mcpat_activated)
	//if (mcpat_activated)
	{
		// initialize McPAT
		int num_cores = system->numContexts();
		rt_stats = new mcpat_stats(num_cores);
		rt_stats_prev = new mcpat_stats(num_cores);
		bool init		= true;
		bool destruct	= false;
	        //std::cout << "going into the mcpat interface" << std::endl;
		mcpat_interface(init, destruct, mcpat_plevel, rt_stats, archFileName);
		// dump & reset stats
                //if(using_mcpat_tick) {
		//  Stats::schedStatEvent(true, true, curTick(), 0);
                //}
		numCycles = 0;
	        //DPRINTF(BaseMcPAT, "power delay int_alu_accesses float_alu_accesses committed_instructions committed_int_instructions committed_float_instructions load_instrutions store_instructions");
	    //std::cout << "curTick()" <<curTick() << std::endl;
	}
	// starting off -- scehdule tickevent
        if(using_mcpat_tick) {
	    schedule(mytickEvent, (curTick() + ticks(3*10000000)));
	    //schedule(mytickEvent, (curTick() + ticks(100000000)));
	    //schedule(mytickEvent, nextCycle(curTick() + ticks(100000)));
	    //system->schedule(tickEvent, nextCycle(curTick() + ticks(1)));
	    //std::cout << "After scheduling mcpat event" << std::endl;
        }
}

void BaseMcPAT::serialize(std::ostream &os)
{
}

void BaseMcPAT::unserialize(Checkpoint *cp, const std::string &section)
{
}

void BaseMcPAT::tick()
{
	if (mcpat_activated)
	{
	        //DPRINTF(BaseMcPAT, "mcpat clock %d\n", mcpat_clock);
		//if( !( curTick() % mcpat_clock) ) {
			//std::cout<<"Calling mcpat_tick" <<std::endl;
			//DPRINTF(BaseMcPAT,"Calling mcpat_tick!\n" );
			numCycles++;
			//std::cout << "\ncall McPAT @ " << curCycle() << " : # " << numCycles << std::endl;
			//int num_cores = system->numContexts();
                        ThreadContext *tc = system->getThreadContext(0);
			//std::cout << "num_cores" << num_cores <<std::endl;
                        //for (ThreadID tid = 0; tid < num_cores; tid++)
                        //{
			  //AP
                          calc_interval_power(tc);
                          //reset_prev_stats(tc);
                        //}
			 
   			 BaseCPU *cpu = tc->getCpuPtr();
   			 assert(cpu);
   			 FullO3CPU<O3CPUImpl> *cpu_o3 = dynamic_cast< FullO3CPU<O3CPUImpl> * >(cpu);
   			 assert(cpu_o3);
		//FROM HEREEE	
	
	//	** gem5 output to testf conversion
			std::system("~/gem5/gem5/svm/ready/test ~/gem5/gem5/gem5-to-ready ~/gem5/gem5/ready-to-ML");
			std::system("chmod 777 ~/gem5/gem5/ready-to-ML");
			std::system("~/gem5/gem5/svm/svm-predict ~/gem5/gem5/ready-to-ML ~/gem5/gem5/svm/model ~/gem5/gem5/ML-to-gem5");
			//char *buf[100];
			float dfs_freq;
			FILE *fp;
   			fp = fopen("ML-to-gem5", "r");
			fscanf(fp,"%f", &dfs_freq );
			std::cout<<"1.6 or 2 or 1?? \n" <<dfs_freq <<std::endl;
			if(dfs_freq ==1.0f)
			   cpu_o3->clock = 1000;
			else if (dfs_freq ==1.6f){
			   
			   cpu_o3->clock = 625;
		     
			}
			else if(dfs_freq ==2.0f){
			   cpu_o3->clock = 500;
			}
			else std::cout <<"somethings wrong" <<std::endl;
			fclose(fp);
			
		//UPTILL HERE FOR RUNTIME
		
			//if(1000000000< curTick() && curTick() < 2000000000){
			//    std:: cout <<"&&&&&&&&&&&&&&& hellow &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& changing Freq &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
			//}
			//if( curTick() > 2000000000){
			//    std:: cout <<"&&&&&&&&&&&&&&& hellow &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& changing Freq &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
			//    cpu_o3->clock = 500;
			//}
   			//ThreadID tid = tc->cpuId();
			//std::cout <<cpu_o3->frequency() <<std::endl;
			if( cpu_o3->frequency() == 2000000000) {
			//// pass stats
			//feed_mcpat_stats(rt_stats);
			//// call McPAT interface
			//bool init		= false;
			//bool destruct	= false;
			//mcpat_interface(init, destruct, mcpat_plevel, rt_stats, archFileName);
			//// dump & reset stats
			//Stats::schedStatEvent(true, true, curTick(), 0);
		//}
	       //std::cout << "nextCycle()" << nextCycle(curTick() + ticks(100000)) <<std::endl;
	//	schedule(mytickEvent, nextCycle(curTick() + ticks(100000)));
	         //schedule(mytickEvent, (curTick() + ticks(100000000)));
			schedule(mytickEvent, (curTick() + ticks(2*10000000)));
			}
			else  if( cpu_o3->frequency() == 1000000000) {
			schedule(mytickEvent, (curTick() + ticks(4*10000000))); 
			}
			else  if( cpu_o3->frequency() == 1300000000) {
			      //std::cout<<"here" <<std::endl;
			schedule(mytickEvent, (curTick() + ticks(1*10000000)));
			}
			//else if ( cpu_o3->frequency() == 4* 10e9) {
			//schedule(mytickEvent, (curTick() + ticks(4*10000000)));
			//}
			else if ( cpu_o3->frequency() == 1.6* 1000000000) {
			schedule(mytickEvent, (curTick() + ticks(30000000)));
			}
			else {std::cout<<"something's wrong with freq\n";}
	         //schedule(mytickEvent, (curTick() + ticks(100000)));
		//system->schedule(tickEvent, nextCycle(curTick() + ticks(1)));
	}
}

//void BaseMcPAT::calc_overall_power()
//{
//    //DPRINTF(BaseMcPAT,"Calling mcpat_overall_power curTick: %10d\n",curTick() );
//    //std::cout << "call McPAT @ " << curCycle() << " : # " << numCycles << std::endl;
//    feed_mcpat_stats(rt_stats);
//    bool init		= false;
//    bool destruct	= false;
//    mcpat_interface(init, destruct, mcpat_plevel, rt_stats, archFileName);
//
//}
//
//void BaseMcPAT::calc_last_interval_power()
//{
//    int num_cores = system->numContexts();
//    
//    for (ThreadID tid = 0; tid < num_cores; tid++)
//    {
//	ThreadContext *tc = system->getThreadContext(tid);
//        calc_interval_power(tc);
//        reset_prev_stats(tc);
//    }
//}

void BaseMcPAT::calc_interval_power(ThreadContext *tc)
{
    //DPRINTF(BaseMcPAT,"Calling mcpat_tick. curTick: %10d\n",curTick() );
   int fre = 0;
    //std::cout << "call McPAT @ " << curCycle() << " : # " << numCycles << std::endl;
    feed_interval_stats(tc);
    //DPRINTF(BaseMcPAT, "check %d \n", rt_stats->clockrate[0]);
    //DPRINTF(BaseMcPAT, "check %d \t %d \t %d \t\n", rt_stats_prev->core_total_cycles[0], rt_stats_prev->clockrate[0], rt_stats->clockrate[0]);
    //feed_interval_stats(rt_stats,tc,rt_stats_prev);
    bool init		= false;
    bool destruct	= false;
    //mcpat_interface(init, destruct, mcpat_plevel, rt_stats, archFileName);
    double energy = mcpat_interface(init, destruct, 1, rt_stats, archFileName, tc->cpuId());
    //double execution_time = INTERVAL/(rt_stats->clockrate[tc->cpuId()]);
    double execution_time = rt_stats->core_total_cycles[tc->cpuId()]/(rt_stats->clockrate[tc->cpuId()]);
    
    //double execution_time = rt_stats->core_total_cycles[tc->cpuId()]/(rt_stats->clockrate[tc->cpuId()]);
    double power = energy/execution_time; 

   //std::cout<<power<<"   "<<execution_time<<std::endl;//"rt_stats cycles "<<rt_stats->core_total_cycles[0] <<std::endl;
    //DPRINTF(PseudoInst,"Energy:%d\t Power:%f\t Cycles:%d\t \n"
    //    ,energy,power, rt_stats->core_total_cycles[tc->cpuId()]);
  // DPRINTF(BaseMcPAT,"stats: committed instructions:%d\t committed integer instructions:%d\t committed floating point istructions :%d\t load instructions:%d\t store instructios:%d\t branch instructionss:%d\t\n",
  //      rt_stats->commit_insts[tc->cpuId()], rt_stats->commit_int_insts[tc->cpuId()], rt_stats->commit_fp_insts[tc->cpuId()],
  //      rt_stats->ld_insts[tc->cpuId()], rt_stats->st_insts[tc->cpuId()], rt_stats->br_insts[tc->cpuId()]);
////
   if(rt_stats->clockrate[tc->cpuId()] ==2000000000) fre = 2;
   else if(rt_stats->clockrate[tc->cpuId()] ==1000000000) fre = 1;
   else if(rt_stats->clockrate[tc->cpuId()] ==1600000000) fre = 3;
   else if(rt_stats->clockrate[tc->cpuId()] ==1300000000) fre = 4;
   else {std::cout<<"something's wrong with variable fre\n";}
   //char *outputFile = (char *) "testfile";
//USE THIS PART FOR MCPAT ENABLED RUN
   FILE *fp;
   fp = fopen("gem5-to-ready", "w");
   if(fp==NULL) { std::cout <<"error" <<std::endl;}
  fprintf(fp, "%d %f %f %f %f %f %f %f %d", fre, rt_stats->ialu_accesses[tc->cpuId()],rt_stats->fpu_accesses[tc->cpuId()],rt_stats->commit_int_insts[tc->cpuId()],rt_stats->commit_fp_insts[tc->cpuId()],rt_stats->ld_insts[tc->cpuId()],rt_stats->st_insts[tc->cpuId()] , rt_stats->dtlb_accesses[tc->cpuId()] ,rt_stats->cpu_idle_cycles );
   //fclose(fp);
//UPTILL HERE!
std::cout << fre << " " << power <<" "<< execution_time*1000000<<" "<<  rt_stats->ialu_accesses[tc->cpuId()]<<" "<< rt_stats->fpu_accesses[tc->cpuId()]<<" "<< rt_stats->commit_int_insts[tc->cpuId()]<<" "<< rt_stats->commit_fp_insts[tc->cpuId()]<<" "<< rt_stats->ld_insts[tc->cpuId()]<<" "<< rt_stats->st_insts[tc->cpuId()] << " " << rt_stats->dtlb_accesses[tc->cpuId()] <<" "<<  rt_stats->cpu_idle_cycles <<std::endl;
}

void BaseMcPAT::reset_prev_stats(ThreadContext* tc)
{
    
    BaseCPU *cpu = tc->getCpuPtr();
    assert(cpu);
    FullO3CPU<O3CPUImpl> *cpu_o3 = dynamic_cast< FullO3CPU<O3CPUImpl> * >(cpu);
    assert(cpu_o3);
    ThreadID tid = tc->cpuId();
    
    /*  reset prev values
     * */
    rt_stats_prev->core_total_cycles[tid]	= 0;
    
    rt_stats_prev->total_insts[tid]			= 0;
    
    rt_stats_prev->int_insts[tid]			= 0;
    rt_stats_prev->fp_insts[tid]				= 0;
    rt_stats_prev->br_insts[tid]				= 0;
    rt_stats_prev->br_mispredict[tid]		= 0;
    rt_stats_prev->ld_insts[tid]				= 0;
    rt_stats_prev->st_insts[tid]				= 0;
    rt_stats_prev->commit_insts[tid]			= 0;
    rt_stats_prev->commit_int_insts[tid]		= 0;
    rt_stats_prev->commit_fp_insts[tid]		= 0;
    rt_stats_prev->pipeline_duty_cycle[tid]	= 0;	// FIXME
    // ROB
    rt_stats_prev->ROB_reads[tid]			= 0;
    rt_stats_prev->ROB_writes[tid]			= 0;
    // RAT
    rt_stats_prev->rename_reads[tid]			= 0;
    rt_stats_prev->rename_writes[tid]		= 0;	// FIXME
    rt_stats_prev->fp_rename_reads[tid]		= 0;
    rt_stats_prev->fp_rename_writes[tid]		= 0;	// FIXME
    // Inst Window Stats
    rt_stats_prev->inst_window_reads[tid]	= 0;
    rt_stats_prev->inst_window_writes[tid]	= 0;
    rt_stats_prev->inst_window_wakeup_accesses[tid]= 0;
    rt_stats_prev->fp_inst_window_reads[tid]	= 0;
    rt_stats_prev->fp_inst_window_writes[tid]= 0;
    rt_stats_prev->fp_inst_window_wakeup_accesses[tid]= 0;
    // Register File
    rt_stats_prev->int_rf_reads[tid]			= 0;
    rt_stats_prev->fp_rf_reads[tid]			= 0;
    rt_stats_prev->int_rf_writes[tid]		= 0;
    rt_stats_prev->fp_rf_writes[tid]			= 0;
    // Accesses to Working Reg
    rt_stats_prev->func_calls[tid]			= 0;
    rt_stats_prev->context_switches[tid]		= 0;	// FIXME
    // Functional Unit
    rt_stats_prev->ialu_accesses[tid]		= 0;
    rt_stats_prev->fpu_accesses[tid]			= 0;
    rt_stats_prev->mul_accesses[tid]			= 0;	// FIXME
    // I-TLB
    rt_stats_prev->itlb_accesses[tid]              = 0; 
    rt_stats_prev->itlb_misses[tid]                = 0; 
    // I-Cache
    rt_stats_prev->icache_accesses[tid]            = 0;
    rt_stats_prev->icache_misses[tid]              = 0;
    // D-TLB
    rt_stats_prev->dtlb_accesses[tid]              = 0; 
    rt_stats_prev->dtlb_misses[tid]                = 0; 
    // D-Cache
    rt_stats_prev->dcache_accesses[tid]            = 0;
    rt_stats_prev->dcache_misses[tid]              = 0;
    
    // BTB
    rt_stats_prev->BTB_reads[tid]			= 0;
    rt_stats_prev->BTB_writes[tid]			= 0;	// FIXME
    
    // L1Dir
    
    // L2Dir
    
    // L2 Cache

}


//void BaseMcPAT::feed_interval_stats(mcpat_stats *rt_stats, ThreadContext* tc, mcpat_stats *rt_stats_prev)
void BaseMcPAT::feed_interval_stats(ThreadContext* tc) //mcpat_stats *rt_stats, ThreadContext* tc, mcpat_stats *rt_stats_prev)
{
    //int num_cores = system->numContexts();
    rt_stats->total_cycles = 0;
    
    BaseCPU *cpu = tc->getCpuPtr();
    assert(cpu);
    FullO3CPU<O3CPUImpl> *cpu_o3 = dynamic_cast< FullO3CPU<O3CPUImpl> * >(cpu);
    assert(cpu_o3);
    ThreadID tid = tc->cpuId();
    //int delay = cpu_o3->Cycles(10000) ;
    //std::cout << "delay <<"delay <<std::endl;
    // general core stats
    //rt_stats->clockrate[tid]  = (double)1000000 / cpu_o3->get_clock();	//pandey
    //rt_stats->clockrate[tid]  = cpu_o3->clockPeriod();	//pandey
    rt_stats->clockrate[tid]  = cpu_o3->frequency();//(double)1000000 / //cpu_o3->clockPeriod();	//pandey
    //std::cout << "get clock object " <<cpu_o3->clockPeriod() << "curTick "<< curTick()<<" numCycles value "<<cpu_o3->numCycles.value()  <<std::endl;
    
    //cpu_o3->clock = 1000000000;
    
    //std::cout <<"changed clock freq to 1Ghz \n"<< cpu_o3->frequency() <<std::endl;
    //DPRINTF(BaseMcPAT,"tid:%d cpu clock rate(frequency):%d \n",  
     //   tid, rt_stats->clockrate[tid]);
    //std::cout << "tid:" << tid <<"clockrate:" <<  rt_stats->clockrate[tid] << std::endl;
    //DPRINTF(BaseMcPAT, "before:::::: rt_stats_prev, core cycles %d \t int_rf_reads %d\n",rt_stats_prev->core_total_cycles[tid], rt_stats_prev->int_rf_reads[tid]);
    rt_stats->core_total_cycles[tid]	= cpu_o3->numCycles.value() - rt_stats_prev->core_total_cycles[tid];
    //DPRINTF(BaseMcPAT,"tid:%d interval cycles:%d cumm cycles:%d prev cycles:%d\n",  
    //    tid, rt_stats->core_total_cycles[tid], cpu_o3->numCycles.value(), rt_stats_prev->core_total_cycles[tid]);
    
    rt_stats->total_cycles = rt_stats->core_total_cycles[tid];
    //if (rt_stats->total_cycles < rt_stats->core_total_cycles[tid]) {
    //	rt_stats->total_cycles = rt_stats->core_total_cycles[tid];
    //}
    
    std::vector<double> temp;
    rt_stats->total_insts[tid]			= cpu_o3->iew.iewExecutedInsts.value() - rt_stats_prev->total_insts[tid] ;
    //DPRINTF(BaseMcPAT,"tid:%d interval insts:%d cumm insts:%d prev insts:%d\n",  
        //tid, rt_stats->total_insts[tid],  cpu_o3->iew.iewExecutedInsts.value(), rt_stats_prev->total_insts[tid]);

    cpu_o3->commit.statComInteger.value(temp);	// FIXME
    rt_stats->int_insts[tid]			= temp[0] - rt_stats_prev->int_insts[tid] ;
    
    cpu_o3->commit.statComFloating.value(temp);	// FIXME
    rt_stats->fp_insts[tid]			= temp[0] - rt_stats_prev->fp_insts[tid];
    rt_stats->br_insts[tid]			= cpu_o3->fetch.fetchedBranches.value() - rt_stats_prev->br_insts[tid];
    rt_stats->br_mispredict[tid]		= cpu_o3->decode.decodeBranchMispred.value() - rt_stats_prev->br_mispredict[tid];
    rt_stats->ld_insts[tid]			= cpu_o3->iew.iewDispLoadInsts.value() - rt_stats_prev->ld_insts[tid];
    rt_stats->st_insts[tid]			= cpu_o3->iew.iewDispStoreInsts.value() - rt_stats_prev->st_insts[tid];
    rt_stats->commit_insts[tid]			= cpu_o3->totalCommittedInsts.value() - rt_stats_prev->commit_insts[tid];
    cpu_o3->commit.statComInteger.value(temp);
    rt_stats->commit_int_insts[tid]		= temp[0] - rt_stats_prev->commit_int_insts[tid];
    cpu_o3->commit.statComFloating.value(temp);
    rt_stats->commit_fp_insts[tid]		= temp[0] - rt_stats_prev->commit_fp_insts[tid];
    rt_stats->pipeline_duty_cycle[tid]	= rt_stats->total_insts[tid] / rt_stats->core_total_cycles[tid];	// FIXME
    // ROB
    rt_stats->ROB_reads[tid]			= cpu_o3->rob.robReads.value() - rt_stats_prev->ROB_reads[tid];
    rt_stats->ROB_writes[tid]			= cpu_o3->rob.robWrites.value() - rt_stats_prev->ROB_writes[tid];
    // RAT
    rt_stats->rename_reads[tid]			= cpu_o3->rename.intRenameLookups.value() - rt_stats_prev->rename_reads[tid];
    rt_stats->rename_writes[tid]		= rt_stats->rename_reads[tid] / 2;	// FIXME
    rt_stats->fp_rename_reads[tid]		= cpu_o3->rename.fpRenameLookups.value() - rt_stats_prev->fp_rename_reads[tid];
    rt_stats->fp_rename_writes[tid]		= rt_stats->fp_rename_reads[tid] / 2;	// FIXME
    // Inst Window Stats
    rt_stats->inst_window_reads[tid]	= cpu_o3->iew.instQueue.intInstQueueReads.value() - rt_stats_prev->inst_window_reads[tid];
    rt_stats->inst_window_writes[tid]	= cpu_o3->iew.instQueue.intInstQueueWrites.value() - rt_stats_prev->inst_window_writes[tid] ;
    rt_stats->inst_window_wakeup_accesses[tid]=cpu_o3->iew.instQueue.intInstQueueWakeupAccesses.value() - rt_stats_prev->inst_window_wakeup_accesses[tid];
    rt_stats->fp_inst_window_reads[tid]	= cpu_o3->iew.instQueue.fpInstQueueReads.value() - rt_stats_prev->fp_inst_window_reads[tid];
    rt_stats->fp_inst_window_writes[tid]= cpu_o3->iew.instQueue.fpInstQueueWrites.value() - rt_stats_prev->fp_inst_window_writes[tid] ;
    rt_stats->fp_inst_window_wakeup_accesses[tid]=cpu_o3->iew.instQueue.fpInstQueueWakeupQccesses.value() - rt_stats_prev->fp_inst_window_wakeup_accesses[tid];
    // Register File
    rt_stats->int_rf_reads[tid]			= cpu_o3->intRegfileReads.value() - rt_stats_prev->int_rf_reads[tid];
    rt_stats->fp_rf_reads[tid]			= cpu_o3->fpRegfileReads.value()  - rt_stats_prev->fp_rf_reads[tid];
    rt_stats->int_rf_writes[tid]		= cpu_o3->intRegfileWrites.value()- rt_stats_prev->int_rf_writes[tid];
    rt_stats->fp_rf_writes[tid]			= cpu_o3->fpRegfileWrites.value() - rt_stats_prev->fp_rf_writes[tid];
    // Accesses to Working Reg
    cpu_o3->commit.statComFunctionCalls.value(temp);
    rt_stats->func_calls[tid]			= temp[0] - rt_stats_prev->func_calls[tid];
    rt_stats->context_switches[tid]		= 0;	// FIXME
    // Functional Unit
    rt_stats->ialu_accesses[tid]		= cpu_o3->iew.instQueue.intAluAccesses.value() - rt_stats_prev->ialu_accesses[tid];
    rt_stats->fpu_accesses[tid]			= cpu_o3->iew.instQueue.fpAluAccesses.value()  - rt_stats_prev->fpu_accesses[tid];
    rt_stats->mul_accesses[tid]			= 0;	// FIXME
    // I-TLB
    //rt_stats->itlb_accesses[tid]              = cpu_o3->itb->fetch_accesses; 
    //rt_stats->itlb_misses[tid]                = cpu_o3->itb->fetch_misses; 
    //// I-Cache
    //rt_stats->icache_accesses[tid]            = g_system_ptr()->;
    //rt_stats->icache_misses[tid]              = g_system_ptr()->;
    //// D-TLB
    rt_stats->dtlb_accesses[tid]              = tc->getDTBPtr()->read_hits.value() -rt_stats_prev->dtlb_accesses[tid];
      //std::cout << "dtb_accesses = " <<rt_stats->dtlb_accesses[tid] <<std::endl;
    int a		      =  cpu_o3->idleCycles.value();
      rt_stats->cpu_idle_cycles = a - rt_stats_prev->cpu_idle_cycles;
    //rt_stats->cpu_idle_cycles[tid]		      =  cpu_o3>idleCycles.value() - rt_stats_prev->cpu_idle_cycles[tid];
     //std::cout << "idle Cycles = " <<rt_stats->cpu_idle_cycles<<std::endl;
    //int a;
    //a		      = cpu_o3->iew.instQueue.fuBusy;
    //std::vector <int>  b = cpu_o3->iew.instQueue.fuBusy[0] ;
    //rt_stats->fuBusy		      = cpu_o3->iew.instQueue.fuBusy[0] -rt_stats_prev->fuBusy;
    //rt_stats->dtlb_misses[tid]                = cpu_o3->dtb->data_misses; 
    //// D-Cache
    //rt_stats->dcache_accesses[tid]            = g_system_ptr()->;
    //rt_stats->dcache_misses[tid]              = g_system_ptr()->;
    
    // BTB
    rt_stats->BTB_reads[tid]			= cpu_o3->fetch.branchPred->BTBLookups.value() - rt_stats_prev->BTB_reads[tid];
    rt_stats->BTB_writes[tid]			= 0;	// FIXME
    // L1Dir
    
    // L2Dir
    
    // L2 Cache


    /*  Updating prev values
     * */
    rt_stats_prev->core_total_cycles[tid]	= cpu_o3->numCycles.value();
    //DPRINTF(BaseMcPAT, "rt_stats_prev, core cycles %d\n",rt_stats_prev->core_total_cycles[tid]);
    
    //std::vector<double> temp;
    rt_stats_prev->total_insts[tid]			= cpu_o3->iew.iewExecutedInsts.value();
    cpu_o3->commit.statComInteger.value(temp);	// FIXME
    
    rt_stats_prev->int_insts[tid]			= temp[0];
    cpu_o3->commit.statComFloating.value(temp);	// FIXME
    rt_stats_prev->fp_insts[tid]				= temp[0];
    rt_stats_prev->br_insts[tid]				= cpu_o3->fetch.fetchedBranches.value();
    rt_stats_prev->br_mispredict[tid]		= cpu_o3->decode.decodeBranchMispred.value();
    rt_stats_prev->ld_insts[tid]				= cpu_o3->iew.iewDispLoadInsts.value();
    rt_stats_prev->st_insts[tid]				= cpu_o3->iew.iewDispStoreInsts.value();
    rt_stats_prev->commit_insts[tid]			= cpu_o3->totalCommittedInsts.value();
    cpu_o3->commit.statComInteger.value(temp);
    rt_stats_prev->commit_int_insts[tid]		= temp[0];
    cpu_o3->commit.statComFloating.value(temp);
    rt_stats_prev->commit_fp_insts[tid]		= temp[0];
    rt_stats_prev->pipeline_duty_cycle[tid]	= rt_stats->total_insts[tid] / rt_stats->core_total_cycles[tid];	// FIXME
    // ROB
    rt_stats_prev->ROB_reads[tid]			= cpu_o3->rob.robReads.value();
    rt_stats_prev->ROB_writes[tid]			= cpu_o3->rob.robWrites.value();
    // RAT
    rt_stats_prev->rename_reads[tid]			= cpu_o3->rename.intRenameLookups.value();
    rt_stats_prev->rename_writes[tid]		= rt_stats->rename_reads[tid] / 2;	// FIXME
    rt_stats_prev->fp_rename_reads[tid]		= cpu_o3->rename.fpRenameLookups.value();
    rt_stats_prev->fp_rename_writes[tid]		= rt_stats->fp_rename_reads[tid] / 2;	// FIXME
    // Inst Window Stats
    rt_stats_prev->inst_window_reads[tid]	= cpu_o3->iew.instQueue.intInstQueueReads.value();
    rt_stats_prev->inst_window_writes[tid]	= cpu_o3->iew.instQueue.intInstQueueWrites.value();
    rt_stats_prev->inst_window_wakeup_accesses[tid]=cpu_o3->iew.instQueue.intInstQueueWakeupAccesses.value();
    rt_stats_prev->fp_inst_window_reads[tid]	= cpu_o3->iew.instQueue.fpInstQueueReads.value();
    rt_stats_prev->fp_inst_window_writes[tid]= cpu_o3->iew.instQueue.fpInstQueueWrites.value();
    rt_stats_prev->fp_inst_window_wakeup_accesses[tid]=cpu_o3->iew.instQueue.fpInstQueueWakeupQccesses.value();
    // Register File
    rt_stats_prev->int_rf_reads[tid]			= cpu_o3->intRegfileReads.value();
    rt_stats_prev->fp_rf_reads[tid]			= cpu_o3->fpRegfileReads.value();
    rt_stats_prev->int_rf_writes[tid]		= cpu_o3->intRegfileWrites.value();
    rt_stats_prev->fp_rf_writes[tid]			= cpu_o3->fpRegfileWrites.value();
    // Accesses to Working Reg
    cpu_o3->commit.statComFunctionCalls.value(temp);
    rt_stats_prev->func_calls[tid]			= temp[0];
    rt_stats_prev->context_switches[tid]		= 0;	// FIXME
    // Functional Unit
    rt_stats_prev->ialu_accesses[tid]		= cpu_o3->iew.instQueue.intAluAccesses.value();
    rt_stats_prev->fpu_accesses[tid]			= cpu_o3->iew.instQueue.fpAluAccesses.value();
    rt_stats_prev->mul_accesses[tid]			= 0;	// FIXME
    // I-TLB
    //rt_stats_prev->itlb_accesses[tid]              = cpu_o3->itb->fetch_accesses; 
    rt_stats_prev->dtlb_accesses[tid]                = tc->getDTBPtr()->read_hits.value();
    rt_stats_prev->cpu_idle_cycles		   = a;
    //rt_stats_prev->cpu_idle_cycles[tid]		   = cpu_o3->idleCycles.value();
    //rt_stats->fuBusy =cpu_o3->iew.instQueue.fuBusy[tid];
    //// I-Cache
    //rt_stats_prev->icache_accesses[tid]            = g_system_ptr()->;
    //rt_stats_prev->icache_misses[tid]              = g_system_ptr()->;
    //// D-TLB
    //rt_stats_prev->dtlb_accesses[tid]              = cpu_o3->dtb->data_accesses; 
    //rt_stats_prev->dtlb_misses[tid]                = cpu_o3->dtb->data_misses; 
    //// D-Cache
    //rt_stats_prev->dcache_accesses[tid]            = g_system_ptr()->;
    //rt_stats_prev->dcache_misses[tid]              = g_system_ptr()->;
    
    // BTB
    rt_stats_prev->BTB_reads[tid]			= cpu_o3->fetch.branchPred->BTBLookups.value();
    rt_stats_prev->BTB_writes[tid]			= 0;	// FIXME
    
    // L1Dir
    
    // L2Dir
    
    // L2 Cache

}

//void BaseMcPAT::feed_mcpat_stats(mcpat_stats *rt_stats)
//{
//	int num_cores = system->numContexts();
//	rt_stats->total_cycles = 0;
//
//	for (ThreadID tid = 0; tid < num_cores; tid++)
//	{
//		ThreadContext *tc = system->getThreadContext(tid);
//		BaseCPU *cpu = tc->getCpuPtr();
//		assert(cpu);
//		FullO3CPU<O3CPUImpl> *cpu_o3 = dynamic_cast< FullO3CPU<O3CPUImpl> * >(cpu);
//		assert(cpu_o3);
//
//		// general core stats
//		rt_stats->clockrate[tid]			= (double)1000000 / clock;	
//		rt_stats->core_total_cycles[tid]	= cpu_o3->numCycles.value();
//		if (rt_stats->total_cycles < rt_stats->core_total_cycles[tid])
//			rt_stats->total_cycles = rt_stats->core_total_cycles[tid];
//
//		std::vector<double> temp;
//		rt_stats->total_insts[tid]			= cpu_o3->iew.iewExecutedInsts.value();
//		cpu_o3->commit.statComInteger.value(temp);	// FIXME
//
//		rt_stats->int_insts[tid]			= temp[0];
//		cpu_o3->commit.statComFloating.value(temp);	// FIXME
//		rt_stats->fp_insts[tid]				= temp[0];
//		rt_stats->br_insts[tid]				= cpu_o3->fetch.fetchedBranches.value();
//		rt_stats->br_mispredict[tid]		= cpu_o3->decode.decodeBranchMispred.value();
//		rt_stats->ld_insts[tid]				= cpu_o3->iew.iewDispLoadInsts.value();
//		rt_stats->st_insts[tid]				= cpu_o3->iew.iewDispStoreInsts.value();
//		rt_stats->commit_insts[tid]			= cpu_o3->totalCommittedInsts.value();
//		cpu_o3->commit.statComInteger.value(temp);
//		rt_stats->commit_int_insts[tid]		= temp[0];
//		cpu_o3->commit.statComFloating.value(temp);
//		rt_stats->commit_fp_insts[tid]		= temp[0];
//		rt_stats->pipeline_duty_cycle[tid]	= rt_stats->total_insts[tid] / rt_stats->core_total_cycles[tid];	// FIXME
//		// ROB
//		rt_stats->ROB_reads[tid]			= cpu_o3->rob.robReads.value();
//		rt_stats->ROB_writes[tid]			= cpu_o3->rob.robWrites.value();
//		// RAT
//		rt_stats->rename_reads[tid]			= cpu_o3->rename.intRenameLookups.value();
//		rt_stats->rename_writes[tid]		= rt_stats->rename_reads[tid] / 2;	// FIXME
//		rt_stats->fp_rename_reads[tid]		= cpu_o3->rename.fpRenameLookups.value();
//		rt_stats->fp_rename_writes[tid]		= rt_stats->fp_rename_reads[tid] / 2;	// FIXME
//		// Inst Window Stats
//		rt_stats->inst_window_reads[tid]	= cpu_o3->iew.instQueue.intInstQueueReads.value();
//		rt_stats->inst_window_writes[tid]	= cpu_o3->iew.instQueue.intInstQueueWrites.value();
//		rt_stats->inst_window_wakeup_accesses[tid]=cpu_o3->iew.instQueue.intInstQueueWakeupAccesses.value();
//		rt_stats->fp_inst_window_reads[tid]	= cpu_o3->iew.instQueue.fpInstQueueReads.value();
//		rt_stats->fp_inst_window_writes[tid]= cpu_o3->iew.instQueue.fpInstQueueWrites.value();
//		rt_stats->fp_inst_window_wakeup_accesses[tid]=cpu_o3->iew.instQueue.fpInstQueueWakeupQccesses.value();
//		// Register File
//		rt_stats->int_rf_reads[tid]			= cpu_o3->intRegfileReads.value();
//		rt_stats->fp_rf_reads[tid]			= cpu_o3->fpRegfileReads.value();
//		rt_stats->int_rf_writes[tid]		= cpu_o3->intRegfileWrites.value();
//		rt_stats->fp_rf_writes[tid]			= cpu_o3->fpRegfileWrites.value();
//		// Accesses to Working Reg
//		cpu_o3->commit.statComFunctionCalls.value(temp);
//		rt_stats->func_calls[tid]			= temp[0];
//		rt_stats->context_switches[tid]		= 0;	// FIXME
//		// Functional Unit
//		rt_stats->ialu_accesses[tid]		= cpu_o3->iew.instQueue.intAluAccesses.value();
//		rt_stats->fpu_accesses[tid]			= cpu_o3->iew.instQueue.fpAluAccesses.value();
//		rt_stats->mul_accesses[tid]			= 0;	// FIXME
//		// I-TLB
//
//		// I-Cache
//
//		// D-TLB
//	
//		// I-Cache
//
//		// BTB
//		rt_stats->BTB_reads[tid]			= cpu_o3->fetch.branchPred->BTBLookups.value();
//		rt_stats->BTB_writes[tid]			= 0;	// FIXME
//	}
//	// L1Dir
//
//	// L2Dir
//
//	// L2 Cache
//
//}

//Tick BaseMcPAT::nextCycle()
//{
//	Tick next_tick = curTick() - phase + mcpat_clock - 1;
//	next_tick -= (next_tick % mcpat_clock);
//	next_tick += phase;
//	return next_tick;
//}
//
//Tick BaseMcPAT::nextCycle(Tick begin_tick)
//{
//	Tick next_tick = begin_tick;
//	if (next_tick % mcpat_clock != 0)
//		next_tick = next_tick - (next_tick % mcpat_clock) + mcpat_clock;
//	next_tick += phase;
//
//	assert(next_tick >= curTick());
//	return next_tick;
//}

//  BaseMcPATParams Simulation Object
//BaseMcPAT * BaseMcPATParams::create()
//{
//	return new BaseMcPAT(this);
//}
