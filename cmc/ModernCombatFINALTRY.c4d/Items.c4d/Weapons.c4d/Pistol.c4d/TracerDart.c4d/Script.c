/*-- Peilsender --*/

#strict 2
#include SHT1

public func TrailColor(int iATime)	{return Color(iATime);}
public func AllowHitboxCheck()		{return true;}

local iShotAngle, iShotSpeed;


/* Schuss */

private func CreateTrail(int iSize, int iTrail)
{
  if(pTrail)
  {
    pTrail->Set(iSize+1,iTrail,this);
    ObjectSetAction(pTrail, "Travel2");
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

public func Launch()
{
  iShotAngle = Par();
  iShotSpeed = Par(1);
  var i = _inherited(...);
  SetPosition(GetX(), GetY() - GetDefCoreVal("Offset", "DefCore", GetID(), 1) / 2);
  if(!GetEffect("TracerTrail", this))
    AddEffect("TracerTrail", this, 1, 1, this);
  return i;
}

protected func FxTracerTrailTimer()
{
  var iX = -Sin(iShotAngle, iShotSpeed/10),
  iY = Cos(iShotAngle, iShotSpeed/10),
  iDist = -1,
  iClr = TrailColor(GetActTime());
  while(++iDist < iShotSpeed/10)
    CreateParticle("Flare", iX + Sin(iShotAngle, iDist) + Random(5) - 2, iY - Cos(iShotAngle, iDist) + Random(5) - 2, Sin(Random(360), Random(10)), Sin(Random(360), Random(10)), 60 + Random(51), iClr);
}

/* Timer */

private func Traveling()
{
  //In Wasser verschwinden
  if(InLiquid()) return Remove();

  var iATime = GetActTime();

  //Ausfaden
  SetClrModulation(Color(iATime));

  //Löschen
  if(iATime >= iTime) return Remove();

  //Bei Verlassen der Map entfernen
  if(GetY()<0) return Remove();
}

/* Treffer */

func Hit(int iXDir, int iYDir)
{
  //Erst nochmal zeichnen
  SetXDir(iXDir, this, 100);
  SetYDir(iYDir, this, 100);
  EffectCall(this, GetEffect("TracerTrail", this), "Timer");
  LaserLight(30, SetRGBaValue(Color(GetActTime())), 0, 0, 10);

  //Entfernen
  Remove();
}

private func HitObject(object pObject)
{
  if(BulletStrike(pObject))
    LaserLight(30, SetRGBaValue(Color(GetActTime())), 0, 0, 10);
}

public func BulletStrike(object pObj)
{
  if(!pObj)
    return false;

  //Passendes Ziel vorhanden?
  if((Hostile(GetOwner(pObj), GetController()) || pObj->~AttractTracer(this)) && !InLiquid(pObj))
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
      for(var i = 0; i < GetPlayerCount(); i++)
      {
        var iPlr = GetPlayerByIndex(i);
        if(Hostile(GetController(), iPlr))
          continue;
        var pRocketLauncher = 0;
        var j = GetCrewCount(iPlr);
        while(j--)
        {
          pRocketLauncher = FindContents(RTLR, GetCrew(iPlr, j));
          if(pRocketLauncher || GetID(Contained(GetCrew(iPlr, j))) == BKHK)
          {
            //Eventnachricht: Feindobjekt markiert
            EventInfo4K(iPlr+1, Format("$TargetMarked$", GetPlrColorDw(GetController()), GetPlayerName(GetController())), TRDT, 0, 0, 0, "Info_Event.ogg");
            Sound("RadioConfirm*.ogg", true, 0, 0, iPlr+1);
            break;
          }
        }
      }

      if(Hostile(GetOwner(pObj), GetController()))
      {
        //Punkte bei Belohnungssystem (Peilsender platziert)
        DoPlayerPoints(BonusPoints("TracerSet"), RWDS_TeamPoints, GetController(), GetCursor(GetController()), IC17);
        //Achievement-Fortschritt (Radio Warfare)
        DoAchievementProgress(1, AC19, GetController());
      }
      return true;
    }

  if(pObj->~BlockTracer())
    Remove();
  return false;
}

/* Farbeffekt */

private func Color(int iATime)
{
  var iAlpha = Interpolate2(0, 128, iATime, iTime) + Random(32),
  iColor = LightenColor(GetPlrColorDw(GetController()), 92);
  return SetRGBaValue(iColor, iAlpha);
}

private func GlowColor()
{
  return Color(...);
}

/* Peilsendereffekt */

global func FxTracerDartStart(object pTarget, int iEffectNumber, int iTemp, int iOwner)
{
  if(iTemp) return;

  //Besitzer des Schusses festlegen (der Schütze)
  EffectVar(0, pTarget, iEffectNumber) = iOwner;

  //Haftzeit festsetzen (50 Sekunden)
  EffectVar(1, pTarget, iEffectNumber) = 50*38;

  //Team speichern
  EffectVar(2, pTarget, iEffectNumber) = GetPlayerTeam(iOwner);
}

global func FxTracerDartTimer(object pTarget, int iEffectNumber)
{
  //Zerstörung wenn das Ziel in einem Schwerverletzten liegt
  if(Contained(pTarget) && GetID(Contained(pTarget)) == FKDT || GetAction(pTarget) == "Dead") return -1;

  //Zerstörung bei Wasserkontakt
  if(GBackLiquid(AbsX(GetX(pTarget)), AbsY(GetY(pTarget)))) return -1;

  //Zerstörung wenn Ziel nicht mehr feindlich (z.B. eingenommene SSA)
  var team;
  if(pTarget && !(team = pTarget->~GetTeam()))
    team = GetPlayerTeam(GetController(pTarget));
  if(team && team == EffectVar(2, pTarget, iEffectNumber))
    return -1;

  //Ziel ordnet Zerstörung an
  if(pTarget && pTarget->~RemoveTracer(this))
    return -1;

  //Haftzeit verringern
  EffectVar(1, pTarget, iEffectNumber)--;
  //Zerstörung bei Ende der Haftzeit
  if(EffectVar(1, pTarget, iEffectNumber) <= 0) return -1;

  //Farbe überprüfen
  var color = GetPlrColorDw(EffectVar(0, pTarget, iEffectNumber));
  if(GetTeamConfig(TEAM_TeamColors) && GetTeamColor(EffectVar(2, pTarget, iEffectNumber)))
    color = GetTeamColor(EffectVar(2, pTarget, iEffectNumber));

  //Lichteffekt
  pTarget->CreateParticle("FapLight",0,0,0,0,60, color, this);
}

global func FxTracerDartStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return;

  var iPlr = EffectVar(0, pTarget, iEffectNumber);
  if(!GetPlayerName(iPlr)) return;
  if(!Hostile(GetKiller(pTarget), iPlr) && GetKiller(pTarget) != iPlr)
    if(Contained(pTarget) && GetID(Contained(pTarget)) == FKDT || GetAction(pTarget) == "Dead")
      //Punkte bei Belohnungssystem (Kill Assist durch Peilsender)
      DoPlayerPoints(BonusPoints("TracerAssist"), RWDS_TeamPoints, iPlr, GetCursor(iPlr), IC22);
}