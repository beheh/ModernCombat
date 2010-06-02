/*-- Clonk Appendto --*/

#strict 2
#appendto CLNK

local assistkiller, machinekill;

public func IsClonk() { return 1; }
public func MenuQueryCancel()	{return(IsFakeDeath());}

/* Erstellung */

protected func Construction()
{
  if(GetID() == CLNK)
  {
    if(!this->~Redefine2(CIVC))
      if(!this->~Redefine(CIVC))
        if(GetOCF(this) & OCF_Alive)
        {
          ChangeDef(CIVC,this);
          ObjectSetAction(this,"Walk");
        }
        
    return this->Construction(...);
  }
  
  return _inherited(...);
}

/* Initalisierung */

protected func Initialize()
{
  _inherited();
  //Schadenseffekt einfügen
  if(!GetEffect("DmgCheck",this()))
    AddEffect("DmgCheck",this(),1,0);
  
  //Assistkillarray
  assistkiller = [];
  for(var i=0; i < GetPlayerCount(); i++)
    assistkiller[i*2] = -1;

  //Fake Death Effekt einfügen
  if(IsClonk() && (GetOwner() != NO_OWNER) && (GetPlayerType(GetOwner()) != C4PT_Script))
    AddEffect("FakeDeath",this(),10,0,this());
}

/* Brennen */

public func Incineration()
{
  Extinguish();
  Sound("ClonkBurn*.ogg");
  if(IsFakeDeath()) return(0); //Fakedeath
  Schedule("DoDmg(5,DMG_Fire,0,1)",1,20,this());
  AddFireEffect(this(),30,FIRE_Red,1);
}

/* Soundauswahl */

public func OnDmg(int iDmg, int iType)
{
  if(!IsFakeDeath()) {
  	HurtSounds(iDmg,iType);
  }
  return _inherited(...);
}

public func HurtSounds(int iDmg, int iType)
{
  if(iDmg <= 0) return;
  //Projektile
  if(iType == DMG_Projectile)
  {
    if(!Random(BoundBy(12-iDmg,0,12)))
      Sound("ClonkPain*.ogg");
    return;
  }
  //Flammen
  if(iType == DMG_Fire)
  {
    if(!Random(BoundBy(13-iDmg,0,13)))
      Sound("ClonkBurn*.ogg");
    return;
  }
  //Explosionen
  if(iType == DMG_Explosion)
  {
    if(!Random(BoundBy(2-iDmg,0,2)))
      Sound("ClonkPain*.ogg");
    return;
  }
  //Energie
  if(iType == DMG_Energy)
  {
    if(!Random(BoundBy(10-iDmg,0,10)))
      Sound("ClonkPain*.ogg");
    return;
  }
  //Bio
  if(iType == DMG_Bio)
  {
    if(!Random(BoundBy(20-iDmg,0,20)))
      Sound("ClonkPoisened*.ogg");
    return;
  }

  //Ansonsten Standard
  if(!Random(BoundBy(10-iDmg,0,10)))
    Sound("ClonkPain*.ogg");
}

/* Assistkiller abspeichern */

public func OnHit(int iChange, int iType, object pFrom)
{
  //Treffender eine Maschine?
  if(pFrom)
  {
    if(pFrom->~IsMachine())
      machinekill = true;
    else
      machinekill = false;
  }
  else
    machinekill = false;

  var iByPlayer = GetController(pFrom);
  if(iByPlayer == GetOwner()) return _inherited(...); //Yay, Selfassist!11
  for(var i=0; i < GetLength(assistkiller)/2; i++)
  {
    if(assistkiller[i*2] == iByPlayer)
      return assistkiller[i*2+1] += iChange;
    if(assistkiller[i*2] == -1)
    {
      assistkiller[i*2] = iByPlayer;
      assistkiller[i*2+1] = iChange;
      return 1;
    }
  }
  _inherited(...);
}

/* Reject Shift */

protected func ControlContents(idTarget)
{
  if(Contents())
    if(Contents()->~RejectShift(idTarget))
      return 1;
  return _inherited(idTarget,...);
}

/* Fallschaden */

func Hit2(int xDir, int yDir)
{ 
  if(IsFakeDeath()) return(_inherited(xDir,yDir,...));
  
  var hit = Distance(xDir,yDir);//Max(xDir,yDir);

  if(hit >= 800)
    Sound("ClonkImpact*.ogg");
  else if(hit >= 600)
    Sound("ClonkFall*.ogg");

  if(!FindObject(FDMG)) return(_inherited(xDir,yDir,...));
	if(!GetAlive(this())) return(_inherited(xDir,yDir,...));
 	
  if(hit <= 700) return(_inherited(xDir,yDir,...));
  DoDmg((hit-700)*2/10,DMG_Melee,this());
  
  if(GetAlive(this()))
    Sound("ClonkPain*.ogg");
  
  return _inherited(xDir,yDir,...);
}

/* Killverfolgung */

private func DeathAnnounce(int plr, object clonk, int killplr, bool fNoPoints)
{
  if(!clonk)
    clonk = this();
  if(!clonk) return;
  if(GetEffect("NoAnnounce", clonk)) return;
  if(!GetAlive(clonk) && GetID(Contained(clonk)) == FKDT) return; //FakeDeath-Hack
  //Selfkill?
  if(plr == killplr || killplr == -1)
    KILL->SKMsg(plr, clonk);
  else
    KILL->KTMsg(plr, killplr, clonk);

  //KILL->KillStat(GetCursor(killplr),plr);//hier auch clonk->~KillIcon()? könnte lustig sein :>
  
  clonk->AddEffect("NoAnnounce", clonk, 20);
  
  if(!fNoPoints) DoPoints();
  
  return true;
}

protected func DoPoints() {
  //Punkte vergeben
  var killer = GetKiller();

  if(!machinekill)
    if(killer < 0 || killer == GetOwner())
      DoPlayerPoints(SuicidePoints(), RWDS_MinusPoints, GetOwner(), this, IC07);

  //Ansonsten Killpunkte geben (und Todespunkte (und Assistkills))
  if(Hostile(killer,GetOwner()) )
  {
    DoPlayerPoints(KillPoints(), RWDS_BattlePoints, killer, GetCursor(killer), IC01);

    //Dem mit dem meisten angerichteten Schaden neben dem Killer Assistpunkte geben
    var highest = CreateArray(2);
    for(var i = 0; i < GetLength(assistkiller)/2; i++)
      if(assistkiller[i*2+1] > highest[0])
      {
        highest[0] = assistkiller[i*2+1];
        highest[1] = assistkiller[i*2];
      }
    if(highest[1] != killer)
    {
      DoPlayerPoints(AssistPoints(), RWDS_BattlePoints, highest[1], GetCursor(highest[1]), IC02);
    }  
  }
  
  //Teamkiller
  if( !Hostile(killer,GetOwner()) && killer != GetOwner() && !(killer < 0))
    DoPlayerPoints(TeamkillPoints(), RWDS_MinusPoints, killer, GetCursor(killer), IC06);

  //this()->CLNK::DeathAnnounce(plr,clonk,killplr);
}

local killicon;

public func KillIcon(id idKillIcon)
{
  if(idKillIcon)
    killicon = idKillIcon;
    
  return(killicon);
}

/* Inventar */

public func MaxContentsCount() { return 3; }

/* Steuerungs-Callbacks */

protected func ControlSpecial()
{
  if (Contained())
  {
    if(Contained()->~ContainedSpecial(this()))
      return 1;
  }
  
  return _inherited(...);
}

protected func ControlSpecial2()
{
  [$CtrlMenuDesc$|Image=CXTX]
 
  // In einem Gebäude oder Fahrzeug: das Kontextmenü des Gebäudes öffnen
  if (Contained())
  {
    if(Contained()->~ContainedSpecial2(this()))
      return 1;
    if ((Contained()->GetCategory() & C4D_Structure) || (Contained()->GetCategory() & C4D_Vehicle))
    {
      ExecuteCommand();
      return SetCommand(this(),"Context",0,0,0,Contained());
    }
  }
  // Fasst ein Objekt an: Kontextmenü des angefassten Objekts öffnen
  if (GetAction() == "Push")
  {
    ExecuteCommand();
    return SetCommand(this(),"Context",0,0,0,GetActionTarget());
  }
  // Trägt ein Objekt: Kontextmenü des ersten getragenen Objekts öffnen
  if (Contents(0))
  {
    ExecuteCommand();
    return SetCommand(this(),"Context",0,0,0,Contents(0));
  }
  // Ansonsten das Kontextmenü des Clonks öffnen
  ExecuteCommand();
  return SetCommand(this(),"Context",0,0,0,this());
}

/* Fake Death */

global func PauseFakeDeath(bool fPause, object pTarget)
{
  return;
}

global func FakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this();
  if(!pTarget) return false;
  if(!pTarget->IsClonk()) return false;
  
  pTarget->OnFakeDeath();

  //Fake Death erstellen
  if(WildcardMatch(GetAction(pTarget),"*Crawl*"))
  {
    var fake = CreateObject(FKDT,0,-5,GetOwner(pTarget));
    if(!ObjectSetAction(pTarget,"Death",0,0,1))
      pTarget->SetAction("Dead");
    SetPhase(5,pTarget);
  }
  else
  {
    var fake = CreateObject(FKDT,0,0,GetOwner(pTarget));
    if(!ObjectSetAction(pTarget,"Death",0,0,1))
      pTarget->SetAction("Dead");
  }
  fake->Set(pTarget);
  pTarget->DeathAnnounce(GetOwner(pTarget), pTarget, GetKiller(pTarget));

  SetComDir(COMD_Stop,pTarget);
  pTarget->Sound("ClonkDie*.ogg");
    
  return true;
}

public func OnFakeDeath(){}

global func StopFakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this();
  if(GetID(pTarget) == FKDT)
    pTarget = pTarget->GetClonk();
  if(!pTarget) return false;
  if(!pTarget->IsClonk()) return false;
  
  Sound("ClonkCough*.ogg",0,pTarget);
  ObjectSetAction(pTarget,"FlatUp",0,0,1);
  RemoveObject(pTarget->Contained(),true);
    
  return true;
}

global func IsFakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this();
  if(!pTarget) return false;
  //if(!pTarget->~IsClonk()) return(false);
  
  return GetID(pTarget->Contained()) == FKDT;
}

global func FxFakeDeathDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  if(!ObjectCount(NOFD) && !IsFakeDeath(pTarget))
  {
    if(GetEnergy(pTarget) <= -iDmgEngy/1000)
    {
      FakeDeath(pTarget);
      return 0;
    }
  }
  
  return iDmgEngy;
}

/* Entgültiger Tod */

func Death(object pTarget)
{
	if(!pTarget) pTarget = this();
	if(!pTarget) return;
  
  //Todesnachricht bei keinem FakeDeath
  if(FindObject(NOFD))
    pTarget->DeathAnnounce(GetOwner(pTarget), pTarget, GetKiller(pTarget));
  
  if(IsFakeDeath())
  {
    SetPhase(5);//Fallanimation überspringen
    Sound("Death", false, pTarget);
  }
  else
  {
   Sound("ClonkDie*.ogg", false, pTarget);
  }
  
  //Verschwinden
  FadeOut(pTarget);

  //Fake Death löschen
  if(IsFakeDeath())
   RemoveObject(Contained(pTarget),true);
}
