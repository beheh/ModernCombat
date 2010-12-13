/*-- Sonne --*/

#strict 2

local SonneX, SonneY;


/* Initialisierung */

protected func Initialize()
{
  //Kategorie setzen
  SetCategory(C4D_StaticBack|C4D_Background);
  //Action setzen
  SetAction("SeiBrav");
  //Sichtbarkeit einschränken
  SetVisibility (VIS_Owner, this);
  if(!FindObject(GetID())) 
  //Erstes Objekt prüft ob jeder Spieler ein eigenes Umweltobjekt besitzt
  {
    SetOwner(-1,this);
    SetCategory(1,this);
    SetAction("IchWillNurSpielerZaehlen");
    return ;
  }
}

/* Spielerprüfung */

private func FindeFreienSpieler()
{
  for(var i=0; i<16; ++i)
    if(GetPlayerName(i)&&(!FindObjectOwner(GetID(),i)))
      return i;
  return -1;
}

protected func PruefeAlleSpieler()
{
  var FreierSpieler;
  while((FreierSpieler=FindeFreienSpieler())!=-1)
    ErstelleLenseflare(FreierSpieler);
}

private func ErstelleLenseflare(FuerDiesenSpieler)
{
  CreateObject(GetID(),0,0,FuerDiesenSpieler);
}
			
protected func TuWasBraves()
{
  //Schon vorhanden?
  if(ObjectCount(LENS))
  {
    RemoveObject(this);
    return ;
  }
  //Kein Besitzer?
  if((!GetPlayerName(GetOwner())) || GetOwner()==-1)
  {
    RemoveObject(this); 
    return ;
  }
  //Position anpassen
  BerechneSonne();
  SetPosition(SonneX,SonneY);
  //Nachts ausblenden
  if(IsDay())
    SetVisibility (VIS_Owner);
  else
    SetVisibility (VIS_None);
  //Sofern der Spieler alleine ist, blenden
  var VektorDist=GibVektorDist(GetOwner());
  if(GetPlayerCount()==1)
    if(IsDay())
    {
      var Gelbstich = BoundBy((500-VektorDist)/5,0,100); // Prozentangabe 0 = weit weg, 100 = nahe; 500 Pixel sind dabei "weit weg"
      SetGamma (RGB(000+(30*Gelbstich)/100,000+(30*Gelbstich)/100,000),
      					RGB(128+(60*Gelbstich)/100,128+(60*Gelbstich)/100,128), 
      					RGB(255,255,255), GammaRampe_Lenseflare());
    }
}

/* Berechnungen */

private func GibMittelpunktX(iSpieler)
{
  if(IsNetwork())
    return BoundBy(GetX(GetCursor(iSpieler)),400,LandscapeWidth()-400);
  else
    return BoundBy(GetPlrViewX(iSpieler),400,LandscapeWidth()-400);
}

private func GibMittelpunktY(iSpieler)
{
  if(IsNetwork())
    return Min(GetY(GetCursor(iSpieler)),LandscapeHeight()-300);
  else
    return Min(GetPlrViewY(iSpieler),LandscapeHeight()-300);
}

private func BerechneSonne()
{
  BerechneSonneX();
  BerechneSonneY();
}

private func BerechneSonneX()
{
  var ZeitObjekt = FindObject(TIME);
  if(!ZeitObjekt)
    SonneX = LandscapeWidth() / 2;
  else
  {
    var Winkel = GibTageszeit()*360/100 +90;
    var VonEck = LandscapeWidth()/2+Cos(Winkel,LandscapeWidth()/2);
    Winkel = (GibTageszeit()+1)*360/100 +90;
    var ZuEck = LandscapeWidth()/2+Cos(Winkel,LandscapeWidth()/2);
    var Zwischenschritte = PrivateCall(ZeitObjekt,"TimeResolution") / 100;
    var Schrittweite = (ZuEck-VonEck)*100000 / (Zwischenschritte*TIME_Tick());
    var Vergangen = Local(1, ZeitObjekt) - GibTageszeit()*100;
    Vergangen=Vergangen*TIME_Tick()+FrameCounter()%10;
    SonneX = VonEck + (Vergangen*Schrittweite)/100000; // 100000 = 100000 wegen der 1/100000 Schrittweite
  }
}

private func BerechneSonneY()
{
  var ZeitObjekt = FindObject(TIME);
  if(!ZeitObjekt)
    SonneY = 150;
  else
  {
    var Winkel = GibTageszeit()*360/100 +90;
    var VonEck = GetHorizonHeight(0)-Sin(Winkel,300)-50;
    Winkel = (GibTageszeit()+1)*360/100 +90;
    var ZuEck = GetHorizonHeight(0)-Sin(Winkel,300)-50;
    var Zwischenschritte = PrivateCall(ZeitObjekt,"TimeResolution") / 100;
    var Schrittweite = (ZuEck-VonEck)*100000 / Zwischenschritte;
    Schrittweite/=TIME_Tick(); // Jedes Time-Tick besteht aus 10 einzelnen Ticks
    var Vergangen = Local(1, ZeitObjekt) - GibTageszeit()*100;
    Vergangen=Vergangen*TIME_Tick()+FrameCounter()%10;
    SonneY = VonEck + (Vergangen*Schrittweite)/100000;
  }
}

private func TIME_Tick()
{
  return 10;
}

private func GibTageszeit()
{
  var ZeitObjekt = FindObject(TIME);
  if (!ZeitObjekt) return -1;
  //0-100=Mittag-Mittag
  return(Local(1, ZeitObjekt)*100/PrivateCall(ZeitObjekt,"TimeResolution"));
}

private func GibVektorDist(iSpieler)
{
  return Distance(GibMittelpunktX(iSpieler),GibMittelpunktY(iSpieler),SonneX,SonneY);
}

private func GetHorizonHeight(iX)
{
  var iY = - GetY(); // Zu globaler Koordinate wandeln
  iX -= GetX(); // Zu globaler Koordinate wandeln
  while ( (iY<LandscapeHeight())&&(!GBackSemiSolid(iX,iY)))
  {
    iY+=10;
  }
  return iY+GetY();
}