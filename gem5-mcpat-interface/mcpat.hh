#ifndef __McPAT_BASE_HH__
#define __McPAT_BASE_HH__

//#include "config/full_system.hh"
#include "sim/sim_object.hh"
#include "sim/eventq.hh"

#include "mcpat8/mcpat_interface.hh"

//class BaseMcPATParams;
class SystemParams;
class System;
class ThreadContext;

class BaseMcPAT : public SimObject
//class BaseMcPAT 
{
	protected:
		System *system;
		Tick clock;	// CPU's clock period in terms of the # of ticks
		Tick mcpat_clock;
		int  mcpat_tick_multiplier;
                char archFileName[100];
	public:
		Tick phase;
		bool mcpat_activated;
		// McPAT statistics
		mcpat_stats *rt_stats;
		mcpat_stats *rt_stats_prev;
		int mcpat_plevel;
                bool using_mcpat_tick;

		inline Tick ticks(int numCycles) const { return numCycles; }
		inline Tick curCycle() const { return curTick() / clock; }
		Tick nextCycle();
		Tick nextCycle(Tick begin_tick);

		//typedef BaseMcPATParams Params;
		//const Params *params() const { return reinterpret_cast<const Params *>(_params); }
		//BaseMcPAT(Params *params);
		BaseMcPAT(SystemParams *params, System *sys);
		~BaseMcPAT();

		void init();
		void startup();
		void tick();
                void calc_overall_power();
                void calc_last_interval_power();
                void calc_interval_power(ThreadContext *tc);

                void feed_interval_stats(/*mcpat_stats *rt_stats,*/ ThreadContext* tc);//, mcpat_stats *rt_stats_prev);
                //void feed_interval_stats(mcpat_stats *rt_stats, ThreadContext* tc, mcpat_stats *rt_stats_prev);
		void feed_mcpat_stats(mcpat_stats *rt_stats);
                void reset_prev_stats(ThreadContext* tc);
		double numCycles;

//#if FULL_SYSTEM
		void serialize(std::ostream &os);
		void unserialize(Checkpoint *cp, const std::string &section);
//#endif
	        void mcpatEvent();
	        EventWrapper<BaseMcPAT, &BaseMcPAT::mcpatEvent>mytickEvent;

	//private:
		class TickEvent : public Event
		{
			protected:
				BaseMcPAT *mcpat;
			public:
				TickEvent(BaseMcPAT *g);
				void process();
				virtual const char *description() const;
		};
		TickEvent tickEvent;

};

#endif
