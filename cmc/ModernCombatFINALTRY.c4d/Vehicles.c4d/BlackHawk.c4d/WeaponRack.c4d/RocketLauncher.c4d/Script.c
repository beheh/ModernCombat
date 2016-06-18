/*-- Raketenwerfer --*/

#strict 2

#include WPN2

public func HandSize()		{return 1000;}
public func HandX()		{return 1000;}
public func HandY()		{return -2500;}
public func NoWeaponChoice()	{return true;}

local aRockets, fView;


protected func Construction()
{
  AddEffect("IntNoSound", this, 1, 5);
  return _inherited(...);
}

/* Raketen - Optische Steuerung */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Missiles$";

  if(data == FM_AmmoID)		return MIAM;	//ID der Munition
  if(data == FM_ProjectileID)	return LRML;	//ID des Projektils
  if(data == FM_AmmoLoad)	return 4;	//Magazingröße

  if(data == FM_Reload)		return 190;	//Zeit für Nachladen

  if(data == FM_BurstAmount)	return 4;	//Anzahl Schussabrufe pro Burst
  if(data == FM_BurstRecharge)	return 10;	//Zeit zwischen einzelnen Bursts

  if(data == FM_Slot)		return 1;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 300;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 400;	//Maximaler Streuungswert

  return Default(data);
}

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Optical$";

  return FMData1(data);
}

public func FMData1T2(int data)
{
  return;
}

public func Fire1T1()
{
  LaunchRocket(LRML,Contained()->~AimAngle(10));
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 600;
  if(data == BOT_DmgType)	return DMG_Explosion;

  return Default(data);
}

/* Raketen - Abschuss */

public func LaunchRocket(id rid, int angle)
{
  //Austritt bestimmen
  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);

  //Rakete abfeuern
  var rocket = CreateObject(rid,x,y+10,GetController(user));
  if(!rocket) return;
  AddEffect("IntHeliProtection", rocket, 1, 20, 0, BKHK, LocalN("heli", Contained()));
  rocket->Launch(angle, user, 0, LocalN("heli", Contained())->GetXDir(), LocalN("heli", Contained())->GetYDir());
  rocket->Sound("RLSA_Fire*.ogg");
  SetController(GetController(), rocket);

  if (!aRockets || GetLength(aRockets) == FMData1(FM_BurstAmount))
    aRockets = [];

  //Sicht auf Rakete
  SetPlrView(GetController(), rocket);
  aRockets[GetLength(aRockets)] = rocket;
  fView = true;

  //Sofern möglich: Neue Rakete auf Ziele der Vorgänger aufschalten
  var pRocket;
  for(var i = 0; i < GetLength(aRockets); i++)
    if(aRockets[i] && GetAction(aRockets[i]) != "Fall" && !aRockets[i]->IsDamaged() && aRockets[i]->Guideable() && GetEffect("Follow", aRockets[i]))
    {
      pRocket = aRockets[i];
      break;
    }
  if(pRocket && LocalN("fTracerChasing", pRocket))
  {
    EffectVar(1, rocket, GetEffect("Follow", rocket)) = EffectVar(1, pRocket, GetEffect("Follow", pRocket));
    rocket->StartChasing();
  }

  //Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  if(GetEffectData(EFSM_ExplosionEffects) > 1) Sparks(5,RGB(255,128),ax-x,ay-y);
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

    for(var i=0; i<20; ++i)
    {
      var rand = RandomX(-10,+10);
      CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
      		RandomX(0,2*xdir),RandomX(0,2*ydir),
      		RandomX(80,140),RGBa(220,200,180,0),0,0);

      var rand = RandomX(-10,+10);
      CreateParticle("BlastSpark1",ax+Sin(angle,rand),ay-Cos(angle,rand),
      		RandomX(0,2*xdir),RandomX(0,2*ydir),
      		RandomX(40,70),RGBa(220,200,180,0),0,0);
    }
  }
  LocalN("heli", Contained())->Echo("RTLR_Echo.ogg");
}

/* Raketenverfolgung */

private func Check()
{
  if(!Contained() || !fView) return;

  var pRocket;
  for(var i; i < GetLength(aRockets); i++)
    if(pRocket = aRockets[i])
      break;

  if(Contained()->~IsThreat()) //Für Waffengeschütz
    if(pRocket && fView)
      SetPlrView(GetController(), pRocket);
}

public func ControlDig(object pBy)
{
  fView = !fView;
  if(!fView)
    SetPlrView(GetController(pBy), pBy);
  else
  {
    SetController(GetController(pBy));
    Check();
  }
}

/*----- Werfen -----*/

public func ControlThrow(caller)
{
  //Nutzer ist Schütze
  SetUser(caller);

  //Feuern! Fehlgeschlagen?
  if(IsRecharging() || !Fire())
  {
    var pRocket;
    for(var i = 0; i < GetLength(aRockets); i++)
      if(aRockets[i] && GetAction(aRockets[i]) != "Fall" && !aRockets[i]->IsDamaged() && aRockets[i]->Guideable() && GetEffect("Follow", aRockets[i]))
      {
        pRocket = aRockets[i];
        break;
      }

    for(var i = 0; i < GetLength(aRockets); i++)
      if(aRockets[i])
      {
        EffectVar(1, aRockets[i], GetEffect("Follow", aRockets[i])) = EffectVar(1, pRocket, GetEffect("Follow", pRocket));
        aRockets[i]->StartChasing();
      }
  }
  else
  {
    if(GetFMData(FM_BurstAmount) > 1 && !GetEffect("BurstFire", this))
    {
      var rechargetime = GetFMData(FM_BurstRecharge);
      if(rechargetime)
        AddEffect("BurstFire", this, 1, rechargetime, this, 0, GetFMData(FM_BurstAmount));
      else
      {
        for(var i = GetFMData(FM_BurstAmount); i > 0; i--)
          if(!Fire())
            return 1;
      }
    }
  }

  return 1;
}

/* Nachladen */

public func OnEmpty()
{
  if(Contained() && Contained()->~IsWeaponRack())
    Contained()->~OnEmpty();
}

public func OnReload()
{
  if(!GetEffect("IntNoSound", this))
    Sound("RLSA_Reload.ogg", false, this);
}