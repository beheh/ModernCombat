/*-- Sprengladung --*/

#strict 2

public func IsSpecialGoalItem()	{return true;}
public func IsBorderTarget()	{return true;}


static const BAR_BASBombBar = 5;

/* Initialisierung */

public func Initialize()
{
  //Bombeneffekt
  AddEffect("BombEffect", this, 101, 2, this);

  //Effekte
  AddLight(300, RGBa(255, 0, 0, 60));
  Sound("RSHL_Deploy.ogg");

  var bar = CreateObject(SBAR, 0, 0, -1);
  bar->Set(this, 0, BAR_BASBombBar, 100, 0, SM24, 0, 11000, true);
  bar->Update(0, true, true);
  bar->SetIcon(0, SM24, 0, 11000, 32);
  bar->PositionToVertex(0, true);

  return _inherited(...);
}

/* Aufnahme */

public func RejectEntrance(object pContainer)
{
  //Kann nur von Clonks aufgehoben werden
  if(!(GetOCF(pContainer) & OCF_CrewMember))
    return true;

  return false;
}

public func Entrance(object pClonk)
{
  //Eventnachricht: Sprengladung aufgehoben
  EventInfo4K(0, Format("$BombCollected$", GetTaggedPlayerName(GetOwner(pClonk))), C4P2, 0, 0, 0, "Info_Event.ogg");

  //Effekt an Clonk übergeben
  AddBombObject(pClonk);
  Sound("RSHL_Deploy.ogg",0,pClonk);

  //Verschwinden
  RemoveObject(this);
  return true;
}

public func AddBombObject(object pTarget)
{
  AddEffect("BaseAssaultBomb", pTarget, 1, 2, 0, C4P2);
  return true;
}

public func FxBombEffectTimer(object pTarget, int iEffect, int iTime)
{
  //Bombe in Grenzgebiet, Lava oder Säure: Entfernen und neue anfordern
  if(FindObject(GBAS) && !(FindObject(GBAS)->SpawningConditions(pTarget)))
  {
    FindObject(GBAS)->DelayedBombRespawn(pTarget, 0, 0);
    return -1;
  }

  //Effekte
  if(!(iTime % 100))
    Beep(pTarget);

  return true;
}

/* Trageeffekt für Bombenträger */

public func Beep(object pTarget)
{
  pTarget->AddLightFlash(300, 0, 0, RGB(255), pTarget);
  Sound("BOMB_Beep.ogg", false, pTarget);

  return true;
}

public func FxBaseAssaultBombStart(object pTarget, int iNr)
{
  //Grafik setzen
  var bar = CreateObject(SBAR, 0, 0, -1);
  bar->Set(pTarget, 0, BAR_BASBombBar, 100, 0, SM24, 0, 11000, true);
  bar->Update(0, true, true);
  bar->SetIcon(0, SM24, 0, 11000, 32);
  bar->PositionToVertex(0, true);

  EffectVar(0, pTarget, iNr) = bar;
  EffectVar(1, pTarget, iNr) = AddLight(300, RGBa(255, 0, 0, 60), pTarget);
  //EffectVar(2) für Counterstrike-Errungenschaft reserviert
  EffectVar(3, pTarget, iNr) = GetTaggedPlayerName(GetOwner(pTarget));
}

public func FxBaseAssaultBombTimer(object pTarget, int iNr, int iTime)
{
  //Blinken
  if(!(iTime % 100))
  {
    //Effekte
    C4P2->Beep(pTarget);
  }

  if(!(Random(5)) && GetProcedure(pTarget) == "WALK" && (Abs(GetXDir(pTarget)) > 5 || Abs(GetYDir(pTarget)) > 5) && !Contained(pTarget))
    Sound("BOMB_Rustle*.ogg", 0, pTarget, RandomX(25,50));

  //Träger verstorben: Sprengladung fallenlassen
  if(pTarget->~IsFakeDeath())
    return -1;
}

public func FxBaseAssaultBombStop(object pTarget, int iNr, int iReason)
{
  if(iReason >= 3 || (pTarget && pTarget->~IsFakeDeath()))
  {
    //Eventnachricht: Sprengladung fallengelassen
    EventInfo4K(0, Format("$BombDropped$", EffectVar(3, pTarget, iNr)), C4P2, 0, 0, 0, "Info_Event.ogg");

    //Sprengladung erstellen und platzieren
    PlaceBombSpawnpoint(GetX(pTarget), GetY(pTarget));
  }

  //Grafik löschen
  if(EffectVar(0, pTarget, iNr))
    RemoveObject(EffectVar(0, pTarget, iNr));

  if(EffectVar(1, pTarget, iNr))
    RemoveObject(EffectVar(1, pTarget, iNr));
}

/* Allgemein */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
  Sound("CrateImpact*.ogg");
}