/*-- Sensorball --*/

#strict 2
#include NADE

local active, rt_defusecnt;

public func DoSmoke()		{return false;}		//Granate erzeugt im Flug keinen Rauch
public func FuseTime()		{return 2*30;}		//Zeit bis zur Zündung
public func ThrowSpeed()	{return 65;}		//Wurfgeschwindigkeit
public func MaxDamage()		{return 20;}		//Benötigter Schaden, um die Granate zu zerstören
public func ContainedDamage()	{return 0;}		//Schaden bei Detonation innerhalb eines Objekts

public func IsActive()		{return active;}	//Aktivität
public func IsDetectable()	{return false;}		//Kann von anderen Sensorbällen nicht erkannt werden
public func SensorDistance()	{return 190;}		//Reichweite des Sensors

public func IsDefusable()	{return active;}
public func LimitationCount()	{return 2;}


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

  CheckLimitation();

  //Entfernung planen
  ScheduleCall(0,"Remove", 40*38);
}

/* Feindbewegung suchen */

protected func Sense()
{
  //Bei Inaktivität nicht suchen
  if(!active)
    return;

  //Kein Besitzer mehr: Verschwinden
  if(GetOwner() == NO_OWNER)
    return Remove();

  //Zu markierende Gefahren suchen
  for (var pObj in FindObjects(Find_Distance(SensorDistance()),			//In Reichweite
  		Find_Exclude(this),						//Selber ausschließen
  		Find_NoContainer(),						//Im Freien
  		Find_Or(Find_OCF(OCF_Alive), Find_Func("IsDetectable"))))	//Lebewesen oder als identifizierbar markiert
  {
    //Nur feindliche Objekte markieren
    if(!Hostile(GetController(), GetController(pObj))) continue;

    //Beep
    Beep();

    //Bereits markierte Objekte auslassen
    if(FindObject2(Find_ID(SM08), Find_Action("Attach"), Find_ActionTarget(pObj), Find_Allied(GetOwner())))
      continue;

    //Ansonsten markieren
    CreateObject(SM08, GetX(pObj), GetY(pObj), GetOwner())->Set(pObj, this, GetOCF(pObj) & OCF_Alive);

    //Achievement-Fortschritt (Intelligence)
    DoAchievementProgress(1, AC21, GetOwner());
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

  //Kreissymbol erstellen
  CreateObject(SM09,0,0,GetOwner())->Set(this);

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

  //Verschwinden
  if(OnFire())
    RemoveObject();
  else
    FadeOut();
}

/* Aufnahme */

public func Entrance(object pObj)
{
  //Träger = Besitzer
  SetOwner(GetOwner(pObj));
}

/* Schaden */

protected func Damage(int iChange) 
{
  //Kein Schaden nehmen wenn gehalten und eventuelles Feuer löschen
  if(Contained())
  {
    if(OnFire())
    {
      Extinguish();
      return;
    }
    else
    return;
  }

  //Ansonsten Zerstörung durch Schaden
  if(GetDamage() < MaxDamage()) return;

  //Effekte
  Sparks(2,RGB(250,100));
  Sparks(2,RGB(0,200));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("BlastDirt",2,10,0,0,400,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("BlastFlame",2,10,0,0,150,100);
  Sound("BBTP_Hit*.ogg");

  //Verschwinden
  RemoveObject();
}

/* Entschärfung */

public func RTDefuse()
{
  rt_defusecnt++;
  if(rt_defusecnt > 8)
  {
    Sound("MISL_ShotDown.ogg");
    DecoExplode(10);

    return true;
  }

  return false;
}

/* Aufprall */

func HitSound()
{
  Sound("BBTP_Hit*.ogg");
}