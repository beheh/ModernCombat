/*-- Peilsender --*/

#strict 2
#include SHT1

public func TrailColor(int iATime)	{return(Color(iATime));}


/* Schuss */

private func CreateTrail(int iSize, int iTrail)
{
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail)
  {
    pTrail->Set(iSize+1,iTrail,this());
    pTrail->SetAction("Travel2");
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
    if(Hostile(GetOwner(pObj), GetController()))
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
        		var k = GetCrew(iPlr, j)->ContentsCount();
		      	while(k--) {
		      		if(GetCrew(iPlr, j)->Contents(k)->GetID() == RTLR) {
		      			fRocketLauncher = true;
		      			break;
		      		}
		      	}
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
  return(GetPlrColorDw(GetController()));
}

private func GlowColor()
{
  return(RGB(255,90,110));
}

/* Peilsendereffekt */

global func FxTracerDartStart(object pTarget, int iEffectNumber, int iTemp, int iOwner)
{
  //Besitzer des Schusses festlegen (der Schütze)
  EffectVar(0, pTarget, iEffectNumber) = iOwner;

  //Haftzeit festsetzen (20 Sekunden)
  EffectVar(1, pTarget, iEffectNumber) = 20*38;
}

global func FxTracerDartTimer(object pTarget, int iEffectNumber)
{
  //FakeDeath?
  if(Contained(pTarget) && GetID(Contained(pTarget)) == FKDT || pTarget->GetAction() == "Dead") return -1;

  //Zielobjekt im Wasser? Entfernen.
  if(GBackLiquid(AbsX(GetX(pTarget)), AbsY(GetY(pTarget)))) return -1;

  //Haftzeit verringern
  EffectVar(1, pTarget, iEffectNumber)--;
  //Haftzeit zuende? Entfernen.
  if(EffectVar(1, pTarget, iEffectNumber) <= 0) return -1;

  //Blinkeffekt
  pTarget->CreateParticle("FapLight",0,0,0,0,60, GetPlrColorDw(EffectVar(0, pTarget, iEffectNumber)),this());
}

global func FxTracerDartStop(object pTarget, int iEffectNumber)
{
	var iPlr = EffectVar(0, pTarget, iEffectNumber);
	if(!Hostile(GetKiller(pTarget), iPlr) && GetKiller(pTarget) != iPlr)
		if(Contained(pTarget) && GetID(Contained(pTarget)) == FKDT || pTarget->GetAction() == "Dead")
			DoPlayerPoints(BonusPoints("TracerAssist"), RWDS_TeamPoints, iPlr, GetCursor(iPlr), IC02);
}
