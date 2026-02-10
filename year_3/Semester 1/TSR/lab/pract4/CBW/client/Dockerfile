FROM tsr-zmq
COPY ./tsr.js tsr.js
RUN mkdir client
WORKDIR client
COPY ./cliente.js myclient.js
#
# Launch a client with the following arguments:
# - HN parameter is special id that indicates the client to get unique host-based id.
# - repeat 10 requests
# - need to edit HOST, change XXX.XXX.XXX.XXX to the right broker IP address
CMD node myclient HN 10 $BROKER_HOST 9998
