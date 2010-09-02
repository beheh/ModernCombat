/*-- Peilsender --*/

#strict 2
#include SHT1

public func TrailColor(int iATime)	{return Color(iATime);}


/* Schuss */

private func CreateTrail(int iSize, int iTrail)
{
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail)
  {
    pTrail->Set(iSize+1,iTrail,this);
    ObjectSetAction(pTrail, "Travel2");
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

/* Treffer */

func Hit()
{
  //Bei Materialaufprall entfernen
  Remove();
}

private func HitObject(object pObject)
{
  if(BulletStrike(pObject))
  {
    LaserLight(30, Color(GetActTime()),0,0,10);
  }
}

public func BulletStrike(object pObj)
{
  if(pObj)
  {
    //Passendes Ziel vorhanden?
    if(Hostile(GetOwner(pObj), GetController()) || pObj->~AttractTracer(this))
    {
      //Nicht bereits markiert?
      if(!GetEffect("TracerDart",pObj) && !pObj->~IgnoreTracer())
      {
        //Treffergeräusch
        Sound("TRDT_Attach.ogg");

        //Effekt übergeben
        AddEffect("TracerDart", pObj, 20, 1, 0, 0, GetController());

        //Verschwinden
        Remove();
        
        //Broadcast
        for(var i = 0; i < GetPlayerCount(); i++) {
        	var iPlr = GetPlayerByIndex(i);
        	if(Hostile(GetController(), iPlr)) continue;
        	var fRocketLauncher = false;
        	var j = GetCrewCount(iPlr);
        	while(j--) {
			fRocketLauncher = FindContents(RTLR, GetCrew(iPlr, j));
		      	if(fRocketLauncher) break;
        	}
        	if(fRocketLauncher) {
		        EventInfo4K(iPlr+1, Format("$TargetMarked$", GetPlrColorDw(GetController()), GetPlayerName(GetController())), TRDT, 0, 0, 0, "RadioConfirm*.ogg");
	        }
	}
				
	//Punkte
	DoPlayerPoints(BonusPoints("TracerSet"), RWDS_TeamPoints, GetController(), GetCursor(GetController()), IC17);
        return 1;
      }
    }
    if(pObj->~BlockTracer()) {
    	Remove();
    }
  }
  return 1;
}

/* Farbeffekt */

private func Color(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return GetPlrColorDw(GetController());
}

private func GlowColor()
{
  return RGB(255,90,110);
}

/* Peilsendereffekt */

global func FxTracerDartStart(object pTarget, int iEffectNumber, int iTemp, int iOwner)
{
  if (iTemp) return;
  
  //Besitzer des Schusses festlegen (der Schütze)
  EffectVar(0, pTarget, iEffectNumber) = iOwner;

  //Haftzeit festsetzen (30 Sekunden)
  EffectVar(1, pTarget, iEffectNumber) = 30*38;
  
  //Team speichern
  EffectVar(2, pTarget, iEffectNumber) = GetPlayerTeam(iOwner);
}

global func FxTracerDartTimer(object pTarget, int iEffectNumber)
{
  //FakeDeath?
  if(Contained(pTarget) && GetID(Contained(pTarget)) == FKDT || GetAction(pTarget) == "Dead") return -1;

  //Zielobjekt im Wasser? Entfernen.
  if(GBackLiquid(AbsX(GetX(pTarget)), AbsY(GetY(pTarget)))) return -1;
  
  //Objekt ist nicht mehr feindlich? (zB eingenommene SSA)
  var team;
  if (pTarget && !(team = pTarget->~GetTeam()))
    team = GetPlayerTeam(GetController(pTarget));
  if (team && team == EffectVar(2, pTarget, iEffectNumber))
    return -1;

  //Objekt will den Tracer entfernen
  if (pTarget && pTarget->~RemoveTracer(this))
    return -1;

  //Haftzeit verringern
  EffectVar(1, pTarget, iEffectNumber)--;
  //Haftzeit zuende? Entfernen.
  if(EffectVar(1, pTarget, iEffectNumber) <= 0) return -1;

  //Erstmal Farbe prüfen
  var color = GetPlrColorDw(EffectVar(0, pTarget, iEffectNumber));
  if (GetTeamColor(EffectVar(2, pTarget, iEffectNumber)))
    color = GetTeamColor(EffectVar(2, pTarget, iEffectNumber));
  
  //Blinkeffekt
  pTarget->CreateParticle("FapLight",0,0,0,0,60, color, this);
}

global func FxTracerDartStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if (fTemp) return;

	var iPlr = EffectVar(0, pTarget, iEffectNumber);
	if (!GetPlayerName(iPlr)) return;
	if(!Hostile(GetKiller(pTarget), iPlr) && GetKiller(pTarget) != iPlr)
		if(Contained(pTarget) && GetID(Contained(pTarget)) == FKDT || GetAction(pTarget) == "Dead")
			DoPlayerPoints(BonusPoints("TracerAssist"), RWDS_TeamPoints, iPlr, GetCursor(iPlr), IC02);
}
