FROM tsr-zmq
COPY ./tsr.js tsr.js
RUN mkdir worker
WORKDIR worker
COPY ./workerReq.js myworker.js
#
# Launch a worker with the following arguments:
# - HN parameter is special id that indicates the worker to get unique host-based id.
# - broker host as specified by docker-compose.yml
CMD node myworker HN $BROKER_HOST 9999
