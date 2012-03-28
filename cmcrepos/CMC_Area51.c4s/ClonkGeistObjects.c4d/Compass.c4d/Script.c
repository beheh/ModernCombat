/*--- Kompass ---*/

#strict 2

static const CMPS_CMAX_Distance = 400;	//Kompass zeigt nur auf dieser Distanz an (Nur X-Achsen-Distanz)

static CMPS_fNoCompass;
static CMPS_iCompassConf;

local plrmark;				//Markierung
local effectnr;

/* Initialisierung */

protected func Initialize()
{
  SetPosition(120, -140);
  SetVisibility(VIS_Owner);
  plrmark = CreateObject(CMPM, 0, 15, GetOwner());
  plrmark->SetAction("Player");
  var intervall = CMPS_iCompassConf;
  if(!intervall) intervall = 10;
  effectnr = AddEffect("Compass", this, 101, intervall, this);
}

protected func ChangeIntervall(int iNewIntervall)
{
	var markers = EffectVar(0, this, effectnr);
	RemoveEffect("Compass", this);
	effectnr = AddEffect("Compass", this, 101, iNewIntervall, this, 0, markers);
}

protected func RemoveMarker()
{
	if(plrmark) RemoveObject(plrmark);
  var marker = EffectVar(0, this, effectnr); 
  if(marker == []) return false;

  for(var mark in marker) // Bisherigen Markierungen löschen
  {
    if(GetType(mark) == C4V_Array)
    {
      var m = mark[1];
      if(m)
        RemoveObject(m);
    }
  }

  return true;
}

public func FxCompassStart(object target, int nr, temp, array markers)
{
	if(!markers) markers = [];
  EffectVar(0, target, nr) = markers; // marker!
  return true;
}

public func FxCompassTimer(object target, int nr)
{
  if(GetOwner(target) == NO_OWNER || !GetPlayerName(GetOwner(target)) || (!GetAlive(GetCrew(GetOwner(target))))) // Erstmal nochmal prüfen ob der Spieler existiert.
  {
    RemoveObject(target);
    return -1;
  }

  var tX = GetX(GetCrew(GetOwner(target)));
  var tY = GetY(GetCrew(GetOwner(target)));

  var clonks = FindObjects(Find_Or(Find_OCF(OCF_CrewMember), Find_ID(SM08), Find_ID(BKHK), Find_ID(OFPL), Find_ID(AHBS), Find_ID(FLA2), Find_ID(MNYS), Find_ID(SPBT))
                           ,Find_InRect(tX-CMPS_CMAX_Distance, 0, CMPS_CMAX_Distance*2, LandscapeHeight())
                           ,Find_Or(Find_Allied(GetOwner(target)), Find_Owner(NO_OWNER))); 
  var marker = EffectVar(0, target, nr);
  //Aktualisieren
  for(var clonk in clonks)
  {
    var nCreate = false;
    var mark = 0;
    if(GetID(clonk) == SM08)
    {
      if(!GetActionTarget(0, clonk))
         continue;

      if(!(GetOCF(GetActionTarget(0, clonk)) & OCF_CrewMember))
        continue;
    }
    else if(GetID(clonk) == MNYS)
    {
      //Nur anzeigen, wenn auch was gespawned wird
      if(!clonk->~GetCurrent())
        continue;
    }

    for(var i = 0; i < GetLength(marker); i++)
    {
      if(GetType(marker[i]) == C4V_Array && marker[i][0] == clonk)
       {
         nCreate = true;
        mark = marker[i][1];
        break;
      }
    }
    if(GetOwner(clonk) != GetOwner(target) || GetID(clonk) == SM08 || GetID(clonk) == BKHK || GetID(clonk) == AHBS)
    {
      if(!nCreate)
        mark = CreateObject(CMPM, 0, 15, GetOwner(target));  

      var W = 90*(100*(GetX(clonk)-tX)/CMPS_CMAX_Distance)/90;

      var y = 0, idT = GetID(clonk);

      if(GetAction(mark) == "Idle")
      {
        
        if(GetOwner(clonk) != NO_OWNER)
          mark->SetAction("Allied");
        else
          mark->SetAction("Neutral");

        if(idT == SM08)
          mark->SetAction("Enemy");

        if(idT == BKHK)
        {
          mark->SetAction("Blackhawk");
        }
        else if(idT == OFPL)
        {
          mark->SetAction("FlagPost");
        }
        else if(idT == AHBS)
        {
          mark->SetAction("COMStation");
        }
        else if(idT == FLA2)
        {
          mark->SetAction("Flag");
        }
        else if(idT == MNYS)
        {
          mark->SetAction("Money");
        }
        else if(idT == SPBT)
        {
          mark->SetAction("PatrolBoat");
        }
      }
      if(idT == BKHK || idT == OFPL || idT == AHBS || idT == FLA2 || idT == MNYS || idT == SPBT)
      {
        y = -15;
      }
      // 8355711 entspricht RGB(127, 127, 127) !
      if(idT == OFPL || idT == FLA2 || idT == AHBS)
      {
        if(clonk->GetTeam())
          SetClrModulation(GetTeamColor(clonk->GetTeam()), mark);
        else
          SetClrModulation(8355711, mark);
      }

      SetPosition(GetX(target)+W, GetY(target)+y, mark);

      var ar = [clonk, mark];
      if(!nCreate)
        marker[GetLength(marker)] = ar; 
    }
  }
  var i = 0;
  for(var elm in marker)
  {
    if(GetType(elm) == C4V_Array)
    {
      var pClonk = elm[0];
      var distance = GetX(pClonk)-tX;
      if(!pClonk || !Inside(distance, -(CMPS_CMAX_Distance), CMPS_CMAX_Distance))
      {
        RemoveObject(elm[1]);
        marker[i] = 0;
      }
      else if(Hostile(GetOwner(target), GetOwner(pClonk)) && GetOwner(pClonk) != NO_OWNER)
      {
        RemoveObject(elm[1]);
        marker[i] = 0;
      }
      else if(GetID(pClonk) == MNYS && !(pClonk->~GetCurrent()))
      {
        RemoveObject(elm[1]);
        marker[i] = 0;
      }
    }
    i++;
  }
  EffectVar(0, target, nr) = marker;
}

protected func Destruction()
{
  RemoveMarker();
}
