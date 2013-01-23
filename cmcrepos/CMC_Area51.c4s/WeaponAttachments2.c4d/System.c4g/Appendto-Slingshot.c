/*-- Granatwerfer-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto SGST

func Initialize()
{
   iPermittedAtts = AT_ExtendedMag | AT_Bayonet | AT_Laserpointer;
   return _inherited(...);
}

public func GetMCData(int data)
{
  if(data == MC_Damage)		
    return _inherited(data) + (iAttachment == AT_Bayonet)*10;	//Schaden eines Kolbenschlages
  
  return _inherited(data);
}

func FxLaserDotTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
var iTime = GetTime();
  for(var i = 0; i < 10; i++)
  {
  //Nutzer festlegen
  var user = this->~GetUser();
  var x, y, z;
  if(!user || !user->~IsClonk() || !user->WeaponAt(x, y, z) || Contents(0, user) != this || (iAttachment != AT_Laserpointer))
  {
    RemoveTrajectory(pTarget);
    return;
  }

  var iAngle = EffectVar(1, user, GetEffect("ShowWeapon", user));
  AddTrajectory(pTarget, GetX(pTarget), GetY(pTarget), Sin(iAngle,120), -Cos(iAngle,120), 35*3, RGB(0, 255, 0));
  }
  Log("%d ms", GetTime() - iTime);
}

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	return _inherited(data) + (iAttachment == AT_ExtendedMag)*1;	//Magazingröße

  return _inherited(data);
}
