#!/usr/bin/env python
# -*- coding: utf-8 -*-

## Get SHA1 of last commit of current git repository

## Bjoern Hendriks 2010, hendriks@ibr.cs.tu-bs.de

# This script can be either called as command or used as Python module

resultNoGitRepo = 2         # we're not in a valid git repo
resultDirty = 1             # not all files are committed
resultOk = 0                # clean repo

import sys

def getRevision():
    """determine current SHA1

    Returns a tupel with the SHA1 or a message and a code number for further processing.
    """
    unstagedTag = 'with unstaged changes'
    uncommittedTag = 'with uncommitted changes'

    import subprocess

    # get current commit's SHA1
    revParse = subprocess.Popen(args=["git", "rev-parse", "HEAD"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if revParse.wait():     # returncode != 0?
	return ("no git repository", resultNoGitRepo)
    sha1 = revParse.communicate()[0].strip()

    # check if all staged
    if subprocess.call(args=["git", "diff", "--exit-code"], stdout=subprocess.PIPE, stderr=subprocess.PIPE):
	return (sha1 + " (" + unstagedTag + ")", resultDirty)

    # check if all committed
    if subprocess.call(args=["git", "diff", "--cached", "--exit-code"], stdout=subprocess.PIPE, stderr=subprocess.PIPE):
	return (sha1 + " (" + uncommittedTag + ")", resultDirty)

    # print result
    return (sha1, resultOk)



# =======================================================================================
# called from command line?
if __name__ == "__main__":
    # parse command options
    (revText, exitCode) = getRevision()
    print revText
    sys.exit(exitCode)
