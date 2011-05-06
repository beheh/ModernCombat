/*-- Lenseflare --*/

//Ermöglicht eine realistischere Blendung mehrerer Spieler.

#strict 2
#appendto LENS

local fixed;


/* Initialisierung */

protected func Initialize()
{
  //Nur für den Herrn und Meister sichtbar
  SetVisibility (VIS_Owner, this);
  //Das erste Lenseflare-Objekt prüft nur, dass jeder Spieler auch sein Umweltobjekt hat
  if(!FindObject2(Find_ID(GetID()), Find_Exclude(this)))
  {
    SetOwner(-1,this); // Unparteiisch sein!
    fMaster = 1;
    //Sind wir noch nicht plaziert?
    var iPosX = LandscapeWidth()/2;
    //Höhe nach Landschaft
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
  if(!GetEffectData(EFSM_Lenseflares))
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
  //Die Modulation ist für alle Flares gleich
  var iColorModulation = RGBa(GetColorMod(),GetColorMod(),GetColorMod(),GetAlphaMod(iVectorDist));
  //Genauso auch die Abstandsangabe
  var iDistance = 1000-BoundBy((3000-iVectorDist*10)/3,0,1000);  // Promilleangabe 0 = nahe, 1000 = weit weg; 300 Pixel sind dabei "weit weg"
  var iAbsolutDistanc = (50*iDistance)/10; // Wieviele 1/100 Pixel zwischen den einzelnen Lenseflares sind
  //Sonne sichtbar und nicht verdeckt?
  var fSunVisible = !fixed && IsDay() && (!GBackSemiSolid(0,0)) && SunFree(GetCursor(GetOwner()));
  //Tagsüber die Sonne einblenden
  if(IsDay())
    SetVisibility (VIS_Owner);
  else
    SetVisibility (VIS_None);
  //Alle Lenseflares durchgehen
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
      //Nachts/bei verdeckter Sonne unsichtbar
      SetVisibility (VIS_None, pFlare);
    i++;
  }
  //Sofern der Spieler alleine ist
  if(GetPlayerCount() == 1)
  {
    if(IsDay())
    {
      var iYellow = BoundBy((500-iVectorDist)/5+(SunFree(GetCursor(GetOwner()))*100),0,100); // Prozentangabe 0 = weit weg, 100 = nahe; 500 Pixel sind dabei "weit weg"
      SetGamma (RGB(000+(30*iYellow)/100,000+(30*iYellow)/100,000),
            RGB(128+(60*iYellow)/100,128+(60*iYellow)/100,128), 
            RGB(255,255,255), GammaRamp_Lenseflare());
    }
  }
}

private func SunFree(object pTo)
{
  while(Contained(pTo)) pTo = Contained(pTo);
  if(Stuck(pTo)) return;
  if(GetEffectData(EFSM_Lenseflares) < 2) return true;
  return PathFree(this->GetX(), this->GetY(), pTo->GetX(), pTo->GetY());
}