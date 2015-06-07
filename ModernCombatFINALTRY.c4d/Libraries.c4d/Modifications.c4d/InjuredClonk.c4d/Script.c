/*-- Schwerverletzter --*/

#strict 2

static const FKDT_SuicideTime = 15; //Standardzeit bei Fake Death in Sekunden
static FKDT_QuickTipIDs;

local clonk, oldvisrange, oldvisstate, killmsg, aTipps, iTippNr;
local rejected, symbol;
local aGrenades, aContents;

public func AimAngle()		{}
public func ReadyToFire()	{}
public func IsAiming()		{}
public func MenuQueryCancel()	{return true;}
public func BlockTime()		{return 35*3;}
public func RWDS_MenuAbort()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  //Anderer Todesschrei zur Unterscheidung zwischen Fake Death und "echtem" Ableben
  Sound("ClonkDie*.ogg");

  aTipps = [];
  aGrenades = [];
  FKDT_QuickTipIDs = [ASTR, MNGN, PSTL, RTLR, PPGN, SGST, SMGN, ATWN, FGRN, FRAG, PGRN, STUN, SGRN, SRBL, AMPK, BTBG, C4PA, DGNN, FAPK, RSHL, CDBT, CUAM, BWTH];

  _inherited(...);
}

/* Munitionsg�rtel */

public func AmmoStoring()
{
  if(!clonk) return;

  //Auf Munitionsg�rtel des Clonks verweisen
  return clonk->~AmmoStoring();
}

/* Ablehnung */

public func RejectReanimation()	{return rejected || (clonk && !GetAlive(clonk));}

private func Reject()
{
  if(!symbol) return DeathMenu();
  rejected = !rejected;

  //Symbol umschalten
  if(rejected)
    symbol->SetGraphics("Negative");
  else
  {
    AddEffect("BlockRejectReanimation", this, 101, BlockTime(), this);
    symbol->SetGraphics("");
  }
  return DeathMenu();
}

public func FxBlockRejectReanimationTimer(object target, int nr, int time)
{
  if(time >= BlockTime())
    return -1;
}

/* Erstellung */

public func Set(object pClonk)
{
  clonk = pClonk;
  SetPosition(GetX(pClonk),GetY(pClonk));
  SetXDir(GetXDir(pClonk));
  SetYDir(GetYDir(pClonk));

  //Hinweissound f�r Schwerverletzten
  Sound("FKDT_FatalHit*.ogg", false, pClonk, 100, GetOwner(pClonk)+1);

  //Reanimationszeichen erstellen
  symbol = CreateObject(SM01,0,0,GetOwner(pClonk));
  symbol->Set(this);

  //CTF-Flagge entfernen
  for(var content in FindObjects(Find_ActionTarget(pClonk),Find_ID(FLA2)))
    if(GetID(content) == FLA2)
      content->~AttachTargetLost();

  //Clonk aufnehmen
  Enter(this, pClonk);

  //Granatensortierung speichern
  var pGrenadeStoring = pClonk->~GetGrenadeStoring();
  if(pGrenadeStoring)
  {
    var nade, i = ContentsCount(0, pGrenadeStoring);
    while(--i >= 0) 
    {
      nade = GetID(Contents(i, pGrenadeStoring));
      if(GetIndexOf(nade, aGrenades) != -1)
        continue;
      
      aGrenades[GetLength(aGrenades)] = nade;
    }
  }

  //Eventuell Granaten holen
  pClonk->~GrabGrenades(this);
  //Objekte des Clonks aufnehmen
  var iCount = ContentsCount(0, pClonk);
  aContents = CreateArray(iCount);
  for(i = 0; i < iCount; i++)
    aContents[i] = GetID(Contents(i, pClonk));
  GrabContents(pClonk);

  //Aussehen des Clonks imitieren
  SetGraphics(0,this,GetID(pClonk),1,GFXOV_MODE_Object,0,0,pClonk);

  //Sichtwerte speichern
  oldvisrange = GetObjPlrViewRange(pClonk);
  oldvisstate = GetPlrFogOfWar(GetOwner(pClonk));

  //Sichtwerte f�r den FakeDeath setzen
  SetFoW(true,GetOwner(pClonk)); 
  SetPlrViewRange(150,pClonk);

  //Soundloop starten
  Sound("FKDT_ClonkDown.ogg", false, pClonk, 100, GetOwner(pClonk)+1, +1);

  //Bildschirmf�rbung
  ScreenRGB(pClonk, RGB(255), 120, 4, false, SR4K_LayerDamage);

  //Verz�gert Auswahlmen� �ffnen
  AddEffect("IntFakeDeathMenu", this, 1, 1, this);
}

public func KillMessage(string msg)
{
  //Killnachricht setzen
  killmsg = msg;

  //Spieler hat Hilfen aktiviert: Quicktipp geben
  if(clonk && !clonk->~ShorterDeathMenu())
    aTipps = GetQuickTipps(this);

  DeathMenu();
}

protected func FxIntFakeDeathMenuTimer(object pTarget, int iEffect, int iTime)
{
  if(!pTarget)
    return -1;

  //Tot: Abbruch
  if(!clonk || iTime >= FKDT_SuicideTime * 35)
  {
    pTarget->~Remove();
    return -1;
  }
  var pClonk = pTarget->~GetClonk();
  if(!pClonk)
    return;

  //Leiche soll nicht zu fr�h ausfaden
  while(GetEffect("*FadeOut*", pClonk))
    RemoveEffect("*FadeOut*", pClonk);
  if(!(iTime % 10))
  {
    pTarget->~DoMenu();

    var iAlpha = Interpolate2(255, 0, iTime, FKDT_SuicideTime * 35), pScreen = EffectVar(0, pTarget, iEffect);
    if(!pScreen)
      pScreen = EffectVar(0, pTarget, iEffect) = ScreenRGB(pClonk, GetScenarioVal("FoWColor"), iAlpha, 0, false, SR4K_LayerFakeDeath);
    if(pScreen)
      pScreen->~SetAlpha(iAlpha);
  }

  //Statistikenmen� geschlossen: Zur�ck zum DeathMen�
  if(GetType(GetMenu(pClonk)) != C4V_C4ID)
    pTarget->~DoMenu();
}

protected func FxIntFakeDeathMenuStop(object pTarget, int iEffect)
{
  var pScreen = EffectVar(0, pTarget, iEffect);
  if(pScreen)
    RemoveObject(pScreen);
}

/* Auswahlmen� */

func DoMenu()
{
  DeathMenu();
}

private func DeathMenu()
{
  if(!GetAlive(clonk))
    return;

  var selection = GetMenuSelection(clonk);

  //Bei offenen Statistiken, Einstellungen oder Effektmanagern nichts unternehmen
  if(GetMenu(clonk) == RWDS || GetMenu(clonk) == EFMN || GetMenu(clonk) == CSTR) return;

  CloseMenu(clonk);

  if(GetMenu(clonk)) return;

  //Men� erstellen
  CreateMenu(FKDT, clonk, this, 0, Format("$DeathCounter$", 1 + TimeLeft() / 35), C4MN_Style_Dialog, true, true);		//Titelzeile mit Zeitanzeige

  if(TimeLeft() < (FKDT_SuicideTime - 1) * 35)
  {
    var blocktime;
    if(GetEffect("BlockRejectReanimation", this))										//Ablehnen-Men�punkt
    {
      blocktime = GetEffect("BlockRejectReanimation", this, 0, 6);
      AddMenuItem(Format("$ReanimationBlocked$", (BlockTime()-blocktime)/35), 0, SM01, clonk, 0, 0, "$ReanimationDescAllow$");
    }
    else
    {
      if(!RejectReanimation())
        AddMenuItem("$ReanimationAllow$", "Reject", SM01, clonk, 0, 0, "$ReanimationDescAllow$");
      else
        AddMenuItem("$ReanimationDisallow$", "Reject", SM06, clonk, 0, 0, "$ReanimationDescDisallow$");
    }
    if(FindObject(SICD))
      AddMenuItem("$Suicide$", "Suicide", PSTL, clonk, 0, 0, "$SuicideDesc$");							//Selbstmord-Men�punkt

    if(FindObject(RWDS))
      AddMenuItem("$Statistics$", Format("FindObject(RWDS)->Activate(%d)", GetOwner(clonk)), RWDS, clonk);			//Statistik-Men�punkt

    if(FindObject(EFMN) && GetOwner(clonk) == GetPlayerByIndex(0, C4PT_User) && !GetLeague())
      AddMenuItem("$EffectLevel$", Format("FindObject(EFMN)->Activate(%d)", GetOwner(clonk)), EFMN, clonk);			//Effektstufe-Men�punkt

    AddMenuItem("$Settings$", "OpenSettings()", CSTR, clonk);									//Einstellungen-Men�punkt

    if(aTipps[iTippNr] && !clonk->~ShorterDeathMenu())
      AddMenuItem("$NextTipp$", "NextTipp", MCMC, clonk);									//N�chster Tipp-Men�punkt
  }

  if(GetType(killmsg) == C4V_String)
  {
    AddMenuItem("", "", NONE, clonk, 0, 0, "", 512, 0, 0);									//Leerzeile
    AddMenuItem(Format("$Killer$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);					//Titel

    AddMenuItem(killmsg, "", NONE, clonk, 0, 0, "", 512);									//Killerinformationen
  }

  if(!clonk->~ShorterDeathMenu())
  {
    var obj;
    if((obj = FindObject(RWDS)))												//Punktestatistik erstellen
    {
      AddMenuItem("", "", NONE, clonk, 0, 0, "", 512, 0, 0);									//Leerzeile
      AddMenuItem(Format("$Points$", GetName(clonk)),"", NONE, clonk, 0, 0, "", 512, 0, 0);					//Titel

      //Einsortieren
      var aList = [], iPlr, aData = obj->~GetData(), szString = "";
      for(iPlr = 0; iPlr < GetLength(aData); ++iPlr)
      {
        if(!aData[iPlr])
          continue;

        var iTeam = obj->~GetPlayerData(RWDS_PlayerTeam, iPlr);
        if(!aList[iTeam]) aList[iTeam] = [];
        szString = Format("%s: %d", obj->~GetPlayerData(RWDS_PlayerName, iPlr), obj->~GetPlayerPoints(RWDS_TotalPoints, iPlr));
        if(clonk->~ShorterDeathMenu())
          szString = Format("%s: %d", obj->~GetPlayerData(RWDS_CPlayerName, iPlr), obj->~GetPlayerPoints(RWDS_TotalPoints, iPlr));

        aList[iTeam][GetLength(aList[iTeam])] = szString;
      }

      //Teamweise ausgeben
      for (var aTeam in aList)
        if(aTeam)
          for (var szString in aTeam)
            if(GetType(szString) == C4V_String)
              AddMenuItem(szString, "", NONE, clonk, 0, 0, "", 512);
    }

    if(GetType(aTipps[iTippNr]) == C4V_Array)
    {
      AddMenuItem("", "", NONE, clonk, 0, 0, "", 512, 0, 0);									//Leerzeile
      AddMenuItem(Format("{{%i}} $Tip$", aTipps[iTippNr][0]), "", NONE, clonk, 0, 0, "", 512, 0, 0);				//Zuf�lliger Tipp
      AddMenuItem(aTipps[iTippNr][1], "", NONE, clonk, 0, 0, "", 512, 0, 0);
    }
  }

  if(selection >= 0) SelectMenuItem(selection, clonk);
}

static const FKDT_MaxTippCount = 10;

public func OpenSettings()
{
  CloseMenu(clonk);
  clonk->~ContextSettings(clonk);
}

public func NextTipp()
{
  iTippNr = (iTippNr + 1) % FKDT_MaxTippCount; 
}

private func GetQuickTipps(object pFake)
{
  var aTippList = [], aIDList = [];
  iTippNr = -1;
  for(var i = 0; i < FKDT_MaxTippCount; i++)
  {
    var tipp, cnt;
    if(!Random(8))
    {
      tipp = GetGeneralTipp();
      while(GetIndexOf(tipp[1], aTippList) != -1)
      {
        tipp = GetGeneralTipp();
        if(++cnt > 6)
        {
          i--;
          tipp = 0;
          break;
        }
      }

      if(tipp)
      {
        aIDList[GetLength(aIDList)] = tipp[0];
        aTippList[GetLength(aTippList)] = tipp[1];
      }
    }
    else
    {
      if(iTippNr == -1)
        iTippNr = i;

      var aObj = FindObjects(Find_Container(pFake), Find_Func("FKDTSupportedID"));
      var id = GetID(aObj[Random(GetLength(aObj))]);
      if(!GetLength(aObj))
        id = FKDT_QuickTipIDs[Random(GetLength(FKDT_QuickTipIDs))];

      tipp = GetRandomTipp(0, id);
      while(GetIndexOf(tipp[1], aTippList) != -1)
      { 
        tipp = GetRandomTipp(0, id);
        if(++cnt > 6)
        {
          i--;
          tipp = 0;
          break;
        }
      }

      if(tipp)
      {
        aIDList[GetLength(aIDList)] = tipp[0];
        aTippList[GetLength(aTippList)] = tipp[1];
      }
    }
  }

  if(iTippNr == -1)
    iTippNr = 0;

  var tipps = [];
  for(var i = 0; i < GetLength(aTippList); i++)
    tipps[i] = [aIDList[i], aTippList[i]];

  return tipps;
}

global func FKDTSupportedID()	{return GetIndexOf(GetID(), FKDT_QuickTipIDs) != -1;}

private func GetGeneralTipp()
{
  return GetRandomTipp([[FGRN, "$NONE0$"], [CSTR, "$NONE1$"], [SM05, "$NONE2$"], [SM04, "$NONE3$"], [XBRL, "$NONE4$"], [PCMK, "$NONE5$"], [PCMK, "$NONE6$"], [SM01, "$NONE7$"], [PSTL, "$NONE8$"], [BKHK, "$NONE9$"], [SM04, "$NONE10$"], [MAVE, "$NONE11$"], [BDSN, "$NONE12$"], [CXTX, "$NONE13$"]]);
}

private func GetRandomTipp(array a, id id)
{
  if(a)
    return a[Random(GetLength(a))];

  //Waffen
  if(id == ASTR) return GetRandomTipp([[ASTR, "$ASTR0$"], [ASTR, "$ASTR1$"]]);
  if(id == MNGN) return GetRandomTipp([[MNGN, "$MNGN0$"]]);
  if(id == PSTL) return GetRandomTipp([[PSTL, "$PSTL0$"], [PSTL, "$PSTL1$"]]);
  if(id == RTLR) return GetRandomTipp([[MISL, "$RTLR0$"], [RTLR, "$RTLR1$"], [MISL, "$RTLR2$"], [MISL, "$RTLR3$"]]);
  if(id == PPGN) return GetRandomTipp([[PPGN, "$PPGN0$"]]);
  if(id == SGST) return GetRandomTipp([[SGST, "$SGST0$"]]);
  if(id == SMGN) return GetRandomTipp([[SMGN, "$SMGN0$"], [SMGN, "$SMGN0$"]]);
  if(id == ATWN) return GetRandomTipp([[ATWN, "$ATWN0$"], [AAMS, "$ATWN1$"]]);

  //Granaten
  if(id->~IsGrenade() && !Random(6)) return GetRandomTipp([[BOOM, "$NADE0$"]]);
  if(id == FGRN) return GetRandomTipp([[FGRN, "$FGRN0$"]]);
  if(id == FRAG) return GetRandomTipp([[FRAG, "$FRAG0$"], [SHRP, "$FRAG1$"]]);
  if(id == PGRN) return GetRandomTipp([[PGRN, "$PGRN0$"]]);
  if(id == STUN) return GetRandomTipp([[STUN, "$STUN0$"], [STUN, "$STUN1$"]]);
  if(id == SGRN) return GetRandomTipp([[SM4K, "$SGRN0$"]]);
  if(id == SRBL) return GetRandomTipp([[SM08, "$SRBL0$"], [SRBL, "$SRBL1$"]]);

  //Equipment
  if(id == AMPK) return GetRandomTipp([[AMPK, "$AMPK0$"]]);
  if(id == BTBG) return GetRandomTipp([[BBTP, "$BTBG0$"], [BBTP, "$BTBG1$"], [BBTP, "$BTBG2$"]]);
  if(id == C4PA) return GetRandomTipp([[C4PA, "$C4PA0$"], [C4PA, "$C4PA1$"]]);
  if(id == DGNN) return GetRandomTipp([[DGNN, "$DGNN0$"], [DGNN, "$DGNN1$"]]);
  if(id == FAPK) return GetRandomTipp([[FAPK, "$FAPK0$"], [FAPK, "$FAPK1$"], [FAPK, "$FAPK2$"], [BECR, "$FAPK3$"]]);
  if(id == RSHL) return GetRandomTipp([[RSHL, "$RSHL0$"], [RSHL, "$RSHL1$"], [RSHL, "$RSHL2$"], [RSHL, "$RSHL3$"], [SDSD, "$RSHL4$"]]);
  if(id == CDBT) return GetRandomTipp([[CDBT, "$CDBT0$"], [CDBT, "$CDBT1$"]]);
  if(id == CUAM) return GetRandomTipp([[CUAM, "$CUAM0$"]]);
  if(id == BWTH) return GetRandomTipp([[BWTH, "$BWTH0$"], [BWTH, "$BWTH1$"], [BWTH, "$BWTH2$"], [BWTH, "$BWTH3$"]]);
}

/* Selbstmord */

public func Suicide()
{
  //Soundloop beenden
  Sound("FKDT_ClonkDown.ogg", false, clonk, 100, GetOwner(clonk)+1, -1);

  //T�ten
  if(clonk && GetAlive(clonk)) Kill(clonk);
  
  if(symbol)
    RemoveObject(symbol);
}

public func Remove()
{
  //Clonkinventar l�schen sofern Arenaregel aktiv
  if(FindObject(NODR))
    for(var item in FindObjects(Find_Container(this),Find_Not(Find_OCF(OCF_Living))))
      RemoveObject(item);

  Suicide();

  if(clonk)
  {
    //Leiche "auswerfen" und ausfaden lassen
    Exit(clonk,0,GetObjHeight(clonk)/2);
    clonk->InstantDie();
    clonk->FadeOut();
  }

  //Verschwinden
  RemoveObject();
}

/* Calls */

public func GetClonk()		{return clonk;}

public func TimeLeft()		{return FKDT_SuicideTime * 35 - GetEffect("IntFakeDeathMenu", this, 0, 6);}

/* Zerst�rung */

public func Destruction()
{
  while(Contents())
  {
    RemoveObject(Contents());
  }

  //Soundloop beenden
  Sound("FKDT_ClonkDown.ogg", false, clonk, 100, GetOwner(clonk)+1, -1);
}

/* Zus�tzliches Inventar indizieren */

public func Collection2(object pObj)
{
  if(!aContents || !clonk || !GetAlive(clonk))
    return;

  if(GetIndexOf(GetID(pObj), aContents) != -1 || GetIndexOf(GetID(pObj), aGrenades) != -1)
    return;

  if(pObj->~IsGrenade())
    aGrenades[GetLength(aGrenades)] = GetID(pObj);
  else
    aContents[GetLength(aContents)] = GetID(pObj);
}

/* Wiederbelebung */

public func Reanimation()
{
  //Kein Clonk?
  if(!clonk || !GetAlive(clonk)) return;
  //Reanimation abgelehnt?
  if(RejectReanimation()) return;

  //Bildschirmf�rbung
  var pScreen = GetScreenRGB(GetOwner(clonk), SR4K_LayerDamage);
  if(pScreen) RemoveObject(pScreen);
  AddEffect("FlashlightBlindness", clonk, 150, 1, 0, LHC2);

  //Clonk "auswerfen"
  if(Contained(clonk) == this)
  {
    Exit(clonk,0,GetObjHeight(clonk)/2);
    if(StuckSolid(clonk))
      AutoUnstuck(clonk);
  }

  //Besitzt�mer weitergeben
  if(GetAlive(clonk))
  {
    for(i = 0; i < GetLength(aContents); i++)
    {
      if(GetAction(clonk) == "Reanimated")
        Collect(FindContents(aContents[i], this), clonk);
      else
        Enter(clonk, FindContents(aContents[i], this));
    }

    if(GetLength(aGrenades))
    {
      //Granatensortierung wiederherstellen
      for(var i = 0; i < GetLength(aGrenades); i++)
      {
        var nade = aGrenades[i];
        for(var item in FindObjects(Find_Container(clonk), Find_ID(nade)))
        {
          clonk->~StoreGrenade(item);
          if(!Contained(item)) RemoveObject(item);
        }
      }
    }
    RemoveEffect("NoAnnounce", clonk);
  }
  while(Contents())
  {
    if(!GetAlive(clonk) || !Contents()->~IsClonk())
      Exit(Contents(),0,+10);
  }
  //Sichtdaten zur�cksetzen
  SetFoW(oldvisstate,GetOwner(clonk));
  SetPlrViewRange(oldvisrange,clonk);

  //Achievement-Fortschritt (Lucky Patient)
  DoAchievementProgress(1, AC27, GetOwner(clonk));

  RemoveObject();
}

public func RejectCollect(id idObj, object pObj)
{
  if(!clonk) return;
  var val = clonk->~RejectCollect(idObj,pObj);
  return val;
}

/* Steuerung */

public func ControlUp(object pCaller)
{
  if(!pCaller || Hostile(GetOwner(pCaller), GetOwner()) || rejected || !clonk || !GetAlive(clonk))
    return _inherited(pCaller, ...);

  //Schwerverletzten nach Defibrillator durchsuchen
  if(!pCaller->~ActivateShockPaddles())
    var defi = FindObject2(Find_Func("IsShockPaddles"), Find_Container(this));
  else
    return _inherited(pCaller, ...);

  //Wenn gefunden: Nehmen und Ausl�sen planen
  if(defi)
  {
    SetCommand(pCaller, "Get", defi);
    Schedule("ActivateShockPaddles()", 2, 0, pCaller);
  }
  else
    PlayerMessage(GetOwner(pCaller), "$NoShockPaddles$", pCaller);

  return _inherited(pCaller, ...);
}

public func ControlDig(object pCaller)
{
  if(pCaller == clonk) return 1;
  //Herausnehmen per Graben: Holen-Men� �ffnen
  SetCommand(pCaller, "Get", this, 0, 0, 0, 1);
}

public func ControlRightDouble(object pByObj)
{
  //lebenden Clonk drehen und anhalten
  pByObj->SetDir(1);
  pByObj->SetAction("Throw");
  pByObj->SetComDir(COMD_Stop);

  //Schwerverletzten anschieben
  Fling(this, 2, -1);

  //Effekte
  Sound("ClonkImpact*.ogg");
  Sound("ClonkRustle*.ogg", 0, 0, 50);
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",RandomX(4,8),10,0,10,20,100);
}

public func ControlLeftDouble(object pByObj)
{
  //lebenden Clonk drehen und anhalten
  pByObj->SetDir();
  pByObj->SetAction("Throw");
  pByObj->SetComDir(COMD_Stop);

  //Schwerverletzten anschieben
  Fling(this, -2, -1);

  //Effekte
  Sound("ClonkImpact*.ogg");
  Sound("ClonkRustle*.ogg", 0, 0, 50);
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",RandomX(4,8),10,0,10,20,100);
}

/* Aufschlag */ 

protected func Hit(int xDir, int yDir)
{
  var hit = Distance(xDir,yDir);//Max(xDir,yDir);

  if(hit >= 300)
  {
    Sound("ClonkImpact*.ogg");
    Sound("ClonkRustle*.ogg", 0, 0, 50);
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",RandomX(4,8),10,0,10,20,100);
  }
  else
    Sound("ClonkCrawl*.ogg", 0, 0, 50);
}