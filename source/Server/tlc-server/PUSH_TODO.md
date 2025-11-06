# GitHub Push Task - TODO Checklist

## Overview
Push the Debian 13 compilation fixes to the GitHub repository 0.10.10 branch.

## Files to Push
Based on `git status`, the following files contain compilation fixes:

### Modified Files (need to be staged and committed)
- [ ] bdt/ExtendedAttribute.cpp - Fixed header path (attr/xattr.h → sys/xattr.h)
- [ ] bdt/FileDigest.cpp - Fixed OpenSSL 3.x API (EVP_MD_CTX_destroy → EVP_MD_CTX_free)  
- [ ] bdt/FileMetaParser.cpp - Fixed Python 3 API (PyString → PyUnicode migration)
- [ ] bdt/Inode.cpp - Fixed header path and errno (ENOATTR → ENODATA)
- [ ] bdt/PriorityTape.h - Fixed C++20 const-correctness (operator() const)
- [ ] bdt/stdafx.h - Added errno.h include

### Untracked Files (new files to add)
- [ ] COMPILATION_TODO.md - Comprehensive documentation of all compilation fixes
- [ ] GITHUB_PUSH_TODO.md - Project tracking documentation
- [ ] Makefile.debian13 - Debian 13 build system configuration

## Implementation Steps
- [ ] Stage all modified and untracked files
- [ ] Commit with descriptive message about Debian 13 compilation fixes
- [ ] Push commit to 0.10.10 branch on GitHub
- [ ] Verify files are accessible in repository
- [ ] Update TODO list progress

## GitHub Repository Details
- Repository: SWIFT-TLC
- Owner: Mikec78660  
- Target Branch: 0.10.10
- Current Branch: master

## Success Criteria
- All compilation fix files are pushed to 0.10.10 branch
- Files are accessible via GitHub web interface
- Documentation files are included
- Commit message clearly describes the changes
