/*-- Fässer 'n' Stuff --*/

#strict 2
#appendto XWCR
#appendto PBRL
#appendto XBRL
#appendto HBRL
#appendto GSBL

public func IsExplosive()
{
  return true;
}

private func Check()
{
  if(FindObject2(Find_Func("IsExplosive"),Find_Distance(20),Find_Exclude(this)))
  {
    PlayerMessage(GetOwner(),"$TooMuch$");
    CreateParticle("PSpark",0,0,0,0,5*30,RGB(255));
    
    if(GetID() == HBRL || GetID() == XBRL)
      var value = 15;
    else
      var value = 10;
    
    DoWealth(GetOwner(),value);
    SetOwner(-1);
    RemoveObject();
  }
  
  if(GetID() != XWCR)
    AutoRespawn();
}
  
  
