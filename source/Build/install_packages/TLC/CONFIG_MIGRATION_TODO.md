# Configuration Files Update - TODO List

## Objective
Update TLC Server configuration files to work optimally with the new systemd-based installation environment.

## Status
- [x] Examine all configuration files in TLC/VS/Config directory
- [x] Analyze existing configurations for systemd compatibility
- [ ] Update log4cplus.cfg to include journald appender for better systemd integration
- [ ] Enhance rsyslog.conf with systemd journal integration
- [ ] Ensure all log directories have proper permissions for systemd
- [ ] Update documentation with new configuration paths

## Configuration Files Analysis

### ✅ **vs_conf.xml** - Main Configuration
- **Status**: No changes needed
- **Reason**: Contains operational parameters, cache settings, and file patterns that are independent of service management system
- **Paths**: Already uses standard paths (/etc/vs/, /var/log/vs/, etc.)

### ✅ **diskSpaceMonitor.cfg** - Disk Monitoring
- **Status**: No changes needed
- **Reason**: Contains threshold values that work with both init.d and systemd
- **Values**: Reasonable defaults (95% low warn, 98% high warn, etc.)

### ✅ **vs** - Logrotate Configuration
- **Status**: No changes needed
- **Reason**: Log rotation works independently of service management system
- **Paths**: Already configured for /var/log/vs/ directory

### ⚠️ **log4cplus.cfg** - Logging Configuration
- **Status**: Minor enhancement recommended
- **Current**: Uses SysLogAppender
- **Recommendation**: Add journald appender for systemd integration

### ⚠️ **rsyslog.conf** - System Logging
- **Status**: Minor enhancement recommended
- **Current**: Routes to /var/log/vs/ files
- **Recommendation**: Add systemd journal integration

## Planned Updates

### 1. log4cplus.cfg Enhancement
Add journald appender for better systemd integration:
```
log4cplus.appender.JOURNALD=log4cplus::JournaldAppender
log4cplus.logger.vfs=INFO, VFS, JOURNALD
log4cplus.logger.vs=INFO, VS, JOURNALD
```

### 2. rsyslog.conf Enhancement
Add systemd journal integration while maintaining file logging:
```
# Enhanced logging for systemd environment
if $msg contains " simulator " then /var/log/vs/simulator.log
if $msg contains " vfs " then /var/log/vs/vfs.log  
if $msg contains " vs " then /var/log/vs/vs.log
if $msg contains " socket " then /var/log/vs/socket.log

# Also forward to systemd journal with proper tagging
:msg, contains, " simulator " :omjournal:tagram="tlc-simulator"
:msg, contains, " vfs " :omjournal:tagram="tlc-vfs"
:msg, contains, " vs " :omjournal:tagram="tlc-vs"
:msg, contains, " socket " :omjournal:tagram="tlc-socket"
```

### 3. Permission Verification
Ensure /var/log/vs/ directory has proper permissions for systemd:
- Owner: root:root
- Permissions: 770 (as already configured in install.sh)

## Benefits of Updates
1. **Better Integration**: Log messages will appear in both traditional files and systemd journal
2. **Centralized Logging**: systemd journal provides unified log access via `journalctl`
3. **Enhanced Debugging**: Log messages tagged with service names for easier filtering
4. **Modern Compatibility**: Full compatibility with systemd-based distributions

## Files to Update
1. `/opt/SWIFT-TLC/source/Build/install_packages/TLC/VS/Config/log4cplus.cfg`
2. `/opt/SWIFT-TLC/source/Build/install_packages/TLC/VS/Config/rsyslog.conf`
