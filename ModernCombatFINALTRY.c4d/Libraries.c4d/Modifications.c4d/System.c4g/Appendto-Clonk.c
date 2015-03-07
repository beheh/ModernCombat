/*-- Clonk Appendto --*/

#strict 2
#appendto CLNK

local assistkiller, machinekill;
local killicon;
local LastDmgType;
local killattachment;


/* Erstellung */

protected func Construction()
{
  //Temporäre Umwandlung in einen CMC-Clonk
  if(GetID() == CLNK)
  {
    if(!Redefine2(CIVC))
      if(!Redefine(CIVC))
        if(GetOCF(this) & OCF_Alive)
        {
          ChangeDef(CIVC,this);
          ObjectSetAction(this,"Walk");
        }

    return this->Construction(...);
  }

  return _inherited(...);
}

/* Initialisierung */

protected func Initialize()
{
  _inherited();

  //Schadenseffekt einfügen
  if(!GetEffect("DmgCheck",this))
    AddEffect("DmgCheck",this,1,0);

  //Assistkiller initialisieren
  ResetAssist();

  //Fake Death-Effekt einfügen
  if(IsClonk() && (GetOwner() != NO_OWNER) && (GetPlayerType(GetOwner()) != C4PT_Script))
    AddEffect("FakeDeath",this,10,0,this);

  AddEffect("AssistDmgReduction", this, 1, 0, this);
}

/* Brennen */

public func Incineration()
{
  //Clonk sofort löschen
  Extinguish();

  //Sound
  if(GetAlive()) 
    Sound("ClonkBurn*.ogg");

  //Schadenseffekt setzen sofern lebendig
  if(IsFakeDeath()) return;
  Schedule("DoDmg(5,DMG_Fire,0,1)",1,20,this);
  AddFireEffect(this,30,FIRE_Red,1);
}

/* Soundauswahl */

public func OnDmg(int iDmg, int iType)
{
  if(!IsFakeDeath() && GetAlive() && iDmg > 0)
    HurtSounds(iDmg, iType);

  return _inherited(iDmg, iType);
}

public func HurtSounds(int iDmg, int iType)
{
  if(iDmg <= 0) return;

  //Projektile
  if(iType == DMG_Projectile)
  {
    if(!Random(BoundBy(4-iDmg,0,4)))
      Sound("ClonkPain*.ogg");
    return;
  }
  //Flammen
  if(iType == DMG_Fire)
  {
    if(!Random(BoundBy(3-iDmg,0,3)))
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
    if(!Random(BoundBy(5-iDmg,0,5)))
      Sound("ClonkPain*.ogg");
    return;
  }
  //Bio
  if(iType == DMG_Bio)
  {
    if(!Random(BoundBy(5-iDmg,0,5)))
      Sound("ClonkPoisened*.ogg");
    return;
  }
  //Ansonsten Standard
  if(!Random(BoundBy(4-iDmg,0,4)))
    Sound("ClonkPain*.ogg");
}

private func Building()
{
  if(!Random(2)) Sound("ClonkBuild*.ogg");
  return 1;
}

private func WalkSound()
{
  if(Contained()) return;
  Sound("ClonkStep*.ogg", 0, 0, 25);
  if(!Random(6)) Sound("ClonkVestRustle*.ogg", 0, 0, 25);
  return 1;
}

private func ClimbSound()
{
  if(!Random(6)) Sound("ClonkVestRustle*.ogg", 0, 0, 25);
  return 1;
}

private func SwimSound()
{
  Sound("ClonkSwim*.ogg", 0, 0, 75);
  return 1;
}

private func CrawlStartSound()
{
  Sound("ClonkFall*.ogg", 0, 0, 50);
  Sound("ClonkRustle*.ogg", 0, 0, 25);
  return 1;
}

private func CrawlSound()
{
  Sound("ClonkCrawl*.ogg", 0, 0, 25);
  if(!Random(6)) Sound("ClonkVestRustle*.ogg", 0, 0, 25);
  return 1;
}

/* Assistkiller abspeichern */

public func ResetAssist()
{
  assistkiller = [[-1, 0]];
}

public func DoHitPoints(int iPoints)
{
  if(!GetAlive() || IsFakeDeath()) return;
  //Achievement-Fortschritt (Meat Shield)
  DoAchievementProgress(iPoints, AC12, GetOwner());
  return true;
}

public func OnHit(int iChange, int iType, object pFrom)
{
  //Geräusche machen
  if(!IsFakeDeath() && GetAlive())
  {
    HurtSounds(iChange, iType);
  }

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

  //Mögliche Assists ermitteln
  var iByPlayer = GetController(pFrom);
  if(iByPlayer == GetOwner() || !Hostile(iByPlayer, GetOwner())) return _inherited(...);
  for(var i=0; i < GetLength(assistkiller); i++)
  {
    if(!assistkiller[i]) assistkiller[i] = [-1, 0];
    if(assistkiller[i][0] == iByPlayer)
    {
      assistkiller[i][1] += iChange;
      break;
    }
    if(assistkiller[i][0] == -1)
    {
      assistkiller[i][0] = iByPlayer;
      assistkiller[i][1] = iChange;
      assistkiller[i+1] = [-1, 0];
      break;
    }
  }
  _inherited(...);
}

public func LastDamageType(int type)
{
  if(type)
    LastDmgType = type;
  return LastDmgType;
}

public func KillAttachment(int iKillAttachment, bool fReset)
{
  if(fReset)
    killattachment = iKillAttachment;
  return killattachment;
}

public func Ejection(object pObj)
{
  AddEffect("MatJumpProtection", pObj, 1, 40, this);

  return _inherited(pObj, ...);
}

/* Objekttreffer */

local pLastObjectHit;

protected func QueryCatchBlow(object pBy)
{
  //Materialjump verhindern
  if(GetEffect("MatJumpProtection", pBy) && GetEffect("MatJumpProtection", pBy, 0, 4) == this && !GameCall("AllowMaterialJump"))
    return true;
  
  pLastObjectHit = pBy;

  return _inherited(pBy, ...);
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
  if(IsFakeDeath() || !GetAlive(this)) return _inherited(xDir,yDir,...);

  var hit = Distance(xDir,yDir);//Max(xDir,yDir);

  //Effekte
  if(hit >= 800)
  {
    Sound("ClonkImpact*.ogg");
    Sound("ClonkRustle*.ogg", 0, 0, 50);
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,10,0,10,20,100);
  }
  else if(hit >= 600)
  {
    Sound("ClonkFall*.ogg");
    Sound("ClonkRustle*.ogg", 0, 0, 25);
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",4,8,0,10,20,50);
  }

  //Achievement-Fortschritt (Totally Planned)
  var prevEnergy = GetEnergy();

  //Schaden bei Fallschaden-Regel
  if(!FindObject(FDMG) || hit <= 700) return _inherited(xDir,yDir,...);
  DoDmg((hit-700)*2/10,DMG_Melee,this,0,GetKiller(this)+1,SM10);

  //Achievement-Fortschritt (Totally Planned)
  if((prevEnergy-GetEnergy()) * 100000 / GetPhysical("Energy") >= 80 && GetEnergy() * 100000 / GetPhysical("Energy") < 5 && GetAlive(this) && GetID(Contained()) != FKDT)
    DoAchievementProgress(1, AC56, GetOwner());


  if(GetAlive(this))
    Sound("ClonkPain*.ogg");

  return _inherited(xDir,yDir,...);
}

/* Killverfolgung */

protected func DeathAnnounce(int plr, object clonk, int killplr, bool fNoPoints, int assistplusone)
{
  if(!clonk) clonk = this;
  if(!clonk) return;
  if(!GetAlive(clonk) && IsFakeDeath(clonk)) return;
  if(GetEffect("NoAnnounce", clonk)) return;

  //Selfkill?
  if(plr == killplr || killplr == NO_OWNER)
    KILL->SKMsg(plr, clonk);
  else
    KILL->KTMsg(plr, killplr, clonk, assistplusone - 1);

  KILL->KillStat(GetCursor(killplr),plr);

  AddEffect("NoAnnounce", clonk, 1);

  if(!fNoPoints)
    DoPoints();

  GameCallEx("OnDeathAnnounce", clonk, killplr, assistplusone - 1);

  //Spielereigene Todesnachricht ausgeben wenn vorhanden
  if(GetObjCoreDeathMessage(clonk))
    Message(GetObjCoreDeathMessage(clonk), clonk, GetName(clonk));

  return true;
}

public func GetAssist(int iPlrExclude)
{
  var highest = [-1, 0];
  for(var i = 0; i < GetLength(assistkiller); i++)
  {
    if(assistkiller[i][0] == GetOwner()) continue;
    if(assistkiller[i][0] == iPlrExclude) continue;
    if(!GetPlayerName(assistkiller[i][0])) continue;
    if(assistkiller[i][1] > highest[1])
    {
      highest[0] = assistkiller[i][0];
      highest[1] = assistkiller[i][1];
    }
  }
  return highest[0];
}

protected func DoPoints()
{
  var killer = GetKiller();

  if(!machinekill)
    if(killer < 0 || killer == GetOwner())
      //Punkte bei Belohnungssystem (Selbstmord)
      DoPlayerPoints(BonusPoints("Suicide"), RWDS_MinusPoints, GetOwner(), this, IC07);
  var aDoomBoom = GetAchievementExtra(AC15, killer);
  if(!aDoomBoom) aDoomBoom = CreateArray();

  if(Hostile(killer,GetOwner()))
  {  
    var killicon = KillIcon();

    //Ehrenbänder
    if(killicon)
    {
      //Ehrenband-Fortschritt (The Boss)
      if(killicon->~IsBlowTorch())
        AttemptAwardRibbon(RB01, killer, GetOwner());
      //Ehrenband-Fortschritt (The Code)
      if(killicon->~IsRocket())
        AttemptAwardRibbon(RB02, killer, GetOwner());
      //Ehrenband-Fortschritt (The Ghost)
      if(killicon->~IsC4Explosive())
        AttemptAwardRibbon(RB03, killer, GetOwner());
      //Ehrenband-Fortschritt (The Gamer)
      if(killicon->~IsMAV())
        AttemptAwardRibbon(RB04, killer, GetOwner());
      //Ehrenband-Fortschritt (The Artist)
      if(killicon->~IsShockPaddles())
        AttemptAwardRibbon(RB05, killer, GetOwner());

      var e = GetEffect("FakeDeathContained", this);
      //Ehrenband-Fortschritt (The Noob)
      if(e && EffectVar(1, this, e) == BKHK)
        AttemptAwardRibbon(RB08, killer, GetOwner());
      //Ehrenband-Fortschritt (The Patch)
      if(GetCursor(killer))
      {
        var pPflasterKillerClonk = GetCursor(killer);
        if(pPflasterKillerClonk->~GetRealCursor())
          pPflasterKillerClonk = pPflasterKillerClonk->~GetRealCursor();

        if((ObjectDistance(pPflasterKillerClonk, this) >= 500) && killicon == ASTR)
          AttemptAwardRibbon(RB09, killer, GetOwner());
      }
      //Ehrenband-Fortschritt (The Glyn)
      if(killicon == AAMS)
        AttemptAwardRibbon(RB11, killer, GetOwner());
      //Ehrenband-Fortschritt (The Bugfixer)
      var effect;
      if(killicon == FGRN && (effect = GetEffect("Ribbon_TheBugfixer", this)) && EffectVar(0, this, effect) == killer)
        AttemptAwardRibbon(RB12, killer, GetOwner());
    }

    //Ehrenband-Fortschritt (The Avenger)
    if(RewardsActive() && LocalN("aRibbonAvenger", FindObject(RWDS))[killer])
      AttemptAwardRibbon(RB10, killer, GetOwner());

    //Achievement-Fortschritt (First Blood)
    DoAchievementProgress(1, AC37, killer);

    //Punkte bei Belohnungssystem (Kill)
    DoPlayerPoints(BonusPoints("Kill"), RWDS_BattlePoints, killer, GetCursor(killer), IC01);

    //Achievements und Punkte für den Killer
    if(GetCursor(killer))
    {
      var pClonk = GetCursor(killer);
      if(pClonk->~GetRealCursor()) pClonk = pClonk->~GetRealCursor();

      if(LastDamageType() == DMG_Projectile && KillIcon()->~IsWeapon2() && !Contained(pClonk))
      {
        if(ObjectDistance(pClonk, this) >= 500)
          //Achievement-Fortschritt (Bullseye)
          DoAchievementProgress(1,AC39,killer);

        if(ObjectDistance(pClonk, this) >= 350)
          //Achievement-Fortschritt (Marksman)
          DoAchievementProgress(1,AC17,killer);
      }

      var pActTarget = GetActionTarget(0, pClonk);

      //Helikopterbezogene Punkte und Achievements
      if(Contained(pClonk) && Contained(pClonk)->~IsHelicopter())
      {
        if(LastDamageType() == DMG_Projectile || LastDamageType() == DMG_Explosion)
        {
          //Achievement-Fortschritt (Get to the Chopper)
          DoAchievementProgress(1,AC18,killer);
          
          if((GetDamage(Contained(pClonk)) >= Contained(pClonk)->MaxDamage() * 3 / 4) && GetAchievementExtra(AC57, killer) == Contained(pClonk))
          	DoAchievementProgress(1, AC57, killer);
        }
        if(Contained(pClonk)->~GetPilot())
        {
          //Eventuellem Piloten Assistpunkte geben
          var pPilot = Contained(pClonk)->~GetPilot();
          if(pPilot->~GetRealCursor()) pPilot = pPilot->~GetRealCursor();
          if(GetOwner(pPilot) != GetOwner(pClonk))
          {
            //Punkte bei Belohnungssystem (Kill Assist aus Fahrzeug heraus)
            DoPlayerPoints(BonusPoints("Assist"), RWDS_TeamPoints, GetOwner(pPilot), pPilot, IC02);

            //Achievement-Fortschritt (Air Superiority)
            DoAchievementProgress(1,AC28,GetOwner(pPilot));
          }
        }
      }
      else if(GetID(GetActionTarget(0, pClonk)) == GNET)
      {
        //Achievement-Fortschritt (To your Stations!)
        DoAchievementProgress(1, AC35, killer);
      }
      else if(pClonk->~GetParachute())
      {
        var killicon = KillIcon();

        if(FindContents(killicon, pClonk))
       	  //Achievement-Fortschritt (Paratrooper)
          DoAchievementProgress(1, AC34, killer);
      }
      else if(KillIcon() == ATRP)
      {
        //Achievement-Fortschritt (Tower Defence)
        DoAchievementProgress(1, AC40, killer);
      }
      else if(KillIcon() == MOTN)
      {
        //Achievement-Fortschritt (Trained)
        DoAchievementProgress(1, AC48, killer);
      }

      if(GetEffect("BaseAssaultBomb", pClonk))
      {
        //Achievement-Fortschritt (Quarterback)
        DoAchievementProgress(1, AC50, killer);
      }
    }

    //Killassist
    var assist = GetAssist(killer); //Ausnahme: Killer
    if(assist != killer && GetPlayerName(assist))
    {
      //Punkte bei Belohnungssystem (Kill Assist)
      DoPlayerPoints(BonusPoints("Assist"), RWDS_BattlePoints, assist, GetCursor(assist), IC02);
      //Achievement-Fortschritt (Helping Hand)
      if(!Hostile(assist, killer)) DoAchievementProgress(1, AC01, assist);
    }
    if(LastDamageType() == DMG_Explosion)
    {
      if(!aDoomBoom[1]) aDoomBoom[1] = 0;
      aDoomBoom[1]++;
    }
  }

  //Punkte für Sensorballmarkierung
  var marker = FindObject2(Find_ID(SM08), Find_Allied(killer), Find_Action("Attach"), Find_ActionTarget(this)); 
    if(marker && GetOwner(marker) != killer) 
      //Punkte bei Belohnungssystem (Kill Assist durch Sichtung)
      DoPlayerPoints(BonusPoints("VisualAssist"), RWDS_TeamPoints, GetOwner(marker), GetCursor(GetOwner(marker)), IC02);

  //Achievement-Fortschritt (Non-Swimmer)
  if(GBackLiquid()) DoAchievementProgress(1, AC11, GetOwner());

  var killicon = KillIcon();

  if(killicon)
  {
    if(KillIcon() == AHBS)
    {
      //Achievement-Fortschritt (Ultimate Sacrifice)
      DoAchievementProgress(1, AC46, GetOwner());
    }
  }

  //Selbstmord
  if(LastDamageType() == DMG_Explosion && killer == GetOwner())
  {
    if(!aDoomBoom[0]) aDoomBoom[0] = true;
  }

  //DoomBoom
  if(LastDamageType() == DMG_Explosion)
  {
    SetAchievementExtra(aDoomBoom, AC15, killer);
    if(aDoomBoom[0] && aDoomBoom[1])
      if(aDoomBoom[0] == true && aDoomBoom[1] >= AC15->~GetAchievementScore())
        AwardAchievement(AC15, killer);
    Schedule(Format("ResetAchievementExtra(AC15, %d)", killer), 10);
  }

  //Teamkiller
  if(!Hostile(killer,GetOwner()) && killer != GetOwner() && !(killer < 0))
    //Punkte bei Belohnungssystem (Teamkill)
    DoPlayerPoints(BonusPoints("Teamkill"), RWDS_MinusPoints, killer, GetCursor(killer), IC06);

  //Punkte für Einräucherer/Blender
  var effectno;
  if(effectno = GetEffect("SmokeGrenade", this))
  {
    if(Hostile(EffectVar(1, this, effectno),GetOwner()) && killer != EffectVar(1, this, effectno))
    {
      //Punkte bei Belohnungssystem (Kill Assist durch Blendung)
      DoPlayerPoints(BonusPoints("VisualAssist"), RWDS_TeamPoints, EffectVar(1, this, effectno), GetCursor(EffectVar(1, this, effectno)), IC19);
      //Achievement-Fortschritt (Stunning Help)
      DoAchievementProgress(1, AC22, EffectVar(1, this, effectno));
    }
  }
  if(effectno = GetEffect("IntFlashbang", this))
  {
    if(Hostile(EffectVar(2, this, effectno),GetOwner()) && killer != EffectVar(2, this, effectno))
    {
      //Punkte bei Belohnungssystem (Kill Assist durch Blendung)
      DoPlayerPoints(BonusPoints("VisualAssist"), RWDS_TeamPoints, EffectVar(2, this, effectno), GetCursor(EffectVar(2, this, effectno)), IC19);
      //Achievement-Fortschritt (Stunning Help)
      DoAchievementProgress(1, AC22, EffectVar(2, this, effectno));
    }
  }
}

public func KillIcon(id idKillIcon)
{
  if(idKillIcon)
    killicon = idKillIcon;
  return killicon;
}

/* Inventar */

public func MaxContentsCount()	{return 3;}

/* Steuerungs-Callbacks */

protected func ControlSpecial()
{
  if(Contained())
  {
    if(Contained()->~ContainedSpecial(this))
      return 1;
  }
  return _inherited(...);
}

protected func ControlSpecial2()
{
  [$CtrlMenuDesc$|Image=CXTX]

  //In einem Gebäude oder Fahrzeug: das Kontextmenü des Gebäudes öffnen
  if(Contained())
  {
    if(Contained()->~ContainedSpecial2(this))
      return 1;
    if((GetCategory(Contained()) & C4D_Structure) || (GetCategory(Contained()) & C4D_Vehicle))
    {
      ExecuteCommand();
      return SetCommand(this,"Context",0,0,0,Contained());
    }
  }
  //Fasst ein Objekt an: Kontextmenü des angefassten Objekts öffnen
  if(GetAction() == "Push" || IsRiding())
  {
    ExecuteCommand();
    return SetCommand(this,"Context",0,0,0,GetActionTarget());
  }
  //Trägt ein Objekt: Kontextmenü des ersten getragenen Objekts öffnen
  if(Contents(0))
  {
    ExecuteCommand();
    return SetCommand(this,"Context",0,0,0,Contents(0));
  }
  //Ansonsten das Kontextmenü des Clonks öffnen
  ExecuteCommand();
  return SetCommand(this,"Context",0,0,0,this);
}

/* Bildschirmfärbung bei Schaden */

global func FxDmgCheckDamage(object pTarget, int iEffect, int iDmg, int iCause)
{
  if(!IsFakeDeath(pTarget) && iDmg < 0)
  {
    var iMaxAlpha = 160;
    var iAlpha = BoundBy(iDmg / -200, 0, iMaxAlpha), pScreen = GetScreenRGB(GetOwner(pTarget), SR4K_LayerDamage);
    if(pScreen)
      pScreen->~DoAlpha(iAlpha, iMaxAlpha, 255);
    else
      ScreenRGB(pTarget, RGB(255), iAlpha, 4, false, SR4K_LayerDamage);
  }

  var idKillIcon;
  if(iCause == 2 || iCause == 35)
    idKillIcon = GSAM;
  else if(iCause == 1 || iCause == 3)
    idKillIcon = BOOM;
  else if(iCause == 38)
    idKillIcon = GLOB;
  else if(iCause == 40)
    idKillIcon = SM04;
  else if(iCause == 34 && pTarget->~IsClonk())
    idKillIcon = GetID(LocalN("pLastObjectHit", pTarget));

  if(idKillIcon)
    pTarget->~KillIcon(idKillIcon);

  return _inherited(pTarget, iEffect, iDmg, iCause, ...);
}

/* Assist: Abbau von Schaden bei Heilung */

public func FxAssistDmgReductionDamage(object pTarget, int iEffect, int iDmg)
{
  if(!IsFakeDeath(pTarget) && iDmg > 0)
  {
    for(var i = 0; i < GetLength(assistkiller); i++)
    {
      assistkiller[i][1] = Max(assistkiller[i][1]-iDmg/1000, 0);
      if(!assistkiller[i][1] && assistkiller[i][0] != -1)
        DelArrayItem4K(assistkiller, i);
    }
  }

  return iDmg;
}

/* Fake Death */

global func PauseFakeDeath(bool fPause, object pTarget)
{
  return;
}

global func FxFakeDeathContainedStart(object pTarget, int iNr, int iTemp, object pContainer, id idContainer)
{
  EffectVar(0, pTarget, iNr) = pContainer;
  EffectVar(1, pTarget, iNr) = idContainer;
}

global func FakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  if(!pTarget->IsClonk()) return false;

  if(Contained(pTarget))
    AddEffect("FakeDeathContained", pTarget, 1, 3, 0, 0, Contained(pTarget), GetID(Contained(pTarget)));
  pTarget->OnFakeDeath();

  //Lebensenergie
  DoEnergy(1 - GetEnergy(pTarget), pTarget);

  //Achievements
  if(Hostile(GetOwner(pTarget), GetKiller(pTarget)))
  {
    var data = GetAchievementExtra(AC08, GetKiller(pTarget));
    if(!data) data = CreateArray();
    data[GetOwner(pTarget)]++;
    SetAchievementExtra(data, AC08, GetKiller(pTarget));
    if(data[GetOwner(pTarget)] >= AC08->GetAchievementScore())
    {
      //Achievement-Fortschritt (Epic Kill)
      AwardAchievement(AC08, GetKiller(pTarget));
    }
  }

  ResetAchievementProgress(AC12, GetOwner());
  ResetAchievementProgress(AC14, GetOwner());

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
  pTarget->DeathAnnounce(GetOwner(pTarget), pTarget, GetKiller(pTarget), false, pTarget->~GetAssist(GetKiller(pTarget)) + 1);

  //Eventuelle Assists zurücksetzen
  pTarget->~ResetAssist();

  SetComDir(COMD_Stop,pTarget);

  //Spielziele benachrichtigen
  GameCallEx("IsFulfilled");

  return true;
}

public func OnFakeDeath()	{}

global func StopFakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(GetID(pTarget) == FKDT)
    pTarget = pTarget->GetClonk();
  if(!pTarget) return false;
  if(!pTarget->IsClonk()) return false;

  Sound("ClonkCough*.ogg",0,pTarget);

  //Spezielle Reanimated-Action vorhanden: Diese verwenden, da kein ObjectDisabled
  if(GetActMapVal("Name", "Reanimated", GetID(pTarget)))
    ObjectSetAction(pTarget,"Reanimated",0,0,1);
  //Ansonsten FlatUp-Action verwenden (Inventar wird dann ausgeworfen)
  else
    ObjectSetAction(pTarget,"FlatUp",0,0,1);

  Contained(pTarget)->Reanimation();

  return true;
}

global func IsFakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;

  return GetID(Contained(pTarget)) == FKDT;
}

global func FxFakeDeathDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  if(IsFakeDeath(pTarget))
    return 0;

  if(GetEnergy(pTarget) <= -iDmgEngy/1000 && (FindObject(NOFD) || !IsFakeDeath(pTarget)))
  {
    //Achievements
    if(Hostile(GetKiller(pTarget), GetOwner(pTarget)))
    {
      var killicon = pTarget->~KillIcon();
      if(killicon && killicon->~IsWeapon())
      {
        var wpn = GetAchievementExtra(AC09, GetKiller(pTarget));
        if(!wpn) wpn = CreateArray();
        wpn[FindInArray4K(AC09->GetIDs(), killicon)] = true;
        var i = 0, count = 0;
        while(i <= GetLength(AC09->GetIDs())-1)
        {
          if(wpn[i]) count++;
          i++;
        }
        SetAchievementExtra(wpn, AC09, GetKiller(pTarget));
        if(count >= GetLength(AC09->GetIDs()))
        {
          //Achievement-Fortschritt (Mission accomplished)
          AwardAchievement(AC09, GetKiller(pTarget));
        } 
       }
      if(GetProcedure(pTarget) == "FLIGHT" && GetProcedure(GetCursor(GetKiller(pTarget))) == "FLIGHT")
        if(GetActTime(pTarget) > 10 && GetActTime(GetCursor(GetKiller(pTarget))) > 10)
        {
          //Achievement-Fortschritt (Fly-By)
          DoAchievementProgress(1, AC10, GetKiller(pTarget)); 
          //Ehrenband-Fortschritt (The Eagle)
          AttemptAwardRibbon(RB06, GetKiller(pTarget), GetOwner());
        }

      if(EffectVar(0,Contents(0, pTarget),GetEffect("Silencer", Contents(0, pTarget))) == 200)
      {
        //Achievement-Fortschritt (Peek-a-Boo)
        DoAchievementProgress(1, AC55, GetKiller(pTarget));
      }
    }
  }

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

func Death()
{
  //Umgehung von Hazard-Effektentfernung
  return Death2();
}

func Death2()
{
  var pTarget = this;
  if(!pTarget) return;

  if(pTarget->~HasCrawled())
    SetPhase(5, pTarget);

  //Achievements
  ResetAchievementExtra(AC08, GetOwner(pTarget));
  var data = GetAchievementExtra(AC08, GetKiller(pTarget));
  if(!data) data = CreateArray();
  data[GetOwner(pTarget)] = 0;
  if(data[GetOwner(pTarget)] >= AC08->GetAchievementScore())
  {
    //Achievement-Fortschritt (Epic Kill)
    AwardAchievement(AC08, GetKiller(pTarget));
  }
  SetAchievementExtra(data, AC08, GetKiller(pTarget));
  ResetAchievementProgress(AC12, GetOwner());
  if(!IsFakeDeath())
  {
    ResetAchievementProgress(AC14, GetOwner());
  }

  //Eventuell Statistiken speichern
  var rewards;
  if((rewards = FindObject(RWDS)))
  {
    if(Hostile(GetKiller(pTarget), GetOwner(pTarget)))
      rewards->SetPlayerData(rewards->GetPlayerData(RWDS_KillCount, GetKiller(pTarget))+1, RWDS_KillCount, GetKiller(pTarget));

    rewards->SetPlayerData(rewards->GetPlayerData(RWDS_DeathCount, GetOwner(pTarget))+1, RWDS_DeathCount, GetOwner(pTarget));
  }

  // Bei Soforttod Todesnachricht einblenden und Achievements bearbeiten
  if(FindObject(NOFD))
  {
    //Todesnachricht
    pTarget->DeathAnnounce(GetOwner(pTarget), pTarget, GetKiller(pTarget), false, pTarget->~GetAssist(GetKiller(pTarget)) + 1);
  }

  if(IsFakeDeath())
  {
    if(RewardsActive())
    {
      if(IsDeveloper(GetPlayerID(GetKiller(pTarget))) && GetPlrExtraData(GetKiller(pTarget), "CMC_Team_Ribbon") == 10)
        LocalN("aRibbonAvenger", FindObject(RWDS))[GetOwner()] = true;
      else
        LocalN("aRibbonAvenger", FindObject(RWDS))[GetOwner()] = false;
    }

    pTarget->InstantDie();
  }
  else
  {
    ScheduleCall(pTarget, "DeathSound", 1, 0, pTarget);
  }

  //Verschwinden
  FadeOut(pTarget);
}

public func InstantDie()
{
  //Fallanimation überspringen
  SetPhase(GetActMapVal("Length", GetAction(this))-1);
  Sound("Death", false, this);
  ClearScheduleCall(this, "DeathSound");
  return true;
}

public func DeathSound(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return;
  Sound("ClonkDie*.ogg", false, pTarget);
}

protected func ContextStatistics(object pCaller)
{
  [$CtxStatistics$|Image=RWDS|Condition=RewardsActive]
  var db = FindObject(RWDS);
  if(db) return db->Activate(GetOwner(pCaller));
  return false;
}

/* Steuerung */

protected func ControlUp()
{
  //Sprunggeräusch
  if(!Contained())
  {
    if(WildcardMatch(GetAction(), "*Walk*"))
      ScheduleCall(0, "JumpSound", 1);
    if(WildcardMatch(GetAction(), "*Jump*") || WildcardMatch(GetAction(), "*Dive*") || WildcardMatch(GetAction(), "JetpackFlight"))
    {
      var pHeli = FindObject2(Find_ID(ENTR), Find_AtPoint(0, 0), Find_Not(Find_Hostile(GetOwner())));
      if(!GetEffect("HeliEnterCooldown", this) && pHeli)
      {
        var parachute = FindObject2(Find_ID(PARA), Find_ActionTarget(this));
        if(parachute)
          parachute->StartFlyFree();

        Enter(pHeli, this);

        if(this->~IsArmed())
          SetAction("WalkArmed");
        else
          SetAction("Walk");

        return true;
      }
    }
  }
  //Steuerung an Effekt weitergeben
  if(Control2Effect("ControlUp"))
    return 1;
  //Steuerung an Pferd weiterleiten
  if(IsRiding())
    return(GetActionTarget()->~ControlUp(this()));
  //Bei JnR Delfinsprung
  if(GetPlrCoreJumpAndRunControl(GetController()))
    DolphinJump();
  //Keine überladene Steuerung
  return;
}

protected func JumpSound()
{
  if(!Contained())
    if(GetCommand() != "Enter")
    {
      Sound("ClonkFall*.ogg", 0, 0, 60);
      if(!Random(6)) Sound("ClonkVestRustle*.ogg", 0, 0, 25);
    }
}

private func Riding()
{
  //Richtung an die des berittenen Objekts anpassen
  if(GetActionTarget()->~VaryingDir())
    SetDir(GetActionTarget()->~VaryingDir()-2);
  else
    SetDir(GetDir(GetActionTarget()));
  //Objekt steht still: Clonk soll auch still sitzen
  if(GetActionTarget()->~IsStill())
  {
    if(GetAction() != "RideStill")
      SetAction("RideStill");
  }
  //Objekt steht nicht still: Clonk soll auch nicht still sitzen
  else
    if(GetAction() != "Ride")
      SetAction("Ride");
  return 1;
}
