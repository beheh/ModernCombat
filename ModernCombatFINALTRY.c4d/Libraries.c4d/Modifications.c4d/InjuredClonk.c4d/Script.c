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
public func BlockTime()		{return 35*3;}
public func RWDS_MenuAbort()	{return true;}
public func MenuQueryCancel(int iSelection, object pMenuObject)
{
  if(GetMenu(pMenuObject) == SPEC)
  {
    SPEC->SpectateObject(pMenuObject);
    return false;
  }

  return true;
}


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

/* Munitionsgürtel */

public func AmmoStoring()
{
  if(!clonk) return;

  //Auf Munitionsgürtel des Clonks verweisen
  return clonk->~AmmoStoring();
}

/* Ablehnung */

public func RejectReanimation()	{return rejected || (clonk && !GetAlive(clonk));}

private func Reject()
{
  if(!symbol) return DoMenu();
  rejected = !rejected;

  //Symbol umschalten
  if(rejected)
    symbol->SetGraphics("Negative");
  else
  {
    AddEffect("BlockRejectReanimation", this, 101, BlockTime(), this);
    symbol->SetGraphics("");
  }
  return DoMenu();
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

  //Hinweissound für Schwerverletzten
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

  //Effekte setzen
  SetFakeDeathEffects(pClonk, false, FKDT_SuicideTime*35);
  Sound("FKDT_Heartbeat.ogg", false, pClonk, 100, GetOwner(pClonk)+1, +1);

  //Verzögert Auswahlmenü öffnen
  AddEffect("IntFakeDeathMenu", this, 1, 1, this);
}

public func SetFakeDeathEffects(object pClonk, bool fNoScreenRGB, int iTime)
{
  //Sichtwerte speichern
  var e = AddEffect("IntFakeDeathEffectsData", pClonk, 1, 10, 0, FKDT);
  EffectVar(0, pClonk, e) = GetObjPlrViewRange(pClonk);
  EffectVar(1, pClonk, e) = GetPlrFogOfWar(GetOwner(pClonk));
  EffectVar(2, pClonk, e) = iTime;

  //Sichtwerte für den FakeDeath setzen
  SetFoW(true,GetOwner(pClonk)); 
  SetPlrViewRange(150,pClonk);

  //Soundloop starten
  Sound("FKDT_ClonkDown.ogg", false, pClonk, 100, GetOwner(pClonk)+1, +1);

  //Bildschirmfärbung
  if(!fNoScreenRGB)
    ScreenRGB(pClonk, RGB(255), 120, 4, false, SR4K_LayerDamage);
}

public func ResetFakeDeathEffects(object pClonk)
{
  //Soundloop beenden
  Sound("FKDT_ClonkDown.ogg", false, pClonk, 100, GetOwner(pClonk)+1, -1);

  //Bildschirmfaerbung
  var pScreen = GetScreenRGB(GetOwner(pClonk), SR4K_LayerDamage);
  if(pScreen) RemoveObject(pScreen);

  //Sichtwerte wiederherstellen
  var e = GetEffect("IntFakeDeathEffectsData", pClonk);
  SetFoW(EffectVar(1, pClonk, e), GetOwner(pClonk));
  SetPlrViewRange(EffectVar(0, pClonk, e), pClonk);
  RemoveEffect("IntFakeDeathEffectsData", pClonk);

  if(FindObject2(Find_ID(SPEC), Find_Owner(GetOwner(pClonk))))
    FindObject2(Find_ID(SPEC), Find_Owner(GetOwner(pClonk)))->Set(pClonk);
}

public func FxIntFakeDeathEffectsDataTimer(object pTarget, int iNr, int iTime)
{
	var remaining_time = 1 + ((EffectVar(2, pTarget, iNr) - iTime) / 35);
  if(GetMenu(pTarget) != FKDT && remaining_time >= 0)
    CustomMessage(Format("$DeathCounter$", remaining_time), FindObject2(Find_ID(1HUD), Find_Owner(GetOwner(pTarget))), GetOwner(pTarget), 0, 80, 0, 0, 0, 1);
  else
    CustomMessage("", FindObject2(Find_ID(1HUD), Find_Owner(GetOwner(pTarget))), GetOwner(pTarget));
  
  if(remaining_time < 0)
  	return -1;
}

public func FxIntFakeDeathEffectsDataStop(object pTarget, int iNr)
{
  CustomMessage("", FindObject2(Find_ID(1HUD), Find_Owner(GetOwner(pTarget))), GetOwner(pTarget));
}

public func KillMessage(string msg)
{
  //Killnachricht setzen
  killmsg = msg;

  //Spieler hat Hilfen aktiviert: Quicktipp geben
  if(clonk && !clonk->~ShorterDeathMenu())
    aTipps = GetQuickTipps(this);

  DoMenu();
}

protected func FxIntFakeDeathMenuTimer(object pTarget, int iEffect, int iTime)
{
  if(!pTarget)
    return -1;

  //Verbleibende Zeit an Spiel weiterleiten
  GameCall("SetPlayerRespawnTime", GetOwner(pTarget), (FKDT_SuicideTime*35)-iTime);

  //Tot: Abbruch
  if(!clonk || iTime >= FKDT_SuicideTime * 35)
  {
    pTarget->~Remove();
    return -1;
  }
  var pClonk = pTarget->~GetClonk();
  if(!pClonk)
    return;

  //Leiche soll nicht zu früh ausfaden
  while(GetEffect("*FadeOut*", pClonk))
    RemoveEffect("*FadeOut*", pClonk);
  if(!(iTime % 10))