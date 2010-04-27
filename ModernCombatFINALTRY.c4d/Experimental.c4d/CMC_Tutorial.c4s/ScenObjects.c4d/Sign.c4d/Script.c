/*-- Schild --*/

#strict

local Text, NewInfo;


/* Initalisierung */

protected func Initialize()
{
  Text = "<i>leervcgggggggg ggg gggggggggg gggggggggggggggg ggggg gggggggggggg</i>";
  SetAction("Blink");    
}

protected func ControlUp(object caller)
{
  CreateMenu(_SGN, caller, caller, 0, "", 0, C4MN_Style_Dialog);
  AddMenuItem(Format("Portrait:%i::ffffff::1", _SGN), "", NONE, caller, 0, 0, "", 5, 0, 0);
  AddMenuItem(Format("$TxtMsg$", GetName(caller), Text), "",
              NONE, caller, 0, 0, "", 512, 0, 0);
  NewInfo = false;
  SetAction("Still");
}

public func SetNewInfo(bool fSet, sText)
{
  if(sText)
    Text = sText;
  if(fSet)
  {
    SetAction("Blink");
    NewInfo = true;
  }
  else
  {
    SetAction("Still");
    NewInfo = false;
  }
}
