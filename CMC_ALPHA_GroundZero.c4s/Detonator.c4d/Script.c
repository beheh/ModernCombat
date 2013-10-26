/*-- Z�nder --*/

#strict 2
#include PACK

public func IsSpecialGoalItem() { return true; }

public func OnSpawnpointCollect(object pSpawn, object pClonk)
{
	EventInfo4K(0, Format("$BombCollected$", GetTaggedPlayerName(GetOwner(pClonk))), C4PA);
	AddBombObject(pClonk);
	RemoveObject(pSpawn);
	RemoveObject(this);
	return true;
}

public func AddBombObject(object pTarget)
{
	AddEffect("BaseAssaultBomb", pTarget, 1, 2, 0, C4P2);
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
    //pTarget->CreateParticle("PSpark", 0, 0, 0, 0, 300, RGB(255), pTarget);
    pTarget->AddLightFlash (300, 0, 0, RGB(255,0,0), pTarget);
    Sound("AHBS_Beep1.ogg", false, pTarget);
  }

  if(pTarget->~IsFakeDeath())
    return -1;
}

public func FxBaseAssaultBombStop(object pTarget, int iNr, int iReason)
{
	if(iReason >= 3 || (pTarget && pTarget->~IsFakeDeath())) //Checken ob innerhalb von Lava/Grenzgebiet
	{
		EventInfo4K(0, Format("$BombDropped$", GetTaggedPlayerName(GetOwner(pTarget))), C4PA);
		PlaceBombSpawnpoint(GetX(pTarget), GetY(pTarget));
	}

	//Grafik l�schen
	SetGraphics(0, pTarget, 0, EffectVar(0, pTarget, iNr), GFXOV_MODE_Base);
}
