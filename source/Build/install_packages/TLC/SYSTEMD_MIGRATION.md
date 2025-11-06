# Systemd Migration for ValueStor (TLC Server)

## Overview
This document describes the migration from init.d to systemd for the ValueStor TLC Server installation system.

## Background
The original ValueStor installation used init.d scripts for service management:
- `/etc/init.d/vs` - Main VFS service
- `/etc/init.d/chkIsDirty` - Dirty shutdown detection
- Cron-based watchdog (`/etc/crontab`)

## Migration to Systemd

### New Service Files Created

#### 1. vs.service
- **Location**: `/etc/systemd/system/vs.service`
- **Purpose**: Main ValueStor VFS service
- **Features**:
  - Automatic dependency on mariadb.service
  - Forking service type for background daemon
  - Configuration file validation
  - Automatic restart on failure
  - Watchdog management integration
  - Cache cleanup on shutdown
  - 300 second timeout for graceful shutdown

#### 2. chkIsDirty.service
- **Location**: `/etc/systemd/system/chkIsDirty.service`
- **Purpose**: Detect dirty shutdowns
- **Features**:
  - One-shot service type
  - Creates/destroys flag files in `/var/tmp/`
  - Automatic execution on system boot

#### 3. watchdog.service
- **Location**: `/etc/systemd/system/watchdog.service`
- **Purpose**: Monitor VFS and rsyncd services
- **Features**:
  - Checks VFS service health
  - Monitors rsyncd service
  - Automatic restart of failed services
  - Simulator version detection
  - Logging to `/var/log/vs/watchdog.log`

#### 4. watchdog.timer
- **Location**: `/etc/systemd/system/watchdog.timer`
- **Purpose**: Schedule watchdog.service execution
- **Features**:
  - Runs every minute (`OnCalendar=minutely`)
  - Persistent across reboots
  - Integrates with systemd timer system

### Updated Installation Script Changes

#### chkonSrv() Function
- **Before**: Copied init.d scripts and used chkconfig
- **After**: Copies systemd service files and uses systemctl
- **Actions**:
  1. Creates `/etc/systemd/system/` directory if needed
  2. Copies all .service and .timer files
  3. Runs `systemctl daemon-reload`
  4. Enables services: `chkIsDirty.service`, `vs.service`, `watchdog.timer`
  5. Starts `chkIsDirty.service` first

### Service Dependencies
```
chkIsDirty.service → local-fs.target
vs.service → network.target + mariadb.service → chkIsDirty.service
watchdog.timer → watchdog.service → vs.service
```

### Service Management Commands

#### Old Commands (init.d)
```bash
# Start service
/etc/init.d/vs start

# Stop service
/etc/init.d/vs stop

# Restart service
/etc/init.d/vs restart

# Check status
/etc/init.d/vs status
```

#### New Commands (systemd)
```bash
# Start service
systemctl start vs.service

# Stop service
systemctl stop vs.service

# Restart service
systemctl restart vs.service

# Check status
systemctl status vs.service

# Enable auto-start
systemctl enable vs.service

# Disable auto-start
systemctl disable vs.service

# Check timer status
systemctl status watchdog.timer
systemctl list-timers
```

### Installation Process Changes

#### During Installation
1. Systemd service files are copied to `/etc/systemd/system/`
2. Systemd daemon is reloaded
3. Services are enabled for auto-start
4. chkIsDirty.service is started first
5. Watchdog timer is enabled

#### Service Start Messages
- **Before**: "Please check everything ready then start the VStor service with command: /etc/init.d/vs start"
- **After**: "Please check everything ready then start the VStor service with command: systemctl start vs.service"

### Benefits of Systemd Migration

1. **Better Dependency Management**: Services start in correct order
2. **Automatic Restart**: Failed services restart automatically
3. **Standard Interface**: Uses universal systemctl commands
4. **Timer Management**: Built-in timer system replaces cron
5. **Logging Integration**: Journald integration for better logging
6. **Resource Management**: Better control over service resources
7. **Security**: Sandboxing and security policies available
8. **Socket Activation**: Support for socket-based activation

### Uninstallation Changes

#### Old Uninstallation
- Removed init.d scripts
- Ran `chkconfig --del` commands
- Deleted symlinks in rc.d directories

#### New Uninstallation
- Removes systemd service files
- Disables and stops services
- Removes timer files
- Cleans up systemd daemon

### Migration Verification

To verify the migration is successful:

```bash
# Check all services are installed
ls -la /etc/systemd/system/vs.service
ls -la /etc/systemd/system/chkIsDirty.service
ls -la /etc/systemd/system/watchdog.service
ls -la /etc/systemd/system/watchdog.timer

# Check services are enabled
systemctl is-enabled chkIsDirty.service
systemctl is-enabled vs.service
systemctl is-enabled watchdog.timer

# Check timer is active
systemctl list-timers | grep watchdog

# Test service start/stop
systemctl start vs.service
systemctl status vs.service
systemctl stop vs.service
```

### Path Conflicts Resolved

1. **Service File Locations**: Changed from `/etc/init.d/` to `/etc/systemd/system/`
2. **PID File Handling**: Maintained compatibility with existing PID file location
3. **Log File Locations**: Preserved existing log paths
4. **Configuration Files**: No changes to existing config file locations

### Rollback Plan

If systemd migration needs to be rolled back:

1. Remove systemd service files: `rm /etc/systemd/system/vs.service`
2. Reload systemd: `systemctl daemon-reload`
3. Reinstall init.d scripts: Copy back original init.d files
4. Re-enable init.d services: `chkconfig --add vs`

### Testing Checklist

- [ ] All service files installed correctly
- [ ] Services start without errors
- [ ] Services stop gracefully
- [ ] Auto-start enabled correctly
- [ ] Watchdog timer runs every minute
- [ ] chkIsDirty service detects dirty shutdowns
- [ ] Service dependencies work correctly
- [ ] Log files written to correct locations
- [ ] Uninstallation removes all systemd files

## Summary

The systemd migration provides a modern, robust service management system while maintaining backward compatibility with existing ValueStor functionality. All services now use systemd's advanced features for better reliability, monitoring, and management.
