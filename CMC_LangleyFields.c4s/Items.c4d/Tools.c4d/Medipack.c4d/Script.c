/*-- Medipack --*/

#strict 2
#include FAPK
#include PCK2

public func StartPoints()	{return 400;}
public func MaxPoints()		{return 400;}
public func RefillTime()	{return 0;}
public func TeamSupportTime()	{return 20;}

public func CanRefill() {return false;}

/* Team-Heilung */

public func DoTeamSupport(array aClonks)
{
  //Können nur Sanitäter oder platzierte Medipacks
  if(!Contained()->~IsMedic() && !fPlaced)
    return;
  //Wenn nicht gerade in Gebrauch
  if(GetEffect("FAPHeal", this))
    return;

  var a = [];
  //Zuerst die mit vollem Leben aussortieren
  for (var pClonk in aClonks)
    if(GetEnergy(pClonk) < GetPhysical("Energy", PHYS_Current, pClonk) / 1000)
      a[GetLength(a)] = pClonk;
  aClonks = a;
  //Keiner mehr übrig
  if(!GetLength(aClonks))
    return;
  
  var owner = GetOwner(Contained());
  if(!Contained())
    owner = iCOwner;
  
  //Je mehr geheilt werden, desto schwächer
  var heal = Max(2, 8 - 2 * GetLength(aClonks));

  for(var pClonk in aClonks)
  {
    DoEnergy(heal, pClonk);
    //Achievement-Fortschritt (I'll fix you up!)
    DoAchievementProgress(heal, AC02, owner);
    iHealed += heal;
    CreateParticle("ShockWave", GetX(pClonk) - GetX(), GetY(pClonk) - GetY(), 0, 0, 5 * (5  + GetObjHeight(pClonk)), RGB(0, 230, 255), pClonk);
    CreateParticle("ShockWave", GetX(pClonk) - GetX(), GetY(pClonk) - GetY(), 0, 0, 5 * (10 + GetObjHeight(pClonk)), RGB(0, 230, 255), pClonk);
    CreateParticle("ShockWave", GetX(pClonk) - GetX(), GetY(pClonk) - GetY(), 0, 0, 5 * (15 + GetObjHeight(pClonk)), RGB(0, 230, 255), pClonk);
    ScreenRGB(pClonk, RGBa(0, 230, 255, 190), 80, 3, false, SR4K_LayerMedicament, 200);
    DoPackPoints(heal / -2);
    Sound("FAPK_Healing*.ogg");
  }
  while(iHealed >= 40)
  {
    iHealed -= 40;
    //Punkte bei Belohnungssystem (Heilung)
    DoPlayerPoints(BonusPoints("Healing", 40), RWDS_TeamPoints, owner, Contained(), IC05);
  }
}
