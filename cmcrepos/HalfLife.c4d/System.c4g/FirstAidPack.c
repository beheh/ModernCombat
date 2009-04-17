#strict
#appendto FAPK

protected func Activate(object caller)
{
  var clonk;
  if(clonk = FindObject2(Find_AtPoint(0,GetDefBottom(caller)-GetY()-3),Find_ID(FKDT)))
  {
    clonk = clonk->GetClonk();
  
    DoHealPoints(-100);
    clonk->StopFakeDeath();
    Sound("Defibrillator");
    caller->CastObjects(SPSK,5,20);
    caller->AddLightFlash(40+Random(20),0,0,RGB(0,140,255));
    EventInfo4K(0,Format("$PlrRescued$",GetPlayerName(GetOwner(caller)),GetPlayerName(GetOwner(clonk))),GetID(),GetPlrColorDw(GetOwner(caller)));
    return(1);
  }
  if(GetEffect("PCRB_Poison",caller))
  {
    DoHealPoints(-20);
    RemoveEffect("PCRB_Poison",caller);
  }
  return(_inherited(caller));
}