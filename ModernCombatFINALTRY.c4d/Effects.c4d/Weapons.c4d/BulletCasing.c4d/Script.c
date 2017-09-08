/*-- Patronenh�lse --*/

#strict 2


/* Einstellung */

public func Set(int iSize)
{
  if(iSize >= 5)
  {
    SetCon(iSize*100/5);
    SetRDir(Distance(GetXDir(),GetYDir()));
  }
  else
  {
    SetGraphics("2");
  }

  if(!GetEffectData(EFSM_BulletCasing))
    FadeOut(this,10);
  else
    FadeOut(this,2);
}

/* Aufschlag */

func Hit(int iXDir, int iYDir)
{
  //Effekte
  if(GetCon() >= 140)
    Sound("CasingBigHit*.ogg",false,0,0,0,0,0,300);
  else
  if(GetCon() > 100)
    Sound("CasingHollowHit*.ogg",false,0,0,0,0,0,300);
  else
    Sound("CasingHit*.ogg",false,0,0,0,0,0,300);

  //Effektstufe pr�fen
  if(!GetEffectData(EFSM_BulletCasing))
    return RemoveObject();

  //Entsprechende H�pfbewegung
  if(GBackSolid(0,+5)) SetYDir(-iYDir/26);
  if(GBackSolid(0,-5)) SetYDir(-iYDir/26);
  if(GBackSolid(-5,0)) SetXDir(-iXDir/12);
  if(GBackSolid(+5,0)) SetXDir(-iXDir/12);

  if(Abs(GetXDir()) < 1 && Abs(GetYDir()) < 1) return;
  SetRDir();
  SetR();
}

/* H�lsenauswurf */

global func BulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor, int bForceLow)
{
  var xd,yd;

  //Bewegung ermitteln
  if(this)
  {
    xd = iXDir + GetXDir(this);
    yd = iYDir + GetYDir(this);
  }

  xd = iXDir + RandomX(-3,3);
  yd = iYDir + RandomX(-3,3);

  //Gr��e festlegen
  if(!iSize) iSize = 5;

  //Patronenh�lse erstellen
  var tmp = CreateObject(BHUL,iX,iY,NO_OWNER);
  SetXDir(xd, tmp);
  SetYDir(yd, tmp);
  if(!iColor) iColor = RGB(255,220,0);
  SetClrModulation(DoColorBrightness(iColor,40), tmp);
  tmp->Set(iSize);
}

/* R�ckw�rtskompatibilit�t */

global func SABulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor, id idType)
{
  //ID des Projektils identifizieren
  var ammoid = idType;
  //Standard nutzen wenn nicht gefunden
  if(!ammoid) ammoid = SHTX;

  //Patronenh�lse erstellen
  return ammoid->CustomBulletCasing(GetX()+iX,GetY()+iY,iXDir,iYDir,iSize,iColor);
}