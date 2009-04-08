/*-- Patronenhülse --*/

#strict


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
  
  FadeOut4K(2);
}

protected func Hit()
{
  if(GetCon() > 100)
    Sound("BigHullHit*",false,0,0,0,0,0,300);
  else
    Sound("HullHit*",false,0,0,0,0,0,300);
  
  SetRDir();
  SetR();
}

global func BulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor, int bForceLow)
{
  if(EffectLevel() == EM4K_Low) return();
  var xd,yd;
  
  if(this())
  {
    xd = iXDir + GetXDir(this());
    yd = iYDir + GetYDir(this());
  }
  
  xd = iXDir + RandomX(-3,3);
  yd = iYDir + RandomX(-3,3);
  
  if(!iColor) iColor = RGB(255,230,0);
  
  if((EffectLevel() == EM4K_Normal) || bForceLow)
  {
    if(iSize >= 5)
      CreateParticle("BulletCasing2",iX,iY,iXDir,iYDir,10,iColor);
    else
      CreateParticle("BulletCasing",iX,iY,iXDir,iYDir,iSize*5,iColor);
    return();
  }
  
  if(!iSize) iSize = 5;
  
  var tmp = CreateObject(BHUL,iX,iY,NO_OWNER);
  tmp->SetXDir(xd);
  tmp->SetYDir(yd);
  tmp->SetClrModulation(iColor);
  tmp->Set(iSize);
}