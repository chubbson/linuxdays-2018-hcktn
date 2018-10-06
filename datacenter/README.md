## PROBLEM: We want to learn/explore Malamute project

### Endpoint
On blackboard

### Ups monitor protocol

Connects UPS peer to MONITOR peer.

UPS peer creates a malamute client and connects to endpoint address with a unique name.
UPS peer publishes the following two messages on 'upses' STREAM channel:

* name/ON
* name/OF

where 'name' is a unique name of a UPS and '/' indicates a multipart (string) message

Each UPS peer MUST publish  3 different 'name's with random 1-5 second intervals (simulate 3 devices).

The MONITOR peer subscribes to ".\*" (everything) on 'upses' STREAM channel and (for now) just prints stuff.








