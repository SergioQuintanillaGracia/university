FROM tsr-zmq
COPY ./tsr.js tsr.js
RUN mkdir broker
WORKDIR broker
COPY ./brokerl.js mybroker.js
EXPOSE 9998 9999
CMD node mybroker 9998 9999 $LOGGER_HOST $LOGGER_PORT
