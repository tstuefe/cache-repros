# Cache experiments

## Find out your topology

- lscpu (reports total L1/L2/L3 sizes, not per core)
- https://www.open-mpi.org/projects/hwloc (very nice, can be installed via normal Debian repos), reports cache sizes per core

## Preface: profiling

### perf

#### Prepare perf

To run without sudo:

sudo sysctl -w kernel.perf_event_paranoid=X

perf_event_paranoid:

Controls use of the performance events system by unprivileged users (without CAP_SYS_ADMIN). The default value is 2.

- -1: Allow use of (almost) all events by all users Ignore mlock limit after perf_event_mlock_kb without CAP_IPC_LOCK
- >=0: Disallow ftrace function tracepoint by users without CAP_SYS_ADMIN Disallow raw tracepoint access by users without CAP_SYS_ADMIN
- >=1: Disallow CPU event access by users without CAP_SYS_ADMIN
- >=2: Disallow kernel profiling by users without CAP_SYS_ADMIN

#### Use perf

`perf stat -e cache-references,cache-misses <program>`

executes program and shows total cache misses and references

`perf record -e cache-misses <program>`
`perf report --stdio`
`perf annotate`

executes program, measures total cache misses and references, and shows code locations.

Note: Skew is an issue here, at least on my machine, its bad. Code locations seem often to be off-by-(1..3). Out-of-order execution hurts here. With a bit of luck and logic counters are still attributable to the right instruction though.

(todo evaluate: Intel PT ?)

`perf mem record -a <prog>`

(Unfortunately not supported on my CPU)

### AMD uProf

Free Tool. Can be downloaded.

Install deb package. Comes with graphical frontend. Needs kernel.perf_event_paranoid too.




### Documentations

Red Hat has good documentation:
https://developers.redhat.com/blog/2014/03/10/determining-whether-an-application-has-poor-cache-performance-2
https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/8/html/monitoring_and_managing_system_status_and_performance/profiling-memory-accesses-with-perf-mem_monitoring-and-managing-system-status-and-performance

Very good:
https://www.reddit.com/r/Amd/comments/9quk0c/amd_zen_performance_counters_review_low_level/

## Demos

### hyper

Test demonstrates hyper-alignment effects (what happens if one naively aligns large structures along 2^N boundaries,
with N > 6 (64 bytes being the typical cacheline size of most CPUs). Theory says with N=7 you would only use every second cache line, N=8 every fourth and
so forth.

build:
	make hyper

Usage:

	hyper <stride>
	
	e.g. 	hyper 64
		hyper 256 (would be overaligned, cache misses should rise drastically)
		hyper 256 +- 64 (should use cache more efficiently)


Execute withW
perf stat -e ache-references,cache-misses hyper <stride>
//
// stride=64: optimal, every access fills a cacheline
// stride=128 or 256: hyper-aligned, number of cache misses raises dramatically since we only use 1/2 or 1/4 of the cache
// stride=256+64: again, optimal

