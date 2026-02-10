FROM tsr-zmq
COPY ./tsr.js tsr.js
RUN mkdir logger
WORKDIR logger
COPY ./logger.js mylogger.js
VOLUME /tmp/cbwlog
EXPOSE 9995
CMD node mylogger 9995 $LOGGER_DIR/logs
