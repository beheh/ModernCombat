/*-- Terminal --*/

#strict

local szText, fNewInfo;


/* Initalisierung */

protected func Initialize()
{
  szText = "";
  fNewInfo = false;
}

protected func ControlUp(object caller)
{
  CreateMenu(_SGN, caller, caller, 0, "", 0, C4MN_Style_Dialog);
  AddMenuItem(Format("Portrait:%i::ffffff::1", _SGN), "", NONE, caller, 0, 0, "", 5, 0, 0);
  AddMenuItem(Format("$TxtMsg$", GetName(caller), szText), "",
              NONE, caller, 0, 0, "", 512, 0, 0);
  fNewInfo = false;
  SetAction("Still");
}

public func SetNewInfo(bool fSet, string sText)
{
  if(sText)
    szText = sText;
  if(fSet)
  {
    SetAction("Blink");
    fNewInfo = true;
  }
  else
  {
    SetAction("Still");
    fNewInfo = false;
  }
}