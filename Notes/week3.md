# Week 3
## Type of Delays
1. Propagation Delay: l/c
2. Packetization Delay: p/r
3. Total time to send a packet across a link: p/r + l/c
4. end to end delay = sum(t + Q(t))
## Simple Model of a Router Queue
Q(t) = A(t) - D(t)
1. Q(t): queuing packets at time t
2. A(t): arrival packets at time t
3. D(t): departure packets at time t
4. question: average occupancy - area of integration(A(t) - D(t)) / time length
## Queuing Algorithms
1. FIFO: No priorities, no guaranteed rates
2. Strict Priorities: high priority traffic "sees" a network with no low priority traffic
3. Weighted Fair Queueing (WFQ): scheduling flows in order of their bit-by-bit finishing times
## Guarantee the Delay
### I. Leaky Bucket
1. send packet if and only if we have tokens in the bucket
2. Number of bits that can be sent in any period of length t is bounded by: `σ + ρt`, also called a `(σ, ρ)` regulator.
3. If `ΦR > ρ` and `b > σ` then delay through the first router for all packets in the flow ≤ `b / ΦR`
4. Φ: channel weight; ρ: token rate; σ: bucket size