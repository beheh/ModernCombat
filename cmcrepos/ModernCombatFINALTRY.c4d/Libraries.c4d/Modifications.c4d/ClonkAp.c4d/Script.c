/*-- Clonk Appendto --*/

#strict 2
#appendto CLNK

local assistkiller, machinekill;

public func IsClonk() { return true; }

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
  if(!GetEffect("DmgCheck",this))
    AddEffect("DmgCheck",this,1,0);
  
  //Assistkillarray
  assistkiller = [];
  for(var i=0; i < GetPlayerCount(); i++)
    assistkiller[i*2] = -1;

  //Fake Death Effekt einfügen
  if(IsClonk() && (GetOwner() != NO_OWNER) && (GetPlayerType(GetOwner()) != C4PT_Script))
    AddEffect("FakeDeath",this,10,0,this);
}

/* Brennen */

public func Incineration()
{
  Extinguish();
  Sound("ClonkBurn*.ogg");
  if(IsFakeDeath()) return(0); //Fakedeath
  Schedule("DoDmg(5,DMG_Fire,0,1)",1,20,this);
  AddFireEffect(this,30,FIRE_Red,1);
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

private func Building()
{
  if (!Random(2)) Sound("ClonkBuild*.ogg");
  return(1);
}

/* Assistkiller abspeichern */

public func DoHitPoints(int iPoints) {
	if(!GetAlive()) return;
	DoAchievementProgress(iPoints, AC12, GetOwner());
	return true;
}

public func OnHit(int iChange, int iType, object pFrom)
{
	//Achievement
	ScheduleCall(this, "DoHitPoints", 1, 0, iChange);	
		
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
  if(iByPlayer == GetOwner() || !Hostile(iByPlayer, GetOwner())) return _inherited(...); //Yay, Selfassist!11
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
	if(!GetAlive(this)) return(_inherited(xDir,yDir,...));
 	
  if(hit <= 700) return(_inherited(xDir,yDir,...));
  DoDmg((hit-700)*2/10,DMG_Melee,this);
  
  if(GetAlive(this))
    Sound("ClonkPain*.ogg");
  
  return _inherited(xDir,yDir,...);
}

/* Killverfolgung */

protected func DeathAnnounce(int plr, object clonk, int killplr, bool fNoPoints, int assistplusone)
{
  if(!clonk) clonk = this;
  if(!clonk) return;
  if(!GetAlive(clonk) && IsFakeDeath(clonk)) return; //FakeDeath-Hack
  if(GetEffect("NoAnnounce", clonk)) return;
  //Selfkill?
  if(plr == killplr || killplr == -1)
    KILL->SKMsg(plr, clonk);
  else
    KILL->KTMsg(plr, killplr, clonk, assistplusone-1);

  KILL->KillStat(GetCursor(killplr),plr); //hier auch clonk->~KillIcon()? könnte lustig sein :>
  
  clonk->AddEffect("NoAnnounce", clonk, 1);
    
  if(!fNoPoints) DoPoints();
  
  return true;
}

protected func GetAssist() {
	var highest = CreateArray(2);
	for(var i = 0; i < GetLength(assistkiller)/2; i++) {
		if(assistkiller[i*2+1] > highest[0])
		{
			highest[0] = assistkiller[i*2+1];
			highest[1] = assistkiller[i*2];
		}
	}
	return highest[1];
}

protected func DoPoints() {
  //Punkte vergeben
  var killer = GetKiller();

  if(!machinekill)
    if(killer < 0 || killer == GetOwner())
      DoPlayerPoints(SuicidePoints(), RWDS_MinusPoints, GetOwner(), this, IC07);

  //Ansonsten Killpunkte geben (und Todespunkte (und Assistkills))
  if(Hostile(killer,GetOwner()))
  {
    DoPlayerPoints(KillPoints(), RWDS_BattlePoints, killer, GetCursor(killer), IC01);

    //Dem mit dem meisten angerichteten Schaden neben dem Killer Assistpunkte geben
		var assist = GetAssist();
    if(assist != killer)
    {
      DoPlayerPoints(AssistPoints(), RWDS_BattlePoints, assist, GetCursor(assist), IC02);
      if(!Hostile(assist, killer)) DoAchievementProgress(1, AC01, assist);
    }  
  }
  if(GBackLiquid()) DoAchievementProgress(1, AC11, GetOwner());
  
  //Teamkiller
  if(!Hostile(killer,GetOwner()) && killer != GetOwner() && !(killer < 0))
    DoPlayerPoints(TeamkillPoints(), RWDS_MinusPoints, killer, GetCursor(killer), IC06);
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
    if(Contained()->~ContainedSpecial(this))
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
    if(Contained()->~ContainedSpecial2(this))
      return 1;
    if ((GetCategory(Contained()) & C4D_Structure) || (GetCategory(Contained()) & C4D_Vehicle))
    {
      ExecuteCommand();
      return SetCommand(this,"Context",0,0,0,Contained());
    }
  }
  // Fasst ein Objekt an: Kontextmenü des angefassten Objekts öffnen
  if (GetAction() == "Push")
  {
    ExecuteCommand();
    return SetCommand(this,"Context",0,0,0,GetActionTarget());
  }
  // Trägt ein Objekt: Kontextmenü des ersten getragenen Objekts öffnen
  if (Contents(0))
  {
    ExecuteCommand();
    return SetCommand(this,"Context",0,0,0,Contents(0));
  }
  // Ansonsten das Kontextmenü des Clonks öffnen
  ExecuteCommand();
  return SetCommand(this,"Context",0,0,0,this);
}

/* Fake Death */

global func PauseFakeDeath(bool fPause, object pTarget)
{
  return;
}

global func FakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  if(!pTarget->IsClonk()) return false;
  
  pTarget->OnFakeDeath();

	//Achievements
  if(Hostile(GetKiller(pTarget), GetOwner(pTarget))) {
		var data = GetAchievementExtra(AC08, GetKiller(pTarget));
		if(!data) data = CreateArray();
		data[GetOwner(pTarget)]++;
		if(data[GetOwner(pTarget)] >= AC08->GetAchievementScore()) {
			AwardAchievement(AC08, GetKiller(pTarget));
		}
		SetAchievementExtra(data, AC08, GetKiller(pTarget));
	}
	if(Hostile(GetKiller(pTarget), GetOwner(pTarget))) {
		var killicon = pTarget->~KillIcon();
		if(killicon && killicon->~IsWeapon()) {
			var wpn = GetAchievementExtra(AC09, GetKiller(pTarget));
			if(!wpn) wpn = CreateArray();
			wpn[FindInArray4K(AC09->GetIDs(), killicon)] = true;
			var i = 0, count = 0;
			while(i <= GetLength(AC09->GetIDs())-1) {
				if(wpn[i]) count++;
				i++;
			}
			if(count >= GetLength(AC09->GetIDs())) {
				AwardAchievement(AC09, GetKiller(pTarget));
			}
			SetAchievementExtra(wpn, AC09, GetKiller(pTarget));
		}
	}
  if(Hostile(GetKiller(pTarget), GetOwner(pTarget))) {
	  if(GetProcedure(pTarget) == "FLIGHT" && GetProcedure(GetCursor(GetKiller(pTarget))) == "FLIGHT")
  		DoAchievementProgress(1, AC10, GetKiller(pTarget));
 	}
	
  //Fake Death erstellen
  if(WildcardMatch(GetAction(pTarget),"*Crawl*"))
  {
    var fake = CreateObject(FKDT,0,-5,GetOwner(pTarget));
    if(!ObjectSetAction(pTarget,"Death",0,0,1))
      ObjectSetAction(pTarget, "Dead");
    SetPhase(5,pTarget);
  }
  else
  {
    var fake = CreateObject(FKDT,0,0,GetOwner(pTarget));
    if(!ObjectSetAction(pTarget,"Death",0,0,1))
      ObjectSetAction(pTarget, "Dead");
  }
  fake->Set(pTarget);
  pTarget->DeathAnnounce(GetOwner(pTarget), pTarget, GetKiller(pTarget));

  SetComDir(COMD_Stop,pTarget);
  Sound("ClonkDie*.ogg", 0, pTarget);
    
  return true;
}

public func OnFakeDeath() {}

global func StopFakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(GetID(pTarget) == FKDT)
    pTarget = pTarget->GetClonk();
  if(!pTarget) return false;
  if(!pTarget->IsClonk()) return false;
  
  Sound("ClonkCough*.ogg",0,pTarget);
  ObjectSetAction(pTarget,"FlatUp",0,0,1);
	Contained(pTarget)->Reanimation();
	   
  return true;
}

global func IsFakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  //if(!pTarget->~IsClonk()) return(false);
  
  return GetID(Contained(pTarget)) == FKDT;
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

/* Endgültiger Tod */

func Death(object pTarget)
{
	if(!pTarget) pTarget = this;
	if(!pTarget) return;
 
	//Achievements
	ResetAchievementExtra(AC08, GetOwner(pTarget));
	var data = GetAchievementExtra(AC08, GetKiller(pTarget));
	if(!data) data = CreateArray();
	data[GetOwner(pTarget)] = 0;
	if(data[GetOwner(pTarget)] >= AC08->GetAchievementScore()) {
		AwardAchievement(AC08, GetKiller(pTarget));
	}
	SetAchievementExtra(data, AC08, GetKiller(pTarget));
	ResetAchievementProgress(AC12, GetOwner());
  
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
}

protected func GetObject2Drop(object pObj) {
  if (GetProcedure() == "PUSH" && GetID(GetActionTarget()) == FKDT) {
    var dropobj, i;

    if (pObj->~IsWeapon()) {

      //Hat die Waffe schon: Geht nicht
      if (FindContents(GetID(pObj)))
        return;

      //Pistole? Dann weg damit
      if (dropobj = FindContents(PSTL))
        return dropobj;

      //Sonst die hinterste Waffe
      for (i = 0; i < ContentsCount(); i++)
        if (Contents(i) && Contents(i)->~IsWeapon())
          dropobj = Contents(i);
      return dropobj;
    }

    //Objekt - hinterstes Objekt rauswerfen
    else {
      for (i = 0; i < ContentsCount(); i++)
        if (Contents(i) && !(Contents(i)->~IsWeapon()))
          dropobj = Contents(i);
      return dropobj;
    }
  }

  return _inherited(...);
}
