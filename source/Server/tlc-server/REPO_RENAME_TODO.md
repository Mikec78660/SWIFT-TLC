# Repository Rename to TDrive - TODO List

## Objective
Rename the repository from "SWIFT-TLC" to "TDrive" and update local directory structure accordingly.

## Status
- [ ] Check current GitHub repository information
- [ ] Rename GitHub repository to TDrive
- [ ] Update local directory structure to reflect new name
- [ ] Update any configuration files that reference the old name
- [ ] Update documentation and branding
- [ ] Test the changes

## Current Status
- Current working directory: `/opt/SWIFT-TLC/source/Server/tlc-server`
- Current repo: SWIFT-TLC (based on directory structure)
- Target repo: TDrive

## Steps to Complete
1. **GitHub Repository Rename**
   - Use GitHub MCP to rename repository from SWIFT-TLC to TDrive
   
2. **Local Directory Structure Update**
   - Rename main directory from SWIFT-TLC to TDrive
   - Update any hardcoded paths in configuration files
   - Update service files if they contain specific paths
   
3. **Configuration Updates**
   - Update service files if they reference specific paths
   - Update any documentation or README files
   - Update installation scripts if they reference old name
   
4. **Testing**
   - Verify all paths work correctly
   - Test installation process
   - Verify Git operations work with new repo name

## Files That May Need Updates
- Service files with hardcoded paths
- Installation scripts
- Configuration files
- Documentation files
- Git configuration files
