/*-- Sprengfalle --*/

#strict 2

local bActive, bReady, iDir, controller, laser;

public func IsDrawable()		{return true;}
public func CanAim()			{return !bActive && Contained();}
public func IsMine()			{return true;}
public func Color()			{return RGB(200,200,200);}
public func IsBulletTarget(idBullet)	{return idBullet == TRDT || !Random(6);}
public func HandX()			{return 5000;}
public func HandSize()			{return 1000;}
public func BarrelXOffset()		{return -850;}
public func IsEquipment()		{return true;}
public func NoArenaRemove()		{return true;}
public func AttractTracer(pT)		{return GetPlayerTeam(GetController()) != GetPlayerTeam(GetController(pT));}
public func LimitationCount()		{return 4;}


/* Initialisierung */

func Initialize()
{
  //inaktiv erstellen
  SetAction("Defused"); 
  return 1;
}

/* Steuerung */

public func ControlThrow(object caller)
{
  //Wird nicht getragen: Werfen gesperrt
  if(!Contained()) return;

  //Nur die Sprengfalle mit dem ersten Index in einem Objekt wird benutzt
  var index;
  for(index = 0; Contents(index) == this; index++);
  for(var i = 0; GetID(Contents(i)) == GetID() && Contents(i) != this && index > i; i++)
    return; 

  //Normales Ablegen
  if (GetPlrDownDouble(GetController(caller)))
    return _inherited(...);

  //Träger = Besitzer
  controller = GetOwner(caller);
  SetController(controller);
  SetOwner(controller);

  Throw();
  return true;
}

public func Throw()
{
  //Winkel übernehmen
  var user = Contained();
  iDir = user->AimAngle();

  var x, y,			//X-/Y-Austrittsposition
  doplace,			//1: Normales Aufstellen, 2: Nach Winkel drehen
  vtx, vty;			//Offset für Zusatzvertex

  //Kriechen
  if (user->~IsCrawling())
  {
    x = 10;
    if (!GetDir(user))
      x = -10;
    y = 1;
    doplace = 1;
  }

  //Klettern
  else if (GetProcedure(user) == "SCALE")
  {
    x = 3;
    iDir = -90;
    if (!GetDir(user))
    {
      x = -3;
      iDir = 90;
    }
    vtx = x * 4 / 3;
    doplace = 2;
  }

  //Hangeln
  else if (GetProcedure(user) == "HANGLE")
  {
    y = -6;
    iDir = 160;
    if (!GetDir(user))
      iDir = -160;
    doplace = 2;
    vty = -2;
  }

  //Klettern an Leitern
  else if (GetAction(user) == "ScaleLadder")
    doplace = 1;

  //Laufen und Zielen
  else if (GetProcedure(user) == "WALK" || user->~IsAiming())
  {
    doplace = 1;
    y = 6;
  }

  //Sonst: Bereit zu feuern?
  else if (user->~ReadyToFire())
    doplace = 1;

  //Bei nicht identifizierbarer Aktion
  if (!doplace) return;
  Exit(this, x, y - GetDefOffset(GetID(), 1));

  //Effekte
  Sound("BBTP_Activate.ogg", 0, 0, 50);
  CreateParticle("PSpark", 0, 0, 0, 0, 60, GetPlrColorDw(GetOwner()), this);

  //Aktivierung
  bActive = true;
  ScheduleCall(0, "FinFuse", 2 * 36);

  //Grafik setzen
  SetAction("Fused");
  if (doplace == 1)
  {
    if(Inside(iDir, -180, -130)) SetPhase(3);
    if(Inside(iDir, -129, -78))  SetPhase(0);
    if(Inside(iDir, -77 , -27))  SetPhase(1);
    if(Inside(iDir, -26 , 25))   SetPhase(2);
    if(Inside(iDir, 26  , 76))   SetPhase(3);
    if(Inside(iDir, 77  , 128))  SetPhase(0);
    if(Inside(iDir, 129 , 179))  SetPhase(1);
    SetObjDrawTransform(1000, 0, 0, 0, 1000, 0);
  }

  if (doplace == 2)
  {
    SetPhase();
    var s = Sin(-iDir, 1000), c = Cos(-iDir, 1000);
    SetObjDrawTransform(c, s, 0, -s, c);
  }

  //Vertex zur besseren Haftung
  if (vtx || vty)
    AddVertex(vtx, vty);

  var nextmine = user->~GrabGrenade(GetID());
  user->~ResetShowWeapon();
  if(user->~IsAiming())
    if(!nextmine)
      user->StopAiming();
}

private func FinFuse()
{
  if(!bActive)
    return;

  var iColor = LightenColor(GetPlrColorDw(controller), 192);
  SetClrModulation(RGBa(255, 255, 255, 80));

  //Blinkeffekt
  CreateParticle("PSpark", 0, 0, 0, 0, 60, iColor, this);
  //Laser erstellen
  laser = CreateObject(LASR, 0, 0, controller);
  laser -> Set(iDir, 3, 60, 0, 0, this, 0, true);
  SetClrModulation(SetRGBaValue(iColor, 180), laser);
  laser ->~ Destruction();
  //Hinweisflagge erstellen
  var flag = CreateObject(MFLG,0,1,controller);
  flag->Set(this);
  SetDir(BoundBy(-iDir, 0, 1), flag);

  CheckLimitation();
  bReady=true;
  Sound("BBTP_Alarm.ogg", 0, 0, 50);
}

/* Deaktivierung */

public func ControlUp(object pObjBy)
{
  if(Contained()) return;

  //Punkte bei Belohnungssystem
  if(Hostile(GetOwner(),GetOwner(pObjBy))) 
    DoPlayerPoints(BonusPoints("TechnicalTask"), RWDS_TeamPoints, GetOwner(pObjBy), pObjBy, IC15);

  //Träger = Besitzer
  controller = GetOwner(pObjBy);
  SetController(controller);
  SetOwner(controller);

  //Deaktivieren
  bActive=false;

  //Eventuell sichern
  if(bReady)
    Defuse();

  //Einsammeln wenn möglich, sonst zumindest loslassen
  if (!Collect(this, pObjBy))
    SetCommand(pObjBy, "UnGrab");
  return true;
}

private func Defuse()
{
  //Deaktivieren
  SetAction("Defused");
  DeactivateLimitation();
  //Laser entfernen
  RemoveObject(laser);
  //Hinweisflagge entfernen
  var flag = FindObject2(Find_ID(MFLG),Find_ActionTarget(this));
  if(flag) RemoveObject(flag);

  SetClrModulation();
  SetObjDrawTransform(1000,0,0,0,1000);
  Sound("BBTP_Charge.ogg");
  bReady=false;
  SetR();
  return 1;
}

/* Gegnererkennung */

private func Check()
{
  //Nicht aktiv, nicht suchen
  if(!bReady) return;

  var pVictim;
  for( var pVictim in FindObjects(Find_OnLine(0,0,Sin(iDir,80,1),-Cos(iDir,80,1)), 
				Find_Func("CheckEnemy",this,0,true),			//Gegner?
				Find_Not(Find_Distance(10)),				//Nah genug?
				Find_OCF(OCF_Alive),					//Lebewesen?
				Find_NoContainer()) )					//Im Freien?
    if(PathFree(GetX(),GetY(),GetX(pVictim),GetY(pVictim)))				//Freies Schussfeld?
      Detonate();
  return 1;
}

/* Zerstörung */

public func Detonate()
{
  //Splitter verschleudern
  var i = 0;
  while(i < 12)
  {
    var ammo = CreateObject(SHRP,0,0,GetOwner());
    ammo->Launch(iDir+RandomX(-15,15),100+Random(80),100+Random(50),3,20,50);
    i++;
  }

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("BlastDirt",5,10,0,0,400,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("BlastFlame",4,20,0,0,150,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",2,150,0,0,45,30,RGB(40,20,20));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",2,100,0,0,30,80);
  Sound("BBTP_Explosion.ogg");

  //Entfernung
  RemoveObject(laser);
  RemoveObject();
}

/* Schaden */

protected func Damage(int iChange) 
{
  //Zündung durch Schaden
  if(GetDamage() < 5) return ;
  Sound("BBTP_Alarm.ogg");
  //Zündung verzögert
  ScheduleCall(this, "Detonate",20);
  //Sofortige Zündung bei hohem Schaden
  if(GetDamage() < 10) return ;
  Detonate();
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire) return 90;	//Feuer
  if(iType == DMG_Bio)	return 100;	//Säure und biologische Schadstoffe
}

/* Aufnahme */

public func RejectEntrance(object pObj)
{
  return bActive;
}

/* Allgemein */

protected func Selection()
{
  Sound("BBTP_Charge.ogg");
  return 1;
}

func Hit()
{
  Sound("BBTP_Hit*.ogg");
}

public func Destruction()
{
  //Entfernung
  if(laser)
   RemoveObject(laser);
}