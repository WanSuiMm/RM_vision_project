先插入串口模块，终端输入 lsusb
查找VID，PID
然后创建文件 /etc/udev/rules.d/99-rm-serial.rules
写
KERNEL=="ttyUSB\*", ATTRS{idVendor}==VID, ATTRS{idProduct}==PID, MODE="0777", SYMLINK+="rm\_serial"
重载规则
sudo udevadm control --reload-rules
sudo udevadm trigger

然后创建run\_vision.sh，编写一键启动脚本
假设项目文件在linux的/home/nvidia/RM\_Vision
文件位置：/home/nvidia/RM\_Vision/run\_vision.sh

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

赋予执行权限：
chmod +x /home/nvidia/RM\_Vision/run\_vision.sh

Systemd 服务配置 (rm\_vision.service)
文件位置：/etc/systemd/system/rm\_vision.service
\[Unit]
Description=RoboMaster Vision System Service
After=network.target multi-user.target

\[Service]
Type=simple
User=nvidia
Group=nvidia
Environment="DISPLAY=:0"
Environment="XAUTHORITY=/home/nvidia/.Xauthority"
ExecStart=/bin/bash /home/nvidia/RM\_Vision/run\_vision.sh
Restart=always
RestartSec=3

\[Install]
WantedBy=multi-user.target

