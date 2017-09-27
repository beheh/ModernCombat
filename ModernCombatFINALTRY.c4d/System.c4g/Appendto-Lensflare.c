/*-- Lensflare --*/

//Modifiziert den clonkeigenen Blendeffekt für CMC.

#strict 2
#appendto LENS

local disabled;
local fSun;
static LENS_MaxDistance;

public func IsSun()		{return fSun;}
public func IsMaster()		{return fMaster;}
public func LenseflareCount()	{return 15;}


/* Initialisierung */

protected func Initialize()
{
  //Erstes Objekt ist Master-Lensflare
  if(!FindObject2(Find_ID(GetID()), Find_Exclude(this)))
  {
    SetOwner(-1,this);
    SetVisibility(VIS_Owner, this);
    fMaster = 1;
    LENS_MaxDistance = LENS_MaxDistance || 470;

    //Möglichst mittig plazieren
    var iPosX = LandscapeWidth()/2;
    var iPosY;
    for(iPosY = 0; iPosY < LandscapeHeight(); iPosY++)
      if(GetMaterial(iPosX-GetX(), iPosY-GetY()) != -1) break;
    SetPosition(iPosX, iPosY/2);
    return;
  }
}

/* Spielerverwaltung */

//Spieler mit eigenem Master-Lensflare ausstatten
public func InitializePlayer(int iPlr)
{
  if(FindObject2(Find_ID(LENS), Find_Owner(iPlr), Find_Action("ManageFlares")))
    return;

  return _inherited(iPlr, ...);
}

//Initialisierung als Spieler-Master-Lensflare
public func InitializeLenseflare()
{
  //Position speichern
  iSunX = GetX(); iSunY = GetY();
  SetCategory(C4D_StaticBack | C4D_Background | C4D_MouseIgnore);
  SetVisibility(VIS_Owner | VIS_God);
  aFlares = aFlares || [];
  for(var i = GetLength(aFlares); i < LenseflareCount(); i++)
  {
    aFlares[i] = CreateObject(GetID(), 0, 0, GetOwner());
    aFlares[i]->SetCategory(C4D_StaticBack | C4D_MouseIgnore | C4D_Foreground);
    aFlares[i]->SetAction(Format("Fleck%d",i % 14));
  }
  SetAction("ManageFlares");
  fSun = true;
}

//Timer-Aufruf des Spieler-Master-Lensflare
protected func ManageFlares()
{
  //Effektstufe prüfen
  if(!GetEffectData(EFSM_Lensflares))
  {
    if(disabled) return;
    disabled = true;
  }
  else
    if(disabled) disabled = false;

  //Geringe Effektstufe: Nur Sonne anzeigen
  if(disabled)
  {
    if(!IsDay())
      SetVisibility(VIS_None);
    //Flares unsichtbar schalten
    var i = 0;
    for(var pFlare in aFlares)
    {
      SetVisibility(VIS_None, pFlare);
      i++;
    }
    return;
  }

  //Nur sichtbar wenn Tag, keine Dunkelheit und kein Material vor der Sonnenposition
  if(IsDay() && !GBackSemiSolid(0,0))
  {
    //Distanz ermitteln
    var iVectorDist = GetVectorDist(GetOwner());
    //Transparenz für Flares ermitteln
    var alpha = GetAlphaMod(iVectorDist);
    //Sichtbarkeit der Sonne gegeben?
    var path_free;
    var path_free = SunFree(GetCursor(GetOwner()));
    var fSunVisible = alpha < 255 && iVectorDist && path_free;
    if(fSunVisible)
    {
      SetVisibility(VIS_Owner | VIS_God);
      var iVectorX = GetVectorX(GetOwner());
      var iVectorY = GetVectorY(GetOwner());
      //Flare-Färbung ermitteln
      var iColorModulation = RGBa(GetColorMod(),GetColorMod(),GetColorMod(),alpha);
      //Abstände zwischen allen Flares ermitteln
      var iDistance = 1000-BoundBy((3000-iVectorDist*10)/3,0,1000);	//Promilleangabe 0 = nahe, 1000 = weit weg; 300 Pixel sind dabei "weit weg"
      var iAbsolutDistance = (50*iDistance)/10;				//Wieviele 1/100 Pixel zwischen den einzelnen Lensflares sind

      //Ermittelte Daten auf Lensflares anwenden
      var i = 0;
      for(var pFlare in aFlares)
      {
        //Tags sichtbar
        SetVisibility(VIS_Owner, pFlare);
        SetClrModulation(iColorModulation,pFlare);
        var LensDist = (i*iAbsolutDistance)/100;
        SetPosition(GibLensPosX(LensDist,iVectorX,iVectorDist), GibLensPosY(LensDist,iVectorY,iVectorDist),pFlare);
        i++;
      }
    }
    else
    {
      //Flares unsichtbar schalten
      var i = 0;
      for(var pFlare in aFlares)
      {
        SetVisibility(VIS_None, pFlare);
        i++;
      }
    }
    //Unabhängige Bildschirmfärbung bei Sichtkontakt
    if(path_free)
    {
      var iYellow = BoundBy((2000-iVectorDist)/5,0,100);
      ScreenRGB(GetCursor(GetOwner()), RGB(000+(30*iYellow)/100,000+(30*iYellow)/100,000), 255, 10, true, SR4K_LayerLensflare);
    }
  }
  else
  {
    //Sonne und Flares unsichtbar schalten
    SetVisibility(VIS_None);
    var i = 0;
    for(var pFlare in aFlares)
    {
      SetVisibility(VIS_None, pFlare);
      i++;
    }
  }
}

private func SunFree(object pTo)
{
  if(!pTo) return;
  while(Contained(pTo)) pTo = Contained(pTo);
  if(Stuck(pTo)) return;
  return PathFree(this->GetX(), this->GetY(), pTo->GetX(), pTo->GetY());
}

private func GetAlphaMod(int distance)
{
  distance = BoundBy(distance - 290, 0, GetMaxDistance());
  return BoundBy(distance / (GetMaxDistance() / 255), 0, 255);
}

/* Allgemein */

public func & GetMaxDistance()	{return LENS_MaxDistance;}

public func SetMaxDistance(int distance)
{
  if(distance < 0) distance = 0;
  LENS_MaxDistance = distance;
}

global func SetSunPosition(int x, int y)
{
  for(var lens in FindObjects(Find_ID(LENS)))
  {
    LocalN("iSunX", lens) = x;
    LocalN("iSunY", lens) = y;
    if(lens->~IsSun())
      SetPosition(x, y, lens);
  }

  return;
}

private func IsDay()
{
  if(GetDarkness(1000) > ELGT_MaxDarkness)
    return false;

  return _inherited(...);
}