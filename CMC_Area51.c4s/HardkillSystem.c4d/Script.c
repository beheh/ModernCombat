/*--- Hardkill-System ---*/

#strict 2
#include CSTR

local ChargeBar;
local iHKShots;

public func BuildCost() {return 25;}
public func AutoRepairDuration()	{return 0;}
public func MaxDamage()			{return 100;}


public func Initialize() {iHKShots = 5; return _inherited(...);}

public func HardKill()
{
	if(!iHKShots)
		return;

  //Statusbalken aktualisieren
  if(!ChargeBar)
  {
    ChargeBar = CreateObject(SBAR, 0, 0, GetOwner());
    ChargeBar->Set(this, RGB(77, 229, 0), BAR_Ammobar, 0, 0, SM11);
  }
  else
    ChargeBar->Update(20*iHKShots, false);

  if(GetEffect("HardKillCooldown", this)) return;

  //Feindliche Raketen und Granaten suchen
  var threat = FindObject2(Find_Distance(120, 0, 0), Find_Hostile(GetOwner(this)), Find_Or(Find_Func("IsRocket"), Find_Func("IsRifleGrenade")), Find_Not(Find_Func("IsDamaged")));
  if(threat && !Inside(Abs(Angle(0, 0, GetXDir(threat), GetYDir(threat)) - Angle(GetX(threat), GetY(threat), GetX(), GetY())), 61, 299) && PathFree(GetX(), GetY(), GetX(threat) + GetXDir(threat)/2, GetY(threat) + GetYDir(threat)/2))
  {
    //Splitter verschleudern
    var i = 0;
    var iDir = Angle(GetX(), GetY(), GetX(threat) + GetXDir(threat)/2, GetY(threat) + GetYDir(threat)/2);
    while(i < 15)
    {
      var ammo = CreateObject(SHRP, 0, 0, GetOwner(this));
      ammo->Launch(iDir+RandomX(-10,10),120+Random(60),190+Random(40),3,15,6);
      i++;
    }

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("BlastFlame",4,30,0,0,150,100);
    Sound("BBTP_Explosion*.ogg");

    AddEffect("HardKillCooldown", this, 1, 120);
    iHKShots--;
    if(!iHKShots)
    {
    	//Statusbalken entfernen
      if(ChargeBar)
        RemoveObject(ChargeBar);
        
      FadeOut(this);

    }
  }
}

public func Destroyed()
{
  fDestroyed = true;
	FadeOut(this);
}
