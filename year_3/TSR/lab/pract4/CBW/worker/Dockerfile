FROM tsr-zmq
COPY ./tsr.js tsr.js
RUN mkdir worker
WORKDIR worker
COPY ./workerReq.js myworker.js
#
# Launch a worker with the following arguments:
# - HN parameter is special id that indicates the worker to get unique host-based id.
# - need to edit HOST, change XXX.XXX.XXX.XXX to the right broker IP address
CMD node myworker HN $BROKER_HOST 9999
