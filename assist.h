//
//  assist.h
//  ROSS_TOP
//
//  Created by Mark Plagge on 6/17/15.
//
//

#ifndef __ROSS_TOP__assist__
#define __ROSS_TOP__assist__

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include "ross.h"

/*Type definitions for the nuron simulation */
#define VERIFY_MAPPING 1
#define _idT uint32_t //!<ID type - local id type for bit shifts and ID cases.
#define _voltT int_fast32_t //!<Voltage data type (membrane potential)
#define _weightT int32_t //!<Weight/probability type
#define _threshT uint_fast32_t //!<threshold data type - In the paper, this is two unsigned values and a reversal flag.
#define _threshT_MAX UINT_FAST32_MAX

#define _randT int32_t //!< Random value storage for neurons.

#define _statT uint64_t //!<Counter data type for stats
/** _regIDT is a "regional id" type. This variable type is for storing
 *	coreIDs and localIDs. It must be half the bit size of tw_lpid.
 */
#define _regIDT uint32_t
	/** _gidIDT is the other half of the local id - since each big local id
	 *	is composed of a i and a j side, this holds the two values. */

#define _gridIDT uint16_t

/* Global Macros */

/** IABS is an integer absolute value function */
#define IABS(a) (((a) < 0) ? (-a) : (a))
	/** Faster version  of IABS (no branching) but needs types. @todo this
	 method will be faster on BGQ, but need to make sure that it works properly */
	//uint_fast64_t ab(int_fast64_t in){
	//int_fast64_t const mask = in >> sizeof(int_fast64_t) * CHAR_BIT - 1;
	//return (in ^ mask) - mask;
	//
	//}

	/** RZER is a floor function - values below zero round up to zero */
#define RZER(a) (((a) < 0) ? (0) : (a))

	//Kronecker delta
	//#define KDLTA(x) (((a) == 0) ? (1) : (0))
#define DT(x) !x //!<Kronecker Delta function.

#define SGN(x) (x > 0) - (x < 0) //!< Signum function

#define BINCOMP(s,p) IABS(s) >= p //!< binary comparison for conditional stochastic evaluation

	/** Defines the time resolution. Run the sim at 1ghz */

/* simulation structs. @todo: Maybe move these into main? */
/** evtType is a message/event identifier flag */
enum evtType {
	AXON_OUT, //!< Message originates from an axon
	AXON_HEARTBEAT, //!< Axon heartbeat message - big clock synchronization.
	SYNAPSE_OUT, //!< Message originates from a synapse
	NEURON_OUT, //!< Message originates from a neuron, and is going to an axion.
	NEURON_HEARTBEAT, //!< Neuron heartbeat messages - for big clock syncronization.
	GEN_HEARTBEAT //!< Signal generator messages -- used to simulate input for benchmarking.
};
enum lpTypeVals {
	AXON = 2,
	SYNAPSE = 1,
	NEURON = 0
};
/** Mapping def */
typedef enum CustomMapTypes{
	LLINEAR,
	SCATTER,
	CUST_LINEAR
} mapTypes;
typedef enum TimeRandomSel {
	RND_UNF = 0,
	RND_NORM_BASED = 1,
	RND_EXP = 2,
	RND_BIN = 3
} timeRandomSel;

/* Message structures */


/**
  Msg_Data is the main message struct.
  */
typedef struct Ms{
	enum evtType eventType;
	unsigned long rndCallCount;
	_idT localID; //!< Sender's local (within a core) id - used for weight lookups.
	_voltT neuronVoltage;
	tw_stime neuronLastActiveTime;
	tw_stime neuronLastLeakTime;

	_idT axonID; //!< Axon ID for neuron value lookups.
}Msg_Data;

/* ***** Global variable defs */
/** @{
 * /name SimParams */
extern int NEURONS_IN_CORE;
extern unsigned int CORES_IN_SIM;
extern int AXONS_IN_CORE;
extern int SYNAPSES_IN_CORE;
extern mapTypes tnMapping;
extern int CORE_SIZE;
extern tw_stime BIG_TICK_ERR; //!< Tick error - tw_stime can be this much under the next big tick and register as a big-tick.


/**  @} */

/** @{ /name Mapping */
extern int NUM_VP_X;
extern int NUM_VP_Y;
extern unsigned int LPS_PER_PE;
extern unsigned int SIM_SIZE;
extern unsigned int LP_PER_KP;

/*@}*/


/** @{
 * /name inputSimParams */
extern unsigned int GEN_ON;
extern bool GEN_RND;
extern unsigned int RND_MODE;
extern unsigned int GEN_PROB;
extern unsigned int GEN_FCT;
extern unsigned int GEN_OUTBOUND;
extern unsigned int GEN_SEL_MODE;
extern unsigned int SP_DBG;


/**  @} */
/** @{
 * /name neuronParams */
extern _threshT THRESHOLD_MAX;
extern _threshT THRESHOLD_MIN;
extern int32_t SYNAPSE_WEIGHT_MAX;
extern int32_t SYNAPSE_WEIGHT_MIN;
/**  @} */

/** @{ */

extern tw_stime littleTick;

extern tw_stime CLOCK_RANDOM_ADJ;
extern timeRandomSel CLOCK_RND_MODE;
/** @} */

/**
 *  Gets the next event time, based on a random function. Moved here to allow for
 *	easier abstraciton, and random function replacement.
 *
 *
 *  @param lp Reference to the current LP so that the function can see the RNG
 *
 *  @return a tw_stime value, such that \f$ 0 < t < 1 \f$. A delta for the next
 *  time slice.
 */
tw_stime getNextEventTime(tw_lp *lp);
/**
 *  @brief  Given a tw_stime, returns the current big tick.
 *
 *  @param now current time
 *
 *  @return the current big tick time.
 */
tw_stime getCurrentBigTick(tw_stime now);

/**
 *  @brief  Given a tw_stime, returns the next big-tick that will happen
 *
 *  @param now Right now!
 *
 *  @return Next big tick time.
 */
tw_stime getNextBigTick(tw_lp *lp);

int testTiming();

#endif /* defined(__ROSS_TOP__assist__) */
