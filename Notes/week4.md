# Week 4
## Why Congestion Control?
### I. TCP and flow control
1. TCP provides a flow-controlled bidirectional byte stream
2. Flow-controlled: Sender respects receiver's capacity
3. But...what about the network's capacity
### II. Window: cap on number of bytes "outstanding"
1. outstanding means sent, and not acked or judged lost.
2. what if the window size is really small: slow
3. what if the window size is really big: a. sender transmits too many segments. Most overflow router's queue and are dropped. b. we call this "congestion". c. Sender must resend the same bytes again and again. Eventually, stream comes out of receiver's TCP correctly
### III. The problem with unlimited sending: collapse and fairness
1. forcing routers to drop lots of packets can lead to "congestion collapse"
2. when some flows send too much, others are starved, network exihits bad "fairness"
### IV. Alpha Fairness
### V. Other "group" objectives
1. minimize flow completion time (or average download)
2. minimize page load time (of websiete with many downloads)
3. maximize "power" (= throughput/delay)
## What Congestion Control
### I. TCP and flow control
1. TCP provides a `flow-controlled` bidirectional byte stream
2. `flow-controlled`: sender respects receiver's capacity
### II. a second window
1. tighter one controls: receiver's window, congestion window (maintained by sender)
2. the congestion window caps # of bytes in flight
### III. BDP - Bandwidth Delay Product
1. bandwidth x delay product
2. BDP is ideal window for that flow (N flows -> each flow could use cwnd = BDP/N)
3. "No loss" window: anything less than BDP + max queue size
## How Congestion Control
### I. problems - TCP sender doesn't know
1. bottleneck link rate
2. min RTT (without queueing)
3. number of other flows
### II. One Possibility: Increase on Success, Decrease on loss
1. AIMD (Additive Increase, Multiplicative Decrease) algorithm
2. starts with cwnd at small value
3. On success, increase by 1 segment per RTT
4. On loss, assume congestion, cut cwnd in half
5. loss inferred when: a. timeout. b. several later-sent segments have been acknowledged
### III. slow-start: exp growth at the beginning
1. start cwnd = 3 segments
2. on success: on each byte acknowledged: cwnd++
3. on first loss, cut cwnd in half and revert to AIMD
