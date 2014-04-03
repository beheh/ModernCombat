/*-- Sprengladung --*/

#strict 2

public func IsSpecialGoalItem()	{return true;}
public func IsBorderTarget()	{return true;}


/* Initialisierung */

public func Initialize()
{
  AddEffect("BombEffect", this, 101, 100, this);
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

  //Verschwinden
  RemoveObject(this);
  return true;
}

public func AddBombObject(object pTarget)
{
  AddEffect("BaseAssaultBomb", pTarget, 1, 2, 0, C4P2);
  return true;
}

public func FxBombEffectTimer(object pTarget)
{
  Beep(pTarget);
  return true;
}

/* Trageeffekt für Bombenträger */

public func Beep(object pTarget)
{
  pTarget->AddLightFlash(300, 0, 0, RGB(255), pTarget);
  Sound("AHBS_Beep1.ogg", false, pTarget);

  return true;
}

public func FxBaseAssaultBombStart(object pTarget, int iNr)
{
  //Grafik setzen
  SetGraphics(0, pTarget, EFLN, EffectVar(0, pTarget, iNr) = GetUnusedOverlayID(1, pTarget), GFXOV_MODE_Base);
}

public func FxBaseAssaultBombTimer(object pTarget, int iNr, int iTime)
{
  //Blinken
  if(!(iTime % 100))
  {
    //Effekte
    C4P2->Beep(pTarget);
  }
  
  if(!(Random(20)) && GetProcedure(pTarget) == "WALK" && (Abs(GetXDir(pTarget)) > 5 || Abs(GetYDir(pTarget)) > 5))
  	Sound("BOMB_Rustle*.ogg", 0, pTarget, 100);

  //Träger verstorben: Sprengladung fallenlassen
  if(pTarget->~IsFakeDeath())
    return -1;
}

public func FxBaseAssaultBombStop(object pTarget, int iNr, int iReason)
{
  if(iReason >= 3 || (pTarget && pTarget->~IsFakeDeath()))
  {
    //Eventnachricht: Sprengladung fallengelassen
    EventInfo4K(0, Format("$BombDropped$", GetTaggedPlayerName(GetOwner(pTarget))), C4P2, 0, 0, 0, "Info_Event.ogg");

    //Sprengladung erstellen und platzieren
    PlaceBombSpawnpoint(GetX(pTarget), GetY(pTarget));
  }

  //Grafik löschen
  SetGraphics(0, pTarget, 0, EffectVar(0, pTarget, iNr), GFXOV_MODE_Base);
}
