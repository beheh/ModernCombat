/*---- MetroCop ----*/

#strict
#include HZCK

/* Allgemeines */
public func Faction() {return(FACTION_Combine);}

protected func Initialize()
{
  InitCombineAI();
  GiveWeapon(H2PW);
}

/* Geräusche */
public func Death()
{
  var plr = GetOwner();

  // Sound und Meldung
  Sound("MCOP_Die*.ogg");
  DeathAnnounce(GetOwner(),this(),GetKiller());

  if(GetPlayerType(GetOwner()) == C4PT_Script)
    GameCallEx("RelaunchPlayer",GetOwner(),this(), GetKiller());
  else
  {
    var gotcrew;
    for(var i; i < GetCrewCount(plr); i++)
      if(GetOCF(GetCrew(plr,i)) & OCF_Alive)
        gotcrew = true;
  
    if(!gotcrew)
      GameCallEx("RelaunchPlayer",GetOwner(),this(), GetKiller());
  }

  if(ammobag)
    RemoveObject(ammobag);

  // Ausrüstung ablegen
  if(HasGear()) TakeOffGear();

  if(GetEffect(0,0,wpneffect))
    RemoveEffect(0,0,wpneffect);

  FadeOut(this());
  return(1);
}

public func Hurt()
{
  Sound("MCOP_Hurt*.ogg");
  return(1);
}

public func HurtSounds(int iDmg, int iType)
{
  iDmg = -iDmg;
  if(iDmg <= 0) return();
  
  var val = BoundBy(iDmg,1,30);
  if(Random(val)*100/val < 50) return();

  Sound("MCOP_Hurt*.ogg");
}