/*-- Clonk Appendto --*/

#strict 2
#appendto CLNK

local assistkiller;

public func IsClonk() { return 1; }


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
  if(GetAction() == "Death") return(0); //Fakedeath
  Schedule("DoDmg(5,DMG_Fire,0,1)",1,20,this());
  AddFireEffect(this(),30,FIRE_Red,1);
}

/* Soundauswahl */

public func OnDmg(int iDmg, int iType)
{
  HurtSounds(iDmg,iType);
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
  var iByPlayer = GetController(pFrom);
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

local killicon;

public func KillIcon(id idKillIcon)
{
  if(idKillIcon)
    killicon = idKillIcon;
    
  return(killicon);
}

private func DeathAnnounce(int plr, object clonk, int killplr)
{
  if(killplr == -1)
    return inherited(plr,clonk,killplr);
  
  //Selfkill?
  if(plr == killplr)
    KILL->SKMsg(plr, clonk);
  else
    KILL->KTMsg(killplr, plr, clonk);

  KILL->KillStat(GetCursor(killplr),plr);//hier auch clonk->~KillIcon()? könnte lustig sein :>
  
  inherited(plr,clonk,killplr);
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

  SetComDir(COMD_Stop,pTarget);
  pTarget->Sound("ClonkDie*.ogg");
    
  return true;
}

public func OnFakeDeath()
{
  if(!FindObject(AR_A))
    return;
  var Database = FindObject(AR_A);
  var killer = GetKiller();
  
  if(killer < 0 || killer == GetOwner())
  {
    Database -> SetPlayerStats("Negativepoints", GetOwner(), SuicidePoints());
    AddEffect("PointMessage",this,130,1,this,0,Format("{{%i}} <c ff0000>%d</c>", IC07, SuicidePoints()));
  }
  
  //Ansonsten Killpunkte geben (und Todespunkte (und Assistkills))
  if( Hostile(killer,GetOwner()) )
  {
    Database -> SetPlayerStats("Battlepoints", killer, KillPoints());
    AddEffect("PointMessage",GetCursor(killer),130,1,GetCursor(killer),0,Format("{{%i}} <c 00ff00>+%d</c>", IC01, KillPoints()));
    
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
      Database -> SetPlayerStats("Battlepoints", highest[1], AssistPoints());
      AddEffect("PointMessage",GetCursor(highest[1]),130,1,GetCursor(highest[1]),0,Format("{{%i}} <c 00ff00>%d</c>", IC02, AssistPoints()));
    }  
  }
  
  //Teamkiller
  if( !Hostile(killer,GetOwner()) && killer != GetOwner() && !(killer < 0))
  {
    Database -> SetPlayerStats("Negativepoints", killer, TeamkillPoints());
    AddEffect("PointMessage",GetCursor(killer),130,1,GetCursor(killer),0,Format("{{%i}} <c ff0000>%d</c>", IC06, TeamkillPoints()));
  }
}

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
  if(IsFakeDeath())
  {
    SetPhase(5);//Fallanimation überspringen
    Sound("Death");
  }
  else
  {
   Sound("ClonkDie*.ogg");
  }
  //Verschwinden
  FadeOut(this());

  //Fake Death löschen
  if(IsFakeDeath())
   RemoveObject(Contained(),true);
}
