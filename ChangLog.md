## 1.3.1 (2013/10/26) ##
  * fixed the registry access under x64, again...

## 1.3.0 (2013/10/20) ##
  * update with latest configuration options
  * add tooltips for the first page
  * unsupported options
    * coproc
    * exclude\_gpu

## 1.2.5 ##
  * fixed the registry access under win7 x64
  * solution files upgraded to VS2010

## 1.2.4 ##
  * bug fix (two break statements in a switch-case clause were omitted inadvertently)

## 1.2.3 ##
  * update MRU list on saving file also

## 1.2.2 ##
  * the first item in MRU list works now (replace the MFC-provided ID\_FILE\_MRU\_FILE with user-defined menu IDs)
  * the Save button and all option pages won't be disabled even if no file is loaded (otherwise a new file could never be created from scratch)
  * if no file is loaded and user clicked the save button, prompt user to specify a directory (default is the local BOINC data directory)
  * add a read-only edit box to show the path of the currently loaded file

## 1.2.1 ##
  * call boinccmd to notify the remote client when cc\_config.xml has been changed (suggested by cuihao@equn)
  * allow saving current configuration to some other file than the original one (suggested by John Mcleod)
  * disable some controls when no file is loaded
  * show balloon tool tip on some controls (suggested by DrLingCN@equn)
  * continue improving CXButtonXP
  * fixed file deletion

## 1.2.0 ##
  * call boinccmd to notify the local boinc client when cc\_config.xml has been changed (suggested by cuihao@equn)

## 1.1.1 ##
  * replace CSplitButton with a modified [CXButtonXP](http://www.codeproject.com/KB/buttons/XButtonXP.aspx) (partially based on [CCoolBtn](http://www.codeproject.com/KB/buttons/pushmenubutton.aspx))

## 1.1.0 ##
  * load/save any local-accessible cc\_config.xml (suggested by John Mcleod)
  * keep recently opened file list (suggested by John Mcleod)

## 1.0.0 ##
  * first public version
  * load/save local in-use cc\_config.xml