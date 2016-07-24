/*-- Patrouillenboot (inaktiv) --*/

#strict 2

local damaged;

public func MaxDamage()		{return 350;}
public func IsCMCVehicle()	{return true;}
public func IsRepairable()	{return true;}
public func RepairSpeed()	{return 0;}


/* Initialisierung */

protected func Initialize()
{
  SetDir(Random(2));
  SetAction("OnLand");
  SetColorDw(RGB(255,255,255));
}

/* Zerstörung */

func Damage()
{
  if(damaged) return;
  if(GetDamage() > MaxDamage())
    Destruction();
}

func Destruction()
{
  damaged = true;

  //Effekte
  CreateParticle("Blast",0,-10,-20,0,5*50,RGB(255,255,128));
  CreateParticle("Blast",0,-10,20,0,5*50,RGB(255,255,128));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",15,15,0,-10,100,200,RGBa(0,0,0,100));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,100,0,0,20,170,RGB(50,250,50));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",2,100,0,0,30,100,RGB(0,0,0));
  Sound("OutOfAir");
  Sound("StructureHit*.ogg");

  //Verschwinden
  ChangeDef(BPBT);
  this->FadeOut();
}

/* Steuerung */

public func ControlRightDouble(object pByObj)
{
  if(GetAction() != "OnLand")
    return;

  //Clonk drehen und anhalten
  pByObj->SetDir(1);
  pByObj->SetAction("Throw");
  pByObj->SetComDir(COMD_Stop);

  //Boot anschieben
  Fling(this, 2, -1);

  Sound("RSHL_Shove.ogg");
}

public func ControlLeftDouble(object pByObj)
{
  if(GetAction() != "OnLand")
    return;

  //Clonk drehen und anhalten
  pByObj->SetDir();
  pByObj->SetAction("Throw");
  pByObj->SetComDir(COMD_Stop);

  //Boot anschieben
  Fling(this, -2, -1);

  Sound("RSHL_Shove.ogg");
}

/* Aktivierung */

private func Floating()
{
  //Aufrichten
  SetRDir(BoundBy(GetR(),-20,+20)/-5);
  //Nicht mehr im Wasser
  if(!(GetOCF()&OCF_InLiquid)) 
    return SetAction("OnLand");
  //Auf segeln vorbereiten wenn aufgerichtet...
  if(Inside(GetR(),-5,+5))
  {
    //...und keine anderen segelnden Patrouillenboote in Reichweite
    var obj;
    if(obj = FindObject2(Find_Distance (70),Find_ID(SPBT)))
      return;
    else
      SetAction("Unpack");
  }
}

private func Unpacked()
{
  if(Inside(GetR(),-5,+5) && InLiquid())
  {
    var dmg = GetDamage(this);
    ChangeDef(SPBT);
    DoDamage(dmg,this);
    this->~Initialize();
  }
  else
  {
    SetAction("Pack");
    Sound("SailDown");
  }
}