/*-- Sensorball --*/

#strict 2
#include NADE

public func Color()		{return RGBa(255,255,255,0);}
public func ContainedDamage()	{return 0;}
public func FuseTime()		{return 2*30;}
public func SensorDistance()	{return 200;}

local active;


/* Aktivierung */

public func Fuse(object pObjBy)
{
  CreateParticle("PSpark", 0, 0, 0, 0, 60, GetPlrColorDw(GetOwner()), this);
  Sound("SNSR_Activate.ogg");
  SetGraphics("Active");
  return AddEffect("IntFuse", this, 200, 1, this);
}

public func Fused()
{
  //Einschalten
  active = true;

  //Effekte
  CreateParticle("PSpark",0,0,0,0,60,GetPlrColorDw(GetOwner()),this);
  Sound("SNSR_Fused.ogg");

  //Entfernung planen
  ScheduleCall(0,"Remove", 35*30);
}

/* Feindbewegung suchen */

protected func Sense()
{
  //Inaktiv: Nicht suchen
  if(!active)
    return;

  //Zu markierende Gefahren suchen
  for (var pObj in FindObjects(Find_Distance(SensorDistance()),			//In Reichweite
  		Find_Exclude(this),						//Selber ausschließen
  		Find_NoContainer(),						//Im Freien
  		Find_Hostile(GetOwner()),					//Feindlich
  		Find_Or(Find_OCF(OCF_Alive), Find_Func("IsDetectable"))))	//Lebewesen oder als identifizierbar markiert
  {
    //Beep.
    Beep();

    //Nicht markieren wenn schon der Fall
    if(FindObject2(Find_ID(SM08), Find_Action("Attach"), Find_ActionTarget(pObj), Find_Allied(GetOwner())))
      continue;

    //Ansonsten markieren
    CreateObject(SM08, 0, 0, GetOwner())->Set(pObj, this);
  }
  return 1;
}

/* Beep */

public func Beep()
{
  //Erst nach Ablauf des letzten Beeps
  if(GetEffect("IntWait", this))
    return;

  //Effekte
  CreateParticle("PSpark", 0, 0, 0, 0, 60, GetPlrColorDw(GetOwner()), this);
  Sound("SNSR_Beep.ogg");

  //Einen Moment lang nicht mehr beepen
  AddEffect("IntWait", this, 1, 50, this);
}

/* Entfernung */

private func Remove()
{
  //Ausschalten
  active = false;

  //Effekte
  Sparks(2,RGB(250,100));
  Sparks(2,RGB(0,200));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",4, 10, 0, 0, 120, 140, RGBa(255,255,255,100), RGBa(255,255,255,130));

  FadeOut();
}

/* Aufnahme */

public func Entrance(object pObj)
{
  //Träger = Besitzer
  SetOwner(GetOwner(pObj));
}

/* Aufschlag */

func HitSound()
{
  Sound("BBTP_Hit*.ogg");
}