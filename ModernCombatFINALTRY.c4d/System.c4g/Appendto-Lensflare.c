/* Lensflare */

#strict 2
#appendto LENS

local fixed;

public func IsMaster() {
  return fMaster;
}

// Initialisierung
protected func Initialize()
{
  // Nur für den Herrn und Meister sichtbar
  SetVisibility (VIS_Owner, this);
  // Das erste Lensflare-Objekt prüft nur, dass jeder Spieler auch sein Umweltobjekt hat
  if(!FindObject2(Find_ID(GetID()), Find_Exclude(this)))
  {
    SetOwner(-1,this); // Unparteiisch sein!
    fMaster = 1;
    // Sind wir noch nicht plaziert?
    var iPosX = LandscapeWidth()/2;
    // Höhe nach Landschaft
    var iPosY;
    for(iPosY = 0; iPosY < LandscapeHeight(); iPosY++)
      if(GetMaterial(iPosX-GetX(), iPosY-GetY()) != -1) break;
    SetPosition(iPosX, iPosY/2);
    return;
  }
}

// Verwaltung der Flares
protected func ManageFlares()
{
  if(!GetEffectData(EFSM_Lensflares)) {
    if(fixed) return;
    fixed = true;
  }
  else {
    if(fixed) fixed = false;
  }  
  // unskalierten Vektor ermitteln
  var iVectorX = GetVectorX(GetOwner());
  var iVectorY = GetVectorY(GetOwner());
  var iVectorDist = GetVectorDist(GetOwner());
  // Die Modulation ist für alle Flares gleich
  var iColorModulation = RGBa(GetColorMod(),GetColorMod(),GetColorMod(),GetAlphaMod(iVectorDist));
  // Genauso auch die Abstandsangabe
  var iDistance = 1000-BoundBy((3000-iVectorDist*10)/3,0,1000);  // Promilleangabe 0 = nahe, 1000 = weit weg; 300 Pixel sind dabei "weit weg"
  var iAbsolutDistanc = (50*iDistance)/10; // Wieviele 1/100 Pixel zwischen den einzelnen Lensflares sind
  // Ist unsere liebe Sonne überhaupt sichtbar (also Tag + unverdeckt)
  var fSunVisible = !fixed && IsDay() && (!GBackSemiSolid(0,0)) && SunFree(GetCursor(GetOwner()));
  // Tagsüber die Sonne einblenden
  if(IsDay())
    SetVisibility (VIS_Owner);
  else
    SetVisibility (VIS_None);
  // Alle Lensflares durchgehen
  var i = 0;
  for(var pFlare in aFlares)
  {
    if(fSunVisible)
    {
      // Tags sichtbar
      SetVisibility (VIS_Owner, pFlare);
      SetClrModulation(iColorModulation,pFlare);
      var LensDist = (i*iAbsolutDistanc)/100;
      SetPosition(GibLensPosX(LensDist,iVectorX,iVectorDist), GibLensPosY(LensDist,iVectorY,iVectorDist),pFlare);
    }
    else
      // Nachts unsichtbar ODER bei verdeckter Sonne
      SetVisibility (VIS_None, pFlare);
    i++;
  }
  // Blenden über ScreenRGB
  if(SunFree(GetCursor(GetOwner()))) {
    var iYellow = BoundBy((2000-iVectorDist)/5,0,100);
    ScreenRGB(GetCursor(GetOwner()), RGB(000+(30*iYellow)/100,000+(30*iYellow)/100,000), 255, 10, true, SR4K_LayerLensflare);
  }
}

private func SunFree(object pTo) {
  while(Contained(pTo)) pTo = Contained(pTo);
  if(Stuck(pTo)) return;
  if(GetEffectData(EFSM_Lensflares) < 2) return true;
  return PathFree(this->GetX(), this->GetY(), pTo->GetX(), pTo->GetY());
}