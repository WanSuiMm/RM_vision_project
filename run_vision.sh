\#!/bin/bash

\# RoboMaster Vision 一键启动脚本

PROJECT\_ROOT="/home/nvidia/RM\_Vision"
BUILD\_DIR="${PROJECT\_ROOT}/build"
BIN\_NAME="rm\_vision"
LOG\_DIR="${PROJECT\_ROOT}/logs"

if \[ ! -d "$LOG\_DIR" ]; then
mkdir -p "$LOG\_DIR"
fi

while true; do
echo "\[$(date)] Starting RM Vision..." >> "${LOG\_DIR}/runtime.log"
cd "$BUILD\_DIR"
./$BIN\_NAME >> "${LOG\_DIR}/output.log" 2>\&1
echo "\[$(date)] Program crashed with exit code $?. Restarting in 1s..." >> "${LOG\_DIR}/runtime.log"
sleep 1
done