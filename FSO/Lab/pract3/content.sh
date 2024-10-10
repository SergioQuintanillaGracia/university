#!/bin/bash
process_count=$(ls -d /proc/[1-9]* | wc -l)
echo Process count: $process_count
