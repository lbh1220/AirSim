#!/usr/bin/env bash
# export_airsim_libs.sh
# 用法： ./export_airsim_libs.sh [/自定义/目标目录]
set -euo pipefail

SRC_ROOT="$(pwd)/Unreal/Plugins/AirSim"                   # 源插件根
DEST_ROOT="${1:-/mnt/shared_data/OneDrive/Phd/Projects/airsim/linux-plugin/AirSim}"       # 目标根（可改）

FILES=(
  Source/AirLib/deps/MavLinkCom/lib/libMavLinkCom.a
  Source/AirLib/deps/rpclib/lib/librpc.a
  Source/AirLib/lib/libAirLib.a
)

echo "目标目录: $DEST_ROOT"

for rel in "${FILES[@]}"; do
  src="$SRC_ROOT/$rel"
  dest="$DEST_ROOT/$rel"

  [[ -f "$src" ]] || { echo "缺少文件: $src"; exit 1; }

  mkdir -p "$(dirname "$dest")"
  cp -p "$src" "$dest"
done

echo "复制完成"
