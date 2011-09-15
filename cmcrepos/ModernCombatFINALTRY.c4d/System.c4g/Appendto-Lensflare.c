/*-- Lensflare --*/

//Modifiziert den clonkeigenen Blendeffekt f�r CMC.

#strict 2
#appendto LENS

local fixed;


public func IsMaster()
{
  return fMaster;
}

/* Initialisierung */

protected func Initialize()
{
  //Unsichtbar
  SetVisibility (VIS_Owner, this);
  //Das erste Lensflare-Objekt pr�ft nur, dass jeder Spieler ein Umweltobjekt hat
  if(!FindObject2(Find_ID(GetID()), Find_Exclude(this)))
  {
    SetOwner(-1,this);
    fMaster = 1;
    //Plazieren
    var iPosX = LandscapeWidth()/2;
    //H�he nach Landschaft
    var iPosY;
    for(iPosY = 0; iPosY < LandscapeHeight(); iPosY++)
      if(GetMaterial(iPosX-GetX(), iPosY-GetY()) != -1) break;
    SetPosition(iPosX, iPosY/2);
    return;
  }
}

/* Flare-Verwaltung */

protected func ManageFlares()
{
  if(!GetEffectData(EFSM_Lensflares))
  {
    if(fixed) return;
    fixed = true;

  }
  else
  {
    if(fixed) fixed = false;
  }  
  //Unskalierten Vektor ermitteln
  var iVectorX = GetVectorX(GetOwner());
  var iVectorY = GetVectorY(GetOwner());
  var iVectorDist = GetVectorDist(GetOwner());
  //Modulation f�r alle Flares gleich
  var iColorModulation = RGBa(GetColorMod(),GetColorMod(),GetColorMod(),GetAlphaMod(iVectorDist));
  //Gleiche Abstandsangabe
  var iDistance = 1000-BoundBy((3000-iVectorDist*10)/3,0,1000);  // Promilleangabe 0 = nahe, 1000 = weit weg; 300 Pixel sind dabei "weit weg"
  var iAbsolutDistanc = (50*iDistance)/10; // Wieviele 1/100 Pixel zwischen den einzelnen Lensflares sind
  //Sonnensichtbarkeit pr�fen
  var fSunVisible = !fixed && IsDay() && (!GBackSemiSolid(0,0)) && SunFree(GetCursor(GetOwner()));
  //Tags�ber Sonne einblenden
  if(IsDay())
    SetVisibility (VIS_Owner);
  else
    SetVisibility (VIS_None);
  //Lensflares durchgehen
  var i = 0;
  for(var pFlare in aFlares)
  {
    if(fSunVisible)
    {
      //Tags sichtbar
      SetVisibility (VIS_Owner, pFlare);
      SetClrModulation(iColorModulation,pFlare);
      var LensDist = (i*iAbsolutDistanc)/100;
      SetPosition(GibLensPosX(LensDist,iVectorX,iVectorDist), GibLensPosY(LensDist,iVectorY,iVectorDist),pFlare);
    }
    else
      //Nachts und bei verdeckter Sonne unsichtbar
      SetVisibility (VIS_None, pFlare);
    i++;
  }
  //Blenden
  if(SunFree(GetCursor(GetOwner())))
  {
    var iYellow = BoundBy((2000-iVectorDist)/5,0,100);
    ScreenRGB(GetCursor(GetOwner()), RGB(000+(30*iYellow)/100,000+(30*iYellow)/100,000), 255, 10, true, SR4K_LayerLensflare);
  }
}

private func SunFree(object pTo)
{
  while(Contained(pTo)) pTo = Contained(pTo);
  if(Stuck(pTo)) return;
  if(GetEffectData(EFSM_Lensflares) < 2) return true;
  return PathFree(this->GetX(), this->GetY(), pTo->GetX(), pTo->GetY());
}