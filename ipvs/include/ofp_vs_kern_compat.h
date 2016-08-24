#ifndef __OFP_VS_KERN_COMPAT_H__
#define __OFP_VS_KERN_COMPAT_H__

#include <rte_config.h>
#include <rte_atomic.h>
#include <rte_rwlock.h>
#include <rte_spinlock.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define IP_VS_POSSIBLE_CPU 32

#define DECLARE_PER_CPU(__type, __varname) \
	extern __type __varname##_array[IP_VS_POSSIBLE_CPU]

#define DEFINE_PER_CPU(__type, __varname) \
	__type __varname##_array[IP_VS_POSSIBLE_CPU]

#define per_cpu(__varname, __cpu) \
	__varname##_array[__cpu]

#define __get_cpu_var(__varname) \
	__varname##_array[rte_lcore_id()]

#define cpu_to_node(__cpu) \
	rte_lcore_to_socket_id(__cpu)

#define for_each_possible_cpu(__cpu) \
	for (__cpu = 0; __cpu <	IP_VS_POSSIBLE_CPU; __cpu++)

#define num_possible_cpus() IP_VS_POSSIBLE_CPU

#define for_each_online_cpu(__cpu) \
	RTE_LCORE_FOREACH(__cpu)

#define smp_processor_id() rte_lcore_id() 


typedef rte_spinlock_t spinlock_t; 

#define spinlock_init(__lock) \
	rte_spinlock_init(__lock)

#define spin_lock(__lock) \
	rte_spinlock_lock(__lock)

#define spin_unlock(__lock) \
	rte_spinlock_unlock(__lock)

#define spin_lock_bh spin_lock
#define spin_unlock_bh spin_unlock

#define spin_lock_init rte_spinlock_init

#define rwlock_init(__lock) \
	rte_rwlock_init(__lock)

#define read_lock(__lock) \
	rte_rwlock_read_lock(__lock)

#define read_unlock(__lock) \
	rte_rwlock_read_unlock(__lock)

#define write_lock(__lock) \
	rte_rwlock_write_lock(__lock)

#define write_unlock(__lock) \
	rte_rwlock_write_unlock(__lock)

#define write_lock_bh write_lock
#define write_unlock_bh write_unlock

typedef rte_rwlock_t rwlock_t;

#define ATOMIC_INIT(__var) RTE_ATOMIC32_INIT(__var)
typedef rte_atomic32_t atomic_t;
#define atomic_inc(__var) rte_atomic32_inc(__var)
#define atomic_dec(__var) rte_atomic32_dec(__var)
#define atomic_set(__dst, __var) rte_atomic32_set(__dst, __var)
#define atomic_read(__var) rte_atomic32_read(__var)
#define atomic_dec_and_test(__var) rte_atomic32_dec_and_test(__var)

typedef rte_atomic64_t atomic64_t;
#define atomic64_inc(__var) rte_atomic64_inc(__var)
#define atomic64_dec(__var) rte_atomic64_dec(__var)
#define atomic64_set(__var) rte_atomic64_set(__var)
#define atomic64_read(__var) rte_atomic64_read(__var)
#define atomic64_dec_and_test(__var) rte_atomic64_dec_and_test(__var)
#define atomic64_inc_return(__var) rte_atomic64_add_return(__var)

#define rcu_read_lock() ofp_rcu_read_lock
#define rcu_read_unlock() ofp_rcu_read_unlock


#define ETH_ALEN ETHER_ADDR_LEN

/*
 * Check at compile time that something is of a particular type.
 * Always evaluates to 1 so you may use it easily in comparisons.
*/
#define typecheck(type,x) \
({	type __dummy; \
	typeof(x) __dummy2; \
	(void)(&__dummy == &__dummy2); \
	1; \
})


#define time_after64(a, b) \
	(typecheck(uint64_t, a) && \
	 typecheck(uint64_t, b) && \
	((int64_t)(b) - (int64_t)(a) < 0))

#define time_before64(a, b) time_after64(b, a)

#define time_after64_eq(a, b) \
	(typecheck(uint64_t, a) && \
	 typecheck(uint64_t, b) && \
	((int64_t)(b) - (int64_t)(a) <= 0))



#define PROT_SOCK 1024

#define jiffies rte_get_timer_cycles()

#define pr_err(__fmt, args...) \
	OFP_ERR( __fmt, ##args)

#define pr_info(__fmt, args...) \
	OFP_INFO(__fmt, ##args)

#define MAX_ERRNO 4095


#define IS_ERR_VALUE(x) unlikely((x) >= (unsigned long)-MAX_ERRNO)

static inline void *ERR_PTR(long error)
{
    return (void *) error;
}

static inline long PTR_ERR(const void *ptr)
{
    return (long) ptr;
}

static inline long IS_ERR(const void *ptr)
{
    return IS_ERR_VALUE((unsigned long)ptr);
}

static inline long IS_ERR_OR_NULL(const void *ptr)
{
    return !ptr || IS_ERR_VALUE((unsigned long)ptr);
}

/**
 *  * ERR_CAST - Explicitly cast an error-valued pointer to another pointer type
 *   * @ptr: The pointer to cast.
 *    *
 *     * Explicitly cast an error-valued pointer to another pointer type in such a
 *      * way as to make it clear that's what's going on.
 *       */
static inline void *ERR_CAST(const void *ptr)
{
    /* cast away the const */
      return (void *) ptr;
}


#endif