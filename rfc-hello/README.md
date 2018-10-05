== RFC-HELLO

Connects PROVIDER peer to USER peer

USER creates a REQ socket and connects to PROVIDER's tcp address, port 5555 and sends the following messages:

* HELLO

PROVIDER creates a REP socket and binds on all interfaces on port 5555 and replies with:

* OK
* ERROR/<reason>, where '/' means multipart string message
