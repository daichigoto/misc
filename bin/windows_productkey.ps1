#!/usr/bin/env pwsh

#========================================================================
# Windows のプロダクトキーを表示するスクリプト
#========================================================================

wmic path softwarelicensingservice get OA3xOriginalProductKey
