/*-- Landmine --*/

#strict 2

local bActive, thrown, controller;

public func IsDrawable()		{return true;}
public func HandSize()			{return 1000;}
public func HandX()			{return 3500;}
public func IsMine()			{return true;}
public func IsBulletTarget(idBullet)	{return idBullet == TRDT || !Random(6);}
public func IsBouncy()			{return true;}
public func IsEquipment()		{return true;}
public func NoArenaRemove()		{return true;}
public func AttractTracer(pT)		{return GetPlayerTeam(GetController()) != GetPlayerTeam(GetController(pT));}
public func LimitationCount()		{return 4;}


/* Steuerung */

public func ControlDigDouble(object caller)
{
  //Nur wenn getragen
  if(!Contained()) return;

  //Träger: Besitzer
  var user = Contained();
  SetOwner(GetOwner(Contained()));

  //Stehend?
  if (GetProcedure(user) == "WALK")
  {
    thrown=true;

    //Aktivieren
    Activate(caller);

    //Plazieren
    Exit(this, 0, 14);
    SetR();

    //Sound
    Sound("BBTP_Hit*.ogg");

    return true;
  }
  //Schwimmend?
  if (GetProcedure(user) == "SWIM")
  {
    thrown=true;

    //Aktivieren
    Activate(caller);

    //Plazieren
    Exit(this, 0, 14);
    SetR();

    //Sound
    Sound("BBTP_Hit*.ogg");

    return true;
  }

  return false;
}

public func ControlThrow(object caller)
{
  //Nur wenn getragen
  if(!Contained()) return;

  //Träger: Besitzer
  var user = Contained();
  SetOwner(GetOwner(Contained()));

  //Schwimmend?
  if (GetProcedure(user) == "SWIM")
  {
    thrown=true;

    //Aktivieren
    Activate(caller);

    //Sounds
    Sound("LDMN_Throw.ogg");
  }

  //Stehend?
  if (GetProcedure(user) == "WALK")
  {
    thrown=true;

    //Sounds
    Sound("GrenadeThrow*.ogg");
    Sound("LDMN_Throw.ogg");
  }

  return false;
}

/* Gegnererkennung */

protected func Check()
{
  //Nicht aktiv, nicht suchen
  if(!bActive || !thrown) return;

  var pObj;
  for(obj in FindObjects(Find_Distance(20), Find_Exclude(this), Find_Func("CheckEnemy",this,0,true), Find_NoContainer()))
  {
    if(obj->~IsBulletTarget(GetID(),this,0) || GetOCF(obj))
    {
      //Ziel in Bewegung?
      if(obj->GetXDir() || obj->GetYDir())
      {
        //Keine Mine?
        if(!(obj->~IsMine()))
        {
          Detonate();
          return 1;
        }
      }
    }
  }
}

/* Aktivierung */

public func Activate(caller)
{
  //Träger = Besitzer
  controller = GetOwner(caller);
  SetOwner(controller);

  //Aktivieren
  ScheduleCall(this, "FinFuse",2*36);
}

public func FinFuse()
{
  //Nichts unternehmen wenn getragen oder bereits aktiv
  if(Contained() || bActive) return;

  //Aktiv
  bActive=true;

  var iColor = LightenColor(GetPlrColorDw(GetOwner(this)), 192);

  //Blinkeffekt
  CreateParticle("PSpark", 0, 0, 0, 0, 60, iColor, this);

  //Sound
  Sound("BBTP_Activate.ogg");
}

/* Deaktivierung */

public func ControlUp(object pObjBy)
{
  if(Contained()) return;

  //Deaktivieren
  bActive=false;
  thrown=false;

  //Sound
  Sound("BBTP_Charge.ogg");

  //Einsammeln wenn möglich, sonst zumindest loslassen
  if (!Collect(this, pObjBy))
    SetCommand(pObjBy, "UnGrab");
  return true;
}

/* Zerstörung */

public func BlastRadius()	{return 35;}

public func Detonate()
{
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",15,20,0,0,220,500, RGBa(255,255,255,0));
  
  if(GBackLiquid())
    Sound("C4EX_WaterDetonation.ogg");
  else
    Sound("C4EX_Detonation*.ogg");

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("BlastDirt",5,10,0,0,400,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("BlastFlame",4,20,0,0,150,100);
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));

  //Extraschaden für Strukturen
  for(var obj in FindObjects(Find_Distance(50), Find_Category(C4D_Structure | C4D_Vehicle), Find_Exclude()))
    DoDmg(BoundBy(InvertA1(ObjectDistance(obj), 100),0,60), DMG_Explosion, obj, 0, GetOwner()+1, GetID());

  Explode(BlastRadius());
}

/* Schaden */

protected func Damage(int iChange) 
{
  //Zündung durch Schaden
  if(GetDamage() < 15) return ;
  Sound("LDMN_Throw.ogg");
  //Zündung verzögert
  ScheduleCall(this, "Detonate",10);
  //Sofortige Zündung bei hohem Schaden
  if(GetDamage() < 20) return ;
  Detonate();
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)	return 100;	//Säure und biologische Schadstoffe
}

/* Aufnahme */

public func RejectEntrance(object pObj)
{
  return thrown;
}

public func Collection(object pObj)
{
  //Träger = Besitzer
  controller = GetOwner(pObj);
  SetOwner(controller);
}

/* Aufschlag */ 

func Hit(int iXDir, int iYDir)
{
  //Sound
  HitSound();

  //Aktivieren wenn geworfen und inaktiv
  if(thrown && !bActive)
  {
    //Aktivieren
    ScheduleCall(this, "FinFuse",2*36);
  }

  //Entsprechende Hüpfbewegung
  if(GBackSolid(0,+5)) return(SetYDir(-iYDir/26));
  if(GBackSolid(0,-5)) return(SetYDir(-iYDir/26));
  if(GBackSolid(-5,0)) return(SetXDir(-iXDir/16));
  if(GBackSolid(+5,0)) return(SetXDir(-iXDir/16));
}

func Hit3()
{
  if(!bActive) return;

  //Detonieren
  ScheduleCall(this, "Detonate",10);
}

func HitSound()
{
  Sound("BBTP_Hit*.ogg");
}

/* Allgemein */

protected func Selection()
{
  Sound("BBTP_Charge.ogg");
  return 1;
}