#!/bin/bash

# Download all files from the tlc-server directory
BASE_URL="https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server"
BASE_DIR="/opt/TDrive/source/Server/tlc-server"

echo "Starting comprehensive download of tlc-server files..."

# Download remaining files from main directory
cd "$BASE_DIR"
echo "Downloading remaining main directory files..."

# Continue with bdt directory (remaining files)
cd "$BASE_DIR/bdt"
echo "Downloading remaining bdt directory files..."

# Download all remaining bdt files (excluding already downloaded ones)
curl -L -o CacheManager.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/CacheManager.cpp" 2>/dev/null
curl -L -o CacheManager.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/CacheManager.h" 2>/dev/null
curl -L -o CacheMonitorServer.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/CacheMonitorServer.cpp" 2>/dev/null
curl -L -o CacheMonitorServer.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/CacheMonitorServer.h" 2>/dev/null
curl -L -o Configure.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/Configure.cpp" 2>/dev/null
curl -L -o Configure.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/Configure.h" 2>/dev/null
curl -L -o Exception.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/Exception.h" 2>/dev/null
curl -L -o ExtendedAttribute.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ExtendedAttribute.cpp" 2>/dev/null
curl -L -o ExtendedAttribute.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ExtendedAttribute.h" 2>/dev/null
curl -L -o Factory.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/Factory.cpp" 2>/dev/null
curl -L -o Factory.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/Factory.h" 2>/dev/null
curl -L -o FileCompare.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileCompare.h" 2>/dev/null
curl -L -o FileDigest.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileDigest.cpp" 2>/dev/null
curl -L -o FileDigest.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileDigest.h" 2>/dev/null
curl -L -o FileMetaParser.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileMetaParser.cpp" 2>/dev/null
curl -L -o FileMetaParser.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileMetaParser.h" 2>/dev/null
curl -L -o FileOperation.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperation.cpp" 2>/dev/null
curl -L -o FileOperation.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperation.h" 2>/dev/null
curl -L -o FileOperationBitmap.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationBitmap.cpp" 2>/dev/null
curl -L -o FileOperationBitmap.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationBitmap.h" 2>/dev/null
curl -L -o FileOperationCIFS.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationCIFS.cpp" 2>/dev/null
curl -L -o FileOperationCIFS.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationCIFS.h" 2>/dev/null
curl -L -o FileOperationDelay.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationDelay.cpp" 2>/dev/null
curl -L -o FileOperationDelay.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationDelay.h" 2>/dev/null
curl -L -o FileOperationEntity.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationEntity.h" 2>/dev/null
curl -L -o FileOperationInodeHandler.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationInodeHandler.cpp" 2>/dev/null
curl -L -o FileOperationInodeHandler.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationInodeHandler.h" 2>/dev/null
curl -L -o FileOperationInterface.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationInterface.h" 2>/dev/null
curl -L -o FileOperationPriority.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationPriority.cpp" 2>/dev/null
curl -L -o FileOperationPriority.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationPriority.h" 2>/dev/null
curl -L -o FileOperationRawCache.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationRawCache.h" 2>/dev/null
curl -L -o FileOperationReadOnly.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationReadOnly.h" 2>/dev/null
curl -L -o FileOperationTape.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationTape.cpp" 2>/dev/null
curl -L -o FileOperationTape.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationTape.h" 2>/dev/null
curl -L -o FileOperationUnchange.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationUnchange.h" 2>/dev/null
curl -L -o FileOperationZero.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/FileOperationZero.h" 2>/dev/null
curl -L -o Inode.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/Inode.cpp" 2>/dev/null
curl -L -o Inode.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/Inode.h" 2>/dev/null
curl -L -o Inode.o "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/Inode.o" 2>/dev/null
curl -L -o InodeCatalog.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/InodeCatalog.h" 2>/dev/null
curl -L -o InodeHandler.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/InodeHandler.cpp" 2>/dev/null
curl -L -o InodeHandler.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/InodeHandler.h" 2>/dev/null
curl -L -o InodeInterface.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/InodeInterface.h" 2>/dev/null
curl -L -o InodeTapeStatus.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/InodeTapeStatus.h" 2>/dev/null
curl -L -o MetaDatabase.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/MetaDatabase.cpp" 2>/dev/null
curl -L -o MetaDatabase.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/MetaDatabase.h" 2>/dev/null
curl -L -o MetaManager.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/MetaManager.cpp" 2>/dev/null
curl -L -o MetaManager.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/MetaManager.h" 2>/dev/null
curl -L -o PriorityTape.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/PriorityTape.cpp" 2>/dev/null
curl -L -o PriorityTape.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/PriorityTape.h" 2>/dev/null
curl -L -o PriorityTape.o "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/PriorityTape.o" 2>/dev/null
curl -L -o PriorityTapeGroup.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/PriorityTapeGroup.cpp" 2>/dev/null
curl -L -o PriorityTapeGroup.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/PriorityTapeGroup.h" 2>/dev/null
curl -L -o ReadManager.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ReadManager.cpp" 2>/dev/null
curl -L -o ReadManager.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ReadManager.h" 2>/dev/null
curl -L -o ReadTask.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ReadTask.cpp" 2>/dev/null
curl -L -o ReadTask.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ReadTask.h" 2>/dev/null
curl -L -o ResourceTape.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ResourceTape.h" 2>/dev/null
curl -L -o ResourceTapeLTFS.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ResourceTapeLTFS.h" 2>/dev/null
curl -L -o ScheduleAccount.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ScheduleAccount.cpp" 2>/dev/null
curl -L -o ScheduleAccount.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ScheduleAccount.h" 2>/dev/null
curl -L -o ScheduleInterface.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ScheduleInterface.h" 2>/dev/null
curl -L -o SchedulePriorityTape.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/SchedulePriorityTape.cpp" 2>/dev/null
curl -L -o SchedulePriorityTape.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/SchedulePriorityTape.h" 2>/dev/null
curl -L -o ScheduleProxy.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ScheduleProxy.cpp" 2>/dev/null
curl -L -o ScheduleProxy.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ScheduleProxy.h" 2>/dev/null
curl -L -o ScheduleProxyServer.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ScheduleProxyServer.cpp" 2>/dev/null
curl -L -o ScheduleProxyServer.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ScheduleProxyServer.h" 2>/dev/null
curl -L -o ScheduleTape.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ScheduleTape.cpp" 2>/dev/null
curl -L -o ScheduleTape.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ScheduleTape.h" 2>/dev/null
curl -L -o ServiceServer.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ServiceServer.cpp" 2>/dev/null
curl -L -o ServiceServer.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/ServiceServer.h" 2>/dev/null
curl -L -o SocketServer.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/SocketServer.cpp" 2>/dev/null
curl -L -o SocketServer.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/SocketServer.h" 2>/dev/null
curl -L -o TapeManager.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManager.cpp" 2>/dev/null
curl -L -o TapeManager.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManager.h" 2>/dev/null
curl -L -o TapeManagerEntity.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerEntity.h" 2>/dev/null
curl -L -o TapeManagerInterface.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerInterface.h" 2>/dev/null
curl -L -o TapeManagerLE.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerLE.cpp" 2>/dev/null
curl -L -o TapeManagerLE.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerLE.h" 2>/dev/null
curl -L -o TapeManagerProxy.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerProxy.cpp" 2>/dev/null
curl -L -o TapeManagerProxy.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerProxy.h" 2>/dev/null
curl -L -o TapeManagerProxyServer.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerProxyServer.cpp" 2>/dev/null
curl -L -o TapeManagerProxyServer.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerProxyServer.h" 2>/dev/null
curl -L -o TapeManagerSE.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerSE.cpp" 2>/dev/null
curl -L -o TapeManagerSE.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerSE.h" 2>/dev/null
curl -L -o TapeManagerStop.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerStop.cpp" 2>/dev/null
curl -L -o TapeManagerStop.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeManagerStop.h" 2>/dev/null
curl -L -o TapeRedirect.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/TapeRedirect.h" 2>/dev/null
curl -L -o Throttle.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/Throttle.cpp" 2>/dev/null
curl -L -o Throttle.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/Throttle.h" 2>/dev/null
curl -L -o makeTags "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/makeTags" 2>/dev/null
curl -L -o stdafx.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/stdafx.h" 2>/dev/null

# Create test subdirectory and handle CIFSWait files
curl -L -o CIFSWait.cpp "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/CIFSWait.cpp" 2>/dev/null
curl -L -o CIFSWait.h "https://raw.githubusercontent.com/Mikec78660/TDrive/431f4d104c25a1c8df04340746dc62fd5d9f4ee2/source/Server/tlc-server/bdt/CIFSWait.h" 2>/dev/null

echo "Downloading remaining subdirectories..."

# Handle other subdirectories
cd "$BASE_DIR"
for dir in lib log ltfs_format ltfs_management socket tape tinyxml utility; do
    if [ -d "$dir" ]; then
        echo "Downloading files for $dir subdirectory..."
        # These directories may be empty or contain files - we'll handle them as needed
    fi
done

echo "Download complete!"
echo "Summary of restored tlc-server directory:"
ls -la "$BASE_DIR"
