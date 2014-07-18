/*-- Munitionspack --*/

#strict 2
#include AMPK
#include PCK2

public func StartPoints()		{return 400;}
public func MaxPoints()			{return 400;}
public func RefillTime()		{return 0;}

public func IsEquipment()		{return !NoAmmo();}

public func CanRefill() {return false;}

/* Team Support */

public func DoTeamSupport(array aClonks)
{
  //Besitzer festlegen
  var owner = GetOwner(Contained());
  if(!Contained())
    owner = iCOwner;

  for(var pTarget in aClonks)
  {
    //Nur Clonks
    if(!pTarget->~IsClonk())
      continue;
      
    //Munitionsbedarf feststellen
    var highestammo = 0, ammoID = 0;
    for(var i = 0; i < ContentsCount(0, pTarget); i++)
      if(Contents(i, pTarget)->~IsWeapon())
        for (var j = 0; j < Contents(i, pTarget)->GetFMCount(); j++)
        {
          var ammocount, weapon = Contents(i, pTarget);
          if(weapon->GetFMData(FM_NoAmmoModify, j)) continue;
          if(weapon->GetFMData(FM_AmmoLoad, j) <= 3)
            ammocount = weapon->GetFMData(FM_AmmoLoad, j) * 10;
          else
          ammocount = weapon->GetFMData(FM_AmmoLoad,j) * 3;
          if(GetAmmo(weapon->GetFMData(FM_AmmoID, j), pTarget) < ammocount)
          {
            if(!ammoID)
              ammoID = weapon->GetFMData(FM_AmmoID,j);
            if(highestammo < ammocount)
              highestammo = ammocount;
          }
        }

    if(!ammoID)
      continue;

    //Munition hinzufügen
    var factor = ammoID->~GetPointFactor();
    if(ammoID->MaxAmmo() / 10 * factor > GetPackPoints() || GetAmmo(ammoID, pTarget) >= highestammo)
      continue;

    PlayerMessage(GetOwner(Contained()), "$AmmoReceived$", pTarget, ammoID->MaxAmmo() / 10, ammoID);
    PlayerMessage(GetOwner(pTarget),"$AmmoReceived$", pTarget, ammoID->MaxAmmo() / 10, ammoID);
    DoAmmo(ammoID, ammoID->MaxAmmo()/10, pTarget);
    Sound("Resupply.ogg");
    DoPackPoints(-ammoID->MaxAmmo() / 10 * factor);

    //Achievement-Fortschritt (Ammo Distributor)
    DoAchievementProgress(ammoID->MaxAmmo() / 10 * factor, AC03, owner);

    //Punkte bei Belohnungssystem (Munitionierung)
    DoPlayerPoints(BonusPoints("Restocking", ammoID->MaxAmmo() / 10 * factor), RWDS_TeamPoints, owner, GetCursor(owner), IC14);
  }
}
