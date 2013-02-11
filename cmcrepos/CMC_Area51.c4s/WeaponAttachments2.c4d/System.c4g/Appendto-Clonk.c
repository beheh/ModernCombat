/*-- Clonk --*/

#strict 2
#appendto CLNK

local killattachment;

public func KillAttachment(int iKillAttachment)
{
  if(iKillAttachment)
    killattachment = iKillAttachment;
  return killattachment;
}
