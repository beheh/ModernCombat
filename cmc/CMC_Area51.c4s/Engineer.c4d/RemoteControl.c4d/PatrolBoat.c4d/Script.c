/*-- Patrouillenboot (inaktiv) --*/

#strict 2

local damaged;

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
  if(GetDamage() > 200)
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

/* Aktivierung */

private func Floating()
{
  //Aufrichten
  SetRDir(BoundBy(GetR(),-20,+20)/-5);
  //Nicht mehr im Wasser
  if(!(GetOCF()&OCF_InLiquid)) 
    return SetAction("OnLand");
  //Segeln, wenn aufgerichtet
  if(Inside(GetR(),-5,+5))
  {
    ChangeDef(SPBT);
    SetAction("LowerSail", this);
    this->~Initialize();
  }
}