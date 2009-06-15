/* Kubra Damm */

#strict


/* Initialisierung */

protected func Initialize()
{
  SetSkyParallax(0,14,14, 0,0,SkyPar_Keep(),SkyPar_Keep());
  SetGamma(RGB(15,15,15),RGB(128,128,128),RGB(215,215,215));
  // Musik bitte.
  Music("Combat Situation.ogg");
  // Karte befüllen
  PlaceMapObjects();
  return(1);
}

func PlaceMapObjects() {

CreateObject(WTSE,1960,300,-1);
CreateObject(PLM2,2228,162,-1);
CreateObject(PLM1,2204,170,-1);
CreateObject(PLM1,2244,165,-1);
CreateObject(PLM2,2310,153,-1);
CreateObject(PBRL,1842,169,-1);
CreateObject(VSPW,1816,155,-1)->SetupVehicleSpawn([SUB2], this(), 1800, 1);
CreateObject(PBRL,1852,169,-1);
PlaceSpawnpoint(TRP1,2184,157);
CreateObject(TRE2,101,354,-1);
CreateObject(RB6C,1188,371,-1);
CreateObject(RB6C,215,421,-1);
CreateObject(TRE3,191,603,-1);
CreateObject(TRE3,1096,649,-1);
CreateObject(TRE2,1084,649,-1);
CreateObject(TRE3,1416,766,-1);
CreateObject(TRE1,487,829,-1);
CreateObject(TRE3,1028,812,-1);
CreateObject(PBRL,1713,789,-1);
CreateObject(PBRL,1703,789,-1);
CreateObject(TRE2,1085,868,-1);
CreateObject(TRE3,1109,870,-1);

  }
