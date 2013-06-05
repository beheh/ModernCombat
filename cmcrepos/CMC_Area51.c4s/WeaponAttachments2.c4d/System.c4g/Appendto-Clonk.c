/*-- Clonk --*/

#strict 2
#appendto CLNK

local killattachment;

public func KillAttachment(int iKillAttachment, bool fReset)
{
  if(fReset)
    killattachment = iKillAttachment;
  return killattachment;
}