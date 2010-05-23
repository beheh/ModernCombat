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

func Hit(int iXDir, int iYDir)
{
  if(GetCon() > 100)
    Sound("BigHullHit*.ogg",false,0,0,0,0,0,300);
  else
    Sound("HullHit*.ogg",false,0,0,0,0,0,300);
    
  //Entsprechende Hüpfbewegung
  if(GBackSolid(0,+5) && iYDir != 0) return(SetYDir(-iYDir/26));
  if(GBackSolid(0,-5) && iYDir != 0) return(SetYDir(-iYDir/26));
  if(GBackSolid(-5,0) && iXDir != 0) return(SetXDir(-iXDir/12));
  if(GBackSolid(+5,0) && iXDir != 0) return(SetXDir(-iXDir/12));
  
  SetRDir();
  SetR();
}

global func BulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor, int bForceLow)
{
  var xd,yd;
  
  if(this())
  {
   xd = iXDir + GetXDir(this());
   yd = iYDir + GetYDir(this());
  }
  
  xd = iXDir + RandomX(-3,3);
  yd = iYDir + RandomX(-3,3);

  if(!iSize) iSize = 5;

  var tmp = CreateObject(BHUL,iX,iY,NO_OWNER);
  tmp->SetXDir(xd);
  tmp->SetYDir(yd);
  if(!iColor) iColor = RGB(255,220,0);
  tmp->SetClrModulation(DoColorBrightness(iColor,40));
  tmp->Set(iSize);
}
